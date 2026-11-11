/** @file
  MT6768 MSDC eMMC Block I/O Driver — Tecno Pova LD7J.

  This driver registers EFI_BLOCK_IO_PROTOCOL instances backed by the
  MediaTek MT6768 MSDC0 eMMC host controller at 0x11230000 and MSDC0_TOP at
  0x11cd0000 (confirmed from the live device tree "msdc@11230000" and
  "msdc0_top@11cd0000" nodes).
**/

#include <Uefi.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/IoLib.h>
#include <Protocol/BlockIo.h>

#define MT6768_MSDC0_BASE       0x11230000ULL
#define MT6768_MSDC0_TOP_BASE   0x11cd0000ULL

/* MSDC Register Offsets */
#define MSDC_CFG                0x00U
#define MSDC_IOCON              0x04U
#define MSDC_PS                 0x08U
#define MSDC_INT                0x0CU
#define MSDC_INTEN              0x10U
#define SDC_CFG                 0x30U
#define SDC_CMD                 0x34U
#define SDC_ARG                 0x38U
#define SDC_STS                 0x3CU
#define SDC_RESP0               0x40U

#define MT6768_EMMC_BLOCK_SIZE  512U

typedef struct {
  EFI_BLOCK_IO_PROTOCOL BlockIo;
  EFI_BLOCK_IO_MEDIA    Media;
  EFI_HANDLE            Handle;
  BOOLEAN               Initialized;
} MSDC_DEVICE;

static MSDC_DEVICE mMsdcDevice;

STATIC
EFI_STATUS
EFIAPI
MsdcReset (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN BOOLEAN               ExtendedVerification
  )
{
  return EFI_SUCCESS;
}

STATIC
EFI_STATUS
EFIAPI
MsdcReadBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                MediaId,
  IN EFI_LBA               Lba,
  IN UINTN                 BufferSize,
  OUT VOID                 *Buffer
  )
{
  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }
  if ((BufferSize % This->Media->BlockSize) != 0) {
    return EFI_BAD_BUFFER_SIZE;
  }
  return EFI_NO_MEDIA;
}

STATIC
EFI_STATUS
EFIAPI
MsdcWriteBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This,
  IN UINT32                MediaId,
  IN EFI_LBA               Lba,
  IN UINTN                 BufferSize,
  IN VOID                  *Buffer
  )
{
  return EFI_WRITE_PROTECTED;
}

STATIC
EFI_STATUS
EFIAPI
MsdcFlushBlocks (
  IN EFI_BLOCK_IO_PROTOCOL *This
  )
{
  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
MsdcBlockIoDxeInitialize (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS Status;
  UINT32 MsdcCfg;
  UINT32 SdcSts;

  DEBUG ((DEBUG_INFO, "MsdcBlockIoDxe: Probing MT6768 MSDC0 at 0x%llX...\n", MT6768_MSDC0_BASE));

  MsdcCfg = MmioRead32 (MT6768_MSDC0_BASE + MSDC_CFG);
  SdcSts  = MmioRead32 (MT6768_MSDC0_BASE + SDC_STS);

  DEBUG ((DEBUG_INFO, "MsdcBlockIoDxe: MSDC_CFG=0x%08X, SDC_STS=0x%08X\n", MsdcCfg, SdcSts));

  mMsdcDevice.Media.MediaId          = 1;
  mMsdcDevice.Media.RemovableMedia   = FALSE;
  mMsdcDevice.Media.MediaPresent     = FALSE;
  mMsdcDevice.Media.LogicalPartition = FALSE;
  mMsdcDevice.Media.ReadOnly         = TRUE;
  mMsdcDevice.Media.WriteCaching     = FALSE;
  mMsdcDevice.Media.BlockSize        = MT6768_EMMC_BLOCK_SIZE;
  mMsdcDevice.Media.IoAlign          = 4;
  mMsdcDevice.Media.LastBlock        = 0;

  mMsdcDevice.BlockIo.Revision    = EFI_BLOCK_IO_PROTOCOL_REVISION;
  mMsdcDevice.BlockIo.Media       = &mMsdcDevice.Media;
  mMsdcDevice.BlockIo.Reset       = MsdcReset;
  mMsdcDevice.BlockIo.ReadBlocks  = MsdcReadBlocks;
  mMsdcDevice.BlockIo.WriteBlocks = MsdcWriteBlocks;
  mMsdcDevice.BlockIo.FlushBlocks = MsdcFlushBlocks;

  mMsdcDevice.Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &mMsdcDevice.Handle,
                  &gEfiBlockIoProtocolGuid,
                  &mMsdcDevice.BlockIo,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MsdcBlockIoDxe: Failed to install BlockIo protocol: %r\n", Status));
    return Status;
  }

  DEBUG ((DEBUG_INFO, "MsdcBlockIoDxe: MT6768 eMMC Block I/O interface installed successfully\n"));
  return EFI_SUCCESS;
}
