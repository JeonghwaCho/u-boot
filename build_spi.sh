#!/bin/bash

make odroidn1_defconfig

make

tools/rk_tools/bin/loaderimage --pack --uboot ./u-boot-dtb.bin uboot.img

tools/mkimage -n rk3399 -T rkspi -d tools/rk_tools/bin/rk33/rk3399_ddr_800MHz_v1.08.bin idbloader_spi.img
cat tools/rk_tools/bin/rk33/rk3399miniloader_spi_v121301.bin >> idbloader_spi.img

tools/rk_tools/bin/trust_merger tools/rk_tools/trust.ini

cp idbloader_spi.img sd_fuse
cp uboot.img sd_fuse
cp trust.img sd_fuse

