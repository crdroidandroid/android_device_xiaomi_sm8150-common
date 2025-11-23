#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/xiaomi/sm8150-common',
    'hardware/qcom-caf/sm8150',
    'hardware/qcom-caf/wlan',
    'hardware/xiaomi',
    'vendor/qcom/opensource/commonsys/display',
    'vendor/qcom/opensource/commonsys-intf/display',
    'vendor/qcom/opensource/dataservices',
    'vendor/qcom/opensource/display',
]

def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None

lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'com.qualcomm.qti.dpm.api@1.0',
        'libmmosal',
        'vendor.qti.hardware.fm@1.0',
        'vendor.qti.imsrtpservice@3.0',
        'vendor.qti.hardware.wifidisplaysession@1.0',
    ): lib_fixup_vendor_suffix,
}

blob_fixups: blob_fixups_user_type = {
    'system_ext/lib64/libwfdnative.so': blob_fixup()
        .add_needed('libinput_shim.so'),
    'vendor/etc/init/init.mi_thermald.rc': blob_fixup()
        .regex_replace(' +seclabel u:r:mi_thermald:s0\n', ''),
    ('vendor/lib64/libdlbdsservice.so', 'vendor/lib/libstagefright_soft_ac4dec.so', 'vendor/lib/libstagefright_soft_ddpdec.so'): blob_fixup()
        .replace_needed('libstagefright_foundation.so', 'libstagefright_foundation-v33.so'),
    ('vendor/lib64/mediadrm/libwvdrmengine.so', 'vendor/lib64/libwvhidl.so'): blob_fixup()
        .add_needed('libcrypto_shim.so'),
    ('vendor/etc/seccomp_policy/atfwd@2.0.policy', 'vendor/etc/seccomp_policy/wfdhdcphalservice.policy'): blob_fixup()
        .add_line_if_missing('gettid: 1'),
    'vendor/etc/seccomp_policy/vendor.qti.hardware.dsp.policy': blob_fixup()
        .add_line_if_missing('madvise: 1'),
    'vendor/etc/wfdconfig.xml': blob_fixup()
        .regex_replace('<AudioStreamInSuspend>0</AudioStreamInSuspend>', '<AudioStreamInSuspend>1</AudioStreamInSuspend>')
        .regex_replace('<HID>0</HID>', '<HID>1</HID>'),
}  # fmt: skip

module = ExtractUtilsModule(
    'sm8150-common',
    'xiaomi',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

module.add_proprietary_file('proprietary-files-fm.txt').add_copy_files_guard(
    'TARGET_HAS_FM', 'true', invert=True
)

module.add_proprietary_file('proprietary-files-phone.txt').add_copy_files_guard(
    'TARGET_IS_TABLET', 'true', invert=True
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
