// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "domain.hpp"
#include "exception.hpp"
#include "mixin_info.hpp"
#include "feature_info.hpp"

#include "bits/data_mutex.hpp"

#include <vector>
#include <cassert>
#include <shared_mutex>

namespace dynamix {

#define BAD_ARG_UNLESS(cond, err) if (!(cond)) do { assert(false); throw domain_error(err); } while(false)

class type;

class domain::impl {
public:
    domain& m_domain;

    allocator m_allocator;

    static const mixin_id& id_from_mixin_info(const mixin_info* const& i) { return i->id; }

    struct registry {
        registry(allocator alloc)
            : sparse_features(alloc)
            , sparse_mixins(alloc)
        {}

        // sparse arrays of info per id
        // null elements have been unregistered and are free slots for future registers
        std::pmr::vector<const feature_info*> sparse_features;
        std::pmr::vector<const mixin_info*> sparse_mixins;
    };

    // registry m_registry;
    itlib::data_mutex<registry, std::shared_mutex> m_registry;

    impl(domain& domain, allocator alloc)
        : m_domain(domain)
        , m_allocator(std::move(alloc))
        , m_registry(m_allocator)
    {
    }

    template <typename T>
    static void basic_register_l(T& info, std::pmr::vector<const T*>& sparse, bool enforce_unique_names) {
        using id_t = decltype(info.id);
        constexpr id_t invalid = id_t{dnmx_invalid_id};
        BAD_ARG_UNLESS(info.id == invalid, "id registered");
        if (info.name.empty()) {
            throw domain_error("empty info name");
        }

        id_t free_id = invalid;

        if (enforce_unique_names) {
            // search in reverse order while also checking for name clashes
            // after this loop, the least free_id will be saved
            for (dnmx_id_int_t i = dnmx_id_int_t(sparse.size()); i-- > 0; ) {
                auto reg = sparse[i];
                if (reg) {
                    assert(reg->iid() == i); // sanity check
                    if (reg->name == info.name) throw domain_error("duplicate name");
                }
                else {
                    free_id = id_t{i};
                }
            }
        }

        if (free_id == invalid) {
            free_id = id_t{dnmx_id_int_t(sparse.size())};
            sparse.push_back(nullptr);
        }

        info.id = free_id;
        sparse[info.iid()] = &info;
    }

    template <typename T>
    static void basic_unregister_l(T& info, std::pmr::vector<const T*>& sparse) {
        sparse[info.iid()] = nullptr; // free slot
        info.id = decltype(info.id){dnmx_invalid_id}; // invalidate info
    }

    void register_feature(feature_info& info) {
        basic_register_l(info, m_registry.unique_lock()->sparse_features, true);
    }

    // to be pedantic when we clear features we should clear all object types which
    // implement them, but this seems to be unnecessary
    // if a feature has found its way into a type info, then there must be a mixin that uses it
    // if the feature is local to a module, then that mixin must be local to the module too
    // since the feature is being unregistered because the module is unloaded,
    // then surely the mixin should be unregistered too
    // then all types infos containing that mixin (and in turn this feature)
    // will be dropped eventually
    // for a short while we will live with incosistent lists of feature and mixins
    // if something breaks because of this inconsitency, then this would have been a
    // break anyway - the mixins referencing this feature would keep on living??
    void unregister_feature(feature_info& info) {
        basic_unregister_l(info, m_registry.unique_lock()->sparse_features);
    }

    void register_mixin(mixin_info& info) {
        // register mixin itself
        basic_register_l(info, m_registry.unique_lock()->sparse_mixins, true);
    }

    template <typename Id, typename T>
    static const T* basic_get_by_id_l(Id id, const std::pmr::vector<const T*>& sparse) noexcept {
        if (id.i >= sparse.size()) return nullptr;
        return sparse[id.i];
    }

    template <typename T>
    static const T* basic_get_by_name_l(std::string_view name, const std::pmr::vector<const T*>& sparse) noexcept {
        for (auto& info : sparse) {
            if (info && info->name == name) return info;
        }
        return nullptr;
    }

    const mixin_info* get_mixin_info(mixin_id id) noexcept {
        return basic_get_by_id_l(id, m_registry.shared_lock()->sparse_mixins);
    }
    const mixin_info* get_mixin_info(std::string_view name) noexcept {
        return basic_get_by_name_l(name, m_registry.shared_lock()->sparse_mixins);
    }

    const feature_info* get_feature_info(feature_id id) noexcept {
        return basic_get_by_id_l(id, m_registry.shared_lock()->sparse_features);
    }
    const feature_info* get_feature_info(std::string_view name) noexcept {
        return basic_get_by_name_l(name, m_registry.shared_lock()->sparse_features);
    }
};

domain::domain(std::string_view name, uintptr_t user_data, void* context, allocator alloc)
    : dnmx_basic_domain{dnmx_sv::from_std(name), user_data, context}
    , m_impl(new impl(*this, alloc))
{}

domain::~domain() {
    delete m_impl;
}

void domain::register_feature(feature_info& info) {
    m_impl->register_feature(info);
}

void domain::unregister_feature(feature_info& info) {
    m_impl->unregister_feature(info);
}

const feature_info* domain::get_feature_info(feature_id id) noexcept {
    return m_impl->get_feature_info(id);
}

const feature_info* domain::get_feature_info(std::string_view name) noexcept {
    return m_impl->get_feature_info(name);
}

}
