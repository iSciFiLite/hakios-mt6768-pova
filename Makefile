# ==============================================================================
# HakiOS MT6768 POVA Unified — Makefile
# Inclusive Development of UEFI Firmware & HakiOS XNU Kernel Ecosystem
# ==============================================================================

SHELL := /usr/bin/env bash
DEVICE ?= pova
JOBS ?= $(shell nproc 2>/dev/null || echo 4)

.PHONY: all help audit check test-config uefi-audit hakios-check \
        split-subtrees sync-subtrees clean

all: help

help:
	@echo "HakiOS MT6768 POVA Unified Development (Tecno Pova LD7J)"
	@echo "========================================================"
	@echo "Inclusive build & audit targets:"
	@echo "  make audit           - Run all UEFI memory-map, firmware, and HakiOS audits"
	@echo "  make check           - Validate HakiOS device profiles and defconfigs"
	@echo "  make test-config     - Test HakiOS Pova defconfig generation"
	@echo "  make uefi-audit      - Run POVA UEFI firmware and memory map checks"
	@echo "  make hakios-check    - Check HakiOS MT6768 driver integrity and platform stubs"
	@echo ""
	@echo "HakiOS OS & Kernel targets (inside hakios/):"
	@echo "  make defconfig DEVICE=pova  - Generate HakiOS configuration for POVA"
	@echo "  make loader DEVICE=pova     - Compile xnuloader.efi for MT6768"
	@echo "  make kernel DEVICE=pova     - Compile HakiOS XNU kernel for MT6768"
	@echo ""
	@echo "UEFI Firmware targets (inside uefi/):"
	@echo "  See uefi/README.md for hosted or local Mu-Silicium build steps"

audit: uefi-audit hakios-check

uefi-audit:
	@echo "==> Auditing POVA UEFI memory map..."
	python3 uefi/tools/audit-memory-map.py
	@echo "==> Auditing POVA UEFI firmware declarations..."
	python3 uefi/tools/firmware-audit.py

hakios-check:
	@echo "==> Checking HakiOS device definitions..."
	python3 hakios/tools/hakios-device.py check pova
	@echo "==> Checking HakiOS driver metadata..."
	python3 hakios/tools/drivers-check.py
	@echo "==> Testing HakiOS POVA configuration..."
	python3 hakios/tools/hakios-config.py defconfig pova
	@echo "==> Defconfig generated successfully!"

check: audit

test-config:
	@echo "==> Testing Pova defconfig..."
	python3 hakios/tools/hakios-config.py defconfig pova

defconfig:
	@echo "==> Configuring HakiOS for $(DEVICE)..."
	python3 hakios/tools/hakios-config.py defconfig $(DEVICE)

loader:
	@echo "==> Building HakiOS xnuloader for $(DEVICE)..."
	$(MAKE) -C hakios DEVICE=$(DEVICE) loader

kernel:
	@echo "==> Building HakiOS XNU kernel for $(DEVICE)..."
	$(MAKE) -C hakios DEVICE=$(DEVICE) JOBS=$(JOBS) kernel

clean:
	rm -f hakios/.config hakios/.config.old
