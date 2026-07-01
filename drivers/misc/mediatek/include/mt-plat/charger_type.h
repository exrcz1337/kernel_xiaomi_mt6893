/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2021 MediaTek Inc.
*/

#ifndef __MTK_CHARGER_TYPE_H__
#define __MTK_CHARGER_TYPE_H__

#include <linux/ktime.h>
#include <linux/notifier.h>
#include <linux/mutex.h>
#include <linux/timer.h>
#include <linux/wait.h>
#include <linux/hrtimer.h>
#include <linux/spinlock.h>

/* charger_manager notify charger_consumer */
enum {
	CHARGER_NOTIFY_EOC,
	CHARGER_NOTIFY_START_CHARGING,
	CHARGER_NOTIFY_STOP_CHARGING,
	CHARGER_NOTIFY_ERROR,
	CHARGER_NOTIFY_NORMAL,
};

enum {
	MAIN_CHARGER = 0,
	SLAVE_CHARGER = 1,
	TOTAL_CHARGER = 2,
	DIRECT_CHARGER = 10,
	MAIN_DIVIDER_CHARGER = 20,
	SLAVE_DIVIDER_CHARGER = 21,
};
struct charger_consumer {
	struct device *dev;
	void *cm;
	struct notifier_block *pnb;
	struct list_head list;
	bool hv_charging_disabled;
};

/* ============================================= */
/* The following are charger consumer interfaces */
/* ============================================= */

/* @supply_name: name of charging port
 * use charger_port1, charger_port2, ...
 * for most cases, use charging_port1
 */
extern struct charger_consumer *charger_manager_get_by_name(
	struct device *dev,
	const char *supply_name);
extern int charger_manager_get_input_current_limit(
	struct charger_consumer *consumer,
	int idx,
	int *input_current_uA);
extern int charger_manager_set_input_current_limit(
	struct charger_consumer *consumer,
	int idx,
	int input_current_uA);
extern int charger_manager_set_charging_current_limit(
	struct charger_consumer *consumer,
	int idx,
	int charging_current_uA);
extern int charger_manager_set_pe30_input_current_limit(
	struct charger_consumer *consumer,
	int idx,
	int input_current_uA);
extern int charger_manager_get_pe30_input_current_limit(
	struct charger_consumer *consumer,
	int idx,
	int *input_current_uA,
	int *min_current_uA,
	int *max_current_uA);
extern int charger_manager_get_current_charging_type(
	struct charger_consumer *consumer);
extern int register_charger_manager_notifier(
	struct charger_consumer *consumer,
	struct notifier_block *nb);
extern int charger_manager_get_charger_temperature(
	struct charger_consumer *consumer,
	int idx,
	int *tchg_min,
	int *tchg_max);
extern int unregister_charger_manager_notifier(
	struct charger_consumer *consumer,
	struct notifier_block *nb);
extern int charger_manager_enable_high_voltage_charging(
	struct charger_consumer *consumer,
	bool en);
extern int charger_manager_enable_power_path(
	struct charger_consumer *consumer,
	int idx,
	bool en);
extern int charger_manager_force_disable_power_path(
	struct charger_consumer *consumer,
	int idx,
	bool disable);
extern int charger_manager_enable_charging(
	struct charger_consumer *consumer,
	int idx,
	bool en);
extern int charger_manager_get_zcv(
	struct charger_consumer *consumer,
	int idx,
	u32 *uV);
extern int charger_manager_enable_chg_type_det(
	struct charger_consumer *consumer,
	bool en);
extern int charger_manager_pd_is_online(void);
extern int charger_manager_get_ibus(int* ibus);
extern int charger_manager_set_input_suspend(int suspend);
extern int mtk_get_prop_input_suspend(void);
extern int mtk_set_prop_input_suspend(int suspend);
extern int charger_manager_is_input_suspend(void);
extern int charger_manager_get_prop_system_brightness(void);
extern int charger_manager_get_prop_system_temp_scene(void);
extern int charger_manager_get_prop_system_temp_level(void);
extern int charger_manager_get_prop_system_temp_level_max(void);
extern void charger_manager_set_prop_system_temp_level(int temp_level);
extern int mtk_chr_is_charger_exist(unsigned char *exist);
extern bool is_power_path_supported(void);
extern int charger_get_vbus(void);
extern bool mt_charger_plugin(void);
extern int get_thermal_charge_ctrl_scene(void);
extern int mt_leds_brightness_get(void);

/* K16 */
extern int charger_manager_get_thermal_limit_fcc(void);
extern void charger_manager_set_thermal_limit_fcc(int thermal_limit_fcc);
extern int charger_manager_get_sic_current(void);
extern void charger_manager_set_sic_current(int sic_current);
extern int charger_manager_get_input_suspend(void);
extern void charger_manager_set_input_suspend_pissarro(bool input_suspend);
extern int charger_manager_get_thermal_level(void);
extern int charger_manager_get_max_thermal_level(void);
extern void charger_manager_set_thermal_level(int thermal_level);
extern int charger_manager_get_charge_status(void);
extern int charger_manager_get_battery_health(void);

/* weak function for other module */
int __attribute__((weak)) get_thermal_charge_ctrl_scene(void)
{
	return 0;
}

int __attribute__((weak)) mt_leds_brightness_get(void)
{
	return 0;
}
enum charger_type {
	CHARGER_UNKNOWN = 0,
	STANDARD_HOST,		/* USB : 450mA */
	CHARGING_HOST,
	NONSTANDARD_CHARGER,	/* AC : 450mA~1A */
	STANDARD_CHARGER,	/* AC : ~1A */
	APPLE_2_1A_CHARGER, /* 2.1A apple charger */
	APPLE_1_0A_CHARGER, /* 1A apple charger */
	APPLE_0_5A_CHARGER, /* 0.5A apple charger */
	WIRELESS_CHARGER,
};
#if defined(CONFIG_USB_MTK_HDRC) || defined(CONFIG_USB_MU3D_DRV) \
	|| defined(CONFIG_EXTCON_MTK_USB)
extern void mt_usb_connect_v1(void);
extern void mt_usb_disconnect_v1(void);
#else
#define mt_usb_connect() do { } while (0)
#define mt_usb_disconnect() do { } while (0)
#endif

extern enum charger_type mt_get_charger_type(void);
extern void mtk_charger_int_handler(void);

#if defined(CONFIG_MTK_PUMP_EXPRESS_PLUS_30_SUPPORT)
extern int register_charger_det_callback(int (*func)(int));
#endif /*CONFIG_MTK_PUMP_EXPRESS_PLUS_30_SUPPORT*/

#if defined(CONFIG_MACH_MT6877) || defined(CONFIG_MACH_MT6893) \
	|| defined(CONFIG_MACH_MT6885) || defined(CONFIG_MACH_MT6785) \
	|| defined(CONFIG_MACH_MT6853) || defined(CONFIG_MACH_MT6873)
extern bool is_usb_rdy(struct device *dev);
#else
extern bool is_usb_rdy(void);
#endif
extern bool mt_usb_is_device(void);
extern int is_otg_en(void);

#ifndef CONFIG_MTK_EXTERNAL_CHARGER_TYPE_DETECT
extern void mtk_pmic_enable_chr_type_det(bool en);
#endif

#endif /* __MTK_CHARGER_TYPE_H__ */
