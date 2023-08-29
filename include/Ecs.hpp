#pragma once

#include <Sov.hpp>

namespace EcsMetaprogramming {

template <typename T>
concept CHasName = requires(T t) {
    std::is_same_v<decltype(t.name), std::string>;
};
template <typename T>
concept CHasKinematics = requires(T t) {
    std::is_same_v<decltype(t.velocity), std::pair<float, float>>;
    std::is_same_v<decltype(t.position), std::pair<float, float>>;
};

struct Name {
    std::string name;

    auto operator()()
    {
        return name;
    }
};

struct Kinematics {
    float x_vel, y_vel;
    float x_pos, y_pos;
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

template <size_t Index = 0, typename T, typename Tuple>
consteval auto indexOfType()
{
    if constexpr (Index == std::tuple_size_v<Tuple>) {
        return Index;
    } else {
        if constexpr (std::is_same_v<std::tuple_element_t<Index, Tuple>, T>) {
            return Index;
        } else {
            return indexOfType<Index + 1, T, Tuple>();
        }
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
    void add(const T& element)
    {
        using TypesTuple = std::tuple<Types...>;
        constexpr auto i = EcsMetaprogramming::indexOfType<0, T, TypesTuple>();
        std::cout << i << '\n';
    }
};