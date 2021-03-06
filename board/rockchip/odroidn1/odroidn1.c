/*
 * (C) Copyright 2017 Hardkernel Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */
#include <common.h>
#include <dm.h>
#include <ram.h>
#include <dm/pinctrl.h>
#include <dm/uclass-internal.h>
#include <asm/arch/periph.h>
#include <power/regulator.h>
#include <usb.h>
#include <dwc3-uboot.h>

DECLARE_GLOBAL_DATA_PTR;

extern int board_scan_boot_storage(void);

#ifdef CONFIG_BOARD_LATE_INIT
int board_late_init(void)
{
#ifndef CONFIG_SPI_FLASH
	if (board_scan_boot_storage() != 0) {
		printf("board: scan boot stoarge fail\n");
		return -1;
	}
#endif
	return 0;
}
#endif

int board_init(void)
{
	struct udevice *pinctrl, *regulator;
	int ret;

	/*
	 * The PWM do not have decicated interrupt number in dts and can
	 * not get periph_id by pinctrl framework, so let's init them here.
	 * The PWM2 and PWM3 are for pwm regulater.
	 */
	ret = uclass_get_device(UCLASS_PINCTRL, 0, &pinctrl);
	if (ret) {
		debug("%s: Cannot find pinctrl device\n", __func__);
		goto out;
	}

	/* Enable pwm0 for panel backlight */
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_PWM0);
	if (ret) {
		debug("%s PWM0 pinctrl init fail! (ret=%d)\n", __func__, ret);
		goto out;
	}

	/* Enable pwm2 to control regulator vdd-log */
	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_PWM2);
	if (ret) {
		debug("%s PWM2 pinctrl init fail!\n", __func__);
		goto out;
	}

	ret = pinctrl_request_noflags(pinctrl, PERIPH_ID_PWM3);
	if (ret) {
		debug("%s PWM3 pinctrl init fail!\n", __func__);
		goto out;
	}

	ret = regulators_enable_boot_on(false);
	if (ret)
		debug("%s: Cannot enable boot on regulator\n", __func__);

	ret = regulator_get_by_platname("vcc5v0_host", &regulator);
	if (ret) {
		debug("%s vcc5v0_host init fail! ret %d\n", __func__, ret);
		goto out;
	}

	ret = regulator_set_enable(regulator, true);
	if (ret) {
		debug("%s vcc5v0-host-en set fail!\n", __func__);
		goto out;
	}

	/* Enable regulator vdd_log to supply LOGIC_VDD on ODROID-N1 HW */
	ret = regulator_get_by_platname("vdd_log", &regulator);
	if (ret) {
		printf("%s vdd_log init fail! ret %d\n", __func__, ret);
		goto out;
	}

	ret = regulator_set_enable(regulator, true);
	if (ret) {
		printf("%s vdd_log set fail!\n", __func__);
		goto out;
	}

out:
	return 0;
}

#ifdef CONFIG_USB_DWC3
static struct dwc3_device dwc3_device_data = {
	.maximum_speed = USB_SPEED_HIGH,
	.base = 0xfe800000,
	.dr_mode = USB_DR_MODE_HOST,
	.index = 0,
	.dis_u2_susphy_quirk = 1,
};

int usb_gadget_handle_interrupts(void)
{
	dwc3_uboot_handle_interrupt(0);
	return 0;
}

int board_usb_init(int index, enum usb_init_type init)
{
	return dwc3_uboot_init(&dwc3_device_data);
}
#endif
