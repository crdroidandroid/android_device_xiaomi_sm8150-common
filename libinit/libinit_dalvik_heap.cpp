/*
 * Copyright (C) 2021-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "include/libinit_dalvik_heap.h"

#include "include/libinit_utils.h"

#include <sys/sysinfo.h>
#include <string>

#define GB(b) (b * 1024ull * 1024 * 1024)

static const std::string kHeapStartSizeProp = "dalvik.vm.heapstartsize";
static const std::string kHeapGrowthLimitProp = "dalvik.vm.heapgrowthlimit";
static const std::string kHeapSizeProp = "dalvik.vm.heapsize";
static const std::string kHeapMinFreeProp = "dalvik.vm.heapminfree";
static const std::string kHeapMaxFreeProp = "dalvik.vm.heapmaxfree";
static const std::string kHeapTargetUtilizationProp = "dalvik.vm.heaptargetutilization";

struct dalvik_heap_info {
    std::string heapstartsize;
    std::string heapgrowthlimit;
    std::string heapsize;
    std::string heapminfree;
    std::string heapmaxfree;
    std::string heaptargetutilization;
};

static const dalvik_heap_info dalvik_heap_info_6144 = {
        .heapstartsize = "16m",
        .heapgrowthlimit = "256m",
        .heapsize = "512m",
        .heapminfree = "8m",
        .heapmaxfree = "32m",
        .heaptargetutilization = "0.5",
};

static const dalvik_heap_info dalvik_heap_info_4096 = {
        .heapstartsize = "8m",
        .heapgrowthlimit = "256m",
        .heapsize = "512m",
        .heapminfree = "8m",
        .heapmaxfree = "16m",
        .heaptargetutilization = "0.6",
};

static const dalvik_heap_info dalvik_heap_info_2048 = {
        .heapstartsize = "8m",
        .heapgrowthlimit = "192m",
        .heapsize = "512m",
        .heapminfree = "512k",
        .heapmaxfree = "8m",
        .heaptargetutilization = "0.75",
};

void set_dalvik_heap() {
    struct sysinfo sys;
    const dalvik_heap_info* dhi;

    sysinfo(&sys);

    if (sys.totalram > GB(5)) {
        dhi = &dalvik_heap_info_6144;
    } else if (sys.totalram > GB(3)) {
        dhi = &dalvik_heap_info_4096;
    } else {
        dhi = &dalvik_heap_info_2048;
    }

    property_override(kHeapStartSizeProp, dhi->heapstartsize);
    property_override(kHeapGrowthLimitProp, dhi->heapgrowthlimit);
    property_override(kHeapSizeProp, dhi->heapsize);
    property_override(kHeapMinFreeProp, dhi->heapminfree);
    property_override(kHeapMaxFreeProp, dhi->heapmaxfree);
    property_override(kHeapTargetUtilizationProp, dhi->heaptargetutilization);
}
