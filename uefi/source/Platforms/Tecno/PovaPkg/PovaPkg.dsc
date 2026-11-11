##
# Tecno Pova LD7J / MediaTek MT6768 / MT6769V (Helio G80).
# SPDX-License-Identifier: BSD-2-Clause-Patent
##
[Defines]
  PLATFORM_NAME                  = POVA
  PLATFORM_GUID                  = C9480112-E51B-4A73-A65B-9721DF762B60
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010005
  OUTPUT_DIRECTORY               = Build/POVA
  SUPPORTED_ARCHITECTURES        = AARCH64
  BUILD_TARGETS                  = RELEASE|DEBUG
  SKUID_IDENTIFIER               = DEFAULT
  FLASH_DEFINITION               = Platforms/Tecno/PovaPkg/PovaPkg.fdf
  DEFINE ENABLE_SECUREBOOT       = 0
  USE_CUSTOM_DISPLAY_DRIVER      = 0

!include Silicon/MediaTek/MT6768Pkg/MT6768Pkg.dsc.inc

[PcdsFixedAtBuild]
  # Live LD7J evidence: /memory base 0x40000000, size 6 GiB span.
  gArmTokenSpaceGuid.PcdSystemMemoryBase|0x40000000
  gArmTokenSpaceGuid.PcdSystemMemorySize|0x180000000
  # 4 MiB FD layout placed at 0x40000000; stack begins at 0x40400000.
  gArmTokenSpaceGuid.PcdFdBaseAddress|0x40000000
  gArmTokenSpaceGuid.PcdFdSize|0x00400000
  gArmTokenSpaceGuid.PcdFvBaseAddress|0x40000000
  gArmTokenSpaceGuid.PcdFvSize|0x00400000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVendor|L"Tecno"
  gEfiMdeModulePkgTokenSpaceGuid.PcdFirmwareVersionString|L"LD7J_MT6768_V289"
  gArmPlatformTokenSpaceGuid.PcdCPUCoresStackBase|0x40400000
  gArmPlatformTokenSpaceGuid.PcdCPUCorePrimaryStackSize|0x40000
  # SimpleFbDxe display description for Pova's 720x1640 panel
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferWidth|720
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferHeight|1640
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferColorDepth|32
  gSiliciumPkgTokenSpaceGuid.PcdFrameBufferPixelsPerScanLine|720
  # Serial SimpleTextInOut must connect without a user-created boot option.
  gEfiMdeModulePkgTokenSpaceGuid.PcdConInConnectOnDemand|FALSE
  gEfiMdePkgTokenSpaceGuid.PcdPlatformBootTimeOut|0

  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemManufacturer|"Tecno"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemModel|"Tecno Pova"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailModel|"LD7j"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemRetailSku|"LD7J-MT6768"
  gSiliciumPkgTokenSpaceGuid.PcdSmbiosSystemBoardModel|"POVA"

[Components]
  Silicon/MediaTek/MT6768Pkg/Sec/PlatformSec.inf
  MdeModulePkg/Core/Dxe/DxeMain.inf
  MdeModulePkg/Universal/PCD/Dxe/Pcd.inf
  ArmPkg/Drivers/CpuDxe/CpuDxe.inf
  MdeModulePkg/Core/RuntimeDxe/RuntimeDxe.inf
  MdeModulePkg/Universal/SecurityStubDxe/SecurityStubDxe.inf
  MdeModulePkg/Universal/WatchdogTimerDxe/WatchdogTimer.inf
  MdeModulePkg/Universal/CapsuleRuntimeDxe/CapsuleRuntimeDxe.inf
  MdeModulePkg/Universal/ResetSystemRuntimeDxe/ResetSystemRuntimeDxe.inf
  MdeModulePkg/Universal/ReportStatusCodeRouter/RuntimeDxe/ReportStatusCodeRouterRuntimeDxe.inf
  MdeModulePkg/Universal/StatusCodeHandler/RuntimeDxe/StatusCodeHandlerRuntimeDxe.inf
  MdeModulePkg/Universal/Variable/RuntimeDxe/VariableRuntimeDxe.inf
  EmbeddedPkg/EmbeddedMonotonicCounter/EmbeddedMonotonicCounter.inf
  EmbeddedPkg/SimpleTextInOutSerial/SimpleTextInOutSerial.inf
  EmbeddedPkg/RealTimeClockRuntimeDxe/RealTimeClockRuntimeDxe.inf
  EmbeddedPkg/MetronomeDxe/MetronomeDxe.inf
  MdeModulePkg/Universal/PrintDxe/PrintDxe.inf
  MdeModulePkg/Universal/DevicePathDxe/DevicePathDxe.inf
  MdeModulePkg/Universal/Console/ConPlatformDxe/ConPlatformDxe.inf
  MdeModulePkg/Universal/Console/ConSplitterDxe/ConSplitterDxe.inf
  MdeModulePkg/Universal/Console/GraphicsConsoleDxe/GraphicsConsoleDxe.inf
  MdeModulePkg/Universal/HiiDatabaseDxe/HiiDatabaseDxe.inf
  MdeModulePkg/Universal/Disk/DiskIoDxe/DiskIoDxe.inf
  MdeModulePkg/Universal/Disk/PartitionDxe/PartitionDxe.inf
  MdeModulePkg/Universal/Disk/UnicodeCollation/EnglishDxe/EnglishDxe.inf
  FatPkg/EnhancedFatDxe/Fat.inf
  MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
  MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
  MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
  SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf
  ArmPkg/Drivers/ArmGicDxe/ArmGicDxe.inf
  ArmPkg/Drivers/TimerDxe/TimerDxe.inf
  Silicon/MediaTek/MT6768Pkg/Drivers/MsdcBlockIoDxe/MsdcBlockIoDxe.inf
  MdeModulePkg/Universal/BdsDxe/BdsDxe.inf
  ShellPkg/Application/Shell/Shell.inf

[BuildOptions]
GCC:*_*_*_CC_FLAGS = -fno-stack-protector
