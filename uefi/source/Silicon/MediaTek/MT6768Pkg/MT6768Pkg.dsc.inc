##
#  Copyright (c) 2011 - 2022, ARM Limited. All rights reserved.
#  Copyright (c) 2014, Linaro Limited. All rights reserved.
#  Copyright (c) 2015 - 2020, Intel Corporation. All rights reserved.
#  Copyright (c) Microsoft Corporation.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
##

[Defines]
  USE_PHYSICAL_TIMER                         = 1

!include SiliciumPkg/SiliciumPkg.dsc.inc

[PcdsFixedAtBuild]
  gEfiMdeModulePkgTokenSpaceGuid.PcdAcpiDefaultOemId|"MTK   "

[BuildOptions]
  *_*_AARCH64_CC_FLAGS = -march=armv8.2-a+crypto+rcpc

[PcdsFixedAtBuild]
  # Cortex-A55 cores 0-5 and Cortex-A75 cores 6-7.
  # Their MPIDR reg values describe eight CPUs across two clusters.
  gArmPlatformTokenSpaceGuid.PcdCoreCount|8
  gArmPlatformTokenSpaceGuid.PcdClusterCount|2
  # DTB /interrupt-controller reg: GICD 0x0C000000/0x40000;
  # GICR 0x0C040000/0x200000.
  gArmTokenSpaceGuid.PcdGicDistributorBase|0x0C000000
  gArmTokenSpaceGuid.PcdGicRedistributorsBase|0x0C040000
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorModel|"MediaTek MT6768"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosProcessorPartNumber|"MT6768"

[LibraryClasses]
  PlatformSecLib|Silicon/MediaTek/MT6768Pkg/Library/PlatformSecLib/PlatformSecLib.inf
  MemoryMapLib|Platforms/Tecno/PovaPkg/Library/MemoryMapLib/MemoryMapLib.inf
  # MT6768 UART0 SerialPortLib — MMIO implementation at 0x11002000.
  # Compatible: mediatek,mt6577-uart (DT-confirmed, pova.dts).
  # Configured at 115200-8N1 using the 26 MHz SoC reference clock and
  # MTK high-speed mode 3 (HIGHS=3, DLL=1, SAMPLE_CNT=225, SAMPLE_PT=111).
  SerialPortLib|Silicon/MediaTek/MT6768Pkg/Library/SerialPortLib/SerialPortLib.inf
  # Standard EDK II Shell application libraries (matches ShellPkg's DSC).
  ShellLib|ShellPkg/Library/UefiShellLib/UefiShellLib.inf
  ShellCommandLib|ShellPkg/Library/UefiShellCommandLib/UefiShellCommandLib.inf
  HandleParsingLib|ShellPkg/Library/UefiHandleParsingLib/UefiHandleParsingLib.inf
  OrderedCollectionLib|MdePkg/Library/BaseOrderedCollectionRedBlackTreeLib/BaseOrderedCollectionRedBlackTreeLib.inf
