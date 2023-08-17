#include <benchmark/benchmark.h>

#include <Map.hpp>
#include <unordered_map>

static void BM_StdMapInsert(benchmark::State& state)
{
    for (auto _ : state) {
        std::unordered_map<std::string, int> map;
        map.emplace("hello world0", 0);
        map.emplace("hello world1", 1);
        map.emplace("hello world2", 2);
        map.emplace("hello world3", 3);
        map.emplace("hello world4", 4);
        map.emplace("hello world5", 5);
        map.emplace("hello world6", 6);
        map.emplace("hello world7", 7);
        map.emplace("hello world8", 8);
        map.emplace("hello world9", 9);

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(BM_StdMapInsert);

static void BM_MapInsert(benchmark::State& state)
{
    for (auto _ : state) {
        Map<std::string, int> map;
        map.insert("hello world0", 0);
        map.insert("hello world1", 1);
        map.insert("hello world2", 2);
        map.insert("hello world3", 3);
        map.insert("hello world4", 4);
        map.insert("hello world5", 5);
        map.insert("hello world6", 6);
        map.insert("hello world7", 7);
        map.insert("hello world8", 8);
        map.insert("hello world9", 9);

        benchmark::DoNotOptimize(map);
    }
}
BENCHMARK(BM_MapInsert);

static void BM_StdMapAt(benchmark::State& state)
{
    std::unordered_map<std::string, int> map;

    for (int i = 0; i < 100; i++) {
        map.emplace(std::format("hello world{}", i), i);
    }

    for (auto _ : state) {
        auto hw1 = map.at("hello world0");
        auto hw2 = map.at("hello world25");
        auto hw3 = map.at("hello world50");

        benchmark::DoNotOptimize(map);
        benchmark::DoNotOptimize(hw1);
        benchmark::DoNotOptimize(hw2);
        benchmark::DoNotOptimize(hw3);
    }
}
BENCHMARK(BM_StdMapAt);

static void BM_MapAt(benchmark::State& state)
{
    Map<std::string, int> map;

    for (int i = 0; i < 100; i++) {
        map.insert(std::format("hello world{}", i), i);
    }
    for (auto _ : state) {
        auto hw1 = map.at("hello world0");
        auto hw2 = map.at("hello world25");
        auto hw3 = map.at("hello world50");

        benchmark::DoNotOptimize(map);
        benchmark::DoNotOptimize(hw1);
        benchmark::DoNotOptimize(hw2);
        benchmark::DoNotOptimize(hw3);
    }
}
BENCHMARK(BM_MapAt);
