# string_benchmark

Simple non-academic performance comparison of various operations on C++ strings:
C strings, character arrays, `std::string` and `std::wstring` classes.

## Build Status

| Branch | Travis CI | AppVeyor| Drone |
|:--- |:--- |:--- |:--- |
| `master` | [![status](https://travis-ci.org/mloskot/string_benchmark.svg?branch=master)](https://travis-ci.org/mloskot/string_benchmark) | [![status](https://ci.appveyor.com/api/projects/status/9xx7o8wjh35m21mn/branch/master?svg=true)](https://ci.appveyor.com/project/mloskot/string-benchmark/branch/master) | [![status](https://drone.io/github.com/mloskot/string_benchmark/status.png)](https://drone.io/github.com/mloskot/string_benchmark/latest) |

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