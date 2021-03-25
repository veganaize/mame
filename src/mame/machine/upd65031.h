// license:BSD-3-Clause
// copyright-holders:Sandro Ronco
/**********************************************************************

    NEC uPD65031 'BLINK' emulation

**********************************************************************/

#ifndef MAME_MACHINE_UPD65031_H
#define MAME_MACHINE_UPD65031_H

#pragma once


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

#define UPD65031_SCREEN_UPDATE(_name) void _name(bitmap_ind16 &bitmap, uint16_t sbf, uint16_t hires0, uint16_t hires1, uint16_t lores0, uint16_t lores1, int flash)
#define UPD65031_MEMORY_UPDATE(_name) void _name(int bank, uint16_t page, int rams)


// ======================> upd65031_device

class upd65031_device : public device_t
{
public:
	typedef device_delegate<void (bitmap_ind16 &bitmap, uint16_t sbf, uint16_t hires0, uint16_t hires1, uint16_t lores0, uint16_t lores1, int flash)> screen_update_delegate;
	typedef device_delegate<void (int bank, uint16_t page, int rams)> memory_update_delegate;

	// construction/destruction
	upd65031_device(const machine_config &mconfig, const char *tag, device_t *owner, uint32_t clock);

	auto kb_rd_callback() { return m_read_kb.bind(); }
	auto int_wr_callback() { return m_write_int.bind(); }
	auto nmi_wr_callback() { return m_write_nmi.bind(); }
	auto spkr_wr_callback() { return m_write_spkr.bind(); }

	template <typename... T> void set_screen_update_callback(T &&... args) { m_screen_update_cb.set(std::forward<T>(args)...); }
	template <typename... T> void set_memory_update_callback(T &&... args) { m_out_mem_cb.set(std::forward<T>(args)...); }

	uint8_t read(offs_t offset);
	void write(offs_t offset, uint8_t data);
	DECLARE_WRITE_LINE_MEMBER( flp_w );
	DECLARE_WRITE_LINE_MEMBER( btl_w );
	uint32_t screen_update(screen_device &screen, bitmap_ind16 &bitmap, const rectangle &cliprect);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_timer(emu_timer const &timer, device_timer_id id, int param, void *ptr) override;

private:
	inline void interrupt_refresh();
	inline void update_rtc_interrupt();
	inline void set_mode(int mode);
	static const device_timer_id TIMER_RTC = 0;
	static const device_timer_id TIMER_FLASH = 1;
	static const device_timer_id TIMER_SPEAKER = 2;

	devcb_read8        m_read_kb;
	devcb_write_line   m_write_int;
	devcb_write_line   m_write_nmi;
	devcb_write_line   m_write_spkr;

	screen_update_delegate m_screen_update_cb;  // callback for update the LCD
	memory_update_delegate m_out_mem_cb;        // callback for update bankswitch

	int     m_mode;
	uint16_t  m_lcd_regs[5];      // LCD registers
	uint8_t   m_tim[5];           // RTC registers
	uint8_t   m_sr[4];            // segment registers
	uint8_t   m_sta;              // interrupt status
	uint8_t   m_int;              // interrupts mask
	uint8_t   m_ack;              // interrupts acknowledge
	uint8_t   m_tsta;             // timer interrupt status
	uint8_t   m_tmk;              // timer interrupt mask
	uint8_t   m_tack;             // timer interrupts acknowledge
	uint8_t   m_com;              // command register
	int     m_flash;            // cursor flash
	int     m_speaker_state;    // spkr line

	// timers
	emu_timer *m_rtc_timer;
	emu_timer *m_flash_timer;
	emu_timer *m_speaker_timer;
};


// device type definition
DECLARE_DEVICE_TYPE(UPD65031, upd65031_device)


#endif // MAME_MACHINE_UPD65031_H
