#!/usr/bin/env python3
"""
pack-pova-ipsw.py — Assemble a HakiOS Darwin iOS 16.5 Restore IPSW & deployment bundle for Tecno Pova LD7J.

Packages:
  - BuildManifest.plist (specifying MT6768 / LD7j / Darwin 22.5.0 / iOS 16.5 20F66)
  - Restore.plist
  - Firmware/all_flash/...: POVA_EFI.fd, POVA_BootShim.bin, loader.efi
  - kernelcache.release.mt6768 (hakios-kernel)
  - SystemOS.apfs (userland/system.apfs, 2.5 GB APFS system container)
  - Root.apfs (userland/root.apfs, redwood init container)
  - POVA_HAKIOS_XNU_BOOT.img (fastboot RAM boot image)
  - Deployment & testing tools (xl, system-flash.sh, instructions)
"""

import argparse
import hashlib
import os
import plistlib
import sys
import zipfile

def sha256_file(path):
    h = hashlib.sha256()
    with open(path, 'rb') as f:
        while chunk := f.read(1024 * 1024):
            h.update(chunk)
    return h.hexdigest()

def make_build_manifest(kernel_sha, uefi_sha, system_sha, root_sha):
    return {
        "BuildIdentities": [
            {
                "BoardConfig": "ld7j_h694",
                "BuildTrain": "SydneyF",
                "DeviceClass": "ld7j",
                "Manifest": {
                    "KernelCache": {
                        "Digest": kernel_sha,
                        "IsLoadedByOS": True,
                        "Path": "kernelcache.release.mt6768"
                    },
                    "OS": {
                        "Digest": system_sha,
                        "Path": "SystemOS.apfs"
                    },
                    "RestoreRAMDisk": {
                        "Digest": root_sha,
                        "Path": "Root.apfs"
                    },
                    "UEFI": {
                        "Digest": uefi_sha,
                        "Path": "Firmware/all_flash/all_flash.ld7j_h694.production/POVA_EFI.fd"
                    }
                },
                "Platform": "MediaTek MT6768 / MT6769V (Helio G80)",
                "ProductBuildVersion": "20F66",
                "ProductMarketingVersion": "16.5",
                "ProductType": "TecnoPova,LD7j"
            }
        ],
        "ManifestVersion": 1,
        "ProductVersion": "16.5"
    }

def make_restore_plist():
    return {
        "DeviceClass": "ld7j",
        "ProductBuildVersion": "20F66",
        "ProductVersion": "16.5",
        "SupportedProductTypes": [
            "TecnoPova,LD7j",
            "iPhone10,3",
            "iPhone10,6"
        ],
        "SystemRestoreImageFileSystemType": "APFS",
        "KernelArchitecture": "arm64",
        "PlatformArchitecture": "MT6768"
    }

def make_instructions():
    return """# Tecno Pova LD7J — HakiOS Darwin iOS 16.5 Testing Instructions

## 1. RAM-Only Fastboot Boot (Recommended — Zero Disk Writes)
Tecno Pova LD7J has 6 GiB of DRAM, allowing both the 2.5 GB Darwin system APFS and the XNU kernel payload to run entirely in RAM without touching your device's eMMC partitions.

### Steps:
1. Put the phone into Fastboot mode:
   `adb reboot bootloader` or hold `Power + Volume Down`.
2. Stage the 2.5 GB system image into phone DRAM:
   `fastboot stage SystemOS.apfs`
3. Boot the temporary HakiOS fastboot image into RAM:
   `fastboot boot POVA_HAKIOS_XNU_BOOT.img`
4. The phone boots via `POVA_BootShim` -> `POVA_EFI.fd` (Mu-Silicium UEFI) -> `loader.efi` -> mounts `SystemOS.apfs` as resident `/dev/md0` root, mounts `Root.apfs` on `/redwood` as `/dev/md1`, and launches Darwin 16.5 userland with the 7 native MT6768 drivers.
5. A simple reboot returns your phone directly back to stock Android.

## 2. eMMC Partition Flash (Optional)
To write the system container to eMMC storage instead:
1. Boot into Android with root shell.
2. Run `./system-flash.sh SystemOS.apfs` to write to the `xnusys` block partition.
"""

def main():
    parser = argparse.ArgumentParser(description="Package HakiOS POVA Restore IPSW")
    parser.add_argument("--system-apfs", required=True, help="Path to userland/system.apfs")
    parser.add_argument("--root-apfs", required=True, help="Path to userland/root.apfs")
    parser.add_argument("--kernel", required=True, help="Path to hakios-kernel")
    parser.add_argument("--bootimg", required=True, help="Path to POVA_HAKIOS_XNU_BOOT.img")
    parser.add_argument("--uefi-fd", required=True, help="Path to POVA_EFI.fd")
    parser.add_argument("--bootshim", required=True, help="Path to POVA_BootShim.bin")
    parser.add_argument("--loader", required=True, help="Path to loader.efi")
    parser.add_argument("--xl", required=False, help="Path to xl tool")
    parser.add_argument("--flash-script", required=False, help="Path to system-flash.sh")
    parser.add_argument("-o", "--output", required=True, help="Output IPSW path")
    args = parser.parse_args()

    for p in [args.system_apfs, args.root_apfs, args.kernel, args.bootimg, args.uefi_fd, args.bootshim, args.loader]:
        if not os.path.isfile(p):
            sys.exit(f"pack-pova-ipsw: required input file not found: {p}")

    print("==> Calculating file hashes for BuildManifest...")
    kernel_sha = sha256_file(args.kernel)
    uefi_sha = sha256_file(args.uefi_fd)
    system_sha = sha256_file(args.system_apfs)
    root_sha = sha256_file(args.root_apfs)

    manifest_data = make_build_manifest(kernel_sha, uefi_sha, system_sha, root_sha)
    restore_data = make_restore_plist()
    instructions_text = make_instructions()

    print(f"==> Packaging Restore IPSW: {args.output}...")
    with zipfile.ZipFile(args.output, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=6, allowZip64=True) as z:
        z.writestr("BuildManifest.plist", plistlib.dumps(manifest_data))
        z.writestr("Restore.plist", plistlib.dumps(restore_data))
        z.writestr("INSTRUCTIONS.md", instructions_text)

        z.write(args.kernel, "kernelcache.release.mt6768")
        z.write(args.bootimg, "POVA_HAKIOS_XNU_BOOT.img")
        z.write(args.uefi_fd, "Firmware/all_flash/all_flash.ld7j_h694.production/POVA_EFI.fd")
        z.write(args.bootshim, "Firmware/all_flash/all_flash.ld7j_h694.production/POVA_BootShim.bin")
        z.write(args.loader, "Firmware/all_flash/all_flash.ld7j_h694.production/loader.efi")
        z.write(args.root_apfs, "Root.apfs")
        
        # Store system.apfs (uncompressed within zip or deflated)
        print("==> Adding SystemOS.apfs (this may take 1-2 minutes)...")
        z.write(args.system_apfs, "SystemOS.apfs")

        if args.xl and os.path.isfile(args.xl):
            z.write(args.xl, "tools/xl")
        if args.flash_script and os.path.isfile(args.flash_script):
            z.write(args.flash_script, "tools/system-flash.sh")

    ipsw_size = os.path.getsize(args.output)
    print(f"==> Successfully generated {args.output} ({ipsw_size / (1024*1024):.2f} MiB)")

if __name__ == "__main__":
    main()
