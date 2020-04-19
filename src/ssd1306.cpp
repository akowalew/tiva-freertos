///////////////////////////////////////////////////////////////////////////////
// SSD1306 I2C OLED display management 
///////////////////////////////////////////////////////////////////////////////

// I2C address of the slave chip
constexpr u8 SSD1306_ADDR = 0x3C;

// After the transmission of the slave address, either the control byte or 
// the data byte may be sent across the SDA. A control byte mainly consists
// of Co and D/C# bits following by six “0” ‘s:
// a) If the Co bit is set as logic “0”, the transmission of the following
//   information will contain data bytes only.
// b) The D/C# bit determines the next data byte is acted as a command
//   or a data. If the D/C# bit is set to logic “0”, it defines the following 
//   data byte as a command. If the D/C# bit is set to logic “1”, it defines 
//   the following data byte as a data which will be stored at the GDDRAM.
//   The GDDRAM column address pointer will be increased by one automatically 
//   after each data write

constexpr u8 SSD1306_CO_BIT = (1 << 7); // Continuation bit
constexpr u8 SSD1306_DC_BIT = (1 << 6); // Data-Command selection bit

#define SSD1306_CHARGEPUMP_OFF 0x00
#define SSD1306_CHARGEPUMP_ON 0x04
#define SSD1306_SET_CHARGEPUMP(chargepump) 0x8D, (0x10 | chargepump)
#define SSD1306_SET_CHARGEPUMP_OFF 0x8D, 0x10
#define SSD1306_SET_CHARGEPUMP_ON 0x8D, 0x14

#define SSD1306_SET_COLUMN_LOW(column) (u8)(column & 0x0F)
#define SSD1306_SET_COLUMN_HIGH(column) (u8)(0x10 | (column >> 4))
#define SSD1306_SET_COLUMN(column) SSD1306_SET_COLUMN_HIGH(column), SSD1306_SET_COLUMN_LOW(column)

#define SSD1306_PUMP_VOLTAGE_6_4V 0x00
#define SSD1306_PUMP_VOLTAGE_7_4V 0x01
#define SSD1306_PUMP_VOLTAGE_8_0V 0x02
#define SSD1306_PUMP_VOLTAGE_9_0V 0x03
#define SSD1306_SET_PUMP_VOLTAGE(voltage) (0x30 | voltage)
#define SSD1306_SET_PUMP_VOLTAGE_6_4V 0x30
#define SSD1306_SET_PUMP_VOLTAGE_7_4V 0x31
#define SSD1306_SET_PUMP_VOLTAGE_8_0V 0x32
#define SSD1306_SET_PUMP_VOLTAGE_9_0V 0x33

#define SSD1306_SET_START_LINE(line) (0x40 | line)

#define SSD1306_SET_CONTRAST(contrast) 0x81, contrast

#define SSD1306_REMAP_NORMAL 0x00
#define SSD1306_REMAP_REVERSE 0x01
#define SSD1306_SET_REMAP(remap) (0xA0 | remap)
#define SSD1306_SET_REMAP_NORMAL 0xA0
#define SSD1306_SET_REMAP_REVERSE 0xA1

#define SSD1306_ENTIREDISPLAY_OFF 0x00
#define SSD1306_ENTIREDISPLAY_ON 0x01
#define SSD1306_SET_ENTIREDISPLAY(entiredisplay) (0xA4 | entiredisplay)
#define SSD1306_SET_ENTIREDISPLAY_OFF 0xA4
#define SSD1306_SET_ENTIREDISPLAY_ON 0xA5

#define SSD1306_INVERT_OFF 0x00
#define SSD1306_INVERT_ON 0x01
#define SSD1306_SET_INVERT(invert) (0xA6 | invert)
#define SSD1306_SET_INVERT_OFF 0xA6
#define SSD1306_SET_INVERT_ON 0xA7

#define SSD1306_SET_MUXRATIO(muxratio) 0xA8, muxratio

#define SSD1306_DCDC_OFF 0x00
#define SSD1306_DCDC_ON 0x01
#define SSD1306_SET_DCDC(dcdc) 0xAD, (0x8A | dcdc)
#define SSD1306_SET_DCDC_OFF 0xAD, 0x8A
#define SSD1306_SET_DCDC_ON 0xAD, 0x8B

