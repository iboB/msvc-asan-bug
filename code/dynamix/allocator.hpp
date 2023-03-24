// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "size.hpp"
#include <memory_resource>

namespace dynamix {
using allocator = std::pmr::polymorphic_allocator<byte_t>;
}
