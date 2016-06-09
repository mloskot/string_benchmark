file(GLOB headers *.hpp)
file(GLOB sources *.cpp)

message(STATUS "Configuring build target") 
add_executable(string_benchmark ${headers} ${sources})
add_dependencies(string_benchmark hayai)
include_directories(${HAYAI_INCLUDE_DIR})

message(STATUS "Configuring run target")
add_custom_target(run ALL $<TARGET_FILE:string_benchmark> DEPENDS string_benchmark)
