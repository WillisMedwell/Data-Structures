#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>
#include <array>

template <typename Key, typename Value>
class Map {
private:
    std::vector<Key> keys;
    std::vector<Value> values;

    size_t sorted_end_index;

public:
    constexpr Map()
        : keys({})
        , values({})
        , sorted_end_index(0)
    {
        keys.reserve(20);
        values.reserve(20);
    }

private:
    constexpr auto sort() -> void
    {
        auto keyComparison = [](const auto& lhs, const auto& rhs) {
            return std::get<0>(lhs) < std::get<0>(rhs);
        };

        auto unsorted_keys = std::ranges::subrange(keys.begin() + sorted_end_index, keys.end());
        auto unsorted_values = std::ranges::subrange(values.begin() + sorted_end_index, values.end());

        std::ranges::sort(std::views::zip(unsorted_keys, unsorted_values), keyComparison);

        auto keys_values = std::views::zip(keys, values);

        std::inplace_merge(
            keys_values.begin(),
            keys_values.begin() + sorted_end_index,
            keys_values.end(),
            keyComparison);

        sorted_end_index = std::ranges::size(keys_values);
    }
    constexpr auto search(const Key& key)
    {
        const auto iter = std::lower_bound(keys.cbegin(), keys.cbegin() + sorted_end_index, key);
        if (*iter == key) {
            return iter;
        } else {
            sort();
            return std::lower_bound(keys.cbegin(), keys.cend(), key);
        }
    }

public:
    constexpr auto insert(const Key& key, const Value& value)
    {
        keys.emplace_back(key);
        values.emplace_back(value);
    }
    constexpr auto operator[](const Key& key) noexcept -> Value&
    {
        return values[std::distance(keys.cbegin(), search(key))];
    }
    constexpr auto contains(const Key& key) noexcept -> bool
    {
        return *search(key) == key;
    }
    constexpr auto at(const Key& key) -> Value&
    {
        const auto iter = search(key);
        if (*iter != key) {
            std::runtime_error("Map.hpp: error the key is not within the map");
        }
        return values.at(std::distance(keys.cbegin(), iter));
    }

    class Iterator {
        Map& map;
        size_t index;

    public:
        Iterator(Map& m, size_t i)
            : map(m)
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
        auto operator==(const Iterator& other) -> bool
        {
            return index == other.index;
        }
        auto operator!=(const Iterator& other) -> bool
        {
            return !(*this == other);
        }
        auto operator*() -> std::pair<Key&, Value&>
        {
            return { map.keys[index], map.values[index] };
        }
    };

    auto begin() -> Iterator
    {
        return Iterator { *this, 0 };
    }
    auto end() -> Iterator
    {
        return Iterator { *this, keys.size() };
    }
};
