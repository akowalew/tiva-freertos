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

constexpr u8 CO_BIT = (1 << 7); // Continuation bit
constexpr u8 DC_BIT = (1 << 6); // Data-Command selection bit

#define SSD1306_CHARGEPUMP_OFF 0x00
#define SSD1306_CHARGEPUMP_ON 0x04
#define SSD1306_SET_CHARGEPUMP(chargepump) 0x8D, (0x10 | chargepump)
#define SSD1306_SET_CHARGEPUMP_OFF 0x8D, 0x10
#define SSD1306_SET_CHARGEPUMP_ON 0x8D, 0x14

#define SSD1306_SET_COLUMN_LOW(column) (column & 0x0F)
#define SSD1306_SET_COLUMN_HIGH(column) (0x10 | (column >> 4))
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

#define SSD1306_CTRL_CMD CO_BIT
#define SSD1306_CTRL_DATA (CO_BIT | DC_BIT)
#define SSD1306_CTRL_LAST_CMDS 0x00
#define SSD1306_CTRL_LAST_DATA (DC_BIT)

// Preamble:
// 	1: CTRL_CMD
// 	2: SET_COLUMN_LOW(0)
//  3: SET_CONTRAST() ???
//  4: SET_INVERT() ???
//
// Page0:
// 	0: CTRL_CMD
//	1: SET_COLUMN_HIGH(0)
//	2: CTRL_CMD
//	3: SET_PAGE(0)
//	4: CTRL_LAST_DATA
//	5..n-1: ... <data> ...
//
// Page1:
// 	0: CTRL_CMD
//	1: SET_COLUMN_HIGH(0)
//	2: CTRL_CMD
//	3: SET_PAGE(1)
//	4: CTRL_LAST_DATA
//	5..n-1: ... <data> ...
//
// etc

constexpr u8 SSD1306_RAMWIDTH = 132;
constexpr u8 SSD1306_RAMHEIGHT = 64;

constexpr u8 SSD1306_COLS = 128;
constexpr u8 SSD1306_ROWS = 64;
constexpr u8 SSD1306_PAGES = (SSD1306_ROWS/8);

//
// Controls and commands headers for each display page 
// They will be sent before sending each page in order to set proper
// page and column numbers in display controller.
// 
constexpr u8 SSD1306_HEADER_SIZE = 7;
constexpr u8 headers[SSD1306_PAGES][SSD1306_HEADER_SIZE] = {
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(0), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(1), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(2), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(3), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(4), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(5), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(6), SSD1306_CTRL_LAST_DATA },
	{ SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_HIGH(0), SSD1306_CTRL_CMD, SSD1306_SET_COLUMN_LOW(0), SSD1306_CTRL_CMD, SSD1306_SET_PAGE(7), SSD1306_CTRL_LAST_DATA },
};

//
// Display initialization commands
//

