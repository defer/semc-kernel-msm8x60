/* arch/arm/mach-msm/keypad-semc_fuji_cdb.c
 *
 * Copyright (C) [2011] Sony Ericsson Mobile Communications AB.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2, as
 * published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 */
#include <linux/input/pmic8058-keypad.h>
#ifdef CONFIG_INPUT_ASETM2034A
#include <linux/input/asetm2034a.h>
#endif /* CONFIG_INPUT_ASETM2034A */
#ifdef CONFIG_FUJI_PMIC_KEYPAD
#include <linux/mfd/pmic8058.h>
#include "keypad-pmic-fuji.h"
#include "gpiomux-semc_fuji.h"
#endif /* CONFIG_FUJI_PMIC_KEYPAD */
#ifdef CONFIG_FUJI_GPIO_KEYPAD
#include <linux/gpio_event.h>
#endif /* CONFIG_FUJI_GPIO_KEYPAD */

static const unsigned int fuji_keymap[] = {
	KEY(0, 0, KEY_BACK),
	KEY(0, 1, KEY_HOME),
	KEY(0, 2, KEY_MENU),

	KEY(1, 0, KEY_CAMERA_FOCUS),
	KEY(1, 1, KEY_CAMERA),
#ifndef CONFIG_FUJI_GPIO_KEYPAD
	KEY(1, 2, KEY_VOLUMEDOWN),
#endif /* !CONFIG_FUJI_GPIO_KEYPAD */
#ifndef CONFIG_FUJI_PMIC_KEYPAD
	KEY(1, 3, KEY_VOLUMEUP),
#endif /* !CONFIG_FUJI_PMIC_KEYPAD */
};

static struct matrix_keymap_data fuji_keymap_data = {
	.keymap_size	= ARRAY_SIZE(fuji_keymap),
	.keymap		= fuji_keymap,
};

struct pmic8058_keypad_data fuji_keypad_data = {
	.input_name		= "fuji-keypad",
	.input_phys_device	= "fuji-keypad/input0",
	.num_rows		= 5,
	.num_cols		= 5,
	.rows_gpio_start	= 8,
	.cols_gpio_start	= 0,
	.debounce_ms		= {8, 10},
	.scan_delay_ms		= 2,
	.row_hold_ns            = 122000,
	.wakeup			= 1,
	.keymap_data		= &fuji_keymap_data,
};

#ifdef CONFIG_INPUT_ASETM2034A
static struct asetm2034a_key asetm2034a_keys[] = {
	{ ASETM2034A_SW1,	KEY_MENU	},
	{ ASETM2034A_SW2,	KEY_HOME	},
	{ ASETM2034A_SW3,	KEY_BACK	},
	{ ASETM2034A_IDLE2,	KEY_WAKEUP	}, /* temporary */
};

struct asetm2034a_keymap asetm2034a_keymap = {
	.keys = asetm2034a_keys,
	.num_keys = ARRAY_SIZE(asetm2034a_keys),
};
#endif /* CONFIG_INPUT_ASETM2034A */

#ifdef CONFIG_FUJI_PMIC_KEYPAD
#define GPIO_KEYPAD_VOLUME_UP_KEY	26

static struct pm8058_gpio pm_gpio_config = {
	.direction    = PM_GPIO_DIR_IN,
	.pull         = PM_GPIO_PULL_NO,
	.vin_sel      = PM_GPIO_VIN_S3,
	.out_strength = PM_GPIO_STRENGTH_NO,
	.function     = PM_GPIO_FUNC_NORMAL,
	.inv_int_pol  = 1,
};

static struct keypad_pmic_fuji_key keymap_pmic[] = {
	{
		.code = KEY_VOLUMEUP,
		.irq  = PM8058_GPIO_IRQ(PM8058_IRQ_BASE,
				GPIO_KEYPAD_VOLUME_UP_KEY - 1),
		.gpio = GPIO_KEYPAD_VOLUME_UP_KEY,
		.wake = 1,
		.debounce_time.tv.nsec = 10 * NSEC_PER_MSEC,
	},
};

struct keypad_pmic_fuji_platform_data keypad_pmic_platform_data = {
	.keymap = keymap_pmic,
	.keymap_size = ARRAY_SIZE(keymap_pmic),
	.input_name = "keypad-pmic-fuji",
	.pm_gpio_config = &pm_gpio_config,
};
#endif /* CONFIG_FUJI_PMIC_KEYPAD */

#ifdef CONFIG_FUJI_GPIO_KEYPAD
#define GPIO_KEYPAD_VOLUME_DOWN_KEY	73

static struct gpio_event_direct_entry gpio_key_gpio_map[] = {
	{GPIO_KEYPAD_VOLUME_DOWN_KEY, KEY_VOLUMEDOWN},
};

static struct gpio_event_input_info gpio_key_gpio_info = {
	.info.func = gpio_event_input_func,
	.flags = GPIOEDF_ACTIVE_HIGH,
	.type = EV_KEY,
	.keymap = gpio_key_gpio_map,
	.keymap_size = ARRAY_SIZE(gpio_key_gpio_map),
	.debounce_time.tv.nsec = 10 * NSEC_PER_MSEC,
};

static struct gpio_event_info *gpio_key_info[] = {
	&gpio_key_gpio_info.info,
};

struct gpio_event_platform_data gpio_key_data = {
	.name		= "gpio-key",
	.info		= gpio_key_info,
	.info_count	= ARRAY_SIZE(gpio_key_info),
};
#endif /* CONFIG_FUJI_GPIO_KEYPAD */
