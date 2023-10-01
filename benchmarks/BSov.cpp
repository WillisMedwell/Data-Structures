#include <benchmark/benchmark.h>

#include "Sov.hpp"

#include <vector>

static void BM_Vec4IndexLoop(benchmark::State& state)
{
    std::vector<int> v1;
    std::vector<int> v2;
    std::vector<int> v3;
    std::vector<int> v4;

    for (int i = 0; i < 10000; i++) {
        v1.emplace_back(10);
        v2.emplace_back(20);
        v3.emplace_back(30);
        v4.emplace_back(40);
    }
    for (auto _ : state) {
        size_t sum = 0;
        for (int i = 0; i < v1.size(); i++) {
            sum += v1[i] + v2[i] + v3[i] + v4[i];
        }
        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(v3);
        benchmark::DoNotOptimize(v4);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vec4IndexLoop);

static void BM_Sov4RangeLoop(benchmark::State& state)
{
    Sov<int, int, int, int> sov;
    for (int i = 0; i < 10000; i++) {
        sov.pushBack(10, 20, 30, 40);
    }
    for (auto _ : state) {
        size_t sum = 0;

        for (auto [a, b, c, d] : sov) {
            sum += a + b + c + d;
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Sov4RangeLoop);
static void BM_Sov4IndexLoop(benchmark::State& state)
{
    Sov<int, int, int, int> sov;
    for (int i = 0; i < 10000; i++) {
        sov.pushBack(10, 20, 30, 40);
    }
    for (auto _ : state) {
        size_t sum = 0;
        for (int i = 0; i < sov.size(); i++) {
            auto [a, b, c, d] = sov[i];
            sum += a + b + c + d;
        }
        benchmark::DoNotOptimize(sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Sov4IndexLoop);

static void BM_VecPush1Int(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> vec;
        for (int i = 0; i < 1000; i++) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_VecPush1Int);

static void BM_SovPush1Int(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int> sov;
        for (int i = 0; i < 1000; i++) {
            sov.pushBack(i);
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPush1Int);

static void BM_VecPush5Int(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> v1;
        std::vector<int> v2;
        std::vector<int> v3;
        std::vector<int> v4;
        std::vector<int> v5;

        for (int i = 0; i < 1000; i++) {
            v1.push_back(i);
            v1.push_back(i + 1);
            v1.push_back(i + 2);
            v1.push_back(i + 3);
            v1.push_back(i + 4);
        }
        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(v3);
        benchmark::DoNotOptimize(v4);
        benchmark::DoNotOptimize(v5);
    }
}
BENCHMARK(BM_VecPush5Int);

static void BM_SovPush5Int(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int, int, int, int, int> sov;
        for (int i = 0; i < 1000; i++) {
            sov.pushBack(i, i + 1, i + 2, i + 3, i + 4);
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPush5Int);

static void BM_VecPush1String(benchmark::State& state)
{
    std::string a { "hello world" };

    for (auto _ : state) {
        std::vector<std::string> vec;
        for (int i = 0; i < 100; i++) {
            vec.push_back(a);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_VecPush1String);

static void BM_SovPush1String(benchmark::State& state)
{
    std::string a { "hello world" };
    for (auto _ : state) {
        Sov<std::string> sov;
        for (int i = 0; i < 100; i++) {
            sov.pushBack("hello world");
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPush1String);