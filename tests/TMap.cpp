#include <Map.hpp>
#include <gtest/gtest.h>

namespace TMap {
thread_local size_t element_constructions = 0;
thread_local size_t element_destructions = 0;

struct Element
 {
    int64_t dummy;
    bool has_been_destroyed = false;
    Element(int arg = 0)
    {
        dummy = arg;
        ++element_constructions;
    }
    Element(const Element& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    Element(Element&& other)
    {
        dummy = other.dummy;
        ++element_constructions;
    }
    ~Element()
    {
        if (has_been_destroyed) {
            throw std::runtime_error("destructor called twice");
        }
        has_been_destroyed = true;
        ++element_destructions;
    }
};
}

TEST(MapTest, Construction)
{
    Map<std::string, int> map;
    EXPECT_EQ(map.size(), 0);
}

TEST(MapTest, push)
{
    Map<std::string, int> map;
    EXPECT_EQ(map.size(), 0);
    map.insert("hi0", 0);
    map.insert("hi1", 1);
    map.insert("hi2", 2);
    map.insert("hi3", 3);
    EXPECT_EQ(map.size(), 4);

    int i = 0;
    for(auto [k, v] : map)
    {
        std::string expected = "hi" + std::to_string(i);
        EXPECT_EQ(expected, k);
        EXPECT_EQ(v, i);
        ++i;
    }
}

TEST(MapTest, sort)
{
    Map<std::string, int> map;
    EXPECT_EQ(map.size(), 0);
    map.insert("hi3", 4);
    map.insert("hi0", 0);
    map.insert("hi2", 2);
    map.insert("hi1", 1);
    EXPECT_EQ(map.size(), 4);

    std::hash<std::string> hasher;
    std::vector<size_t> hashes;
    for(auto [k, v] : map)
    {
        hashes.push_back(hasher(k));    
    }
    
    std::ranges::sort(hashes);
    map.sort();

    int i = 0;
    for(const auto [k, v] : map)
    {
        EXPECT_EQ(hashes[i++], hasher(k));
    }
}
TEST(MapTest, at)
{
    Map<std::string, int> map;
    map.insert("hi3", 3);
    map.insert("hi0", 0);
    map.insert("hi2", 2);
    map.insert("hi1", 1);

    EXPECT_EQ(map.at("hi0"), 0);
    EXPECT_EQ(map.at("hi1"), 1);
    EXPECT_EQ(map.at("hi2"), 2);
    EXPECT_EQ(map.at("hi3"), 3);
    EXPECT_THROW(map.at("hi4"), std::runtime_error);
}
