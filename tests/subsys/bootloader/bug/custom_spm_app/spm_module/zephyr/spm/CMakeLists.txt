#
# Copyright (c) 2019 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
#
cmake_minimum_required(VERSION 3.13.1)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(NONE)

zephyr_include_directories(
  kernel/include
  ${ARCH_DIR}/${ARCH}/include
)

list(APPEND spm_DTC_OVERLAY_FILE "/workspaces/recovrdevice_wsl_act/samples/custom_spm_app/spm_module/zephyr/spm/nrf9160dk_nrf9160.overlay")
add_compile_definitions(
  spm_DTC_OVERLAY_FILE=/workspaces/recovrdevice_wsl_act/samples/custom_spm_app/spm_module/zephyr/spm/nrf9160dk_nrf9160.overlay
    )
zephyr_library_sources(src/main.c)
