#
# Copyright (c) 2021 Nordic Semiconductor ASA
#
# SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
#

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(sha256)

target_sources(app PRIVATE
  src/main.c
  src/sdfw_psa_ns_api.c
  # tfm_crypto_api.c provides and serializes the PSA Crypto API.
  ${NRF_DIR}/../modules/tee/tf-m/trusted-firmware-m/interface/src/tfm_crypto_api.c
  )

target_include_directories(app PRIVATE
  # to get tfm_builtin_key_ids.h (and more?)
  ${NRF_DIR}/include/tfm
  src
  )