#define SSD1306_DISPLAY_OFF 0x00
#define SSD1306_DISPLAY_ON 0x01
#define SSD1306_SET_DISPLAY(display) 0xAE
#define SSD1306_SET_DISPLAY_OFF 0xAE
#define SSD1306_SET_DISPLAY_ON 0xAF

#define SSD1306_SET_PAGE(page) (u8)((u8)0xB0 | page)

#define SSD1306_SCANDIR_NORMAL 0x00
#define SSD1306_SCANDIR_REVERSE 0x08
#define SSD1306_SET_SCANDIR(scandir) (0xC0 | scandir)
#define SSD1306_SET_SCANDIR_NORMAL 0xC0
#define SSD1306_SET_SCANDIR_REVERSE 0xC8

#define SSD1306_SET_OFFSET(offset) 0xD3, offset

#define SSD1306_OSCFREQ_0_75_FOSC 0x00 // 0.75 x Fosc
#define SSD1306_OSCFREQ_0_80_FOSC 0x01 // 0.80 x Fosc
#define SSD1306_OSCFREQ_0_85_FOSC 0x02 // 0.85 x Fosc
#define SSD1306_OSCFREQ_0_90_FOSC 0x03 // 0.90 x Fosc
#define SSD1306_OSCFREQ_0_95_FOSC 0x04 // 0.95 x Fosc
#define SSD1306_OSCFREQ_1_00_FOSC 0x05 // 1.00 x Fosc
#define SSD1306_OSCFREQ_1_05_FOSC 0x06 // 1.05 x Fosc
#define SSD1306_OSCFREQ_1_10_FOSC 0x07 // 1.10 x Fosc
#define SSD1306_OSCFREQ_1_15_FOSC 0x08 // 1.15 x Fosc
#define SSD1306_OSCFREQ_1_20_FOSC 0x09 // 1.20 x Fosc
#define SSD1306_OSCFREQ_1_25_FOSC 0x0A // 1.25 x Fosc
#define SSD1306_OSCFREQ_1_30_FOSC 0x0B // 1.30 x Fosc
#define SSD1306_OSCFREQ_1_35_FOSC 0x0C // 1.35 x Fosc
#define SSD1306_OSCFREQ_1_40_FOSC 0x0D // 1.40 x Fosc
#define SSD1306_OSCFREQ_1_45_FOSC 0x0E // 1.45 x Fosc
#define SSD1306_OSCFREQ_1_50_FOSC 0x0F // 1.50 x Fosc
#define SSD1306_SET_CLKDIV_OSCFREQ(clkdiv, oscfreq) 0xD5, (clkdiv | (oscfreq << 4))

#define SSD1306_SET_CHARGEPERIOD_PRE(pre) (pre)
#define SSD1306_SET_CHARGEPERIOD_DIS(dis) (dis << 4)
#define SSD1306_SET_CHARGEPERIOD(pre, dis) 0xD9, SSD1306_SET_CHARGEPERIOD_PRE(pre) | SSD1306_SET_CHARGEPERIOD_DIS(dis)

#define SSD1306_PADSCONFIG_SEQUENTIAL 0x00
#define SSD1306_PADSCONFIG_ALTERNATIVE 0x10
#define SSD1306_SET_PADSCONFIG(pads) 0xDA, (0x02 | pads)
#define SSD1306_SET_PADSCONFIG_SEQUENTIAL 0xDA, 0x02
#define SSD1306_SET_PADSCONFIG_ALTERNATIVE 0xDA, 0x12

// Vcom = Beta * Vref = (0.430 + Vcomdesel*0.006415) * Vref
#define SSD1306_VCOMDESEL_0_430 0x00
#define SSD1306_VCOMDESEL_0_770 0x35
#define SSD1306_VCOMDESEL_1_000 0x40
#define SSD1306_SET_VCOMDESEL(vcomdesel) 0xDB, vcomdesel
#define SSD1306_SET_VCOMDESEL_0_430 0xDB, 0x00
#define SSD1306_SET_VCOMDESEL_0_770 0xDB, 0x35
#define SSD1306_SET_VCOMDESEL_1_000 0xDB, 0x40

#define SSD1306_READ_MODIFY_WRITE 0xE0

#define SSD1306_END 0xEE

