.. _ironside_se_snapshot_capture_recover:

IronSide SE: Snapshot capture and recover
#########################################

.. contents::
   :local:
   :depth: 2

Overview
********

This sample demonstates IronSide SE snapshot capture and recovery.

It performs a couple of snapshot capture's and snapshot recovery
operations and then goes into an infinite loop where the user may
choose to apply a magnet to corrupt MRAM and observe that the device
recovers.

To keep track of which boot we are on we use an IronSide SE NV
counter.

UICR configuration:

* **UICR.LOCK** — Not written by :file:`sysbuild/uicr.conf`. After each boot the sample reads the NV counter, if the value was ``0``, it then calls ``uicr_deploy_lock_contents()`` and cold-reboots.
* **PROTECTEDMEM** — Protects ``cpuapp_boot_partition`` and ``periphconf_partition`` (72 KB), consistent with other IronSide SE samples that place periphconf immediately after the boot partition.
* **UICR.SNAPSHOT.REGIONS** — Region 0 at physical ``0x0E030000`` (72 KB) for boot + periphconf; additional regions in :file:`sysbuild/uicr.conf` cover secure storage and the MRAM area used by the sample stress path.

The application validates the boot report, increments an NV counter, logs the snapshot status, and then drives capture → recovery → cold reboot.
To keep the sample bounded and repeatable, the flow stops once the counter reaches ``SNAPSHOT_MAX_CYCLES`` in :file:`src/main.c`.
After that, it enters a heartbeat loop and periodically performs high-volume MRAM read sweeps over a large region to help surface possible MRAM corruption.

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