constexpr u8 ssd1306_initcmds[] = {
	SSD1306_CTRL_LAST_CMDS,
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

//
// Display buffer
// It is organized as 8-bit pages, so writing e.g. 0xFF to some column causes
// to turn ON 8 pixels in 8 next rows in given column.
//
u8 display[SSD1306_PAGES][SSD1306_COLS];

// // 
// // Display I2C buffers configuration 
// // 
// constexpr u8 SSD1306_BUFFERS_COUNT = 2;
// constexpr Buffer buffers[SSD1306_BUFFERS_COUNT] = {
// 	{&headers[0][0], SSD1306_HEADER_SIZE}, 
// 	{&display[0][0], SSD1306_COLS}
// };

static void ssd1306_task([[maybe_unused]] void* params)
{
	// Initialize display
	CHECK(i2c_write(ssd1306_initcmds, sizeof(ssd1306_initcmds), SSD1306_ADDR));

	while(true)
	{
		// Do nothing
		vTaskDelay(portMAX_DELAY);
	}
}

void ssd1306_init()
{
	const auto stacksize = configMINIMAL_STACK_SIZE;
	const auto params = nullptr;
	const auto priority = (tskIDLE_PRIORITY + 1);
	CHECK(xTaskCreate(ssd1306_task, "ssd1306", stacksize, params, priority, nullptr));
}

// int ssd1306_write()
// {
// 	if(i2c_multiwrite(&buffers[0], SSD1306_BUFFERS_COUNT, SSD1306_PAGES, SSD1306_ADDR) == -1) {
// 		uart_put("Could not write buffers to SSD1306\n");
// 		return -1;
// 	}

// 	return 0;
// }

// void clear_display()
// {
// 	memset(display, 0x00, sizeof(display));
// }

// void draw_hline(u8 x1, u8 y1, u8 x2)
// {
// 	assert(x1 <= x2);
// 	assert(x2 < SSD1306_COLS);
// 	assert(y1 < SSD1306_ROWS);

// 	const u8 page = (u8)(y1 >> 3);
// 	const u8 bit = (u8)(y1 & 0x07);
// 	const u8 mask = (u8)(1 << bit);

// 	for(u8 x = x1; x <= x2; ++x) {
// 		display[page][x] |= mask;
// 	}
// }

// // ...
// // 0
// // 1
// // 1
// // ============
// // 1
// // 1
// // 1
// // 1
// // 1
// // 1
// // 1
// // 1
// // ============
// // 1
// // 1
// // 0
// // ...

// u8 getmask(u8 bit_from, u8 bit_to)
// {
// 	assert(bit_from <= bit_to);
// 	assert(bit_to < 8);

// 	constexpr u8 lut[8][8] = {
// 	    //       0     1     2     3     4     5     6     7    <- bit2
// 	    /* 0 */ 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF,
// 	    /* 1 */  0,   0x02, 0x06, 0x0E, 0x1E, 0x3E, 0x7E, 0xFE,
// 	    /* 2 */  0,    0,   0x04, 0x0C, 0x1C, 0x3C, 0x7C, 0xFC,
// 	    /* 3 */  0,    0,    0,   0x08, 0x18, 0x38, 0x78, 0xF8,
// 	    /* 4 */  0,    0,    0,    0,   0x10, 0x30, 0x70, 0xF0,
// 	    /* 5 */  0,    0,    0,    0,    0,   0x20, 0x60, 0xE0,
// 	    /* 6 */  0,    0,    0,    0,    0,    0,   0x40, 0xC0,
// 	    /* 7 */  0,    0,    0,    0,    0,    0,    0,   0x80,
// 	    // ^
// 	    // bit1
// 	};

// 	return lut[bit_from][bit_to];
// }

// void draw_vline(u8 x1, u8 y1, u8 y2)
// {
// 	assert(y1 <= y2);
// 	assert(y2 < SSD1306_ROWS);
// 	assert(x1 < SSD1306_COLS);

// 	const u8 page1 = (u8)(y1 >> 3);
// 	const u8 page2 = (u8)(y2 >> 3);
// 	const u8 bit1 = (y1 & 0x07);
// 	const u8 bit2 = (y2 & 0x07);

// 	assert(page1 <= page2);
// 	if(page1 == page2)
// 	{
// 		assert(bit1 <= bit2);
// 		display[page1][x1] |= getmask(bit1, bit2);
// 	}
// 	else
// 	{
// 		display[page1][x1] |= getmask(bit1, 7);
// 		for(u8 page = page1+1; page < page2; ++page)
// 			display[page][x1] = 0xFF;
// 		display[page2][x1] |= getmask(0, bit2);
// 	}
// }

// void draw_rect(u8 x1, u8 y1, u8 x2, u8 y2)
// {
// 	draw_hline(x1, y1, x2);
// 	draw_hline(x1, y2, x2);
// 	draw_vline(x1, y1, y2);
// 	draw_vline(x2, y1, y2);
// }

// struct crd_x {};
// struct crd_y {};
// struct dim_x {};
// struct dim_y {};

// void draw_rectf(u8 x1, u8 y1, u8 x2, u8 y2)
// {
// 	assert(x1 <= x2);
// 	assert(y1 <= y2);
// 	assert(y2 < SSD1306_ROWS);
// 	assert(x2 < SSD1306_COLS);

// 	const u8 page1 = (y1 >> 3);
// 	const u8 page2 = (y2 >> 3);
// 	const u8 bit1 = (y1 & 0x07);
// 	const u8 bit2 = (y2 & 0x07);

// 	assert(page1 <= page2);
// 	if(page1 == page2)
// 	{
// 		assert(bit1 <= bit2);
// 		const auto mask = getmask(bit1, bit2);
// 		for(u8 x = x1; x <= x2; ++x)
// 			display[page1][x] |= mask;
// 	}
// 	else
// 	{
// 		const auto mask1 = getmask(bit1, 7);
// 		for(u8 x = x1; x <= x2; ++x)
// 			display[page1][x] |= mask1;

// 		const auto mask = 0xFF;
// 		for(u8 page = page1+1; page < page2; ++page)
// 			memset(&display[page][x1], mask, x2-x1+1);

// 		const auto mask2 = getmask(0, bit2);
// 		for(u8 x = x1; x <= x2; ++x)
// 			display[page2][x] |= mask2;
// 	}
// }