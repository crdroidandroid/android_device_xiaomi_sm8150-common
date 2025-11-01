/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <libvariant.h>

#include <vector>

void search_variant(const std::vector<variant_info>& variants);

void set_variant_props(const variant_info& variant);
