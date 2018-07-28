# timsort

This is an implementation of the [timsort](https://bugs.python.org/file4451/timsort.txt) sorting algorithm. It was inspired by [this](https://hackernoon.com/timsort-the-fastest-sorting-algorithm-youve-never-heard-of-36b28417f399) article and is only meant to be useful as a programming exercise.

## Build

The implementation is header only. See `include/timsort/timsort.h`.

### Tests and benchmarks

```bash
mkdir build
cd build
# (optional) BENCHMARK_ROOT is only needed for the benchmarks
BENCHMARK_ROOT=/path/to/google-benchmark/prefix cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install # (optional) installs the header
```

This will build two binaries:
* `simple-test`: runs a test on randomized data and compares the speed of `timsort` to `std::sort`
* `benchmark`: runs some benchmarks on `timsort` and `std::sort` (for comparison)

## Benchmark results

This implementation is slower than `std::sort` on random data. This is to be expected, `timsort` is designed to exploit existing structure in real data where the data is already partially sorted.

```
2018-07-28 14:38:44
Running ./benchmark
Run on (4 X 2900 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 256K (x2)
  L3 Unified 3072K (x1)
--------------------------------------------------------------------
Benchmark                             Time           CPU Iterations
--------------------------------------------------------------------
timsort_bench_asc/8                 157 ns        157 ns    4476218   48.6059MB/s
timsort_bench_asc/64                161 ns        161 ns    4351347   378.639MB/s
timsort_bench_asc/512               265 ns        265 ns    2629177   1.80083GB/s
timsort_bench_asc/4096              926 ns        926 ns     800817   4.12077GB/s
timsort_bench_asc/32768            5765 ns       5761 ns     121776   5.29687GB/s
timsort_bench_asc/262144          44737 ns      44734 ns      15706   5.45757GB/s
timsort_bench_asc/2097152        365116 ns     364872 ns       1931    5.3529GB/s
timsort_bench_asc/4194304        781371 ns     781276 ns        925   4.99983GB/s
timsort_bench_desc/8                342 ns        344 ns    2033713   22.1977MB/s
timsort_bench_desc/64               358 ns        360 ns    1946358   169.459MB/s
timsort_bench_desc/512              534 ns        536 ns    1309289   910.813MB/s
timsort_bench_desc/4096            1601 ns       1603 ns     447072   2.37916GB/s
timsort_bench_desc/32768           9192 ns       9207 ns      76268   3.31446GB/s
timsort_bench_desc/262144         72479 ns      72491 ns       9690   3.36786GB/s
timsort_bench_desc/2097152       597860 ns     597883 ns       1177   3.26673GB/s
timsort_bench_desc/4194304      1285445 ns    1285607 ns        533   3.03845GB/s
timsort_bench_eq/8                  136 ns        136 ns    5108830   56.1105MB/s
timsort_bench_eq/64                 145 ns        145 ns    4839095   419.804MB/s
timsort_bench_eq/512                265 ns        265 ns    2548905   1.79731GB/s
timsort_bench_eq/4096               934 ns        934 ns     755191   4.08459GB/s
timsort_bench_eq/32768             5745 ns       5743 ns     122343   5.31348GB/s
timsort_bench_eq/262144           44641 ns      44620 ns      15654   5.47149GB/s
timsort_bench_eq/2097152         357819 ns     357743 ns       1947   5.45957GB/s
timsort_bench_eq/4194304         750993 ns     750750 ns        910   5.20313GB/s
timsort_bench_random/8              348 ns        350 ns    1999962   21.8022MB/s
timsort_bench_random/64             550 ns        551 ns    1272243   110.693MB/s
timsort_bench_random/512           4199 ns       4199 ns     167235   116.279MB/s
timsort_bench_random/4096         48657 ns      48631 ns      14571   80.3243MB/s
timsort_bench_random/32768       512310 ns     512261 ns       1363   61.0041MB/s
timsort_bench_random/262144     5153300 ns    5152997 ns        136   48.5155MB/s
timsort_bench_random/2097152   50011879 ns   50009819 ns         10   39.9921MB/s
timsort_bench_random/4194304  105658461 ns  105652254 ns          7   37.8601MB/s
stdsort_bench_asc/8                  55 ns         55 ns   12669061   138.512MB/s
stdsort_bench_asc/64                137 ns        137 ns    5139185   444.845MB/s
stdsort_bench_asc/512              1682 ns       1682 ns     417603   290.356MB/s
stdsort_bench_asc/4096            16553 ns      16552 ns      42630   235.995MB/s
stdsort_bench_asc/32768          211132 ns     211122 ns       3322   148.019MB/s
stdsort_bench_asc/262144        2016801 ns    2016704 ns        348   123.965MB/s
stdsort_bench_asc/2097152      21062520 ns   21061231 ns         33   94.9612MB/s
stdsort_bench_asc/4194304      44584478 ns   44581675 ns         16    89.723MB/s
stdsort_bench_desc/8                255 ns        257 ns    2733388   29.6947MB/s
stdsort_bench_desc/64               367 ns        369 ns    1900650   165.583MB/s
stdsort_bench_desc/512             1801 ns       1804 ns     385941   270.635MB/s
stdsort_bench_desc/4096           18655 ns      18658 ns      37366   209.362MB/s
stdsort_bench_desc/32768         211920 ns     211907 ns       3292   147.471MB/s
stdsort_bench_desc/262144       2162547 ns    2162531 ns        324   115.605MB/s
stdsort_bench_desc/2097152     21374634 ns   21373856 ns         33   93.5723MB/s
stdsort_bench_desc/4194304     46917827 ns   46916790 ns         15   85.2573MB/s
stdsort_bench_eq/8                   56 ns         56 ns   12413189   135.203MB/s
stdsort_bench_eq/64                 139 ns        139 ns    5077013   439.914MB/s
stdsort_bench_eq/512               1679 ns       1679 ns     410267   290.866MB/s
stdsort_bench_eq/4096             16464 ns      16463 ns      42461   237.273MB/s
stdsort_bench_eq/32768           213314 ns     213304 ns       3214   146.504MB/s
stdsort_bench_eq/262144         2012875 ns    2012796 ns        349   124.205MB/s
stdsort_bench_eq/2097152       21139136 ns   21138061 ns         33   94.6161MB/s
stdsort_bench_eq/4194304       44677490 ns   44674666 ns         16   89.5362MB/s
stdsort_bench_random/8              261 ns        263 ns    2654289   28.9576MB/s
stdsort_bench_random/64             576 ns        579 ns    1223203   105.411MB/s
stdsort_bench_random/512           4520 ns       4523 ns     154609   107.961MB/s
stdsort_bench_random/4096         44935 ns      44917 ns      15580   86.9659MB/s
stdsort_bench_random/32768       482134 ns     482094 ns       1449   64.8214MB/s
stdsort_bench_random/262144     4597504 ns    4597334 ns        152   54.3793MB/s
stdsort_bench_random/2097152   44374282 ns   44371978 ns         16   45.0735MB/s
stdsort_bench_random/4194304   93104151 ns   93100606 ns          8   42.9643MB/s
```
