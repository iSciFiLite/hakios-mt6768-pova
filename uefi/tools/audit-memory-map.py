#!/usr/bin/env python3
# SPDX-License-Identifier: MIT
"""Check Pova's static descriptor ranges for invalid sizes and overlap."""
from pathlib import Path
import re

source = Path(__file__).resolve().parents[1] / "source/Platforms/Tecno/PovaPkg/Library/MemoryMapLib/MemoryMapLib.c"
text = source.read_text()
ranges = []

for match in re.finditer(
    r'\{\s*"([^"]+)"\s*,\s*(0x[0-9A-Fa-f]+)(?:ULL)?\s*,\s*(0x[0-9A-Fa-f]+)(?:ULL)?\s*,\s*AddMem',
    text,
):
    name, start, size = match.groups()
    start, size = int(start, 16), int(size, 16)
    if size <= 0 or start + size <= start:
        raise SystemExit(f"{name}: invalid region extent")
    ranges.append((start, start + size, name, "memory map"))

if not ranges:
    raise SystemExit("No memory descriptors were parsed")

payload = [r for r in ranges if "HakiOS XNUP Carveout" in r[2] or "XNU" in r[2]]
if not payload:
    raise SystemExit("The HakiOS XNUP handoff region is not reserved")
payload_start, payload_end = payload[0][0], payload[0][1]
if (payload_start, payload_end - payload_start) != (0xF0000000, 0x04000000):
    raise SystemExit("The XNUP reservation must match HakiOS's 0xF0000000/64 MiB contract")

ranges.sort()
for previous, current in zip(ranges, ranges[1:]):
    if previous[1] > current[0]:
        raise SystemExit(
            f"Overlapping descriptors: {previous[2]} ends at 0x{previous[1]:x}; "
            f"{current[2]} starts at 0x{current[0]:x}"
        )

print(f"Validated {len(ranges)} non-overlapping POVA memory descriptors spanning 0x{ranges[0][0]:x} to 0x{ranges[-1][1]:x}")
for start, end, name, kind in ranges:
    print(f"0x{start:08x}-0x{end-1:08x} {kind}: {name}")
