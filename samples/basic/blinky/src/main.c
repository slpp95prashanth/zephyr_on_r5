/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

/* 1000 msec = 1 sec */
#define SLEEP_TIME_MS   100

/* The devicetree node identifier for the "led0" alias. */
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define LED3_NODE DT_ALIAS(led3)
#define LED4_NODE DT_ALIAS(led4)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(LED4_NODE, gpios);

int main(void)
{
    printk("blinky\n");
	int ret;

	if (!gpio_is_ready_dt(&led0)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&led3, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	ret = gpio_pin_configure_dt(&led4, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return 0;
	}
	while (1) {
  printk("timer_isr: uptime_ticks = %lld uptime_get = %lld\n", k_uptime_ticks(), k_uptime_get());
    printk("blinky board = %s\n", CONFIG_BOARD);
		ret = gpio_pin_toggle_dt(&led0);
		if (ret < 0) {
			return 0;
		}
		ret = gpio_pin_toggle_dt(&led1);
		if (ret < 0) {
			return 0;
		}
		ret = gpio_pin_toggle_dt(&led2);
		if (ret < 0) {
			return 0;
		}
		ret = gpio_pin_toggle_dt(&led3);
		if (ret < 0) {
			return 0;
		}
		ret = gpio_pin_toggle_dt(&led4);
		if (ret < 0) {
			return 0;
		}
    printk("blinky\n");
		k_msleep(SLEEP_TIME_MS);
	}
	return 0;
}
