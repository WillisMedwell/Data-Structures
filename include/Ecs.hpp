#pragma once

#include <Sov.hpp>

namespace EcsMetaprogramming {

template <typename T>
concept CHasName = requires(T t) {
    std::is_same_v<decltype(t.name), std::string>;
};
struct Name {
    std::string name;

    auto operator()()
    {
        return name;
    }

    static Name getCorresponding(const CHasName auto& thing)
    {   
        return Name{thing.name};
    }
};

template <typename T>
concept CHasKinematics = requires(T t) {
    std::is_same_v<decltype(t.velocity), std::pair<float, float>>;
    std::is_same_v<decltype(t.position), std::pair<float, float>>;
};

struct Kinematics {
    float x_vel, y_vel;
    float x_pos, y_pos;

    static Kinematics getCorresponding(const CHasKinematics auto& thing)
    {   
        return {
            thing.velocity.first, 
            thing.velocity.second, 
            thing.position.first, 
            thing.position.second 
        };
    }
};

template <typename Tuple, typename T>
auto addCompositionIfKinematic(Tuple tuple, T t)
{
    if constexpr (CHasKinematics<T>) {
        return std::tuple_cat(tuple, std::tuple<Kinematics> {});
    } else {
        return tuple;
    }
}

template <typename Tuple, typename T>
auto addCompositionIfNamed(Tuple tuple, T t)
{
    if constexpr (CHasName<T>) {
        return addCompositionIfKinematic(
            std::tuple_cat(tuple, std::tuple<Name> {}),
            t);
    } else {
        return addCompositionIfKinematic(tuple, t);
    }
}

template <typename T>
auto getCompositonOf()
{
    return addCompositionIfNamed(std::tuple<size_t> {}, T {});
}

// Helper function to create an instance of MyVariadicClass from a tuple
template <typename... Types>
Sov<Types...> makeSovFromTuple(const std::tuple<Types...>& t)
{
    return std::apply([](auto&&... args) {
        return Sov<Types...>(std::forward<decltype(args)>(args)...);
    },
        t);
}

template <typename T, typename Tuple, int Index = 0>
consteval int indexOfType()
{
    if constexpr (Index == std::tuple_size_v<Tuple>) {
        return -1;
    } else {
        if (std::is_same_v<std::tuple_element_t<Index, Tuple>, T>) {
            return Index;
        } else {
            return indexOfType<T, Tuple, Index + 1>();
        }
    }
}

template<typename T, typename StoredTuple, size_t Index = 1>
StoredTuple construct(const T& t, StoredTuple&& tuple = StoredTuple{})
{
    if constexpr(Index == std::tuple_size_v<StoredTuple>)
    {
        return tuple;
    }
    else 
    {
        std::get<Index>(tuple) = std::tuple_element_t<Index, StoredTuple>::getCorresponding(t);
        return construct<T, StoredTuple, Index + 1>(t, std::forward<StoredTuple>(tuple));
    }
}

}

template <typename... Types>
class Ecs {
public:


    std::tuple<decltype(EcsMetaprogramming::makeSovFromTuple(EcsMetaprogramming::getCompositonOf<Types>()))...> data;

    void printNamable()
    {
        auto printIfNamed = [&](const auto& container) {
            if constexpr (requires { container.field<EcsMetaprogramming::Name>(); }) {
                for (auto name : container.field<EcsMetaprogramming::Name>()) {
                    std::cout << name() << " ";
                }
            }
        };
        std::apply(printIfNamed, data);
    }

    template <typename T>
    auto add(const T& element)
    {
        using TypesTuple = std::tuple<Types...>;

        constexpr auto i = EcsMetaprogramming::indexOfType<T, TypesTuple>();
        static_assert(i != -1, "Type is not defined in the ECS template, so it cannot be added to system");

        using StoredType = decltype(EcsMetaprogramming::getCompositonOf<T>());
        
        StoredType t = EcsMetaprogramming::construct<T, StoredType>(element);
        
        std::get<i>(data).pushBack(t);
        
    }
};