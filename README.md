# string_benchmark

Simple non-academic performance comparison of various operations on C++ strings:
C strings, character arrays, `std::string` and `std::wstring` classes.

## Build Status

| Branch | Travis CI | AppVeyor| Drone |
|:--- |:--- |:--- |:--- |
| `master` | [![status](https://travis-ci.org/mloskot/string_benchmark.svg?branch=master)](https://travis-ci.org/mloskot/string_benchmark) | [![status](https://ci.appveyor.com/api/projects/status/w07moe4jimo6cqp1/branch/master?svg=true)](https://ci.appveyor.com/project/mloskot/string-benchmark/branch/master) | [![status](https://drone.io/github.com/mloskot/string_benchmark/status.png)](https://drone.io/github.com/mloskot/string_benchmark/latest) |

## Results

### `std::string::append` vs `sprintf`

* `append` single is **single call** to the method.
* `append` multiple is **repeated call** (x10) to the method in `for` loop
* `sprintf` single is a **single call** to the function with `%s` formatter.
* `sprintf` multiple is a **single call** to the function with  `/%s/%s/%s/%s/%s/%s/%s/%s/%s/%s` formatter.

| Benchmark | Average (us) | Fastest (us) | Slowest (us) |
|:--- |:--- |:--- |:--- |
| string.append_single_chars10 | 24.494 | 20.134 | 47.801 |
| string.sprintf_single_chars10 | 263.820 | 257.984 | 367.315 |
| wstring.append_single_chars10 | 106.396 | 95.996 | 169.181 |
| wstring.sprintf_single_chars10 | 355.817 | 344.556 | 477.092 |
|:--- |:--- |:--- |:--- |
| string.append_single_chars100 | 160.759 | 146.868 | 318.228 |
| string.sprintf_single_chars100 | 386.864 | 376.240 | 520.378 |
| wstring.append_single_chars100 | 101.447 | 95.996 | 158.024 |
| wstring.sprintf_single_chars100 | 372.627 | 365.976 | 475.753 |
|:--- |:--- |:--- |:--- |
| string.append_multiple | 1140.287 | 1077.296 | 1966.223 |
| wstring.append_multiple | 1408.417 | 1378.960 | 3367.890 |
| string.sprintf_multiple | 1640.404 | 1603.423 | 2152.755 |
| wstring.sprintf_multiple | 1773.422 | 1714.539 | 2272.796 |
|:--- |:--- |:--- |:--- |
| string.append_multiple_sql_example | 1340.085 | 1306.026 | 1472.923 |
| string.sprintf_multiple_sql_example | 1347.877 | 1314.505 | 1496.128 |
| wstring.append_multiple_sql_example | 755.745 | 722.781 | 1285.945 |
| wstring.sprintf_multiple_sql_example | 1468.084 | 1434.992 | 1550.570 |
|:--- |:--- |:--- |:--- |

### `CREATE TABLE` example

Detailed comparison of two 64-bit builds:
  * VS2015: https://ci.appveyor.com/project/mloskot/string-benchmark/branch/master/job/wqe2u8arva6f9j90
  * GCC (Linux): https://travis-ci.org/mloskot/string_benchmark/jobs/140082916

| Benchmark | Toolset | Average (us) | Fastest (us) |
|:--- |:--- |:--- |:--- |
| string.append_multiple_sql_example | | | |
| | VS2015 | 454.590 | 400.376 |
| | GCC | 642.554 | 634.241 |
| string.sprintf_multiple_sql_example | | | |
| | VS2015 | 1468.441 | 1060.076 |
| | GCC | 899.210 | 889.241 |
| wstring.append_multiple_sql_example | | | |
| | VS2015 | 577.422 | 480.476 |
| | GCC | 751.459 | 746.241 |
| wstring.sprintf_multiple_sql_example | | | |
| | VS2015 | 1412.931 |989.676 |
| | GCC | 1394.854 | 1360.241 |

## Usage

Build and run the benchmark with: 

### Unix

```
cmake -DCMAKE_BUILD_TYPE=Release /path/to/string_benchmark
make
```

### Windows

```
cmake -G "Visual Studio 2015" /path/to/string_benchmark
msbuild /p:Configuration=Release /p:Platform=Win32 string-benchmark.sln
```

## Credits

* Nick Bruun for [Hayai](https://github.com/nickbruun/hayai)

## Disclaimer

This project is driven by curiosity and for my own purposes.
This is not a rocket science, but a simple set of C++ programs,
with likelyhood of bugs, incorrectness or inconsistencies.
Found any, please report.
Comments and improvements are always welcome!