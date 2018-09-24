# timsort

This is an implementation of the [timsort](https://bugs.python.org/file4451/timsort.txt) sorting algorithm. It was inspired by [this](https://hackernoon.com/timsort-the-fastest-sorting-algorithm-youve-never-heard-of-36b28417f399) article and is only meant to be useful as a programming exercise. The Wikipedia [article](https://en.wikipedia.org/wiki/Timsort) was also useful for implementing this.

## Build

The implementation is header only. See `include/timsort/timsort.h`.

### Benchmarks

```bash
mkdir build
cd build
# (optional) BENCHMARK_ROOT is only needed for the benchmark binary
BENCHMARK_ROOT=/path/to/google-benchmark/prefix cmake -DCMAKE_BUILD_TYPE=Release ..
make
make install # (optional) installs the header
```

This will build two binaries:
* `simple-test`: runs a test on randomized data and compares the speed of `timsort` to `std::stable_sort`
* `benchmark`: runs some benchmarks on `timsort` and `std::stable_sort` (for comparison)

## Benchmark results

### Comparison to `std::stable_sort`

The comparison is done against `std::stable_sort` instead of `std::sort` because `timsort` is a stable sorting
algorithm. This implementation is a little bit slower than `std::stable_sort` on random data. However, because `timsort`
is designed to exploit existing structure in real data (where the data is often already partially ordered), it is very
fast in the other cases.

Benchmark details:

* Sorting on arrays of random integers
* `us/sort`: Time per sorting in microseconds
* `diff`: shows speedup/slowdown compared to `std::stable_sort` (negative means `timsort` is faster)

```
$ ./simple-test
Max memory: 1073741824B, iterations: 10
Test[size=1(4B),iter=10]: timsort 0.2011 us/sort, stdsort 0.1701 us/sort, diff 0.182246x
Test[size=2(8B),iter=10]: timsort 0.133 us/sort, stdsort 0.0914 us/sort, diff 0.455142x
Test[size=4(16B),iter=10]: timsort 0.1751 us/sort, stdsort 0.1364 us/sort, diff 0.283724x
Test[size=8(32B),iter=10]: timsort 0.2062 us/sort, stdsort 0.2283 us/sort, diff -0.0968025x
Test[size=16(64B),iter=10]: timsort 0.327 us/sort, stdsort 0.578 us/sort, diff -0.434256x
Test[size=32(128B),iter=10]: timsort 0.6098 us/sort, stdsort 0.9157 us/sort, diff -0.334061x
Test[size=64(256B),iter=10]: timsort 1.7358 us/sort, stdsort 2.0376 us/sort, diff -0.148115x
Test[size=128(512B),iter=10]: timsort 4.3089 us/sort, stdsort 4.2736 us/sort, diff 0.00826001x
Test[size=256(1024B),iter=10]: timsort 9.4442 us/sort, stdsort 9.3705 us/sort, diff 0.00786511x
Test[size=512(2048B),iter=10]: timsort 20.892 us/sort, stdsort 20.2219 us/sort, diff 0.0331373x
Test[size=1024(4096B),iter=10]: timsort 46.2039 us/sort, stdsort 44.8054 us/sort, diff 0.0312128x
Test[size=2048(8192B),iter=10]: timsort 99.3161 us/sort, stdsort 97.8457 us/sort, diff 0.0150277x
Test[size=4096(16384B),iter=10]: timsort 231.452 us/sort, stdsort 222.316 us/sort, diff 0.0410929x
Test[size=8192(32768B),iter=10]: timsort 473.011 us/sort, stdsort 461.851 us/sort, diff 0.0241638x
Test[size=16384(65536B),iter=10]: timsort 1026.37 us/sort, stdsort 981.211 us/sort, diff 0.0460268x
Test[size=32768(131072B),iter=10]: timsort 2166.92 us/sort, stdsort 2096.75 us/sort, diff 0.0334614x
Test[size=65536(262144B),iter=10]: timsort 4648.58 us/sort, stdsort 4443.24 us/sort, diff 0.0462143x
Test[size=131072(524288B),iter=10]: timsort 9756.62 us/sort, stdsort 9394.63 us/sort, diff 0.0385322x
Test[size=262144(1048576B),iter=10]: timsort 20716.4 us/sort, stdsort 19940.4 us/sort, diff 0.0389139x
Test[size=524288(2097152B),iter=10]: timsort 43821 us/sort, stdsort 42512.1 us/sort, diff 0.0307881x
Test[size=1048576(4194304B),iter=10]: timsort 93476 us/sort, stdsort 90643.6 us/sort, diff 0.0312473x
Test[size=2097152(8388608B),iter=10]: timsort 215545 us/sort, stdsort 209061 us/sort, diff 0.031015x
Test[size=4194304(16777216B),iter=10]: timsort 417086 us/sort, stdsort 406990 us/sort, diff 0.0248067x
Test[size=8388608(33554432B),iter=10]: timsort 866671 us/sort, stdsort 851549 us/sort, diff 0.0177585x
```

### Other benchmarks

```
$ ./benchmark
2018-09-24 21:02:06
Running ./benchmark
Run on (4 X 3100 MHz CPU s)
CPU Caches:
  L1 Data 32K (x2)
  L1 Instruction 32K (x2)
  L2 Unified 256K (x2)
  L3 Unified 3072K (x1)
------------------------------------------------------------------------------------------
Benchmark                                                   Time           CPU Iterations
------------------------------------------------------------------------------------------
timsort_bench<std::vector<int>, RandomGen>/8              550 ns        553 ns    1275951   13.8042MB/s
timsort_bench<std::vector<int>, RandomGen>/64             803 ns        806 ns     872437   75.7653MB/s
timsort_bench<std::vector<int>, RandomGen>/512           4394 ns       4389 ns     160709   111.247MB/s
timsort_bench<std::vector<int>, RandomGen>/4096         45214 ns      45145 ns      15482   86.5264MB/s
timsort_bench<std::vector<int>, RandomGen>/32768       461596 ns     461007 ns       1511   67.7863MB/s
timsort_bench<std::vector<int>, RandomGen>/262144     4496614 ns    4493118 ns        155   55.6406MB/s
timsort_bench<std::vector<int>, RandomGen>/2097152   43169726 ns   43131576 ns         16   46.3697MB/s
timsort_bench<std::vector<int>, RandomGen>/4194304   91735534 ns   91610246 ns          8   43.6632MB/s
stdsort_bench<std::vector<int>, RandomGen>/8              531 ns        534 ns    1340720   14.2991MB/s
stdsort_bench<std::vector<int>, RandomGen>/64             858 ns        860 ns     823991   71.0031MB/s
stdsort_bench<std::vector<int>, RandomGen>/512           4413 ns       4404 ns     157588   110.862MB/s
stdsort_bench<std::vector<int>, RandomGen>/4096         45111 ns      45007 ns      13563   86.7918MB/s
stdsort_bench<std::vector<int>, RandomGen>/32768       500477 ns     499911 ns       1429   62.5112MB/s
stdsort_bench<std::vector<int>, RandomGen>/262144     4488627 ns    4483285 ns        145   55.7627MB/s
stdsort_bench<std::vector<int>, RandomGen>/2097152   42846196 ns   42786882 ns         17   46.7433MB/s
stdsort_bench<std::vector<int>, RandomGen>/4194304   91471049 ns   91334055 ns          7   43.7953MB/s
timsort_bench<std::vector<int>, AscGen>/8                 544 ns        547 ns    1293321   13.9459MB/s
timsort_bench<std::vector<int>, AscGen>/64                566 ns        569 ns    1204067   107.261MB/s
timsort_bench<std::vector<int>, AscGen>/512               651 ns        655 ns    1073235   745.942MB/s
timsort_bench<std::vector<int>, AscGen>/4096             1303 ns       1304 ns     541041   2.92495GB/s
timsort_bench<std::vector<int>, AscGen>/32768            6041 ns       6040 ns     116708   5.05277GB/s
timsort_bench<std::vector<int>, AscGen>/262144          45244 ns      45196 ns      15425   5.40181GB/s
timsort_bench<std::vector<int>, AscGen>/2097152        391767 ns     391056 ns       1839   4.99449GB/s
timsort_bench<std::vector<int>, AscGen>/4194304        748654 ns     747231 ns        925   5.22763GB/s
stdsort_bench<std::vector<int>, AscGen>/8                 525 ns        528 ns    1302849   14.4509MB/s
stdsort_bench<std::vector<int>, AscGen>/64                591 ns        594 ns    1070255   102.837MB/s
stdsort_bench<std::vector<int>, AscGen>/512              1157 ns       1160 ns     582880   420.973MB/s
stdsort_bench<std::vector<int>, AscGen>/4096             6282 ns       6288 ns     114160   621.193MB/s
stdsort_bench<std::vector<int>, AscGen>/32768           55012 ns      55042 ns      12693   567.751MB/s
stdsort_bench<std::vector<int>, AscGen>/262144         525237 ns     524923 ns       1373   476.261MB/s
stdsort_bench<std::vector<int>, AscGen>/2097152       5347673 ns    5337566 ns        140   374.703MB/s
stdsort_bench<std::vector<int>, AscGen>/4194304      13526895 ns   13492051 ns         46   296.471MB/s
timsort_bench<std::vector<int>, DescGen>/8                544 ns        548 ns    1286903   13.9338MB/s
timsort_bench<std::vector<int>, DescGen>/64               574 ns        576 ns    1252706   105.897MB/s
timsort_bench<std::vector<int>, DescGen>/512              687 ns        689 ns    1016792   708.537MB/s
timsort_bench<std::vector<int>, DescGen>/4096            1667 ns       1674 ns     363126   2.27887GB/s
timsort_bench<std::vector<int>, DescGen>/32768           9363 ns       9399 ns      77985   3.24679GB/s
timsort_bench<std::vector<int>, DescGen>/262144         65823 ns      65759 ns      11568   3.71267GB/s
timsort_bench<std::vector<int>, DescGen>/2097152       432091 ns     431518 ns       1263   4.52617GB/s
timsort_bench<std::vector<int>, DescGen>/4194304       966117 ns     963971 ns        729   4.05225GB/s
stdsort_bench<std::vector<int>, DescGen>/8                515 ns        518 ns    1355039   14.7171MB/s
stdsort_bench<std::vector<int>, DescGen>/64               601 ns        604 ns    1165048   101.047MB/s
stdsort_bench<std::vector<int>, DescGen>/512             1358 ns       1361 ns     515562    358.64MB/s
stdsort_bench<std::vector<int>, DescGen>/4096            8672 ns       8674 ns      80851    450.32MB/s
stdsort_bench<std::vector<int>, DescGen>/32768          73921 ns      73912 ns       9502   422.801MB/s
stdsort_bench<std::vector<int>, DescGen>/262144        681437 ns     681049 ns       1030   367.081MB/s
stdsort_bench<std::vector<int>, DescGen>/2097152      6478262 ns    6467173 ns        110   309.254MB/s
stdsort_bench<std::vector<int>, DescGen>/4194304     16121734 ns   16084612 ns         44   248.685MB/s
timsort_bench<std::vector<int>, EqGen>/8                  538 ns        541 ns    1294043   14.1059MB/s
timsort_bench<std::vector<int>, EqGen>/64                 560 ns        563 ns    1231319   108.321MB/s
timsort_bench<std::vector<int>, EqGen>/512                652 ns        655 ns    1070006   745.237MB/s
timsort_bench<std::vector<int>, EqGen>/4096              1259 ns       1262 ns     567148   3.02232GB/s
timsort_bench<std::vector<int>, EqGen>/32768             6037 ns       6041 ns     114552    5.0521GB/s
timsort_bench<std::vector<int>, EqGen>/262144           43460 ns      43524 ns      15954   5.60928GB/s
timsort_bench<std::vector<int>, EqGen>/2097152         350425 ns     350060 ns       1999   5.57939GB/s
timsort_bench<std::vector<int>, EqGen>/4194304         740591 ns     738660 ns        919    5.2883GB/s
stdsort_bench<std::vector<int>, EqGen>/8                  512 ns        515 ns    1362325   14.8011MB/s
stdsort_bench<std::vector<int>, EqGen>/64                 580 ns        584 ns    1201745    104.56MB/s
stdsort_bench<std::vector<int>, EqGen>/512               1157 ns       1160 ns     605839   420.953MB/s
stdsort_bench<std::vector<int>, EqGen>/4096              6209 ns       6216 ns     113976   628.427MB/s
stdsort_bench<std::vector<int>, EqGen>/32768            58533 ns      58553 ns      12874   533.703MB/s
stdsort_bench<std::vector<int>, EqGen>/262144          523856 ns     523605 ns       1004   477.459MB/s
stdsort_bench<std::vector<int>, EqGen>/2097152        5196075 ns    5186531 ns        100   385.614MB/s
stdsort_bench<std::vector<int>, EqGen>/4194304       13507930 ns   13470680 ns         53   296.941MB/s
```
