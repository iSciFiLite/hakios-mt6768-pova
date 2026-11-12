#!/usr/bin/env bash
# ==============================================================================
# HakiOS Launcher for Tecno Pova LD7J (MT6768 / Helio G80)
# Adapted from HackiOS boot-one.sh for the MediaTek MT6768 inclusive port.
# Works across Linux, macOS, and Windows (Git Bash / WSL / MSYS2).
# ==============================================================================
set -u

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BOOT="${BOOT:-$ROOT}"

winpath() {
    if command -v cygpath >/dev/null 2>&1; then
        cygpath -w "$1"
    else
        printf '%s' "$1"
    fi
}

# Resolve timeout utility
if command -v gtimeout >/dev/null 2>&1; then
    TIMEOUT_BIN="$(command -v gtimeout)"
elif command -v timeout >/dev/null 2>&1; then
    TIMEOUT_BIN="$(command -v timeout)"
else
    # Fallback if no timeout utility is available
    TIMEOUT_BIN=""
fi

run_timeout() {
    local t=$1; shift
    if [ -n "$TIMEOUT_BIN" ]; then
        "$TIMEOUT_BIN" "$t" "$@"
    else
        "$@"
    fi
}

FB=${FB:-fastboot}
fb() {
    run_timeout "${FB_T:-30}" "$FB" "$@"
}

say() { printf '\n== %s\n' "$*"; }

# Check required commands
for tool in adb fastboot; do
    command -v "$tool" >/dev/null 2>&1 || {
        echo "boot-pova: required command not found: $tool" >&2
        echo "Please ensure Android SDK platform-tools are installed and in your PATH." >&2
        exit 1
    }
done

# Image Auto-Discovery
find_file() {
    for path in "$1" "$ROOT/$1" "$BOOT/$1" "$ROOT/boot/$1"; do
        if [ -f "$path" ]; then
            printf '%s' "$path"
            return 0
        fi
    done
    return 1
}

# Locate Loader Image
IMG_NAME="POVA_USB_LOADER.img"
IMG="$(find_file "$IMG_NAME" || find_file "pova-boot.img" || echo "$BOOT/$IMG_NAME")"

# Locate System APFS Image
SYS_NAME="SystemOS.apfs"
SYS="$(find_file "$SYS_NAME" || find_file "system.apfs" || echo "$BOOT/$SYS_NAME")"

# Locate XNU Kernel Payload
PAYLOAD_NAME="POVA_XNUP.bin"
PAYLOAD="$(find_file "$PAYLOAD_NAME" || find_file "run-payload.bin" || echo "$BOOT/$PAYLOAD_NAME")"

SECS=${SECS:-60}
SHOTS=${SHOTS:-2}
GAP=${GAP:-5}
TAG=${TAG:-pova_$(date +%H%M%S)}
SERIAL=${SERIAL:-}
OUT=${OUT:-$ROOT/runs}
mkdir -p "$OUT"
LOG="$OUT/$TAG.log"
LOGW="$(winpath "$LOG")"

say "HakiOS Tecno Pova LD7J (MT6768) — RAM Boot Launcher"
echo "Target Device: Tecno Pova LD7J (MediaTek Helio G80 / 6 GB LPDDR4X)"
echo "Loader Image : $(basename "$IMG")"
echo "Payload      : $(basename "$PAYLOAD")"
if [ -f "$SYS" ]; then
    echo "System APFS  : $(basename "$SYS") ($(du -h "$SYS" | cut -f1))"
else
    echo "System APFS  : None (kernel-only / payload minimal ramdisk)"
    SYS=""
fi

[ -f "$IMG" ] || { echo "boot-pova: missing loader image: $IMG" >&2; exit 1; }
[ -f "$PAYLOAD" ] || { echo "boot-pova: missing payload: $PAYLOAD" >&2; exit 1; }

say "Checking Phone State"
state=none
if adb devices 2>/dev/null | awk '$2=="device"{print $1}' | grep -q .; then
    state=android
elif fb devices 2>/dev/null | grep -q fastboot; then
    if fb getvar product 2>&1 | grep -q "product: *xnuloader"; then
        state=xnuloader
    else
        state=bootloader
    fi
fi
echo "Current Phone State: $state"

case $state in
android)
    if [ -z "$SERIAL" ]; then
        serials=$(adb devices | awk '$2 == "device" { print $1 }')
        count=$(printf '%s\n' "$serials" | grep -c .)
        if [ "$count" -ne 1 ]; then
            echo "boot-pova: multiple devices detected: $(echo "$serials"); set SERIAL=<serial>" >&2
            exit 1
        fi
        SERIAL=$serials
    fi
    echo "Connected serial: $SERIAL"
    ADB=(adb -s "$SERIAL")
    say "Rebooting to Fastboot Bootloader..."
    "${ADB[@]}" reboot bootloader >/dev/null 2>&1
    fbdev=
    for i in $(seq 1 30); do
        if fb devices 2>/dev/null | grep -q fastboot; then fbdev=1; break; fi
        sleep 1
    done
    [ -n "$fbdev" ] || { echo "boot-pova: device failed to reach fastboot; giving up." >&2; exit 1; }
    ;;
bootloader)
    echo "Phone is already in Fastboot mode."
    ;;
