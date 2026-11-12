@echo off
setlocal enabledelayedexpansion

echo ==============================================================================
echo HakiOS RAM Boot Launcher for Tecno Pova LD7J (MediaTek MT6768)
echo Port: HakiOS XNU Darwin 16.5 (20F66) on Helio G80 (6 GB RAM)
echo ==============================================================================

:: Find fastboot
where fastboot >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] fastboot.exe not found!
    echo Please install Android platform-tools or add fastboot to your PATH.
    pause
    exit /b 1
)

:: Locate files
set "LOADER=POVA_USB_LOADER.img"
if not exist "%LOADER%" set "LOADER=boot\POVA_USB_LOADER.img"
if not exist "%LOADER%" (
    echo [ERROR] Loader image POVA_USB_LOADER.img not found!
    pause
    exit /b 1
)

set "SYS=SystemOS.apfs"
if not exist "%SYS%" set "SYS=system.apfs"
if not exist "%SYS%" set "SYS=boot\SystemOS.apfs"

set "PAYLOAD=POVA_XNUP.bin"
if not exist "%PAYLOAD%" set "PAYLOAD=run-payload.bin"
if not exist "%PAYLOAD%" set "PAYLOAD=boot\POVA_XNUP.bin"
if not exist "%PAYLOAD%" (
    echo [ERROR] Payload POVA_XNUP.bin not found!
    pause
    exit /b 1
)

echo [OK] Loader  : %LOADER%
echo [OK] Payload : %PAYLOAD%
if exist "%SYS%" (
    echo [OK] System  : %SYS%
) else (
    echo [WARN] System: None (using minimal payload ramdisk)
    set "SYS="
)

echo.
echo Checking fastboot device connection...
fastboot devices
for /f "tokens=1" %%d in ('fastboot devices 2^>nul') do set "DEVICE=%%d"
if "%DEVICE%"=="" (
    echo [ERROR] No device detected in Fastboot mode!
    echo Please connect your Tecno Pova via USB in Fastboot mode:
    echo   1. Turn off phone.
    echo   2. Hold Volume Down + Power until FASTBOOT appears on screen.
    pause
    exit /b 1
)
echo Connected device: %DEVICE%

echo.
echo ==============================================================================
echo [Step 1/4] Booting HakiOS Resident Loader into DRAM (Zero Flash)...
echo ==============================================================================
fastboot boot "%LOADER%"

echo.
echo Waiting for HakiOS xnuloader listener to initialize...
set "UP="
for /l %%i in (1,1,30) do (
    for /f "tokens=2" %%p in ('fastboot getvar product 2^>^&1 ^| findstr "product:"') do (
        if "%%p"=="xnuloader" set "UP=1"
    )
    if defined UP goto :loader_ready
    timeout /t 1 /nobreak >nul
)

:loader_ready
if not defined UP (
    echo.
    echo ------------------------------------------------------------------
    echo Notice: Device did not report 'product: xnuloader' via RAM boot.
    echo Some MediaTek Little Kernel (LK) firmware ignores 'fastboot boot'.
    echo If your phone booted Android instead of the loader, you can
    echo flash the loader temporarily to the boot partition:
    echo   fastboot flash boot %LOADER%
    echo   fastboot reboot bootloader
    echo ------------------------------------------------------------------
    pause
    exit /b 1
)

echo [SUCCESS] HakiOS resident loader is active in phone DRAM!

echo.
echo ==============================================================================
if defined SYS (
    echo [Step 2/4] Allocating DRAM ramdisk buffer and streaming SystemOS.apfs...
    echo ==============================================================================
    fastboot oem ramdisk
    fastboot stage "%SYS%"
) else (
    echo [Step 2/4] Configuring minimal ramdisk...
    echo ==============================================================================
    fastboot oem ramdisk none
)

echo.
echo ==============================================================================
echo [Step 3/4] Staging XNU Darwin Kernel Payload (%PAYLOAD%)...
echo ==============================================================================
fastboot stage "%PAYLOAD%"

echo.
echo ==============================================================================
echo [Step 4/4] Executing Darwin XNU Kernel on MediaTek Helio G80...
echo ==============================================================================
fastboot oem run

echo.
echo Kernel is now executing in RAM!
echo Waiting 15 seconds to collect logs...
timeout /t 15 /nobreak >nul

if not exist "runs" mkdir runs
set "RUN_TAG=pova_%time:~0,2%%time:~3,2%%time:~6,2%"
set "RUN_TAG=%RUN_TAG: =0%"

fastboot oem log >nul 2>nul
fastboot get_staged "runs\%RUN_TAG%.log" >nul 2>nul
if exist "runs\%RUN_TAG%.log" (
    echo Kernel log saved to: runs\%RUN_TAG%.log
)

fastboot oem screenshot >nul 2>nul
fastboot get_staged "runs\%RUN_TAG%.raw" >nul 2>nul
if exist "runs\%RUN_TAG%.raw" (
    echo Live framebuffer screenshot saved to: runs\%RUN_TAG%.raw
)

echo.
echo ==============================================================================
echo HakiOS Darwin is running on your Tecno Pova!
echo All data is resident in RAM.
echo To return cleanly to Android at any time:
echo   fastboot reboot
echo Or hold the Power button for 5 seconds.
echo ==============================================================================
pause
