#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Audit POVA UEFI repository claims and preserved historical artifacts."""
from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
readme = (ROOT / "README.md").read_text()
for phrase in (
    "Do not flash",
    "not hardware-tested",
    "Roadmap",
    "HakiOS",
):
    if phrase.lower() not in readme.lower():
        raise SystemExit(f"README is missing required status text: {phrase}")

evidence_path = ROOT / "docs/pova-device-evidence.md"
if not evidence_path.is_file():
    raise SystemExit("The sanitized POVA device-evidence report is missing")
evidence = evidence_path.read_text()
for phrase in (
    "d8f21f125ec880d91b1769a5c6c6d6a3b0c7f76f4fd0663d6649682f421dfc5a",
    "0x180000000",
    "0x11230000",
    "0x7e9f0000",
    "0x01500000",
    "LD7j",
):
    if phrase.lower() not in evidence.lower():
        raise SystemExit(f"POVA evidence report is missing expected device fact: {phrase}")

required_files = (
    "source/Platforms/Tecno/PovaPkg/PovaPkg.dsc",
    "source/Platforms/Tecno/PovaPkg/PovaPkg.dec",
    "source/Platforms/Tecno/PovaPkg/PovaPkg.fdf",
    "source/Platforms/Tecno/PovaPkg/DeviceBuild.py",
    "source/Platforms/Tecno/PovaPkg/Include/DXE.inc",
    "source/Platforms/Tecno/PovaPkg/Library/MemoryMapLib/MemoryMapLib.inf",
    "source/Platforms/Tecno/PovaPkg/Library/MemoryMapLib/MemoryMapLib.c",
    "source/Silicon/Silicium/SiliciumPkg/Drivers/SimpleFbDxe/SimpleFb.c",
    "source/Silicon/Silicium/SiliciumPkg/Drivers/SimpleFbDxe/SimpleFbDxe.inf",
    "source/Silicon/MediaTek/MT6768Pkg/Library/PlatformSecLib/PlatformSecLib.inf",
    "source/Silicon/MediaTek/MT6768Pkg/Library/PlatformSecLib/PlatformSec.c",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/PlatformSec.c",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/PlatformSec.inf",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/Sec.h",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/AArch64/ArchSec.c",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/AArch64/Exception.S",
    "source/Silicon/MediaTek/MT6768Pkg/Sec/AArch64/Helper.S",
    "Resources/Configs/Pova.toml",
    "tools/verify-uefi-fd.py",
)
for relative in required_files:
    if not (ROOT / relative).is_file():
        raise SystemExit(f"Required file is missing: {relative}")

print("## POVA repository audit\n")
print("**Result:** required build files and sanitized device evidence are present.")
