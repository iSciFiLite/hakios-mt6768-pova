/** @file
  MemoryMapLib for Tecno Pova LD7J (MT6768 / Helio G80).
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryMapLib.h>

STATIC EFI_MEMORY_REGION_DESCRIPTOR mMemoryMap[] = {
  // 4 MiB FD and 1 MiB stack windows
  { "UEFI FD",                    0x40000000ULL, 0x00400000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, BsCode, WRITE_BACK },
  { "UEFI Stack",                 0x40400000ULL, 0x00100000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, BsData, WRITE_BACK_XN },
  { "HLOS 1",                     0x40500000ULL, 0x0C900000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-4-atf-reserved",      0x4CE00000ULL, 0x00060000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 2",                     0x4CE60000ULL, 0x001A0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-8-ram_console",       0x4D000000ULL, 0x00010000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "mblock-9-pstore",            0x4D010000ULL, 0x000E0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "mblock-10-minirdump",        0x4D0F0000ULL, 0x00010000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 3",                     0x4D100000ULL, 0x22F00000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-7-tee-reserved",      0x70000000ULL, 0x02A00000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 4",                     0x72A00000ULL, 0x055F0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-14-SPM-reserved",     0x77FF0000ULL, 0x00010000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "mblock-18-md_smem_ncache",   0x78000000ULL, 0x01800000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 5",                     0x79800000ULL, 0x02800000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-19-ccci",             0x7C000000ULL, 0x01620000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 6",                     0x7D620000ULL, 0x013D0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "Display Reserved",           0x7E9F0000ULL, 0x01500000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, NS_DEVICE },
  { "HLOS 7",                     0x7FEF0000ULL, 0x00008000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-11-tkv",              0x7FEF8000ULL, 0x00008000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "mblock-6-SSPM-reserved",     0x7FF00000ULL, 0x000C0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "mblock-3-log_store",         0x7FFC0000ULL, 0x00040000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 8",                     0x80000000ULL, 0x1F900000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-15-SCP-reserved",     0x9F900000ULL, 0x00600000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 9",                     0x9FF00000ULL, 0x000F0000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-16-ccci_tag_mem",     0x9FFF0000ULL, 0x00010000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 10",                    0xA0000000ULL, 0x0E000000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-17-ccci",             0xAE000000ULL, 0x10000000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 11",                    0xBE000000ULL, 0x01E00000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-5-atf-log-reserved",  0xBFE00000ULL, 0x00040000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 12",                    0xBFE40000ULL, 0x00130000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-13-atf-ramdump",      0xBFF70000ULL, 0x00080000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 13",                    0xBFFF0000ULL, 0x0000F000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-1-dramc-rk0",         0xBFFFF000ULL, 0x00001000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 14",                    0xC0000000ULL, 0x30000000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "HakiOS XNUP Carveout",       0xF0000000ULL, 0x04000000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },
  { "HLOS 15",                    0xF4000000ULL, 0xCBFFF000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Conv, WRITE_BACK_XN },
  { "mblock-2-dramc-rk1",         0x1BFFFF000ULL, 0x00001000ULL, AddMem, SYS_MEM, SYS_MEM_CAP, Reserv, WRITE_BACK_XN },

  // MMIO regions
  { "GIC Distributor",            0x0C000000ULL, 0x00040000ULL, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "GIC Redistributor",          0x0C040000ULL, 0x00200000ULL, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "UART",                       0x11002000ULL, 0x00001000ULL, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
  { "MSDC0 eMMC",                 0x11230000ULL, 0x00001000ULL, AddDev, MMAP_IO, UNCACHEABLE, MmIO, NS_DEVICE },
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                         *MemoryDescriptorCount
  )
{
  *MemoryDescriptor = mMemoryMap;
  *MemoryDescriptorCount = (UINT8)ARRAY_SIZE (mMemoryMap);
}
