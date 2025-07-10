/*
 * Copyright (c) 2025
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_NVS_H
#define APP_NVS_H

//  ========== includes ====================================================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/flash.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

//  ========== defines =====================================================================
#define NVS_PARTITION			storage_partition
#define NVS_PARTITION_DEVICE	FIXED_PARTITION_DEVICE(NVS_PARTITION)
#define NVS_PARTITION_OFFSET	FIXED_PARTITION_OFFSET(NVS_PARTITION)  
#define NVS_DEVNONCE_ID			0                     

//  ========== prototypes ==================================================================
int8_t app_nvs_init(struct nvs_fs *fs);
int8_t app_nvs_init_param(struct nvs_fs *fs, uint16_t id, void *data);

#endif /* APP_NVS_H */