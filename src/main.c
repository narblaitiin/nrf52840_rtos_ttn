/*
 * Copyright (c) 2025
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

//  ========== includes ====================================================================
#include "app_lorawan.h"

//  ========== globals =====================================================================
// define GPIO specifications for the LEDs used to indicate transmission (TX) and reception (RX)
static const struct gpio_dt_spec led_tx = GPIO_DT_SPEC_GET(LED_TX, gpios);
static const struct gpio_dt_spec led_rx = GPIO_DT_SPEC_GET(LED_RX, gpios);

//char payload[] =  {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

static void dl_callback(uint8_t port, bool data_pending,
			int16_t rssi, int8_t snr,
			uint8_t len, const uint8_t *hex_data)
{
	printk("Port %d, Pending %d, RSSI %ddB, SNR %ddBm", port, data_pending, rssi, snr);
	// if (hex_data) {
	// 	printk(hex_data, len, "Payload: ");
	// }
}

static void lorwan_datarate_changed(enum lorawan_datarate dr)
{
	uint8_t unused, max_size;

	lorawan_get_payload_sizes(&unused, &max_size);
	printk("New Datarate: DR_%d, Max Payload %d", dr, max_size);
}

//  ========== main ========================================================================
int8_t main(void)
{
	char payload[] =  {'h', 'e', 'l', 'l', 'o', 'w', 'o', 'r', 'l', 'd'};

	// configure LEDs for TX and RX indication
	gpio_pin_configure_dt(&led_tx, GPIO_OUTPUT_ACTIVE);
	gpio_pin_configure_dt(&led_rx, GPIO_OUTPUT_ACTIVE);
	gpio_pin_set_dt(&led_tx, 0);		// turn off TX LED
	gpio_pin_set_dt(&led_rx, 0);		// turn off TX LED

	// initialize LoRaWAN protocol and register the device
	// int8_t ret = app_lorawan_init();
	// if (ret != 1) {
	// 	printk("failed to initialze LoRaWAN protocol\n");
	// 	return 0;
	// }

	const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;
	int ret;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		printk("%s: device not ready.", lora_dev->name);
		return 0;
	}

	ret = lorawan_start();
	if (ret < 0) {
		printk("lorawan_start failed: %d", ret);
		return 0;
	}

	lorawan_register_downlink_callback(&downlink_cb);
	lorawan_register_dr_changed_callback(lorwan_datarate_changed);

	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;
	join_cfg.otaa.dev_nonce = 0u;

	printk("Joining network over OTAA");
	ret = lorawan_join(&join_cfg);
	if (ret < 0) {
		printk("lorawan_join_network failed: %d", ret);
		return 0;
	}

	printk("Test of LoRaWAN and TTN\n");

	// start the main loop for data simulation and transmission
	for (int8_t i = 0; i < 5; i++) {

		// indicate data transmission with the TX LED
		printk("sending random data...\n");
		gpio_pin_set_dt(&led_tx, 1);

		// send the payload over LoRaWAN (unconfirmed message)
		ret = lorawan_send(LORAWAN_PORT, payload, sizeof(payload), LORAWAN_MSG_CONFIRMED);
		
		// handle transmission errors
		if (ret == -EAGAIN) {
			printk("LoRaWAN send failed (retry). error: %d\n", ret);
			gpio_pin_set_dt(&led_tx, 0);
			k_sleep(K_SECONDS(10));
			continue;
		}
		
		// handle transmission errors
		if (ret < 0) {
			printk("LoRaWAN send failed. error: %d. retrying\n", ret);
			gpio_pin_set_dt(&led_tx, 0);
        	// k_sleep(K_SECONDS(10));
        	// continue;
			return 0;
		}

		printk("data sent successfully!\n");
		gpio_pin_set_dt(&led_tx, 0);

		// wait before the next iteration
		k_sleep(DELAY);
	}
	return 0;
}