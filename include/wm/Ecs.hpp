#pragma once

#include "wm/Sov.hpp"

#include <array>

namespace wm {

namespace EcsMetaprogramming {
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

    template <typename T, typename StoredTuple, size_t Index = 0>
    StoredTuple&& construct(const T& t, StoredTuple&& tuple = StoredTuple {})
    {
        if constexpr (Index == std::tuple_size_v<StoredTuple>) {
            return std::forward<StoredTuple>(tuple);
        } else {
            using Component = std::tuple_element_t<Index, StoredTuple>;
            std::get<Index>(tuple) = Component { t };
            return construct<T, StoredTuple, Index + 1>(t, std::forward<StoredTuple>(tuple));
        }
    }

    template <typename Tuple, typename Pred, size_t Index = 0>
    constexpr auto forEach(Tuple& tuple, Pred predicate)
    {
        if constexpr (Index == std::tuple_size_v<Tuple>) {
            return;
        } else {
            predicate(std::get<Index>(tuple));
            return forEach<Tuple, Pred, Index + 1>(tuple, predicate);
        }
    }

    template <typename T, typename... Ts>
    struct contains : std::false_type { };

    template <typename T, typename U, typename... Ts>
    struct contains<T, U, Ts...> : contains<T, Ts...> { };

    template <typename T, typename... Ts>
    struct contains<T, T, Ts...> : std::true_type { };

    template <typename Tuple, typename T>
    concept CTupleHasType = contains<T, std::tuple_element_t<0, Tuple>>::value;

}

template <typename T>
concept CHasKinematics = requires(T t) {
    {
        t.kinematics
    };
    // requires std::is_same_v<Component::Kinematics, std::decay_t<decltype(t.kinematics)>> == true;
};

template <typename T>
concept CHasDescription = requires(T t) {
    {
        t.description
    };
    // requires std::is_same_v<Component::Kinematics, std::decay_t<decltype(t.kinematics)>> == true;
};

struct Component {
    struct Description {
        std::string name;
        std::string info;

        constexpr Description()
            : name {}
            , info {}
        {
        }
        Description(const CHasDescription auto& obj)
            : name(obj.description.name)
            , info(obj.description.info)
        {
        }
        Description(const auto& n, const auto& i)
            : name(n)
            , info(i)
        {
        }
    };
    struct Kinematics {
        std::array<float, 2> pos;
        std::array<float, 2> vel;

        constexpr Kinematics()
            : pos {}
            , vel {}
        {
        }
        Kinematics(const CHasKinematics auto& obj)
            : pos(obj.kinematics.pos)
            , vel(obj.kinematics.vel)
        {
        }
    };

    using Components = std::tuple<Description, Kinematics>;
};

struct Player {
    Component::Description description;
    Component::Kinematics kinematics;
};

struct Ball {
    Component::Kinematics kinematics;
};

struct Id {
    Component::Description description;
};

template <typename T, typename C>
concept CHasComponent
    = (CHasKinematics<T> && std::is_same_v<C, Component::Kinematics>)
    || (CHasDescription<T> && std::is_same_v<C, Component::Description>);

template <typename... Ts, typename T>
constexpr auto append_to_tuple(const std::tuple<Ts...>&, T)
{
    return std::tuple<Ts..., T> {};
}

template <typename Components, typename T, size_t Index = 0, typename Tuple = std::tuple<>>
consteval auto getCompositionOf()
{
    if constexpr (Index == std::tuple_size_v<Components>) {
        return Tuple {};
    } else {
        using Component = std::tuple_element_t<Index, Components>;
        if constexpr (CHasComponent<T, Component>) {
            using NewTuple = decltype(append_to_tuple(Tuple {}, Component {}));
            return getCompositionOf<Components, T, Index + 1, NewTuple>();
        } else {
            return getCompositionOf<Components, T, Index + 1, Tuple>();
        }
    }
}

/*
 * Components = what can is 'componentised'
 * Types = things that can be broken down into components (i.e. entity types)
 */
template <typename Components, typename... Types>
// TODO: assert that all Types generate at least one component.
class Ecs2 {
private:
    using TupleTypes = std::tuple<Types...>;
    using TupleComponents = std::tuple<decltype(getCompositionOf<Components, Types>())...>;
    using TupleSovComponents = std::tuple<decltype(EcsMetaprogramming::makeSovFromTuple(getCompositionOf<Components, Types>()))...>;
    TupleSovComponents data;

