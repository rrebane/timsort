#include <algorithm>
#include <benchmark/benchmark.h>
#include <random>
#include <vector>

#include <timsort/timsort.h>

template <typename Container,
          typename T = typename Container::value_type,
          typename = typename std::enable_if<std::is_integral<T>::value>::type>
void random_fill(std::random_device & rd, Container & values) {
    std::default_random_engine re(rd());
    std::uniform_int_distribution<T> dist;

    for (auto & val : values) {
        val = dist(re);
    }
}

static void stdsort_bench(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        random_fill(rd, values);
        state.ResumeTiming();
        std::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(stdsort_bench)->Range(8, 8 << 19);

static void timsort_bench(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        random_fill(rd, values);
        state.ResumeTiming();
        timsort::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(timsort_bench)->Range(8, 8 << 19);

BENCHMARK_MAIN();
