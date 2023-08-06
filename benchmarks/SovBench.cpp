#include <benchmark/benchmark.h>

#include "Sov.hpp"

#include <vector>

static void BM_Vec1Insert10(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> empty_vector;
        for(int i = 0; i < 10; i++)
        {
            empty_vector.emplace_back(10);
        }
        benchmark::DoNotOptimize(empty_vector);
    }
}
BENCHMARK(BM_Vec1Insert10);
static void BM_Vec1Insert100(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> empty_vector;
        for(int i = 0; i < 100; i++)
        {
            empty_vector.emplace_back(10);
        }
        benchmark::DoNotOptimize(empty_vector);
    }
}
BENCHMARK(BM_Vec1Insert100);
static void BM_Vec1Insert1000(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> empty_vector;
        for(int i = 0; i < 1000; i++)
        {
            empty_vector.emplace_back(10);
        }
        benchmark::DoNotOptimize(empty_vector);
    }
}
BENCHMARK(BM_Vec1Insert1000);

static void BM_Sov1Insert10(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int> empty_sov;
        for(int i = 0; i < 10; i++)
        {
            empty_sov.emplaceBack(10);
        }
        benchmark::DoNotOptimize(empty_sov);
    }
}
BENCHMARK(BM_Sov1Insert10);
static void BM_Sov1Insert100(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int> empty_sov;
        for(int i = 0; i < 100; i++)
        {
            empty_sov.emplaceBack(10);
        }
        benchmark::DoNotOptimize(empty_sov);
    }
}
BENCHMARK(BM_Sov1Insert100);
static void BM_Sov1Insert1000(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int> empty_sov;
        for(int i = 0; i < 1000; i++)
        {
            empty_sov.emplaceBack(10);
        }
        benchmark::DoNotOptimize(empty_sov);
    }
}
BENCHMARK(BM_Sov1Insert1000);

static void BM_Vec4Insert10(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> empty_vector1;
        std::vector<int> empty_vector2;
        std::vector<int> empty_vector3;
        std::vector<int> empty_vector4;

        for(int i = 0; i < 10; i++)
        {
            empty_vector1.emplace_back(10);
            empty_vector2.emplace_back(10);
            empty_vector3.emplace_back(10);
            empty_vector4.emplace_back(10);
        }
        benchmark::DoNotOptimize(empty_vector1);
        benchmark::DoNotOptimize(empty_vector2);
        benchmark::DoNotOptimize(empty_vector3);
        benchmark::DoNotOptimize(empty_vector4);
    }
}
BENCHMARK(BM_Vec4Insert10);
static void BM_Vec4Insert1000(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> empty_vector1;
        std::vector<int> empty_vector2;
        std::vector<int> empty_vector3;
        std::vector<int> empty_vector4;

        for(int i = 0; i < 1000; i++)
        {
            empty_vector1.emplace_back(10);
            empty_vector2.emplace_back(10);
            empty_vector3.emplace_back(10);
            empty_vector4.emplace_back(10);
        }
        benchmark::DoNotOptimize(empty_vector1);
        benchmark::DoNotOptimize(empty_vector2);
        benchmark::DoNotOptimize(empty_vector3);
        benchmark::DoNotOptimize(empty_vector4);
    }
}
BENCHMARK(BM_Vec4Insert1000);

static void BM_Sov4Insert10(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int, int, int, int> empty_sov;
        for(int i = 0; i < 10; i++)
        {
            empty_sov.emplaceBack(10, 10, 10, 10);
        }
        benchmark::DoNotOptimize(empty_sov);
    }
}
BENCHMARK(BM_Sov4Insert10);
static void BM_Sov4Insert1000(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int, int, int, int> empty_sov;
        for(int i = 0; i < 1000; i++)
        {
            empty_sov.emplaceBack(10, 10, 10, 10);
        }
        benchmark::DoNotOptimize(empty_sov);
    }
}
BENCHMARK(BM_Sov4Insert1000);

static void BM_Vec1RangeBasedLoop(benchmark::State& state)
{
    std::vector<int> empty_vec;
    for(int i = 0; i < 1000; i++)
    {
        empty_vec.emplace_back(10);
    }

    for (auto _ : state) {
        size_t sum = 0;
        for(auto& e : empty_vec)
        {
            sum += e;
        }
        benchmark::DoNotOptimize(empty_vec);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Vec1RangeBasedLoop);


static void BM_Sov1RangeBasedLoop(benchmark::State& state)
{
    Sov<int> empty_sov;
    for(int i = 0; i < 1000; i++)
    {
        empty_sov.emplaceBack(10);
    }

    for (auto _ : state) {
        size_t sum = 0;
        for(auto& e : empty_sov.field<0>())
        {
            sum += e;
        }
        benchmark::DoNotOptimize(empty_sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Sov1RangeBasedLoop);

static void BM_Sov4RangeBasedLoop(benchmark::State& state)
{
    Sov<int, int, int, int> empty_sov;
    for(int i = 0; i < 1000; i++)
    {
        empty_sov.emplaceBack(10, 10, 10, 10);
    }

    for (auto _ : state) {
        size_t sum = 0;
        for(auto& e : empty_sov.field<2>())
        {
            sum += e;
        }
        benchmark::DoNotOptimize(empty_sov);
        benchmark::DoNotOptimize(sum);
    }
}
BENCHMARK(BM_Sov4RangeBasedLoop);