    template <typename ComponentType, size_t Index = 0, typename Predicate>
    constexpr auto forEachWith(TupleSovComponents& tsc, Predicate predicate)
    {
        if constexpr (Index == std::tuple_size_v<TupleSovComponents>) {
            return;
        } else {
            using S = std::tuple_element_t<Index, TupleSovComponents>;
            auto& sov = std::get<Index>(tsc);
            if constexpr (S::hasField(ComponentType {})) {
                for (auto& e : sov.template field<ComponentType>()) {
                    predicate(e);
                }
            }
            forEachWith<ComponentType, Index + 1>(tsc, predicate);
        }
    }

    template <size_t Index = 0, typename Predicate>
    constexpr auto forEachSov(TupleSovComponents& tsc, Predicate predicate)
    {
        if constexpr (Index == std::tuple_size_v<TupleSovComponents>) {
            return;
        } else {
            using Sov = std::tuple_element_t<Index, TupleSovComponents>;
            predicate(std::get<Index>(tsc));
            forEachSov<Index + 1>(tsc, predicate);
        }
    }

public:
    void printNamable()
    {
        auto print = [&](Component::Description& e) {
            std::cout << e.name << '\n';
        };
        forEachWith<Component::Description>(data, print);
    }

    template <typename T>
    auto add(const T& element)
    {
        using TypesTuple = std::tuple<Types...>;

        constexpr auto i = EcsMetaprogramming::indexOfType<T, TypesTuple>();
        static_assert(i != -1, "Type is not defined in the ECS template, so it cannot be added to system");

        using ComposedType = decltype(getCompositionOf<Components, T>());

        ComposedType reconstructed_element = EcsMetaprogramming::construct<T, ComposedType>(element);
        // TODO std::get<i>(data).pushBack(reconstructed_element);
    }

    auto storageLayout() -> std::string
    {
        std::string storage_layout = "std::tuple {\n";

        auto addLayout = [&](auto& sov) {
            storage_layout += std::string { "\t" }
                + std::string { typeid(decltype(sov)).name() }
                + std::string { "\n" };
        };

        forEachSov(data, addLayout);
        storage_layout += "};";

        return storage_layout;
    }

    template <typename T>
    auto begin()
    {
        constexpr bool is_component_iterator = EcsMetaprogramming::CTupleHasType<Components, T>;
        constexpr bool is_entity_iterator = EcsMetaprogramming::CTupleHasType<TupleTypes, T>;

        if constexpr (is_component_iterator) {

        } else if constexpr (is_entity_iterator) {
            using ComponentType = decltype(getCompositionOf<Components, T>());
            using ComponentTypeSov = decltype(EcsMetaprogramming::makeSovFromTuple(ComponentType {}));
            return std::get<ComponentTypeSov>(data).begin();
        } else {
            throw std::runtime_error("cannot iterate over this type");
        }
    }

    template <typename T>
    auto end()
    {
        constexpr bool is_component_iterator = EcsMetaprogramming::CTupleHasType<Components, T>;
        constexpr bool is_entity_iterator = EcsMetaprogramming::CTupleHasType<TupleTypes, T>;

        if constexpr (is_component_iterator) {

        } else if constexpr (is_entity_iterator) {
            using ComponentType = decltype(getCompositionOf<Components, T>());
            using ComponentTypeSov = decltype(EcsMetaprogramming::makeSovFromTuple(ComponentType {}));
            return std::get<ComponentTypeSov>(data).end();
        } else {
            throw std::runtime_error("cannot iterate over this type");
        }
    }

    template <typename T>
    struct EntityIterator {
        using ComposedType = decltype(getCompositionOf<Components, T>());
        Sov<ComposedType>& data;
        EntityIterator(Sov<ComposedType>& d)
            : data(d)
        {
        }
    };

    template <typename T>
    struct ComponentIterator {
        Ecs2& ecs;
        friend Ecs2;
    };
};

}
