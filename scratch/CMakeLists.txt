# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_executable(dynamix-c-scratch
    c-scratch.c
)
target_link_libraries(dynamix-c-scratch dynamix::dynamix)

add_custom_target(run-demo COMMAND dynamix-c-scratch)
