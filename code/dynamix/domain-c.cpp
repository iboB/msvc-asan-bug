// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "../dnmx/domain.h"
#include "domain.hpp"

#include <memory>

using namespace dynamix;

extern "C" {

#define self domain::from_c_handle(hd)

dnmx_domain_handle dnmx_create_domain(dnmx_sv name, uintptr_t user_data, void* context) {
    auto new_dom = new dynamix::domain(name.to_std(), user_data, context);
    return new_dom->to_c_hanlde();
}
void dnmx_destroy_domain(dnmx_domain_handle hd) {
    delete self;
}

dnmx_error_return_t dnmx_register_feature(dnmx_domain_handle hd, dnmx_feature_info* info) {
    try {
        self->register_feature(*info);
        return dnmx_result_success;
    }
    catch (std::exception& ex) {
        printf("Error: %s\n", ex.what());
        return -1;
    }
}
void dnmx_unregister_feature(dnmx_domain_handle hd, dnmx_feature_info* info) {
    self->unregister_feature(*info);
}

const dnmx_feature_info* dnmx_get_feature_info_by_id(dnmx_domain_handle hd, dnmx_feature_id id) {
    return self->get_feature_info(id);
}

const dnmx_feature_info* dnmx_get_feature_info_by_name(dnmx_domain_handle hd, dnmx_sv name) {
    return self->get_feature_info(name.to_std());
}

}
