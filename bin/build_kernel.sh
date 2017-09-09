#!/bin/bash

cd /home/saritha/Desktop/AES/linux-4.12.10

make defconfig

make -j4

sudo make modules_install

sudo make install

sudo update-grub2
