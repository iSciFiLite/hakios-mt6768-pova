# POVA UEFI — Tecno Pova (LD7J / MT6768 / MT6769V / Helio G80)

> [!NOTE]
> **Inclusive Development Active**: This component is developed in lockstep with the HakiOS kernel and XnuLoader in the unified inclusive repository: [**iSciFiLite/hakios-mt6768-pova**](https://github.com/iSciFiLite/hakios-mt6768-pova). All cross-layer enhancements, shared memory map definitions, and unified fastboot boot image packaging are authored atomically here.

**Status updated 2026-11-11.** This repository contains a POVA EDK II platform overlay for [Mu-Silicium](https://github.com/Project-Silicium/Mu-Silicium), plus a pinned [HakiOS UEFI](https://github.com/oosexgang/hakios-uefi) integration and an automated combined HakiOS/XNU Android-v2 boot image build pipeline.

> **Do not flash these images.** The combined image is intended only for a temporary `fastboot boot` RAM test from an external USB host, after the owner has a known recovery route and an unlocked bootloader. It does not write the boot or LK partitions. Its LK acceptance and hardware behavior are unverified and not hardware-tested on phone. The FD base overlaps Android's normal kernel load area, so it must replace the stock kernel payload exactly as packaged.

## Support and verification status

| Area | Implemented now | Evidence and remaining limit |
|---|---|---|
| **POVA platform** | AArch64 SEC/DXE, board package `PovaPkg`, memory descriptors, SimpleFb GOP, standard EDK II BDS/Shell | Mu-Silicium platform overlay based on verified V289 OFP and live DTB; not hardware-tested on phone. |
| **Mu-Silicium** | Pinned workspace overlays and reproducible package configuration | Generic Mu-Silicium overlay adapted to MT6768 Helio G80. |
| **HakiOS UEFI** | Upstream BootShim/XNUP and BootManager/XnuLoader patch stack; POVA overlay; separate hosted build and BootShim job | Build result is recorded in the GitHub Actions workflow. |
| **HakiOS/XNU fastboot handoff** | POVA MT6768 kernel config and UEFI profile; hosted XNU build and XNUP packaging | The combined image uses the matching HakiOS FD and BootShim plus the pinned MT6768 kernel in HakiOS's XNUP format. |
| **POVA HakiOS/XNU boot image** | **Combined image packaging pipeline ready** | Android v2 boot image (`POVA_HAKIOS_XNU_BOOT.img`). Combines gzip(BootShim + POVA FD + XNUP payload with native MT6768 drivers + open hello ramdisk) with verified stock V289 ramdisk and DTB. `fastboot boot` only; no phone test yet. |
| **GOP / Framebuffer** | Uses live-DT display reservation and reports 720×1640, 32 bpp, **720 pixels per scan line** | Physical base at **`0x7E9F0000`**, length **`0x01500000`** (22,020,096 bytes = 21 MiB), matching `mblock-12-framebuffer` and `atag,videolfb`. Stride 720 pixels per scanline (2880 bytes/row). |
| **Internal eMMC** | Read-only MSDC0 register probe at `0x11230000` | Stock V289 and live DT confirm MediaTek MSDC0 at `0x11230000` and TOP at `0x11cd0000`. MSDC Block I/O stub publishes protocol. |
| **UART / Serial** | MT6768 SerialPortLib configures `0x11002000`, MTK mode 3, 115200-8N1 from 26 MHz clock | Pinmux: GPIO53 (RX), GPIO54 (TX) at 1.8V TTL. |
| **USB** | Generic USB bus, keyboard, and mass-storage class modules are included | Standard UEFI USB class drivers. |
| **CPU, GIC, timer, reset** | Generic Arm/GIC code and MT6768 core/PSCI declarations are included | PSCI SMC conduit declared; 8 cores (6× A55 + 2× A75). Boot core 7. |
| **Touchscreen** | FocalTech SPI Touch Screen on `spi2` (`0x11012000`) | Live DT confirms FocalTech FTS SPI touch. |
| **Physical Keys** | Keypad (`0x10010000`), MT6358 PWRKEY / HOMEKEY | Dedicated buttons driver in OS layer; LK powers key defaults. |

## Device identity and evidence

| Observation | Value |
|---|---|
| `ro.product.model` | `TECNO-LD7j` (Tecno Pova / LD7 / LD7j) |
| Project Code | `ld7j_h694` |
| Stock Firmware | `Tecno_Pova_LD7J_MT6768_V289_220927` |
| Stock Boot SHA-256 | `d8f21f125ec880d91b1769a5c6c6d6a3b0c7f76f4fd0663d6649682f421dfc5a` |
| Live DT memory | Base `0x40000000`, size `0x180000000` (6 GiB physical span) |
| Live DT eMMC declarations | MSDC0 `0x11230000`, TOP `0x11cd0000` |
| Live DT display reservation | `0x7e9f0000`, length `0x01500000` (22,020,096 bytes) |
| Panel & Stride | Novatek NT36525B, 720×1640, stride `720` pixels |
| Historical FD layout | Base `0x40000000`, size 4 MiB; stack begins at `0x40400000` |

## Roadmap

1. **Device Facts**: Grounded in V289 stock firmware and live DTB (Complete).
2. **PovaPkg Platform**: Full EDK II package with 37 non-overlapping descriptors (Complete).
3. **Hosted Build**: GitHub Actions pipeline for dual compilation and boot image assembly (Complete).
4. **Hardware Verification**: One-shot temporary RAM boot test on unlocked device (`fastboot boot POVA_HAKIOS_XNU_BOOT.img`).
