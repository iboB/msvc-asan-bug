// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "feature_id.h"

#include "bits/sv.h"

#include <stdbool.h>

#if defined(__cplusplus)
extern "C" {
#endif

// it is not safe to change the values of a feature info once it's registered
typedef struct dnmx_feature_info {
    // will be set by the domain once registered
    dnmx_feature_id id;

    // if domain_settings::unique_feature_names is true (the default),
    // the name must be unique for the domain
    dnmx_sv name;

#if defined(__cplusplus)
    dnmx_id_int_t iid() const noexcept { return id.i; }
#endif
} dnmx_feature_info;

static inline dnmx_feature_info dnmx_make_feature_info(void) {
    dnmx_feature_info ret = DNMX_EMPTY_VAL;
    ret.id.i = dnmx_invalid_id;
    return ret;
}

#if defined(__cplusplus)
}
#endif
