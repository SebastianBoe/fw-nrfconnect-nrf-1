.. _ironside_se_snapshot_capture_recover:

IronSide SE: Snapshot capture and recover
#########################################

.. contents::
   :local:
   :depth: 2

Overview
********

This sample shows IronSide SE snapshot capture and recovery while an NV counter advances across boots.

UICR configuration:

* **PROTECTEDMEM** — Protects ``cpuapp_boot_partition`` and ``periphconf_partition`` (72 KB), consistent with other IronSide SE samples that place periphconf immediately after the boot partition.
* **UICR.SNAPSHOT.REGIONS** — One region at physical ``0x0E030000``, size 72 KB: ``cpuapp_boot_partition`` and ``periphconf_partition`` only (same span as PROTECTEDMEM; ``cpuapp_slot0`` is not included).

The application prints the NV counter and snapshot status from the IronSide SE boot report, then drives capture → recovery → cold reboot in a short loop (unless ``RUN_FOREVER`` is defined in :file:`src/main.c`).

Requirements
************

* nRF54H20 DK (application core)
* IronSide SE with snapshot and counter services

Building and running
********************

.. code-block:: console

   west build -b nrf54h20dk/nrf54h20/cpuapp --sysbuild nrf/samples/ironside_se/snapshot_capture_recover
   west flash

To reset persistent state (including counters) before a clean run:

.. code-block:: console

   west flash --recover
