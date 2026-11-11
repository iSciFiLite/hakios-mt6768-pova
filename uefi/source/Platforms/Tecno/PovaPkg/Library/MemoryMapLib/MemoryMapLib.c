/** @file
  MemoryMapLib for Tecno Pova LD7J (MT6768 / Helio G80).
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#include <Library/MemoryMapLib.h>

static ARM_MEMORY_REGION_DESCRIPTOR_EX gDeviceMemoryDescriptorEx[] = {
  // Address, Length, ArmAttributes, MemoryType
  {"HLOS 0",                    0x40000000, 0x00400000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiBootServicesCode},
  {"HLOS Stack",                0x40400000, 0x00100000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiBootServicesData},
  {"HLOS 1",                    0x40500000, 0x0C900000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-4-atf-reserved",     0x4CE00000, 0x00060000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 2",                    0x4CE60000, 0x001A0000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-8-ram_console",      0x4D000000, 0x00010000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"mblock-9-pstore",           0x4D010000, 0x000E0000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"mblock-10-minirdump",       0x4D0F0000, 0x00010000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 3",                    0x4D100000, 0x22F00000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-7-tee-reserved",     0x70000000, 0x02A00000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 4",                    0x72A00000, 0x055F0000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-14-SPM-reserved",    0x77FF0000, 0x00010000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"mblock-18-md_smem_ncache",  0x78000000, 0x01800000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 5",                    0x79800000, 0x02800000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-19-ccci",            0x7C000000, 0x01620000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 6",                    0x7D620000, 0x013D0000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"FrameBuffer",               0x7E9F0000, 0x01500000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_THROUGH, EfiMaxMemoryType},
  {"HLOS 7",                    0x7FEF0000, 0x00008000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-11-tkv",             0x7FEF8000, 0x00008000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"mblock-6-SSPM-reserved",    0x7FF00000, 0x000C0000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"mblock-3-log_store",        0x7FFC0000, 0x00040000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 8",                    0x80000000, 0x1F900000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-15-SCP-reserved",    0x9F900000, 0x00600000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 9",                    0x9FF00000, 0x000F0000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-16-ccci_tag_mem",    0x9FFF0000, 0x00010000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 10",                   0xA0000000, 0x0E000000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-17-ccci",            0xAE000000, 0x10000000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 11",                   0xBE000000, 0x01E00000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-5-atf-log-reserved", 0xBFE00000, 0x00040000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 12",                   0xBFE40000, 0x00130000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-13-atf-ramdump",     0xBFF70000, 0x00080000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 13",                   0xBFFF0000, 0x0000F000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-1-dramc-rk0",        0xBFFFF000, 0x00001000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 14",                   0xC0000000, 0x30000000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"HakiOS XNUP Carveout",      0xF0000000, 0x04000000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"HLOS 15",                   0xF4000000, 0xCBFFF000, AddMem, SYS_MEM, SYS_MEM_CAP, WRITE_BACK,    EfiConventionalMemory},
  {"mblock-2-dramc-rk1",        0x1BFFFF000, 0x00001000, AddMem, MEM_RES, UNCACHEABLE, WRITE_THROUGH, EfiReservedMemoryType},
  {"Terminator",                0,          0,          0,      0,       0,           0,             0}
};

VOID
GetMemoryMap (
  OUT EFI_MEMORY_REGION_DESCRIPTOR **MemoryDescriptor,
  OUT UINT8                        *MemoryDescriptorCount
  )
{
  *MemoryDescriptor      = (EFI_MEMORY_REGION_DESCRIPTOR *)gDeviceMemoryDescriptorEx;
  *MemoryDescriptorCount = (sizeof (gDeviceMemoryDescriptorEx) / sizeof (gDeviceMemoryDescriptorEx[0])) - 1;
}
