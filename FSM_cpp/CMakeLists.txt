cmake_minimum_required(VERSION 3.16)
project(Fsm_cpp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(src)

# Source files
set(SOURCES
    src/Fsm/Fsm.cpp
    src/State/State.cpp
    src/Subroutine/Subroutine.cpp
)

set(HEADERS
    src/Fsm/Fsm.h
    src/State/State.h
    src/Subroutine/Subroutine.h
    src/Subroutine/CallbackStatus.h
)

# Create static library
add_library(fsm_cpp STATIC ${SOURCES} ${HEADERS})

# Set properties for the library
set_target_properties(fsm_cpp PROPERTIES
    OUTPUT_NAME "fsm_cpp"
    POSITION_INDEPENDENT_CODE ON
)

# Test executable
add_executable(test_1 test/test_1.cpp)
target_link_libraries(test_1 PRIVATE fsm_cpp)

# Optional: Enable testing
enable_testing()

# Optional: Add test
add_test(NAME test_1 COMMAND test_1)

# Optional: Install targets (uncomment if you want install support)
install(TARGETS fsm_cpp
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin)

install(FILES ${HEADERS} DESTINATION include/fsm_cpp)

# Optional: Print build summary
message(STATUS "Build configuration:")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")

# mkdir build
# cd build
# cmake ..
# cmake --build .
# ./bin/test_1  # Run the test