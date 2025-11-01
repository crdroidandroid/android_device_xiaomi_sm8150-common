/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "vendor_init.h"

#include "include/libinit_dalvik_heap.h"
#include "include/libinit_variant.h"

#include <libvariant.h>

void vendor_load_properties() {
    search_variant(variants);

    set_dalvik_heap();
}
