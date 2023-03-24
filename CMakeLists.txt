# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(msvc-asan-bug
    LANGUAGES C CXX
)

#######################################
# cmake lib

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

#######################################
# options

set(CMAKE_LINK_DEPENDS_NO_SHARED ON)

option(SAN_THREAD "${CMAKE_PROJECT_NAME}: sanitize thread" OFF)
option(SAN_ADDR "${CMAKE_PROJECT_NAME}: sanitize address" OFF)
option(SAN_UB "${CMAKE_PROJECT_NAME}: sanitize undefined behavior" OFF)
option(SAN_LEAK "${CMAKE_PROJECT_NAME}: sanitize leaks" OFF)

set(icm_compiler_flags "")
set(icm_linker_flags "")
set(icm_compiler_and_linker_flags "")

if(MSVC)
    # /Zc:preprocessor - incompatible with Windows.h
    # /Zc:templateScope - TODO: add when msvc 17.5 is the norm
    set(icm_compiler_flags "/W4 -D_CRT_SECURE_NO_WARNINGS /Zc:__cplusplus /permissive-\
        /volatile:iso /Zc:throwingNew /utf-8 -DNOMINMAX=1\
        /w34100 /w34189 /w34701 /w34702 /w34703 /w34706 /w34714 /w34913\
        /wd4251 /wd4275"
    )
else()
    set(icm_compiler_flags "-Wall -Wextra")
endif()

if(MSVC)
    if(SAN_ADDR)
        set(icm_compiler_flags "${icm_compiler_flags} /fsanitize=address")
    endif()
    if(SAN_THREAD OR SAN_UB OR SAN_LEAK)
        message(WARNING "Unsupported sanitizers requested for msvc. Ignored")
    endif()
else()
    if(SAN_THREAD)
        set(icm_compiler_and_linker_flags "${icm_compiler_and_linker_flags} -fsanitize=thread -g")
        if(SAN_ADDR OR SAN_UB OR SAN_LEAK)
            message(WARNING "Incompatible sanitizer combination requested. Only 'SAN_THREAD' will be respected")
        endif()
    else()
        if(SAN_ADDR)
            set(icm_compiler_and_linker_flags "${icm_compiler_and_linker_flags} -fsanitize=address -pthread")
        endif()
        if(SAN_UB)
            set(icm_compiler_and_linker_flags "${icm_compiler_and_linker_flags} -fsanitize=undefined")
        endif()
        if(SAN_LEAK)
            if(APPLE)
                message(WARNING "Unsupported leak sanitizer requested for Apple. Ignored")
            else()
                set(icm_compiler_and_linker_flags "${icm_compiler_and_linker_flags} -fsanitize=leak")
            endif()
        endif()
    endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${icm_compiler_flags} ${icm_compiler_and_linker_flags}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${icm_compiler_flags} ${icm_compiler_and_linker_flags}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${icm_linker_flags} ${icm_compiler_and_linker_flags}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${icm_linker_flags} ${icm_compiler_and_linker_flags}")

# all binaries to bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#######################################
# code
add_subdirectory(code)
add_subdirectory(scratch)

