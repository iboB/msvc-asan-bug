// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "bits/sv.h"

#include "size.h"
#include "mixin_id.h"

#if defined(__cplusplus)
extern "C" {
#endif
struct dnmx_mixin_info {
    // will be set by the domain once registered
    // having a valid id implies the type info is registered with some domain
    // to register a type in multiple domains, you need make copies of the info per domain
    dnmx_mixin_id id;

    // name of mixin
    // if domain_settings::unique_mixin_names is true (the default),
    // the name must be unique for the domain
    // this is the only value without a sane default
    dnmx_sv name;

#if defined(__cplusplus)
    dnmx_id_int_t iid() const noexcept { return id.i; }
#endif
};

#if defined(__cplusplus)
} // extern "C"
#endif
