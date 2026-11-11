# Tecno Pova LD7J

Tecno Pova uses MediaTek MT6768 / MT6769V (Helio G80). This profile inherits the shared MT6768 configuration with full 8-core affinity, boot CPU 7, eMMC 5.1 MSDC0 service, and native hardware drivers.

## Verified Hardware Profile & Evidence

- **FrameBuffer**: SimpleFb GOP at physical address `0x7E9F0000` (length `0x01500000`, 22,020,096 bytes) with stride **720 pixels per scan line** (2880 bytes/row), 720×1640 @ 60 Hz.
- **Touch**: FocalTech SPI Touch Screen on SPI2 (`0x11012000`).
- **Physical Keys**: MediaTek Keypad (`0x10010000`), MT6358 PWRKEY and HOMEKEY via PWRAP (`0x1000D000`).
- **PMIC & Battery**: MT6358 PMIC fuel gauge communicating over PWRAP.
- **GPU**: ARM Mali-G52 2EEMC2 Bifrost v7 at `0x13000000` / `0x13040000` with Panfrost DRM service.
- **Storage**: MediaTek MSDC0 eMMC 5.1 at `0x11230000`.

The machine-readable feature status is in [`device.conf`](device.conf).
Do not flash device partitions. Use temporary fastboot RAM boot only: `fastboot boot POVA_HAKIOS_XNU_BOOT.img`.
