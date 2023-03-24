// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "exception.hpp"

namespace dynamix {
// exporting the vtable
exception::~exception() = default;

domain_error::~domain_error() = default;

}
