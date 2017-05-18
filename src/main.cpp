#include <hayai.hpp>
#include <hayai_main.hpp>
#include "string_append.hpp"
#include "string_sprintf.hpp"
#include "string_starts_with.hpp"

int main(int argc, char** argv)
{
    // Set up the main runner.
    ::hayai::MainRunner runner;

    // Parse the arguments.
    int result = runner.ParseArgs(argc, argv);
    if (result)
    {
        return result;
    }

    // Execute based on the selected mode.
    return runner.Run();
}