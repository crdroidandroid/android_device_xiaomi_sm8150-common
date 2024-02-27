/*
 * Copyright (C) 2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/properties.h>
#include <libinit_utils.h>

#include <libinit_variant.h>

using android::base::GetProperty;

#define HWC_PROP "ro.boot.hwc"
#define SKU_PROP "ro.boot.product.hardware.sku"

void search_variant(const std::vector<variant_info_t> variants) {
    std::string hwc_value = GetProperty(HWC_PROP, "");
    std::string sku_value = GetProperty(SKU_PROP, "");
    bool devicefound = false;
    variant_info_t globalvariant;

    for (const auto& variant : variants) {
        if ((variant.hwc_value == "" || variant.hwc_value == hwc_value) &&
            (variant.sku_value == "" || variant.sku_value == sku_value)) {
            devicefound = true;
            set_variant_props(variant);
            break;
        }
        if (variant.hwc_value == "GLOBAL")
            globalvariant = variant;
    }
    if (!devicefound && globalvariant.hwc_value == "GLOBAL")
        set_variant_props(globalvariant);
}

void set_variant_props(const variant_info_t variant) {
    set_ro_build_prop("brand", variant.brand, true);
    set_ro_build_prop("device", variant.device, true);
    set_ro_build_prop("name", variant.mod_device, true);
    set_ro_build_prop("marketname", variant.marketname, true);
    set_ro_build_prop("model", variant.model, true);
    set_ro_build_prop("cert", variant.model, true);
    set_ro_build_prop("product", variant.device, false);
    set_ro_build_prop("fingerprint", variant.build_fingerprint, false);
    set_ro_build_prop("description", fingerprint_to_description(variant.build_fingerprint), false);

    property_override("ro.com.google.clientidbase", "android-xiaomi");
    property_override("ro.com.google.clientidbase.ms", "android-xiaomi-rev1");
    property_override("ro.product.mod_device", variant.mod_device);
    property_override("bluetooth.device.default_name", variant.marketname);
    property_override("vendor.usb.product_string", variant.marketname);

    if (variant.nfc)
        property_override(SKU_PROP, "nfc");
}
