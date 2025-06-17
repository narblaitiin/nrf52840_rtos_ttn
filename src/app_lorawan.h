/*
 * Copyright (c) 2025
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_LORAWAN_H
#define APP_LORAWAN_H

//  ========== includes ====================================================================
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/lora.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/random/random.h>

//  ========== defines =====================================================================
#define LED_TX                  DT_ALIAS(ledtx)     // declared in device tree 
#define LED_RX                  DT_ALIAS(ledrx)     // declared in device tree
#define DELAY 			        K_MSEC(20000)

// customize based on network configuration
#define LORAWAN_DEV_EUI			{ 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x06, 0xB4, 0x67 }
#define LORAWAN_JOIN_EUI		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
#define LORAWAN_APP_KEY			{ 0xC7, 0x32, 0x0F, 0x37, 0xFF, 0x62, 0xE0, 0xA8, 0x4E, 0x94, 0xC1, 0x9C, 0x27, 0x2B, 0xFA, 0x4C }
#define LORAWAN_PORT            2       // application port
#define MAX_JOIN_ATTEMPTS       10      // limiting join attempts

//  ========== prototypes ==================================================================
int8_t app_lorawan_init(void);

#endif /* APP_LORAWAN_H */