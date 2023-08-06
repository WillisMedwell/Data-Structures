#include <benchmark/benchmark.h>

// static void BM_StringCreation(benchmark::State& state) {
//   for (auto _ : state)
//   {
//     std::string empty_string;
//     benchmark::DoNotOptimize(empty_string);
//   }
// }
// BENCHMARK(BM_StringCreation);

// static void BM_StringCopy(benchmark::State& state) {
//   std::string x = "hello";
//   for (auto _ : state)
//   {
//     std::string copy(x);
//     benchmark::DoNotOptimize(copy);
//   }
// }
// BENCHMARK(BM_StringCopy);

int main(int argc, char** argv) {
  ::benchmark::Initialize(&argc, argv);
  ::benchmark::RunSpecifiedBenchmarks();
}