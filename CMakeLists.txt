cmake_minimum_required(VERSION 3.8)
project(HomeWork2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "-std=c++0x -Wall -Werror -Wpedantic -ggdb3")

include(ExternalProject)

include_directories(project/include)
include_directories(SYSTEM ${CMAKE_BINARY_DIR}/include)

include(ExternalProject)
find_package(Git REQUIRED)

set(TARGET1 parallel)
set(TARGET2 consistent)
set(TARGET metric)

add_executable(${TARGET} project/main.c)

add_library(hw_lib_parallel SHARED project/parallel/src/directory_processing.c
        project/parallel/src/misc.c
        project/parallel/src/files_processing.c
        project/parallel/src/calculations.c
        project/parallel/src/control_units.c
        )

add_library(hw_lib_consistent STATIC project/consistent/src/directory_processing.c
        project/consistent/src/misc.c
        project/consistent/src/files_processing.c
        project/consistent/src/calculations.c
        project/consistent/src/control_units.c
        )

target_link_libraries(${TARGET} hw_lib_parallel)

option(PARALLEL "Build parallel" OFF)
if (PARALLEL)
    add_executable(${TARGET1} project/main.c)
    target_link_libraries(${TARGET1} hw_lib_parallel)
    target_link_libraries(${TARGET1} m)
endif()

option(CONSISTENT "Build consistent" OFF)
if (CONSISTENT)
    add_executable(${TARGET2} project/main.c)
    target_link_libraries(${TARGET2} hw_lib_consistent)
    target_link_libraries(${TARGET2} m)
endif()

target_link_libraries(${TARGET} m)

option(ENABLE_COVERAGE "Build tests coverage statistic" OFF)
if (ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif ()

option(ENABLE_CPPCHECK "Configure and run cppcheck." OFF)
if (ENABLE_CPPCHECK)

    list(APPEND CPPCHECK_CMAKE_ARGS
            "-DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}"
            )

    ExternalProject_Add(
            cppcheck
            GIT_REPOSITORY https://github.com/danmar/cppcheck.git
            GIT_TAG 1.79
            GIT_SHALLOW 1
            CMAKE_ARGS ${CPPCHECK_CMAKE_ARGS}
            PREFIX ${CMAKE_BINARY_DIR}/external/cppcheck/prefix
            TMP_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/tmp
            STAMP_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/stamp
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/download
            SOURCE_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/src
            BINARY_DIR ${CMAKE_BINARY_DIR}/external/cppcheck/build
    )

    list(APPEND CPPCHECK_ARGS1
            --enable=all
            --std=c11
            --verbose
            --error-exitcode=1
            --language=c
            --suppress=missingIncludeSystem
            ${CMAKE_SOURCE_DIR}/project/parallel/src/*.c
            ${CMAKE_SOURCE_DIR}/project/consistent/src/*.c
            )

    list(APPEND CPPCHECK_ARGS2
            --enable=all
            --verbose
            --error-exitcode=1
            --language=c++
            ${CMAKE_SOURCE_DIR}/test/*.cpp
            )

    add_custom_target(
            check1
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS1}
            COMMENT "running cppcheck"
    )

    add_custom_target(
            check2
            COMMAND ${CMAKE_BINARY_DIR}/bin/cppcheck ${CPPCHECK_ARGS2}
            COMMENT "running cppcheck"
    )

endif ()

option(BUILD_TESTS "Build all tests." OFF) # Makes boolean 'test' available.
if (BUILD_TESTS)
    configure_file(CMakeLists.txt.in
            googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/googletest-download)

    add_subdirectory(${CMAKE_BINARY_DIR}/googletest-src
            ${CMAKE_BINARY_DIR}/googletest-build)

    enable_testing()

    file(GLOB general tests/dir_processing.cpp
                          tests/misc.cpp)


    foreach (file ${general})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" gtest_main)
        target_link_libraries("${name}_tests" hw_lib_consistent)
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach ()

    file(GLOB consistent tests/consistent/file_processing_cons.cpp
                         tests/consistent/control_units_cons.cpp)

    foreach (file ${consistent})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" gtest_main)
        target_link_libraries("${name}_tests" hw_lib_consistent)
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach ()

    file(GLOB parallel tests/parallel/file_processing_par.cpp
                       tests/parallel/control_units_par.cpp)

    foreach (file ${parallel})
        set(name)
        get_filename_component(name ${file} NAME_WE)
        add_executable("${name}_tests" ${file})
        target_link_libraries("${name}_tests" gtest_main)
        target_link_libraries("${name}_tests" hw_lib_parallel)
        add_test(NAME ${name} COMMAND "${name}_tests")
    endforeach ()

endif ()