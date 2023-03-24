// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../dnmx/api.h"
#include "../dnmx/domain_handle.h"
#include "../dnmx/basic_domain.h"

#include "feature_info_fwd.hpp"

#include "feature_id.hpp"
#include "mixin_id.hpp"
#include "mixin_info_fwd.hpp"

#include "allocator.hpp"

#include <string_view>

namespace dynamix {

// a domain is used to register mixins and features
// it manages object types
// an object is always "of a domain"
// mixins and features are not usable across domains
class DYNAMIX_API domain : private dnmx_basic_domain {
public:
    domain(std::string_view name = {}, uintptr_t user_data = 0, void* context = nullptr, allocator alloc = {});
    ~domain();
    domain(const domain&) = delete;
    domain& operator=(const domain&) = delete;
    domain(domain&&) = delete;
    domain& operator=(domain&&) = delete;

    // this value is not used by the library but can be helpful in debugging multiple domains
    [[nodiscard]] std::string_view name() const noexcept { return m_name.to_std(); }

    // registrators
    // all registered infos for a domain are registered by address
    // the user is responsible for preserving the lifetime of the infos
    // with default settings get_type will register all unregistered mixins and features
    void register_feature(feature_info& info); // explicitly registering features is optional

    // unregistrators
    // optionally unregister infos (they must have been registered sucessfully before that)
    // unregistrators also remove all object types which use such an info
    // the user is responsible to make sure that there are no living objects of such types
    // if objects do remain, using them in any way (even to destroy them) is ub (likely crash)
    void unregister_feature(feature_info& info);

    // get registered infos
    // return nullptr if nothing matches the arg
    // these functions are not const, as they are not safe to use where a const domain
    // would be available for the risk of recursive mutex locks
    const feature_info* get_feature_info(feature_id id) noexcept;
    const feature_info* get_feature_info(std::string_view name) noexcept;

    allocator get_allocator() const noexcept;

    dnmx_domain_handle to_c_hanlde() const noexcept { return const_cast<domain*>(this); }
    static domain* from_c_handle(dnmx_domain_handle hd) noexcept { return static_cast<domain*>(hd); }
private:
    // TODO: make into fast pimpl buffer to avoid all allocs
    class impl;
    impl* m_impl;
};

}