xnuloader)
    echo "Phone is already running HakiOS xnuloader resident in RAM."
    ;;
none)
    echo "boot-pova: phone not responding to adb or fastboot." >&2
    echo "Please connect the phone via USB in Fastboot mode (Hold Volume Down + Power)." >&2
    exit 1
    ;;
esac

# Stage 1: Boot the Resident Loader into RAM
say "Booting HakiOS Resident Loader into DRAM (Zero Flash)"
if [ "$state" != xnuloader ]; then
    echo "Sending $(basename "$IMG") to device RAM..."
    fb boot "$IMG" 2>&1 | tail -2
fi

echo "Waiting for HakiOS xnuloader Fastboot listener..."
up=
for i in $(seq 1 35); do
    if fb getvar product 2>&1 | grep -q "product: *xnuloader"; then up=1; break; fi
    sleep 1
done

if [ -z "$up" ]; then
    echo ""
    echo "------------------------------------------------------------------"
    echo "Notice: xnuloader did not report 'product: xnuloader' via RAM boot."
    echo "Some MediaTek Little Kernel (LK) builds ignore 'fastboot boot'."
    echo "If your device booted stock Android instead of xnuloader, you can"
    echo "temporarily flash the loader partition:"
    echo "  fastboot flash boot $(basename "$IMG")"
    echo "  fastboot reboot bootloader"
    echo "------------------------------------------------------------------"
    exit 1
fi
echo "HakiOS resident loader is active in DRAM!"

# Stage 2: Staging System APFS
if [ -n "$SYS" ]; then
    say "Allocating DRAM buffer and staging Darwin system.apfs"
    fb oem ramdisk 2>&1 | tail -1
    echo "Streaming $(basename "$SYS") over USB into DRAM (/dev/md0)..."
    time run_timeout 900 "$FB" stage "$SYS" 2>&1 | tail -1
else
    say "Booting minimal payload root"
    fb oem ramdisk none 2>&1 | tail -1
fi

# Stage 3: Staging Darwin Kernel Payload
say "Staging XNU Kernel Payload ($(basename "$PAYLOAD"))"
run_timeout 300 "$FB" stage "$PAYLOAD" 2>&1 | tail -1

# Stage 4: Execute XNU Darwin Kernel
say "Executing Darwin Kernel on MediaTek Helio G80 (Running for $SECS seconds)"
fb oem run 2>&1 | tail -1
sleep "$SECS"

# Stage 5: In-Flight Framebuffer Capture
say "Capturing live framebuffer screenshot over USB..."
shot() {
    local name=$1 geom
    geom=$(fb getvar screen 2>&1 | sed -n 's/^screen: //p')
    # Default to Tecno Pova LD7J geometry if not returned: 720 1640 2880
    if [ -z "$geom" ]; then geom="720 1640 2880"; fi
    
    if fb oem screenshot 2>&1 | grep -q OKAY; then
        fb get_staged "$name.raw" >/dev/null 2>&1
        if [ -s "$name.raw" ]; then
            python3 - "$name.raw" "$name.png" $geom <<'PY' 2>/dev/null || true
import struct, sys, zlib
try:
    raw, out, width, height, stride = sys.argv[1], sys.argv[2], *map(int, sys.argv[3:6])
    data = open(raw, 'rb').read()
    rows = bytearray()
    for y in range(height):
        line = data[y * stride:y * stride + width * 4]
        rows.append(0)
        rgb = bytearray(width * 3)
        rgb[0::3], rgb[1::3], rgb[2::3] = line[2::4], line[1::4], line[0::4]
        rows += rgb
    def chunk(t, d):
        c = t + d
        return struct.pack('>I', len(d)) + c + struct.pack('>I', zlib.crc32(c) & 0xffffffff)
    png = (b'\x89PNG\r\n\x1a\n'
           + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 2, 0, 0, 0))
           + chunk(b'IDAT', zlib.compress(bytes(rows), 6))
           + chunk(b'IEND', b''))
    open(out, 'wb').write(png)
    print("Saved screenshot: %s (%dx%d)" % (out, width, height))
except Exception as e:
    pass
PY
            rm -f "$name.raw"
        fi
    fi
}

shot "$OUT/$TAG"

# Stage 6: Retrieve Kernel Log (dmesg)
say "Retrieving Darwin kernel log over USB..."
fb oem log 2>&1 | tail -1
fb get_staged "$LOGW" >/dev/null 2>&1
if [ -s "$LOG" ]; then
    LC_ALL=C tr -d '\000' < "$LOG" > "$LOG.txt"
    echo "Darwin dmesg log saved to: $LOG.txt ($(wc -c < "$LOG.txt") bytes)"
fi

# Exit / Return to Android
if [ "${REBOOT:-0}" = 1 ]; then
    say "Rebooting phone back to Android (RAM cleared)..."
    fb reboot >/dev/null 2>&1
    for i in $(seq 1 90); do
        adb devices 2>/dev/null | grep -q "device$" && break
        sleep 1
    done
    echo "Phone rebooted into Android."
else
    echo ""
    echo "=== HakiOS Darwin is active on Tecno Pova LD7J ==="
    echo "To return to stock Android at any time:"
    echo "  1. Run: fastboot reboot"
    echo "  2. Or hold the Power button for 5 seconds."
fi
