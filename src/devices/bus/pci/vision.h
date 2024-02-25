// license:BSD-3-Clause
// copyright-holders: Angelo Salese

#ifndef MAME_BUS_PCI_S3VISION_PCI_H
#define MAME_BUS_PCI_S3VISION_PCI_H

#pragma once

#include "pci_slot.h"

#include "video/pc_vga_s3.h"

class vision864_device : public pci_card_device
{
public:
	vision864_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	static constexpr feature_type imperfect_features() { return feature::GRAPHICS; }

	void legacy_io_map(address_map &map);

protected:
	vision864_device(const machine_config &mconfig, device_type type, const char *tag, device_t *owner, uint32_t clock);

	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_add_mconfig(machine_config &config) override;

	virtual const tiny_rom_entry *device_rom_region() const override;

	virtual void map_extra(uint64_t memory_window_start, uint64_t memory_window_end, uint64_t memory_offset, address_space *memory_space,
						   uint64_t io_window_start, uint64_t io_window_end, uint64_t io_offset, address_space *io_space) override;

	required_device<s3_vga_device> m_vga;
	required_memory_region m_bios;
private:
	u8 vram_r(offs_t offset);
	void vram_w(offs_t offset, uint8_t data);
};

class vision968_device : public vision864_device
{
public:
	vision968_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

protected:
	virtual void device_add_mconfig(machine_config &config) override;

	virtual const tiny_rom_entry *device_rom_region() const override;
};

DECLARE_DEVICE_TYPE(VISION864_PCI, vision864_device)
DECLARE_DEVICE_TYPE(VISION968_PCI, vision968_device)

#endif // MAME_BUS_PCI_S3VISION_PCI_H
