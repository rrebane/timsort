#include <benchmark/benchmark.h>
#include <random>
#include <vector>

#include <timsort/timsort.h>

namespace {

std::random_device gRandom;

} // namespace

class AscGen {
public:
    template <typename Container, typename T = typename Container::value_type>
    void operator()(Container & c) {
        T value = 0;
        for (auto & val : c) {
            val = value++;
        }
    }
};

class DescGen {
public:
    template <typename Container, typename T = typename Container::value_type>
    void operator()(Container & c) {
        T value = c.size();
        for (auto & val : c) {
            val = --value;
        }
    }
};

class EqGen {
public:
    template <typename Container, typename T = typename Container::value_type>
    void operator()(Container & c) {
        for (auto & val : c) {
            val = 0;
        }
    }
};

class RandomGen {
public:
    template <typename Container, typename T = typename Container::value_type,
              typename = typename std::enable_if<std::is_integral<T>::value>::type>
    void operator()(Container & c) {
        std::uniform_int_distribution<T> distributon;
        for (auto & val : c) {
            val = distributon(m_engine);
        }
    }

private:
    std::default_random_engine m_engine = std::default_random_engine{gRandom()};
};

template <class Container, class Generator>
void stdsort_bench(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(typename Container::value_type);

    Container values;
    Generator generator;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        generator(values);
        state.ResumeTiming();
        std::stable_sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

template <class Container, class Generator>
void timsort_bench(benchmark::State & state) {
    std::size_t bytes = state.range(0);
    std::size_t size = bytes / sizeof(typename Container::value_type);

    Container values;
    Generator generator;
    values.resize(size);

    for (auto _ : state) {
        state.PauseTiming();
        generator(values);
        state.ResumeTiming();
        timsort::sort(values.begin(), values.end());
    }

    state.SetBytesProcessed(state.iterations() * bytes);
}

BENCHMARK_TEMPLATE(timsort_bench, std::vector<int>, RandomGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(stdsort_bench, std::vector<int>, RandomGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(timsort_bench, std::vector<int>, AscGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(stdsort_bench, std::vector<int>, AscGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(timsort_bench, std::vector<int>, DescGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(stdsort_bench, std::vector<int>, DescGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(timsort_bench, std::vector<int>, EqGen)->Range(8, 8 << 19);
BENCHMARK_TEMPLATE(stdsort_bench, std::vector<int>, EqGen)->Range(8, 8 << 19);

BENCHMARK_MAIN();
