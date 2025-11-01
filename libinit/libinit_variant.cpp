/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/libinit_variant.h"

#include "include/libinit_utils.h"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <string>

using android::base::GetProperty;

static const std::string kHwcProp = "ro.boot.hwc";
static const std::string kSkuProp = "ro.boot.product.hardware.sku";

void search_variant(const std::vector<variant_info>& variants) {
    std::string hwc_value = GetProperty(kHwcProp, "");
    std::string sku_value = GetProperty(kSkuProp, "");

    for (const auto& variant : variants) {
        if (variant.hwc_value != "" && variant.hwc_value != hwc_value) {
            continue;
        }

        if (variant.sku_value != "" && variant.sku_value != sku_value) {
            continue;
        }

        set_variant_props(variant);
        return;
    }
}

void set_variant_props(const variant_info& variant) {
    // Older devices don't have marketname
    auto marketname = !variant.marketname.empty() ? variant.marketname : variant.model;

    set_ro_build_prop("brand", variant.brand, true);
    set_ro_build_prop("device", variant.device, true);
    set_ro_build_prop("marketname", marketname, true);
    set_ro_build_prop("model", variant.model, true);
    property_override("vendor.usb.product_string", marketname, true);

    if (access("/system/bin/recovery", F_OK) != 0) {
        property_override("bluetooth.device.default_name", marketname, true);
        set_ro_build_prop("fingerprint", variant.build_fingerprint);
        property_override("ro.bootimage.build.fingerprint", variant.build_fingerprint);

        property_override("ro.build.description",
                          fingerprint_to_description(variant.build_fingerprint));
    }

    if (variant.nfc) {
        property_override(kSkuProp, "nfc");
    }
}
