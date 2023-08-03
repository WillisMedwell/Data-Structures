#pragma once
// Structure of vectors implementation

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iterator>
#include <memory>
#include <numeric>
#include <span>
#include <stdexcept>
#include <tuple>

template <typename... Types>
class Sov {
public:
    using FieldsRef = typename std::tuple<Types&...>;
    using FieldsValue = typename std::tuple<Types...>;
    using FieldsPtr = typename std::tuple<Types*...>;
    constexpr static size_t num_types = std::tuple_size<FieldsValue>();

    constexpr static size_t bytes_per_entry = sizeof(FieldsValue);

    size_t entry_capacity = 0;
    size_t entry_count = 0;
    std::unique_ptr<uint8_t[]> data;
    FieldsPtr beginnings;

private: // tuple iteration helpers
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

    template <size_t index = 0>
    constexpr static void pushTuple(FieldsPtr& destination, const FieldsValue& source, const size_t i)
    {
        if constexpr (index == num_types) {
            return;
        } else {
            using ElementType = std::remove_reference_t<decltype(std::get<index>(source))>;
            new (std::get<index>(destination) + i)(ElementType)(std::get<index>(source));
            return pushTuple<index + 1>(destination, source, i);
        }
    }

    template <size_t index = 0, typename T>
    constexpr static auto getElement(FieldsPtr& source, const size_t i, T zipped_element)
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

    template <size_t index = 0>
    constexpr static void copyFields(FieldsPtr& destination, FieldsPtr& source, size_t count)
    {
        if constexpr (index == num_types) {
            return;
        } else {
            auto src = std::span { std::get<index>(source), count };
            auto dest = std::span { std::get<index>(destination), count };
            std::ranges::copy(src, dest.begin());
            return copyFields<index + 1>(destination, source, count);
        }
    }

public:
    Sov(size_t init_capacity = 20)
        : entry_capacity(0)
        , entry_count(0)
        , data(nullptr)
        , beginnings({})
    {
        grow(init_capacity);
    }
    Sov(const Sov& other)
        : entry_capacity(0)
        , entry_count(0)
        , data(nullptr)
        , beginnings({})
    {
        grow(other.entry_capacity);
        copyFields(beginnings, other.beginnings, other.entry_count);
    }
    Sov(Sov&& other)
        : entry_capacity(0)
        , entry_count(0)
        , data(nullptr)
        , beginnings({})
    {
        entry_capacity = other.entry_capacity;
        entry_count = other.entry_count;
        data = std::move(other.data);
        beginnings = other.beginnings;
    }

    auto pushBack(Types... value) -> void
    {
        if (entry_count == entry_capacity) {
            grow(entry_capacity * 2);
        }
        pushTuple(beginnings, FieldsValue(value...), entry_count++);
    }

    auto popBack() -> void
    {
        entry_count = std::min(--entry_count, size_t(0));
    }

    auto grow(size_t new_entry_capacity)
    {
        assert(new_entry_capacity > entry_capacity);
        auto new_data = std::make_unique<uint8_t[]>(bytes_per_entry * new_entry_capacity + 64);
        auto ptr = new_data.get();
        FieldsPtr new_beginnings;

        auto assign_beginning = [&ptr, &new_entry_capacity](auto& begin) {
            using Ptr = decltype(begin);
            using Value = decltype(*begin);
            // ensure proper memory alignment
            while (reinterpret_cast<std::uintptr_t>(ptr) % alignof(Value)) {
                ++ptr;
            }
            begin = reinterpret_cast<Ptr>(ptr);
            ptr += sizeof(Value) * new_entry_capacity;
        };
        forEachTuple(new_beginnings, assign_beginning);

        bool needs_copy = data.get() != nullptr;
        copyFields(new_beginnings, beginnings, entry_count);

        data = std::move(new_data);
        entry_capacity = new_entry_capacity;
        beginnings = new_beginnings;
        assert(reinterpret_cast<std::uintptr_t>(ptr) <= reinterpret_cast<std::uintptr_t>(data.get()) + (entry_capacity * bytes_per_entry));
    }

    template <size_t i>
    auto field()
    {
        static_assert(i < num_types, "Trying to access inaccessible vector");
        return std::span { std::get<i>(beginnings), entry_count };
    }

    auto operator[](int index) -> FieldsRef
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