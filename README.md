# string_benchmark

Simple non-academic performance comparison of various operations on C++ strings:
C strings, character arrays, `std::string` and `std::wstring` classes
as well as some of the Boost string algorithms.

The benchmarks use [Celero](https://github.com/DigitalInBlue/Celero) library.

The tests use [Catch](https://github.com/philsquared/Catch/).

## Build Status

| Branch | Travis CI | AppVeyor|
|:--- |:--- |:--- |
| `master` | [![status](https://travis-ci.org/mloskot/string_benchmark.svg?branch=master)](https://travis-ci.org/mloskot/string_benchmark) | [![status](https://ci.appveyor.com/api/projects/status/w07moe4jimo6cqp1/branch/master?svg=true)](https://ci.appveyor.com/project/mloskot/string-benchmark/branch/master) |

## Results

See [charts](https://mloskot.github.io/string_benchmark/results) generate with [Bokeh](http://bokeh.pydata.org/).

Detailed results are displayed in tabular form in the
[Travis CI](https://travis-ci.org/mloskot/string_benchmark) and
[AppVeyor](https://ci.appveyor.com/project/mloskot/string-benchmark)
build logs.

## Usage

Build and run the benchmark with:

### Unix

```
cmake -DCMAKE_BUILD_TYPE=Release /path/to/string_benchmark
make
```

### Windows

```
cmake -G "Visual Studio 15 2017 Win64" -DBOOST_ROOT=C:/local/boost_1_63_0 /path/to/string_benchmark
msbuild /p:Configuration=Release /p:Platform=x64 string_benchmark.sln
```

### Run

CMake generates `RUN` target. It runs all benchmarks automatically after successful
build and tests run, saving results in CSV files named after benchmark programs.

[Celero](https://github.com/DigitalInBlue/Celero#command-line) command line options are available.

```
benchmark_iends_with -t benchmark_iends_with.csv
benchmark_istarts_with -t benchmark_istarts_with.csv
```

## Credits

* [John Farrier](https://github.com/DigitalInBlue/) for the Celero benchmarking library.

## Disclaimer

This project is driven by curiosity and for my own purposes.
This is not a rocket science, but a simple set of C++ programs,
with likelyhood of bugs, incorrectness or inconsistencies.
Found any, please report.
Comments and improvements are always welcome!