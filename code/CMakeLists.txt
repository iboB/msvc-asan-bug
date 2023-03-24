# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_library(dynamix SHARED)
if(NOT WIN32)
    target_compile_options(dynamix PRIVATE -fvisibility=hidden)
endif()

target_compile_definitions(dynamix
    PRIVATE -DBUILDING_DYNAMIX=1
    PUBLIC -DDYNAMIX_SHARED=1
)
add_library(dynamix::dynamix ALIAS dynamix)
target_include_directories(dynamix INTERFACE .)

target_sources(dynamix PRIVATE
    dynamix/domain.cpp
    dynamix/exception.cpp
    dynamix/domain-c.cpp
 )
