# Tecno Pova LD7J Hardware Profile & Verified Evidence

| Parameter | Value | Source / Evidence |
|---|---|---|
| **Product Model** | `TECNO-LD7j` (Tecno Pova / LD7 / LD7j) | `android-info.txt` & `pova.dts` |
| **Project Code** | `ld7j_h694` | `MT6768_Android_scatter.txt` |
| **Stock Firmware** | `Tecno_Pova_LD7J_MT6768_V289_220927` | Verified stock OFP/ZIP |
| **Stock Boot Image SHA-256** | `d8f21f125ec880d91b1769a5c6c6d6a3b0c7f76f4fd0663d6649682f421dfc5a` | Stock `boot.img` (33,554,432 B) |
| **SoC Variant** | MediaTek MT6768 / MT6769V (Helio G80) | `model = "MT6769V/CU"`, `compatible = "mediatek,MT6768"` |
| **CPU Architecture** | ARMv8.2-A, 8 Cores: 6× Cortex-A55 @ 1.8 GHz + 2× Cortex-A75 @ 2.0 GHz | Boot CPU 7, PSCI 1.0 SMC conduit |
| **Physical DRAM Span** | Base `0x40000000`, Length `0x180000000` (6 GiB span: `0x40000000–0x1C0000000`) | Live DT `/memory` reg `<0x00 0x40000000 0x01 0x80000000>` |
| **Display Framebuffer Base** | **`0x7E9F0000`** | Live DT `mblock-12-framebuffer` and `atag,videolfb` |
| **Display VRAM Allocation** | **`0x01500000`** (22,020,096 bytes = 21 MiB) | Live DT `mblock-12-framebuffer` reg |
| **Display Panel & Resolution** | Novatek NT36525B (`nt36525b_hdp_dsi_vdo_txd_boe_ld7`), **720 × 1640** | `atag,videolfb` and `tpd-resolution = <0x2d0 0x668>` |
| **Scanline Stride** | **720 pixels per scan line** (2880 bytes/row pitch) | Standard 1:1 HD+ DSI scanline |
| **Touch Screen** | FocalTech SPI Touch (`compatible = "focaltech,fts"`) on `spi2` (`0x11012000`) | Live DT `spi2@11012000/touch@0` |
| **eMMC Storage** | MediaTek MSDC0 eMMC 5.1 at `0x11230000` (MSDC0_TOP at `0x11cd0000`) | Live DT `msdc@11230000`, 200 MHz HS400/HS200 |
| **UART Serial Port** | MT6768 High-Speed UART0 at `0x11002000`, 115200-8N1 from 26 MHz clock | Live DT `serial@11002000` |
| **PMIC & Fuel Gauge** | MediaTek MT6358 PMIC communicating over PWRAP (`0x1000D000`) | Live DT `mt6358-pmic`, `mt6358_gauge` |
| **Physical Buttons** | Keypad (`0x10010000`), MT6358 PWRKEY / HOMEKEY | Live DT `kp@10010000`, MT6358 interrupts |
| **GPU Subsystem** | ARM Mali-G52 2EEMC2 (Bifrost v7) at `0x13000000` / `0x13040000` | Stock kernel strings `Mali-G52`, `mali_bifrost/mali-r20p0` |
| **XNUP Reservation** | `0xF0000000–0xF3FFFFFF` (64 MiB carveout) | Verified 100% collision-free against all 19 reserved regions |
