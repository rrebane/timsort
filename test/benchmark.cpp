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

static void timsort_bench_asc(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    for (std::size_t i = 0u; i < size; ++i) {
        values.emplace_back(static_cast<int>(i));
    }

    for (auto _ : state) {
        timsort::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(timsort_bench_asc)->Range(8, 8 << 19);

static void timsort_bench_desc(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        assert(values.size() == size);
        for (std::size_t i = 0u; i < size; ++i) {
            values[i] = static_cast<int>(size - i - 1u);
        }
        state.ResumeTiming();
        timsort::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(timsort_bench_desc)->Range(8, 8 << 19);

static void timsort_bench_eq(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);
    std::fill(values.begin(), values.end(), 1);

    for (auto _ : state) {
        timsort::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(timsort_bench_eq)->Range(8, 8 << 19);

static void timsort_bench_random(benchmark::State & state) {
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

BENCHMARK(timsort_bench_random)->Range(8, 8 << 19);

static void stdsort_bench_asc(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    for (std::size_t i = 0u; i < size; ++i) {
        values.emplace_back(static_cast<int>(i));
    }

    for (auto _ : state) {
        std::stable_sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(stdsort_bench_asc)->Range(8, 8 << 19);

static void stdsort_bench_desc(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        assert(values.size() == size);
        for (std::size_t i = 0u; i < size; ++i) {
            values[i] = static_cast<int>(size - i - 1u);
        }
        state.ResumeTiming();
        std::stable_sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(stdsort_bench_desc)->Range(8, 8 << 19);

static void stdsort_bench_eq(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);
    std::fill(values.begin(), values.end(), 1);

    for (auto _ : state) {
        std::stable_sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(stdsort_bench_eq)->Range(8, 8 << 19);

static void stdsort_bench_random(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(int);

    std::random_device rd;
    std::vector<int> values;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        random_fill(rd, values);
        state.ResumeTiming();
        std::stable_sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK(stdsort_bench_random)->Range(8, 8 << 19);

BENCHMARK_MAIN();
