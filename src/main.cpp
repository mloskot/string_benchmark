#include <hayai.hpp>
#include "string_append.hpp"
#include "string_sprintf.hpp"
int main()
{
    hayai::ConsoleOutputter console;
    hayai::Benchmarker::AddOutputter(console);
    hayai::Benchmarker::RunAllTests();
    return 0;
}