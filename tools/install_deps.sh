#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Install package dependencies for development environment.

sudo pacman -Sy --needed \
    cmake \
    gettext \
    pkg-config \
    dosfstools \
    jfstools \
    e2fsprogs \
    os-prober \
    parted \
    squashfs-tools \
    udev \
    xfsprogs \
    btrfs-progs \
    mtools \
    ntfs-3g \
    util-linux \
    wireshark-cli \
    gcc \
    libx11 \
    libxext \
    libxrandr \
    libxss \
    libxtst \
    qt5-x11extras \
    qt5-base \
    qt5-tools \
    zlib \
    xorg-utils \
    xkeyboard-config