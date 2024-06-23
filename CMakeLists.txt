cmake_minimum_required(VERSION 3.10)
project(TetrisTornado VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(main "${SOURCES}")

# Change to compile with different defaults.
#add_definitions(-DFPS=120)
add_definitions(-DVSYNC)

if(TARGET_PLATFORM STREQUAL "windows")
    # For Windows
    set(EXT_LIB_PATH "${CMAKE_SOURCE_DIR}/extern/lib")
    set(EXT_INCL_PATH "${CMAKE_SOURCE_DIR}/extern/lib/include")

    include_directories("${EXT_INCL_PATH}")
    link_directories("${EXT_LIB_PATH}")
    target_link_libraries(main PRIVATE winmm "${EXT_LIB_PATH}/libraylib.a" "${EXT_LIB_PATH}/libbox2d.a")

elseif(TARGET_PLATFORM STREQUAL "linux")
    # For Linux
    target_link_libraries(main PRIVATE raylib box2d)

else()
    message(FATAL_ERROR "Unknown target platform")
    
endif()

