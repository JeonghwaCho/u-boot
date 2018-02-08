/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef __EVB_RK3399_H
#define __EVB_RK3399_H

#include <configs/rk3399_common.h>

#ifdef CONFIG_SPI_FLASH
#define CONFIG_ENV_IS_IN_SPI_FLASH
#define CONFIG_ENV_SPI_BUS              0
#define CONFIG_ENV_SPI_CS               0
#define CONFIG_ENV_SPI_MAX_HZ           10000000
#define CONFIG_ENV_SPI_MODE             0
#undef CONFIG_ENV_OFFSET
#define CONFIG_ENV_OFFSET               0x140000        /* 1MB+256kB */
#define CONFIG_ENV_SECT_SIZE            0x10000
#else
#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_SYS_MMC_ENV_DEV 0
/*
 * SPL @ 32k for ~128k
 * ENV @ 240k
 * u-boot @ 256K
 */
#undef CONFIG_ENV_OFFSET
#define CONFIG_ENV_OFFSET (240 * 1024)
#endif

#define SDRAM_BANK_SIZE			(2UL << 30)

#define CONFIG_BOOTP_BOOTPATH
#define CONFIG_BOOTP_DNS
#define CONFIG_BOOTP_GATEWAY
#define CONFIG_BOOTP_HOSTNAME
#define CONFIG_BOOTP_PXE
#define CONFIG_BOOTP_SUBNETMASK

#define CONFIG_CMDLINE_EDITING
#define CONFIG_AUTO_COMPLETE
#define CONFIG_SYS_LONGHELP
#define CONFIG_SUPPORT_RAW_INITRD
#define CONFIG_ENV_VARS_UBOOT_CONFIG

#define CONFIG_MTD_DEVICE
#define CONFIG_MTD_PARTITION
#define CONFIG_CMD_MTDPARTS

#define MTDIDS_DEFAULT		"nor0=spi-flash.0"
#define MTDPARTS_DEFAULT	"mtdparts=spi-flash.0:" \
				"32k(reseved1),224k(idbspl)," \
				"1m(uboot),8k(env)," \
				"7m(kernel),100k(dtb),7m(initrd)"

#undef CONFIG_EXTRA_ENV_SETTINGS
#define CONFIG_EXTRA_ENV_SETTINGS	\
	"setbootargs=setenv bootargs earlyprintk swiotlb=1 "		\
		"console=ttyFIQ0,115200n8 "				\
		"rw root=/dev/sda2 rootfstype=ext4 rootwait "	\
		"mtdparts="MTDPARTS_DEFAULT"\0"	\
	"bootcmd=sf probe; mtdparts; "		\
		"sf read 0x01000000 kernel; "	\
		"unzip 0x01000000 0x02000000 "	\
		"sf read 0x04000000 initrd; "	\
		"sf read 0x01f00000 dtb; "	\
		"booti 0x02000000 - 0x01f00000\0"

#define CONFIG_BOARD_LATE_INIT

#endif
