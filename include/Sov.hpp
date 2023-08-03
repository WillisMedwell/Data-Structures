#pragma once
// Structure of vectors implementation

#include <cstdint>
#include <iterator>
#include <memory>
#include <span>
#include <stdexcept>
#include <tuple>

template <typename... Types>
class Sov {
public:
    template <size_t index = 0, typename Tuple, typename T, class Pred>
    constexpr static auto accumulateTuple(Tuple& tuple, T& init, Pred predicate)
    {
        if constexpr (index == std::tuple_size<Tuple>()) {
            return init;
        } else {
            auto& element = std::get<index>(tuple);
            return accumulateTuple<index + 1>(tuple, predicate(init, element), predicate);
        }
    }

    template <size_t index = 0, typename Tuple, class Pred>
    constexpr static void forEachTuple(Tuple& tuple, Pred predicate)
    {
        if constexpr (index == std::tuple_size<Tuple>()) {
            return;
        } else {
            auto& element = std::get<index>(tuple);
            predicate(element);
            return forEachTuple<index + 1>(tuple, predicate);
        }
    }

    using TypesRefs = typename std::tuple<Types&...>;
    using TypesValues = typename std::tuple<Types...>;
    using TypesBegin = typename std::tuple<Types*...>;
    constexpr static size_t num_types = std::tuple_size<TypesValues>();

    constexpr static size_t bytes_per_entry = sizeof(TypesValues);

    size_t entry_capacity = 0;
    size_t entry_count = 0;
    std::unique_ptr<uint8_t[]> data;
    TypesBegin beginnings;

    template <size_t index = 0>
    constexpr static void pushTuple(TypesBegin& dest, const TypesValues& source, const size_t i)
    {
        if constexpr (index == num_types) {
            return;
        } else {
            using ElementType = std::remove_reference_t<decltype(std::get<index>(source))>;
            new (std::get<index>(dest) + i)(ElementType)(std::get<index>(source));
            return pushTuple<index + 1>(dest, source, i);
        }
    }

    template <size_t index = 0, typename T>
    constexpr static auto getElement(TypesBegin& source, const size_t i, T zipped_element)
    {

        if constexpr (index == num_types) {
            // return the completed zip.
            return zipped_element;
        } else {
            // add element to the zip.
            auto& element = std::get<index>(source)[i];
            auto updated_zipped_element = std::tuple_cat(zipped_element, std::tuple<decltype(element)> { element });
            return getElement<index + 1>(source, i, updated_zipped_element);
        }
    }

    Sov(size_t init_capacity = 20)
        : entry_capacity(init_capacity)
        , entry_count(0)
        , data(std::make_unique<uint8_t[]>(bytes_per_entry * entry_capacity + 100)) // add memory alignment padding...
    {
        uint8_t* ptr = data.get();

        auto assign_beginning = [&](auto& begin) {
            using Ptr = decltype(begin);
            using Value = decltype(*begin);

            // in case of memory alignment issues.
            size_t alignment = alignof(Value);
            std::uintptr_t address = reinterpret_cast<std::uintptr_t>(ptr);
            while (address % alignment) {
                ++ptr;
                address = reinterpret_cast<std::uintptr_t>(ptr);
            }
            begin = reinterpret_cast<Ptr>(ptr);
            ptr += sizeof(Value) * entry_capacity;
        };
        forEachTuple(beginnings, assign_beginning);
        assert(reinterpret_cast<std::uintptr_t>(ptr) <= reinterpret_cast<std::uintptr_t>(data.get()) + (entry_capacity * bytes_per_entry));
    }

    auto pushBack(Types... value) -> void
    {
        if (entry_count == entry_capacity) {
            throw std::runtime_error("Expansion not implemented");
        }
        pushTuple(beginnings, TypesValues(value...), entry_count++);
    }

    template <size_t i>
    auto field()
    {
        static_assert(i < num_types, "Trying to access inaccessible vector");
        return std::span { std::get<i>(beginnings), entry_count };
    }

    auto operator[](int index) -> TypesRefs
    {
        auto result = getElement(beginnings, index, std::tuple<>());
        return result;
    }

    class Iterator {
    private:
        Sov& sov;
        size_t index = 0;

    public:
        Iterator(Sov& s, size_t i)
            : sov(s)
            , index(i)
        {
        }
        auto operator++() -> Iterator&
        {
            ++index;
            return *this;
        }
        auto operator++(int) -> Iterator
        {
            auto cpy = *this;
            ++cpy;
            return cpy;
        }
        auto operator==(const Iterator& other) const -> bool
        {
            return this->index == other.index;
        }
        auto operator!=(const Iterator& other) const -> bool
        {
            return !(*this == other);
        }
        auto operator*()
        {
            return sov[index];
        }
    };

    auto begin() -> Iterator
    {
        return Iterator(*this, 0);
    }
    auto end() -> Iterator
    {
        return Iterator(*this, entry_count);
    }
};
