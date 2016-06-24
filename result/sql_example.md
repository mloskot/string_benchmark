# Benchmark `CREATE TABLE` example

Comparing 64-bit builds:
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
