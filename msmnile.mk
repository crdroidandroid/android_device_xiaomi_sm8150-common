#
# Copyright (C) 2021 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

PRODUCT_BUILD_SUPER_PARTITION := false
PRODUCT_USE_DYNAMIC_PARTITIONS := true

# Inherit the proprietary files
$(call inherit-product, vendor/xiaomi/sm8150-common/sm8150-common-vendor.mk)
