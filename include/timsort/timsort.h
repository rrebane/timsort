#pragma once

#include <algorithm>
#include <cassert>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <vector>

// https://bugs.python.org/file4451/timsort.txt

namespace timsort {

namespace detail {

template <class LoInputIt, class HiInputIt, class OutputIt, class Compare>
void merge_lo(LoInputIt loFirst, LoInputIt loLast,
        HiInputIt hiFirst, HiInputIt hiLast,
        OutputIt destFirst, OutputIt destLast,
        Compare comp)
{
    (void) destLast;
    assert(std::distance(loFirst, loLast) + std::distance(hiFirst, hiLast)
            == std::distance(destFirst, destLast));

    auto loCurrent = loFirst;
    auto hiCurrent = hiFirst;
    auto destCurrent = destFirst;

    while (true) {
        if (comp(*loCurrent, *hiCurrent)) {
            if (loCurrent == loLast) {
                break;
            }
            *(destCurrent++) = *(loCurrent++);
        } else {
            if (hiCurrent == hiLast) {
                break;
            }
            *(destCurrent++) = *(hiCurrent++);
        }
    }

    // Copy the remaining elements.
    while (loCurrent != loLast) {
        *(destCurrent++) = *(loCurrent++);
    }

    while (hiCurrent != hiLast) {
        *(destCurrent++) = *(hiCurrent++);
    }

    assert(loCurrent == loLast);
    assert(hiCurrent == hiLast);
    assert(destCurrent == destLast);
}

template <class LoInputIt, class HiInputIt, class OutputIt, class Compare>
void merge_hi(LoInputIt loFirst, LoInputIt loLast,
        HiInputIt hiFirst, HiInputIt hiLast,
        OutputIt destFirst, OutputIt destLast,
        Compare comp)
{
    using ValueT = typename std::iterator_traits<OutputIt>::value_type;

    return merge_lo(
            std::make_reverse_iterator(hiLast), std::make_reverse_iterator(hiFirst),
            std::make_reverse_iterator(loLast), std::make_reverse_iterator(loFirst),
            std::make_reverse_iterator(destLast), std::make_reverse_iterator(destFirst),
            [&comp](ValueT const & lhs, ValueT const & rhs) {
                return !comp(lhs, rhs);
            });
}

template <class LoInputIt, class HiInputIt, class BufferIt, class Compare>
void merge_lo_with_buffer(LoInputIt loFirst, LoInputIt loLast,
        HiInputIt hiFirst, HiInputIt hiLast,
        BufferIt bufferFirst, BufferIt bufferLast,
        Compare comp)
{
    assert(loFirst < loLast);
    assert(bufferFirst < bufferLast);
    assert(std::distance(loFirst, loLast) == std::distance(bufferFirst, bufferLast));

    std::copy(loFirst, loLast, bufferFirst);
    try {
        merge_lo(bufferFirst, bufferLast, hiFirst, hiLast, loFirst, hiLast, comp);
    } catch (...) {
        // Restore the initial state.
        std::copy(bufferFirst, bufferLast, loFirst);
        throw;
    }
}

template <class LoInputIt, class HiInputIt, class BufferIt, class Compare>
void merge_hi_with_buffer(LoInputIt loFirst, LoInputIt loLast,
        HiInputIt hiFirst, HiInputIt hiLast,
        BufferIt bufferFirst, BufferIt bufferLast,
        Compare comp)
{
    assert(hiFirst < hiLast);
    assert(bufferFirst < bufferLast);
    assert(std::distance(hiFirst, hiLast) == std::distance(bufferFirst, bufferLast));

    std::copy(hiFirst, hiLast, bufferFirst);
    try {
        merge_hi(loFirst, loLast, bufferFirst, bufferLast, loFirst, hiLast, comp);
    } catch (...) {
        // Restore the initial state.
        std::copy(bufferFirst, bufferLast, hiFirst);
        throw;
    }
}

template <class RandomIt, class Compare, typename ValueT = typename std::iterator_traits<RandomIt>::value_type>
void merge_sort(RandomIt aFirst, RandomIt aLast, RandomIt bFirst, RandomIt bLast, Compare comp, std::vector<ValueT> & buffer) {
    assert(aFirst < aLast);
    assert(bFirst < bLast);
    assert(aLast == bFirst);

    // Skip elements that are already in their final place, i.e. values from A
    // that are smaller than B_first and values from B that are larger than
    // A_last - 1.
    aFirst = std::lower_bound(aFirst, aLast, *bFirst);
    bLast = std::upper_bound(bFirst, bLast, *(aLast - 1));

    if (aFirst == aLast || bFirst == bLast) {
        return;
    }

    static_assert(
            std::numeric_limits<decltype(std::distance(aFirst, aLast))>::max() <=
            std::numeric_limits<std::size_t>::max(),
            "Size type must be at least as large as the distance type");

    auto const aSize = std::distance(aFirst, aLast);
    auto const bSize = std::distance(bFirst, bLast);

    // Copy the contents from the smaller of A and B into temporary memory and
    // then merge.
    if (aSize < bSize) {
        if (static_cast<std::size_t>(aSize) <= buffer.size()) {
            merge_lo_with_buffer(aFirst, aLast, bFirst, bLast, buffer.begin(), buffer.begin() + aSize, comp);
        } else {
            std::vector<ValueT> temp;
            temp.resize(aSize);
            merge_lo_with_buffer(aFirst, aLast, bFirst, bLast, temp.begin(), temp.end(), comp);
        }
    } else {
        if (static_cast<std::size_t>(bSize) <= buffer.size()) {
            merge_hi_with_buffer(aFirst, aLast, bFirst, bLast, buffer.begin(), buffer.begin() + bSize, comp);
        } else {
            std::vector<ValueT> temp;
            temp.resize(bSize);
            merge_hi_with_buffer(aFirst, aLast, bFirst, bLast, temp.begin(), temp.end(), comp);
        }
    }
}

template <class RandomIt, class Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp) {
    auto const rEnd = std::make_reverse_iterator(first);
    for (auto current = first + 1; current != last; ++current) {
        auto rCurrent = std::make_reverse_iterator(current + 1);
        auto rNext = rCurrent + 1;

        while (rNext != rEnd) {
            if (!comp(*rCurrent, *rNext)) {
                break;
            }

            std::swap(*rCurrent, *rNext);

            ++rCurrent;
            ++rNext;
        }
    }
}

} // namespace detail

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void sort(RandomIt first, RandomIt last, Compare comp = Compare{}) {
    assert(std::distance(first, last) >= 0);

    if (first == last) {
        return;
    }

    using Run = std::pair<RandomIt, RandomIt>;
    std::deque<Run> runs;

    // Compute the minimum run length. We want the minimum run length to be in
    // range [32, 64] and close to a power of 2. For inputs of size <= 64, the
    // minimum run length will be the size of the input.
    auto merge_compute_minirun =
        [](RandomIt first, RandomIt last) {
            auto n = std::distance(first, last);
            assert(n >= 0);

            // Get the 6 most-significant bits starting from the highest set
            // bit. Add 1 to the result if any other bit was set.
            decltype(n) r = 0;
            while (n >= 0b1000000) {
                r |= n & 1;
                n >>= 1;
            }
            return n + r;
        };

    auto const minRunSize = merge_compute_minirun(first, last);

    // Allocate a buffer for common run sizes.
    using ValueT = typename std::iterator_traits<RandomIt>::value_type;
    std::vector<ValueT> buffer;
    buffer.resize(minRunSize);

    // The main loop:
    // 1. Find runs
    // 2. Merge runs
    auto current = first;
    while (current != last) {
        // 1. Find the "natural runs"
        auto const runFirst = current++;
        if (current == last) {
            break;
        }

        // Ascending: A[0] <= A[1] <= ... <= A[n]
        // Descending: A[0] > A[1] > ... > A[n]
        bool const isDescending = comp(*current, *runFirst);

        auto runPrev = current;
        while (++current != last) {
            if (isDescending != comp(*current, *runPrev)) {
                break;
            }
            runPrev = current;
        }

        // Extend small runs to the minimum run length.
        auto const runSize = std::distance(runFirst, current);
        if (runSize < minRunSize) {
            // Do insertion sort on small runs, because it is fast on small
            // inputs.
            auto runLast = current + std::min(minRunSize - runSize,
                                              std::distance(current, last));
            detail::insertion_sort(runFirst, runLast, comp);
            current = runLast;
        } else {
            if (isDescending) {
                // Because this is a stable sort, the values in the descending runs have
                // to be strictly decreasing (> not >=) to keep the stable order.
                std::reverse(runFirst, current);
            }
        }

        runs.emplace_back(runFirst, current);

        // 2. Merge runs until the following conditions hold (C being the most
        // recent run):
        // * |A| > |B| + |C|
        // * |B| > |C|
        //
        // These conditions maintain a balance between delaying a merge for more
        // balanced merge chunks vs merging soon to exploit cache freshness.
        while (runs.size() >= 2u) {
            auto & c = *runs.rbegin();
            auto const cSize = std::distance(c.first, c.second);
            auto & b = *(runs.rbegin() + 1);
            auto const bSize = std::distance(b.first, b.second);

            if (runs.size() >= 3u) {
                auto & a = *(runs.rbegin() + 2);
                auto const aSize = std::distance(a.first, a.second);

                // |A| > |B| + |C|
                if (aSize <= bSize + cSize) {
                    // Merge the smaller run of A and C with B
                    if (aSize < cSize) {
                        // Merge A and B
                        detail::merge_sort(a.first, a.second, b.first, b.second, comp, buffer);
                        a.second = b.second;
                        b = c;
                        runs.pop_back();
                    } else {
                        // Merge B and C
                        detail::merge_sort(b.first, b.second, c.first, c.second, comp, buffer);
                        b.second = c.second;
                        runs.pop_back();
                    }

                    // Check conditions again.
                    continue;
                }
            }

            // |B| > |C|
            if (bSize <= cSize) {
                // Merge B and C
                detail::merge_sort(b.first, b.second, c.first, c.second, comp, buffer);
                b.second = c.second;
                runs.pop_back();

                // Check conditions again.
                continue;
            }

            // Both conditions hold. Continue with the runs.
            break;
        }
    }

    // Merge all remaining runs.
    while (runs.size() > 1u) {
        auto b = runs.back();
        runs.pop_back();
        auto & a = runs.back();

        detail::merge_sort(a.first, a.second, b.first, b.second, comp, buffer);
        a = {a.first, b.second};
    }
}

} // namespace timsort
