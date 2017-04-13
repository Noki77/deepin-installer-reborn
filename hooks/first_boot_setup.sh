#!/bin/bash
# Copyright (c) 2016 Deepin Ltd. All rights reserved.
# Use of this source is governed by General Public License that can be found
# in the LICENSE file.

# Setup username, password, timezone and keyboard layout.
# This script is used to setup system info after installation.

# Absolute path to config file.
# Do not read/write this file directly, call installer_get and installer_set
# instead.
CONF_FILE=/etc/deepin-installer.conf

. ./basic_utils.sh

. ./in_chroot/01_setup_locale_timezone.job
. ./in_chroot/03_configure_users.job
. ./in_chroot/04_setup_keyboard.job
. ./in_chroot/05_setup_avatar.job
. ./in_chroot/57_configure_lightdm.job

# Purge installer package
uninstall_installer() {
    affected=("/usr/bin/deepin-installer-*" "/usr/share/applications/deepin-installer-*" "/usr/share/deepin-installer-reborn" "/usr/share/doc/deepin-installer-reborn" "/usr/share/icons/hicolor/scalable/apps/deepin-installer-*" "/usr/share/locale/*/LC_MESSAGES/deepin-installer-timezones.mo" "/usr/share/polkit-1" "/etc/live" "/lib/live" "/lib/systemd/system/systemd-timesyncd.service.d/disable-timesyncd-with-installer.conf")
    for current in "${affected[@]}" do
        rm -rf $current
    done
}

main() {
  setup_avatar && \
  setup_keyboard && \
  setup_locale_timezone && \
  setup_username_password

  # Restore lightdm.conf
  cleanup_reboot_setup_mode

  # Replace default lightdm greeter.
  enable_deepin_lightdm_greeter

  uninstall_installer
}

main
