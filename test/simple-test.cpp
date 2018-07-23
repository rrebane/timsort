#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <random>
#include <stdexcept>
#include <vector>

#include <timsort/timsort.h>

using Duration = std::chrono::duration<double, std::micro>;

template <class RandomIt>
void print(RandomIt first, RandomIt last) {
    while (first != last) {
        std::cout << *first;
        if (++first != last) {
            std::cout << ", ";
        }
    }

    std::cout << '\n';
}

template <typename T>
void print(std::vector<T> vec) {
    return print(vec.begin(), vec.end());
}

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

template <typename T>
std::pair<Duration, Duration> test_random_vector(std::random_device & rd, std::size_t n) {
    std::vector<T> values1;
    values1.resize(n);
    random_fill(rd, values1);

    auto values2 = values1;

    auto start1 = std::chrono::system_clock::now();
    timsort::sort(values1.begin(), values1.end());
    auto end1 = std::chrono::system_clock::now();

    auto start2 = std::chrono::system_clock::now();
    std::stable_sort(values2.begin(), values2.end());
    auto end2 = std::chrono::system_clock::now();

    for (auto it1 = values1.begin(), it2 = values2.begin();
            it1 != values1.end() && it2 != values2.end();
            ++it1, ++it2)
    {
        if (*it1 != *it2) {
            throw std::runtime_error("Sorting returned invalid result.");
        }
    }

    return {end1 - start1, end2 - start2};
}

int main() {
    try {
        std::random_device rd;

        std::size_t maxMemory = 1073741824; // 1GB
        std::size_t totalIterations = 10u;

        std::cout << "Max memory: " << maxMemory << "B, iterations: " << totalIterations << '\n';

        for (std::size_t size = 1u; size <= maxMemory / sizeof(int); size <<= 1) {
            std::cerr << "Test[size=" << size << '(' << (size * sizeof(int)) << "B),iter=10]:";
            Duration timsortTimeTotal{};
            Duration stdsortTimeTotal{};
            for (std::size_t iter = 0u; iter < totalIterations; ++iter) {
                auto durations = test_random_vector<int>(rd, size);
                timsortTimeTotal += durations.first;
                stdsortTimeTotal += durations.second;
            }
            std::cout << " timsort " << (timsortTimeTotal.count() / totalIterations) << " us/sort" << ','
                      << " stdsort " << (stdsortTimeTotal.count() / totalIterations) << " us/sort" << ','
                      << " diff " << (timsortTimeTotal.count() / totalIterations - stdsortTimeTotal.count() / totalIterations) / (stdsortTimeTotal.count() / totalIterations) << "x"
                      << '\n';
        }
    } catch (std::exception const & e) {
        std::cerr << "std::exception(\"" << e.what() << "\")" << '\n';
        return EXIT_FAILURE;
    } catch (...) {
        std::cerr << "Unknown exception." << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
