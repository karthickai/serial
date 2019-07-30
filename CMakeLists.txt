cmake_minimum_required(VERSION 3.13)

project(serial)

set(CMAKE_CXX_STANDARD 17)


if (MSVC)
    set (CMAKE_VERBOSE_MAKEFILE FALSE)

    add_definitions (
        -D_WIN32_WINNT=0x0601
        -D_SCL_SECURE_NO_WARNINGS=1
        -D_CRT_SECURE_NO_WARNINGS=1
        -D_SILENCE_CXX17_ALLOCATOR_VOID_DEPRECATION_WARNING
        -D_SILENCE_CXX17_ADAPTOR_TYPEDEFS_DEPRECATION_WARNING
    )    


else()
    set (THREADS_PREFER_PTHREAD_FLAG ON)
    find_package (Threads)

    set( CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -Wpedantic -Wno-unused-parameter")

    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wrange-loop-analysis")
    endif ()
endif()



set (Boost_USE_STATIC_LIBS ON)
set (Boost_USE_STATIC_RUNTIME ON)
set (Boost_USE_MULTITHREADED ON)
unset (Boost_INCLUDE_DIR CACHE)
unset (Boost_LIBRARY_DIRS CACHE)

set (BOOST_ROOT "C:\\tools\\vcpkg\\installed\\x86-windows`") 
set (BOOST_INCLUDEDIR "C:\\tools\\vcpkg\\installed\\x86-windows\\include")

find_package(Boost "1.70.0" COMPONENTS ${BOOST_MANDATORY_COMPONENTS}  REQUIRED)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories (${Boost_INCLUDE_DIRS})



include_directories( include )
add_executable(serial src/SerialPort.cpp)


target_link_libraries(serial ${Boost_LIBRARIES})
