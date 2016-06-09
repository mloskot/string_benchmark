#include "samples.hpp"

namespace string_benchmark
{

std::array<char const*, 10> nstring_samples = {
    "something1",
    "something1something2",
    "something1something2something3",
    "something1something2something3something4",
    "something1something2something3something4something5",
    "something1something2something3something4something5something6",
    "something1something2something3something4something5something6something7",
    "something1something2something3something4something5something6something7something8",
    "something1something2something3something4something5something6something7something8something9",
    "something1something2something3something4something5something6something7something8something9something10",
};

std::array<wchar_t const*, 10> wstring_samples = {
    L"something1",
    L"something1something2",
    L"something1something2something3",
    L"something1something2something3something4",
    L"something1something2something3something4something5",
    L"something1something2something3something4something5something6",
    L"something1something2something3something4something5something6something7",
    L"something1something2something3something4something5something6something7something8",
    L"something1something2something3something4something5something6something7something8something9",
    L"something1something2something3something4something5something6something7something8something9something10",
};

}
