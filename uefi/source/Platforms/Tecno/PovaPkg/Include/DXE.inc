##
# DXE driver inclusions for Tecno Pova LD7J.
# SPDX-License-Identifier: BSD-2-Clause-Patent
##

# Display: SimpleFbDxe (720x1640 @ Stride 720)
INF Silicon/Silicium/SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf

# Storage: MSDC0 eMMC Block I/O
INF Silicon/MediaTek/MT6768Pkg/Drivers/MsdcBlockIoDxe/MsdcBlockIoDxe.inf

# USB Class drivers
INF MdeModulePkg/Bus/Usb/UsbBusDxe/UsbBusDxe.inf
INF MdeModulePkg/Bus/Usb/UsbKbDxe/UsbKbDxe.inf
INF MdeModulePkg/Bus/Usb/UsbMassStorageDxe/UsbMassStorageDxe.inf
