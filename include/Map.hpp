#pragma once
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <ranges>
#include <vector>

#include <Sov.hpp>

template <typename Key, typename Value>
class Map {
private:
    Sov<size_t, Key, Value> data;
    int sorted_size;
    std::hash<Key> hasher;

public:
    constexpr Map()
        : data()
        , sorted_size(0)
    {
    }
    constexpr auto size() -> size_t
    {
        return data.size();
    }
    constexpr auto insert(const Key& key, const Value& value) -> void
    {
        data.pushBack(hasher(key), key, value);
    }

private:
    template <typename Compare = std::less<>>
    auto mergeSort(Compare predicate = Compare {})
    {
        auto hashes = data.field<0>();
        auto keys = data.field<1>();
        auto values = data.field<2>();

        for (int i = 0; i < data.size(); i++) {
            for (int j = i; j > 0; j--) {
                const int lhs_index = j - 1;
                const int rhs_index = j;

                if (!predicate(hashes[lhs_index], hashes[rhs_index])) {
                    std::swap(hashes[lhs_index], hashes[rhs_index]);
                    std::swap(keys[lhs_index], keys[rhs_index]);
                    std::swap(values[lhs_index], values[rhs_index]);
                    continue;
                }
                break;
            }
        }
    }

public:
    auto sort() -> void
    {
        mergeSort();
        sorted_size = data.size();
    }

private:
    auto getIndexOf(const Key& key) -> int
    {
        auto hash_to_find = hasher(key);

        const auto all_hashes = data.field<0>();
        const auto all_keys = data.field<1>();

        auto sorted_hashes = all_hashes | std::views::take(sorted_size);
        auto iter = std::ranges::lower_bound(sorted_hashes, hash_to_find);
        auto index = std::distance(sorted_hashes.begin(), iter);
        if (iter != sorted_hashes.end() && all_keys[index] == key) {
            return index;
        }
        sort();

        iter = std::ranges::lower_bound(all_hashes, hash_to_find);
        index = std::distance(all_hashes.begin(), iter);
        if (iter != all_hashes.end() && all_keys[index] == key) {
            return index;
        }
        return data.size();
    }

public:
    auto at(const Key& key) -> Value&
    {
        if(data.size() <= 100)
        {
            return atLinear(key);
        }
        else 
        {
            return atBinary(key);
        }
    }

    auto atLinear(const Key& key) -> Value&
    {
        
        const auto hashes = data.field<0>();
        auto iter = std::ranges::find(hashes, hasher(key));
        if (iter == hashes.end()) {
            throw std::runtime_error(std::format("Map::at(\"{}\") does not contain the key.", key));
        }
        return data.field<2>()[std::distance(hashes.begin(), iter)];
    }

    auto atBinary(const Key& key) -> Value&
    {
        auto index = getIndexOf(key);
        if (index == data.size()) {
            throw std::runtime_error(std::format("Map::at(\"{}\") does not contain the key.", key));
        }
        return data.field<2>()[index];
    }

    /*
    constexpr auto sort() -> void
    {
        //auto keyComparison = [](const auto& lhs, const auto& rhs) {
        //    return std::get<0>(lhs) < std::get<0>(rhs);
        //};

        //auto unsorted_keys = std::ranges::subrange(keys.begin() + sorted_end_index, keys.end());
        //auto unsorted_values = std::ranges::subrange(values.begin() + sorted_end_index, values.end());

        // this method requires random access iterators on gcc
        // std::ranges::sort(std::views::zip(unsorted_keys, unsorted_values), keyComparison);
        // auto ukv = std::views::zip(unsorted_keys, unsorted_values);
        // std::stable_sort(ukv.begin(), ukv.end(), keyComparison);

        //auto keys_values = std::views::zip(keys, values);

        //std::inplace_merge(
        //    keys_values.begin(),
        //    keys_values.begin() + sorted_end_index,
        //    keys_values.end(),
        //    keyComparison);

        //sorted_end_index = std::ranges::size(keys_values);
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

    */

public:
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
            auto [h, k, v] = map.data[index];
            return { k, v };
        }
    };

    auto begin() -> Iterator
    {
        return Iterator { *this, 0 };
    }
    auto end() -> Iterator
    {
        return Iterator { *this, data.size() };
    }
};

template class Map<std::string, int>;