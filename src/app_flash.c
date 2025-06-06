/*
 * Copyright (c) 2025
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

//  ========== include =====================================================================
#include "app_flash.h"

//  ========== app_flash_init ==============================================================
int8_t app_flash_init(struct nvs_fs *fs)
{
	struct flash_pages_info info;
	int8_t ret = 0;
	ssize_t size = 0;

	// configure the storage partition in flash memory
	fs->flash_device = NVS_PARTITION_DEVICE;

	// check if the flash device is ready
	if (!device_is_ready(fs->flash_device)) {
		printk("flash device \"%s\" is not ready\n", fs->flash_device->name);
		return 0;
	} else {
		printk("- found device: \"%s\", getting nvs memory\n", fs->flash_device->name);
	}

	// set the offset of the NVS partition in flash memory
	fs->offset = NVS_PARTITION_OFFSET;

	// retrieve information about the flash memory page at the specified offset
	ret = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (ret) {
		printk("unable to get page info. error: %d\n", ret);
		return ret;
	}
	
	// set the sector size for the NVS partition
	fs->sector_size = info.size;

	// validate the sector size
	if (!fs->sector_size) {
		printk("invalid sector size\n");
		return -1;
	}
	printk("sector size: %d\n", info.size);

	// mount the NVS partition on the flash memory
	fs->sector_count = 4U;		 // number of sectors allocated to the NVS partition
	ret = nvs_mount(fs);
	if (ret) {
		printk("flash to initialize flash memory. error: %d\n", ret);
		return 0;
	}

	return 0;
}

//  ========== app_flash_init_param ========================================================
int8_t app_flash_init_param(struct nvs_fs *fs, uint16_t id, void *data)
{
	int8_t ret = 0;

	// try to read the data associated with the given ID from NVS
	ret = nvs_read(fs, id, data, sizeof(data));
	if (ret > 0) {
		// data exists: print the ID and the associated address
		printk("ID: %d, address: %s\n", id, data);
	} else {
		// no data found: log the absence and write the provided data to NVS
		printk("no address found, adding %s at id %d\n", data, id);
		(void)nvs_write(fs, id, data, sizeof(data));	
	}
	return 0;
}

