# Install script for directory: /home/sebo/ncs/zephyr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "TRUE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/home/sebo/bin/zephyr-sdk-1.0.1/gnu/arm-zephyr-eabi/bin/arm-zephyr-eabi-objdump")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/arch/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/lib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/soc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/boards/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/subsys/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/drivers/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/nrf/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/mcuboot/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/mbedtls/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/trusted-firmware-m/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cjson/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/azure-sdk-for-c/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cirrus-logic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/openthread/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/memfault-firmware-sdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/hostap/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/canopennode/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/chre/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cmsis/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cmsis-dsp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cmsis-nn/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/cmsis_6/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/dhara/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/fatfs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/hal_nordic/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/hal_st/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/hal_tdk/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/hal_wurthelektronik/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/liblc3/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/libmetal/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/libsbc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/littlefs/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/loramac-node/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/lvgl/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/mipi-sys-t/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/nanopb/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/nrf_wifi/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/open-amp/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/percepio/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/picolibc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/segger/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/uoscore-uedhoc/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/zcbor/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/nrfxlib/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/nrf_hw_models/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/modules/connectedhomeip/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/kernel/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/cmake/flash/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/cmake/usage/cmake_install.cmake")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for the subdirectory.
  include("/home/sebo/ncs/nrf/samples/ironside_se/snapshot_capture_recover/build/snapshot_capture_recover/zephyr/cmake/reports/cmake_install.cmake")
endif()

