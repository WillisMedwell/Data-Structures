#include <benchmark/benchmark.h>

#include "Sov.hpp"

#include <vector>

// static void BM_Vec1Insert(benchmark::State& state)
//{
//     for (auto _ : state) {
//         std::vector<int> empty_vector;
//         for (int i = 0; i < state.range(0); i++) {
//             empty_vector.emplace_back(10);
//         }
//         benchmark::DoNotOptimize(empty_vector);
//     }
// }
// BENCHMARK(BM_Vec1Insert)->Range(0, 1024);
// static void BM_Sov1Insert(benchmark::State& state)
//{
//     for (auto _ : state) {
//         Sov<int> empty_sov;
//         for (int i = 0; i < state.range(0); i++) {
//             empty_sov.emplaceBack(10);
//         }
//         benchmark::DoNotOptimize(empty_sov);
//     }
// }
// BENCHMARK(BM_Sov1Insert)->Range(0, 1024);
//
// static void BM_Vec4Insert(benchmark::State& state)
//{
//     for (auto _ : state) {
//         std::vector<int> empty_vector1;
//         std::vector<int> empty_vector2;
//         std::vector<int> empty_vector3;
//         std::vector<int> empty_vector4;
//
//         for (int i = 0; i < state.range(0); i++) {
//             empty_vector1.emplace_back(10);
//             empty_vector2.emplace_back(10);
//             empty_vector3.emplace_back(10);
//             empty_vector4.emplace_back(10);
//         }
//         benchmark::DoNotOptimize(empty_vector1);
//         benchmark::DoNotOptimize(empty_vector2);
//         benchmark::DoNotOptimize(empty_vector3);
//         benchmark::DoNotOptimize(empty_vector4);
//     }
// }
// BENCHMARK(BM_Vec4Insert)->Range(0, 1024);
// static void BM_Sov4Insert(benchmark::State& state)
//{
//     for (auto _ : state) {
//         Sov<int, int, int, int> empty_sov;
//         for (int i = 0; i < state.range(0); i++) {
//             empty_sov.emplaceBack(10, 10, 10, 10);
//         }
//         benchmark::DoNotOptimize(empty_sov);
//     }
// }
// BENCHMARK(BM_Sov4Insert)->Range(0, 1024);
//
// static void BM_Vec1RangeLoop(benchmark::State& state)
//{
//     std::vector<int> empty_vec;
//     for (int i = 0; i < 1000; i++) {
//         empty_vec.emplace_back(10);
//     }
//
//     for (auto _ : state) {
//         size_t sum = 0;
//         for (auto& e : empty_vec) {
//             sum += e;
//         }
//         benchmark::DoNotOptimize(empty_vec);
//         benchmark::DoNotOptimize(sum);
//     }
// }
// BENCHMARK(BM_Vec1RangeLoop);
// static void BM_Sov1RangeLoop(benchmark::State& state)
//{
//     Sov<int> empty_sov;
//     for (int i = 0; i < 1000; i++) {
//         empty_sov.emplaceBack(10);
//     }
//
//     for (auto _ : state) {
//         size_t sum = 0;
//         for (auto [e] : empty_sov) {
//             sum += e;
//         }
//         benchmark::DoNotOptimize(empty_sov);
//         benchmark::DoNotOptimize(sum);
//     }
// }
// BENCHMARK(BM_Sov1RangeLoop);
//
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

// static void BM_VecEmplace(benchmark::State& state)
//{
//     std::vector<std::string> vec;
//     for (auto _ : state) {
//         for (int i = 0; i < 1000; i++) {
//             vec.emplace_back("Hello world!");
//         }
//         benchmark::DoNotOptimize(vec);
//     }
// }
// BENCHMARK(BM_VecEmplace);


static void BM_VecPush1(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> vec;
        for (int i = 0; i < 1000; i++) {
            vec.push_back(i);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_VecPush1);

static void BM_SovPush1(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int> sov;
        for (int i = 0; i < 1000; i++) {
            sov.pushBack(i);
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPush1);

static void BM_VecPush5(benchmark::State& state)
{
    for (auto _ : state) {
        std::vector<int> v1;
        std::vector<int> v2;
        std::vector<int> v3;
        std::vector<int> v4;
        std::vector<int> v5;

        for (int i = 0; i < 1000; i++) {
            v1.push_back(i);
            v1.push_back(i+1);
            v1.push_back(i+2);
            v1.push_back(i+3);
            v1.push_back(i+4);

        }
        benchmark::DoNotOptimize(v1);
        benchmark::DoNotOptimize(v2);
        benchmark::DoNotOptimize(v3);
        benchmark::DoNotOptimize(v4);
        benchmark::DoNotOptimize(v5);
    }
}
BENCHMARK(BM_VecPush5);

static void BM_SovPush5(benchmark::State& state)
{
    for (auto _ : state) {
        Sov<int, int, int, int ,int> sov;
        for (int i = 0; i < 1000; i++) {
            sov.pushBack(i, i+1, i+2, i+3, i+4);
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPush5);


static void BM_VecPushString(benchmark::State& state)
{
    std::string a { "hello world" };

    for (auto _ : state) {
        std::vector<std::string> vec;
        for (int i = 0; i < 1000; i++) {
            vec.push_back(a);
        }
        benchmark::DoNotOptimize(vec);
    }
}
BENCHMARK(BM_VecPushString);

static void BM_SovPushString(benchmark::State& state)
{
    std::string a { "hello world" };
    for (auto _ : state) {
        Sov<std::string> sov;
        for (int i = 0; i < 1000; i++) {
            sov.pushBack(a);
        }
        benchmark::DoNotOptimize(sov);
    }
}
BENCHMARK(BM_SovPushString);