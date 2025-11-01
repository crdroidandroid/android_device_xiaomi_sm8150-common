/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/libinit_utils.h"

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>
#include <vector>

void property_override(const std::string& prop, const std::string& value, bool add) {
    auto pi = (prop_info*)__system_property_find(prop.c_str());
    if (pi != nullptr) {
        __system_property_update(pi, value.c_str(), value.length());
    } else if (add) {
        __system_property_add(prop.c_str(), prop.length(), value.c_str(), value.length());
    }
}

static const std::vector<std::string> ro_props_default_source_order = {
        "odm.", "odm_dlkm.", "product.", "system.", "system_ext.", "vendor.", "vendor_dlkm.", "",
};

void set_ro_build_prop(const std::string& prop, const std::string& value, bool product) {
    std::string prop_name;

    for (const auto& source : ro_props_default_source_order) {
        if (product) {
            prop_name = "ro.product." + source + prop;
        } else {
            prop_name = "ro." + source + "build." + prop;
        }

        property_override(prop_name, value, true);
    }
}

#define FIND_AND_REMOVE(s, delimiter, variable_name)            \
    std::string variable_name = s.substr(0, s.find(delimiter)); \
    s.erase(0, s.find(delimiter) + delimiter.length());

#define APPEND_STRING(s, to_append) \
    s.append(" ");                  \
    s.append(to_append);

static const std::string kFingerprintDelimiter = "/";
static const std::string kFingerprintDelimiter2 = ":";

std::string fingerprint_to_description(const std::string& fingerprint) {
    std::string build_fingerprint_copy = fingerprint;

    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter, brand)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter, product)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter2, device)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter, platform_version)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter, build_id)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter2, build_number)
    FIND_AND_REMOVE(build_fingerprint_copy, kFingerprintDelimiter, build_variant)
    std::string build_version_tags = build_fingerprint_copy;

    std::string description = product + "-" + build_variant;
    APPEND_STRING(description, platform_version)
    APPEND_STRING(description, build_id)
    APPEND_STRING(description, build_number)
    APPEND_STRING(description, build_version_tags)

    return description;
}
