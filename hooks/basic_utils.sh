#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# This module defines basic utilities used in almost all scripts.

# Set environment
export LANG=C LC_ALL=C
export DEBIAN_FRONTEND="noninteractive"
export APT_OPTIONS='-y -o Dpkg::Options::="--force-confdef" \
  -o Dpkg::Options::="--force-confold" --force-yes --no-install-recommends \
  --allow-unauthenticated'

# Absolute path to config file.
# Do not read from/write to this file, call installer_get/installer_set instead.
CONF_FILE=/etc/deepin-installer.conf

# Print error message and exit
error() {
  local msg="$@"
  echo " "
  echo "!! Error: ${msg}" >&2
  echo " "
  exit 1
}

warn() {
  local msg="$@"
  echo "Warn: ${msg}" >&2
}

warn_exit() {
  local msg="$@"
  echo "Warn: ${msg}" >&2
  exit 0
}

# standard message
msg() {
  local msg="$@"
  echo "Info: ${msg}"
}

debug() {
  local msg="$@"
  echo "Debug: ${msg}"
}

# Get value in conf file. Section name is ignored.
# NOTE(xushaohua): Global variant or environment $CONF_FILE must not be empty.
installer_get() {
  local key="$1"
  [ -z "${CONF_FILE}" ] && exit "CONF_FILE is not defined"
  deepin-installer-settings get "${CONF_FILE}" "${key}"
}

# Set value in conf file. Section name is ignored.
installer_set() {
  local key="$1"
  local value="$2"
  [ -z "${CONF_FILE}" ] && exit "CONF_FILE is not defined"
  deepin-installer-settings set "${CONF_FILE}" "${key}" "${value}"
}

# Check whether current platform is loongson or not.
is_loongson() {
  case $(uname -m) in
    loongson | mips*)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

# Check whether current platform is sw or not.
is_sw() {
  case $(uname -m) in
    sw*)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

# Check whether current platform is x86/x86_64 or not.
is_x86() {
  case $(uname -m) in
    i386 | amd64 | x86 | x86_64)
      return 0
      ;;
    *)
      return 1
      ;;
  esac
}

install_from_aur() {
    pwd="$(pwd)"
    cd /tmp/deepin-installer-reborn/aur
    wget "http://aur.archlinux.org/cgit/aur.git/snapshot/$1.tar.gz"
    if [[ $? == 0 ]]; then
        tar xvf $1.tar.gz
        cd $1
        source PKGBUILD
        pacman -S --noconfirm --needed "${makedepends[@]}"
        chown -R nobody .
        sudo -u nobody makepkg
        pacman -U --noconfirm *.pkg.tar.xz
        cd ..
        rm -rf "${1}*"
    else
        error "Failed to download package '$1' from aur"
    fi
    cd $pwd
}