#define SSD1306_NOP 0xE3

#define SSD1306_CTRL_ONE_CMD SSD1306_CO_BIT
#define SSD1306_CTRL_ONE_DATA (SSD1306_CO_BIT | SSD1306_DC_BIT)
#define SSD1306_CTRL_CMDS 0x00
#define SSD1306_CTRL_DATA (SSD1306_DC_BIT)

constexpr u8 SSD1306_RAMWIDTH = 132;
constexpr u8 SSD1306_RAMHEIGHT = 64;

constexpr u8 SSD1306_COLS = 128;
constexpr u8 SSD1306_ROWS = 64;
constexpr u8 SSD1306_PAGES = (SSD1306_ROWS/8);

//
// Public functions
//

void ssd1306_init()
{
}

void ssd1306_write_cmds(const u8* cmds, u8 size)
{
	CHECK(i2c_write(cmds, size, 
		SSD1306_CTRL_CMDS, SSD1306_ADDR));	
}

template<u8 N>
void ssd1306_write_cmds(u8 const (&cmds)[N])
{
	ssd1306_write_cmds(cmds, N);
}

void ssd1306_write_data(const u8* data, u8 size)
{
	CHECK(i2c_write(data, size, 
		SSD1306_CTRL_DATA, SSD1306_ADDR));	
}

template<u8 N>
void ssd1306_write_data(u8 const (&data)[N])
{
	ssd1306_write_data(data, N);
}

void ssd1306_setpos(u8 x, u8 page)
{
	assert(x < SSD1306_COLS);
	assert(page < SSD1306_PAGES);

	u8 cmds[] = {
		SSD1306_SET_COLUMN_HIGH(x), 
		SSD1306_SET_COLUMN_LOW(x), 
		SSD1306_SET_PAGE(page)
	};

	ssd1306_write_cmds(cmds);
}

void ssd1306_setxy(u8 x, u8 y)
{
	assert(x < SSD1306_COLS);

	const auto page = (y/8);
	assert(page < SSD1306_PAGES);

	u8 cmds[] = {
		SSD1306_SET_COLUMN_HIGH(x), 
		SSD1306_SET_COLUMN_LOW(x), 
		SSD1306_SET_PAGE(page)
	};

	ssd1306_write_cmds(cmds);
}

void ssd1306_clear()
{
	u8 data[SSD1306_COLS] = { 0 };
	// memset(ssd1306_data, 0x00, sizeof(ssd1306_data));
	for(u8 page = 0; page < SSD1306_PAGES; ++page) {
		ssd1306_setpos(0, page);
		ssd1306_write_data(data);
	}
}

void ssd1306_startup()
{
	// Display initialization commands
	constexpr u8 SSD1306_INITDATA[] = {
		SSD1306_SET_DISPLAY_OFF,
		SSD1306_SET_COLUMN_LOW(0),
		SSD1306_SET_START_LINE(0),
		SSD1306_SET_CONTRAST(0x50),
		SSD1306_SET_PUMP_VOLTAGE_7_4V,
		SSD1306_SET_REMAP_REVERSE,
		SSD1306_SET_ENTIREDISPLAY_OFF,
		SSD1306_SET_INVERT_OFF,
		SSD1306_SET_MUXRATIO(63),
		SSD1306_SET_SCANDIR_REVERSE,
		SSD1306_SET_OFFSET(0),
		SSD1306_SET_CLKDIV_OSCFREQ(1, SSD1306_OSCFREQ_1_15_FOSC),
		SSD1306_SET_CHARGEPERIOD(1, 15),
		SSD1306_SET_PADSCONFIG_ALTERNATIVE,		
		SSD1306_SET_VCOMDESEL_0_770,
		SSD1306_SET_CHARGEPUMP_ON,
		SSD1306_SET_DISPLAY_ON,
	};

	ssd1306_write_cmds(SSD1306_INITDATA);
	ssd1306_clear();
}

void ssd1306_display_on()
{
	u8 cmds[] = {
		SSD1306_SET_DISPLAY_ON
	};

	ssd1306_write_cmds(cmds);
}

void ssd1306_display_off()
{
	u8 cmds[] = {
		SSD1306_SET_DISPLAY_OFF
	};

	ssd1306_write_cmds(cmds);
}