#!/bin/bash

make odroidn1_defconfig

make
make u-boot.itb

tools/mkimage -n rk3399 -T rkspi -d spl/u-boot-spl-dtb.bin idbspl.bin

cp idbspl.bin ./sd_fuse/
cp u-boot.itb ./sd_fuse/
