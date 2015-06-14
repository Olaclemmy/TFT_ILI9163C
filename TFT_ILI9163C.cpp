#include "TFT_ILI9163C.h"

#ifdef __AVR__
	#include <avr/pgmspace.h>
#elif defined(ESP8266)
//none
#elif defined(__SAM3X8E__)
	#include <include/pio.h>
	#define PROGMEM
	#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
	#define pgm_read_word(addr) (*(const unsigned short *)(addr))
	typedef unsigned char prog_uchar;
#endif
	
//constructors

#if defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0, Teensy 3.1
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin,uint8_t mosi,uint8_t sclk)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
	}
#elif defined(__MKL26Z64__)//Teensy LC
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin,uint8_t mosi,uint8_t sclk)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
		_mosi = mosi;
		_sclk = sclk;
		_useSPI1 = false;
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) _useSPI1 = true;
	}
#else//Arduino's and unknown CPU
	TFT_ILI9163C::TFT_ILI9163C(uint8_t cspin,uint8_t dcpin,uint8_t rstpin)
	{
		_cs   = cspin;
		_rs   = dcpin;
		_rst  = rstpin;
	}
#endif




//Arduino Uno, Leonardo, Mega, Teensy 2.0, etc
#if defined(__AVR__)
	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite(d >> 8);
			spiwrite(d);
		endTransaction();
	} 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		#if !defined (SPI_HAS_TRANSACTION)
			if (n >= 8000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV2);
			} else if (n >= 4000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV4);
			} else if (n >= 2000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV8);
			} else {
				SPI.setClockDivider(SPI_CLOCK_DIV16);
			}
		#endif
	}
#elif defined(__SAM3X8E__)// Arduino Due

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	}


	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		#if !defined(SPI_HAS_TRANSACTION)
			uint32_t divider = 1;
			while (divider < 255) {
				if (n >= 84000000 / divider) break;
				divider = divider - 1;
			}
			SPI.setClockDivider(divider);
		#endif
	}
#elif defined(__MKL26Z64__)//Teensy LC

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 
	

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	} 
	


	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
#elif defined(__MK20DX128__) || defined(__MK20DX256__)//Teensy 3.0 & 3.1 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		//nop
	}
#else

	void TFT_ILI9163C::writecommand(uint8_t c)
	{
		startTransaction();
			enableCommandStream();
			spiwrite(c);
		endTransaction();
	}

	void TFT_ILI9163C::writedata(uint8_t c)
	{
		startTransaction();
			enableDataStream();
			spiwrite(c);
		endTransaction();
	} 

	void TFT_ILI9163C::writedata16(uint16_t d)
	{
		startTransaction();
			enableDataStream();
			spiwrite16(d);
		endTransaction();
	} 

	void TFT_ILI9163C::setBitrate(uint32_t n)
	{
		#if !defined (SPI_HAS_TRANSACTION)
			if (n >= 8000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV2);
			} else if (n >= 4000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV4);
			} else if (n >= 2000000) {
				SPI.setClockDivider(SPI_CLOCK_DIV8);
			} else {
				SPI.setClockDivider(SPI_CLOCK_DIV16);
			}
		#endif
	}
#endif


void TFT_ILI9163C::begin(void) 
{
	sleep = 0;
	_portrait = false;
	_initError = 0b00000000;
	_width    = _TFTWIDTH;
	_height   = _TFTHEIGHT;
	rotation  = 0;
	cursor_y  = cursor_x    = 0;
	textsize  = 1;
	_defaultBackground = _ILI9163C_BACKGROUND;
	_defaultForeground = _ILI9163C_FOREGROUND;
	textcolor = textbgcolor = _defaultForeground;
	wrap      = true;
	setFont(GFXFONT_GLCD);
	_arcAngleMax = 360;
	_arcAngleOffset = -90;
#if defined(__AVR__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = portOutputRegister(digitalPinToPort(_cs));
	rsport    = portOutputRegister(digitalPinToPort(_rs));
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	*csport |= cspinmask;//hi
	enableDataStream();
#elif defined(__SAM3X8E__)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	csport    = digitalPinToPort(_cs);
	rsport    = digitalPinToPort(_rs);
	cspinmask = digitalPinToBitMask(_cs);
	rspinmask = digitalPinToBitMask(_rs);
    SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(5); // 8 MHz
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
	#endif
	csport->PIO_SODR  |=  cspinmask;//HI
	enableDataStream();
#elif defined(__MKL26Z64__)//Teensy LC (preliminary)
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (_useSPI1){
		if ((_mosi == 0 || _mosi == 21) && (_sclk == 20)) {//identify alternate SPI channel 1 (24Mhz)
			ILI9163C_SPI = SPISettings(24000000, MSBFIRST, SPI_MODE0);
			SPI1.setMOSI(_mosi);
			SPI1.setSCK(_sclk);
			SPI1.begin();
			_useSPI1 = true; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	} else {
		if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {//valid SPI pins?
			ILI9163C_SPI = SPISettings(12000000, MSBFIRST, SPI_MODE0);
			SPI.setMOSI(_mosi);
			SPI.setSCK(_sclk);
			SPI.begin();
			_useSPI1 = false; //confirm
		} else {
			bitSet(_initError,0);
			return;
		}
		if (!SPI.pinIsChipSelect(_cs)) {//ERROR
			bitSet(_initError,1);
			return;
		}
	}
	#if defined(_TEENSYLC_FASTPORT)
		csportSet    	= portSetRegister(digitalPinToPort(_cs));
		csportClear     = portClearRegister(digitalPinToPort(_cs));
		cspinmask 		= digitalPinToBitMask(_cs);
		_dcState = 1;
		dcportSet       = portSetRegister(digitalPinToPort(_rs));
		dcportClear     = portClearRegister(digitalPinToPort(_rs));
		dcpinmask	    = digitalPinToBitMask(_rs);
	#endif
		#if !defined(_TEENSYLC_FASTPORT)
			digitalWriteFast(_cs,HIGH);
		#else
			*csportSet = cspinmask;
		#endif
		enableDataStream();
#elif defined(__MK20DX128__) || defined(__MK20DX256__)
	ILI9163C_SPI = SPISettings(30000000, MSBFIRST, SPI_MODE0);
	if ((_mosi == 11 || _mosi == 7) && (_sclk == 13 || _sclk == 14)) {
        SPI.setMOSI(_mosi);
        SPI.setSCK(_sclk);
	} else {
		bitSet(_initError,0);
		return;
	}
	SPI.begin();
	if (SPI.pinIsChipSelect(_cs, _rs)) {
		pcs_data = SPI.setCS(_cs);
		pcs_command = pcs_data | SPI.setCS(_rs);
	} else {
		pcs_data = 0;
		pcs_command = 0;
		bitSet(_initError,1);
		return;
	}
#else//all the rest of possible boards
	pinMode(_rs, OUTPUT);
	pinMode(_cs, OUTPUT);
	SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		SPI.setClockDivider(4);
		SPI.setBitOrder(MSBFIRST);
		SPI.setDataMode(SPI_MODE0);
	#else
		ILI9163C_SPI = SPISettings(8000000, MSBFIRST, SPI_MODE0);
	#endif
	digitalWrite(_cs,HIGH);
	enableDataStream();
#endif
	if (_rst != 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(500);
		digitalWrite(_rst, LOW);
		delay(500);
		digitalWrite(_rst, HIGH);
		delay(500);
	}

/*
7) MY:  1(bottom to top), 0(top to bottom) 	| Row Address Order
6) MX:  1(R to L),        0(L to R)        	| Column Address Order
5) MV:  1(Exchanged),     0(normal)        	| Row/Column exchange
4) ML:  1(bottom to top), 0(top to bottom) 	| Vertical Refresh Order
3) RGB: 1(BGR), 		  0(RGB)            | Color Space
2) MH:  1(R to L),        0(L to R)        	| Horizontal Refresh Order
1)
0)

     MY, MX, MV, ML,RGB, MH, D1, D0
	 0 | 0 | 0 | 0 | 1 | 0 | x | x	//normal
	 1 | 0 | 0 | 0 | 1 | 0 | x | x	//Y-Mirror
	 0 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Mirror
	 1 | 1 | 0 | 0 | 1 | 0 | x | x	//X-Y-Mirror
	 0 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange
	 1 | 0 | 1 | 0 | 1 | 0 | x | x	//X-Y Exchange, Y-Mirror
	 0 | 1 | 1 | 0 | 1 | 0 | x | x	//XY exchange
	 1 | 1 | 1 | 0 | 1 | 0 | x | x
*/
	_Mactrl_Data = 0b00000000;
	_colorspaceData = __COLORSPC;//start with default data;
	chipInit();
}

uint8_t TFT_ILI9163C::getErrorCode(void) 
{
	return _initError;
}

void TFT_ILI9163C::chipInit() {
	uint8_t i;
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		
		writecommand_cont(CMD_SWRESET);//software reset
		delay(500);
		
		writecommand_cont(CMD_SLPOUT);//exit sleep
		delay(5);
		
		writecommand_cont(CMD_PIXFMT);//Set Color Format 16bit   
		writedata8_cont(0x05);
		delay(5);
		
		writecommand_cont(CMD_GAMMASET);//default gamma curve 3
		writedata8_cont(0x08);//0x04
		delay(1);
		
		writecommand_cont(CMD_GAMRSEL);//Enable Gamma adj    
		writedata8_cont(0x01); 
		delay(1);
		
		writecommand_cont(CMD_NORML);
	
		writecommand_cont(CMD_DFUNCTR);
		writedata8_cont(0b11111111);//
		writedata8_cont(0b00000110);//

		writecommand_cont(CMD_PGAMMAC);//Positive Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata8_cont(pGammaSet[i]);
		}
		
		writecommand_cont(CMD_NGAMMAC);//Negative Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata8_cont(nGammaSet[i]);
		}
	
		writecommand_cont(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
		writedata8_cont(0x08);//0x0C//0x08
		writedata8_cont(0x02);//0x14//0x08
		delay(1);
	
		writecommand_cont(CMD_DINVCTR);//display inversion 
		writedata8_cont(0x07);
		delay(1);
	
		writecommand_cont(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
		writedata8_cont(0x0A);//4.30 - 0x0A
		writedata8_cont(0x02);//0x05
		delay(1);
	
		writecommand_cont(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
		writedata8_cont(0x02);
		delay(1);
	
		writecommand_cont(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
		writedata8_cont(0x50);//0x50
		writedata8_cont(99);//0x5b
		delay(1);
	
		writecommand_cont(CMD_VCOMOFFS);
		writedata8_cont(0);//0x40
		delay(1);
  
		writecommand_cont(CMD_CLMADRS);//Set Column Address  
		writedata16_cont(0x00);
		writedata16_cont(_GRAMWIDTH); 
  
		writecommand_cont(CMD_PGEADRS);//Set Page Address  
		writedata16_cont(0x00);
		writedata16_cont(_GRAMHEIGH); 
		// set scroll area (thanks Masuda)
		writecommand_cont(CMD_VSCLLDEF);
		writedata16_cont(__OFFSET);
		writedata16_cont(_GRAMHEIGH - __OFFSET);
		writedata16_last(0);
		
		endTransaction();
		
		colorSpace(_colorspaceData);
		
		setRotation(0);
		
		startTransaction();
		
		writecommand_cont(CMD_DISPON);//display ON 
		delay(1);
		writecommand_last(CMD_RAMWR);//Memory Write
		
		endTransaction();
		delay(1);
	#else
		writecommand(CMD_SWRESET);//software reset
		delay(500);
		
		writecommand(CMD_SLPOUT);//exit sleep
		delay(5);
		
		writecommand(CMD_PIXFMT);//Set Color Format 16bit   
		writedata(0x05);
		delay(5);
		
		writecommand(CMD_GAMMASET);//default gamma curve 3
		writedata(0x04);//0x04
		delay(1);
		
		writecommand(CMD_GAMRSEL);//Enable Gamma adj    
		writedata(0x01); 
		delay(1);
		
		writecommand(CMD_NORML);
	
		writecommand(CMD_DFUNCTR);
		writedata(0b11111111);//
		writedata(0b00000110);//

		writecommand(CMD_PGAMMAC);//Positive Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata(pGammaSet[i]);
		}
		
		writecommand(CMD_NGAMMAC);//Negative Gamma Correction Setting
		for (i=0;i<15;i++){
			writedata(nGammaSet[i]);
		}

		writecommand(CMD_FRMCTR1);//Frame Rate Control (In normal mode/Full colors)
		writedata(0x08);//0x0C//0x08
		writedata(0x02);//0x14//0x08
		delay(1);
		
		writecommand(CMD_DINVCTR);//display inversion 
		writedata(0x07);
		delay(1);
		
		writecommand(CMD_PWCTR1);//Set VRH1[4:0] & VC[2:0] for VCI1 & GVDD   
		writedata(0x0A);//4.30 - 0x0A
		writedata(0x02);//0x05
		delay(1);
		
		writecommand(CMD_PWCTR2);//Set BT[2:0] for AVDD & VCL & VGH & VGL   
		writedata(0x02);
		delay(1);
		
		writecommand(CMD_VCOMCTR1);//Set VMH[6:0] & VML[6:0] for VOMH & VCOML   
		writedata(0x50);//0x50
		writedata(99);//0x5b
		delay(1);
		
		writecommand(CMD_VCOMOFFS);
		writedata(0);//0x40
		delay(1);
  
		writecommand(CMD_CLMADRS);//Set Column Address  
		writedata16(0x00); 
		writedata16(_GRAMWIDTH); 
  
		writecommand(CMD_PGEADRS);//Set Page Address  
		writedata16(0X00); 
		writedata16(_GRAMHEIGH);
		// set scroll area (thanks Masuda)
		writecommand(CMD_VSCLLDEF);
		writedata16(__OFFSET);
		writedata16(_GRAMHEIGH - __OFFSET);
		writedata16(0);
		
		colorSpace(_colorspaceData);
		
		setRotation(0);
		writecommand(CMD_DISPON);//display ON 
		delay(1);
		writecommand(CMD_RAMWR);//Memory Write

		delay(1);
	#endif
	fillScreen(_defaultBackground);
}

/*
Colorspace selection:
0: RGB
1: GBR
*/
void TFT_ILI9163C::colorSpace(uint8_t cspace) {
	if (cspace < 1){
		bitClear(_Mactrl_Data,3);
	} else {
		bitSet(_Mactrl_Data,3);
	}
}

void TFT_ILI9163C::invertDisplay(boolean i) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writecommand_last(i ? CMD_DINVON : CMD_DINVOF);
		endTransaction();
	#else
		writecommand(i ? CMD_DINVON : CMD_DINVOF);
	#endif
}

void TFT_ILI9163C::display(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_DISPON);
			endTransaction();
		#else
			writecommand(CMD_DISPON);
		#endif
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_DISPOFF);
			endTransaction();
		#else
			writecommand(CMD_DISPOFF);
		#endif
	}
}

void TFT_ILI9163C::idleMode(boolean onOff) {
	if (onOff){
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_IDLEON);
			endTransaction();
		#else
			writecommand(CMD_IDLEON);
		#endif
	} else {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_IDLEOF);
			endTransaction();
		#else
			writecommand(CMD_IDLEOF);
		#endif
	}
}

void TFT_ILI9163C::sleepMode(boolean mode) {
	if (mode){
		if (sleep == 1) return;//already sleeping
		sleep = 1;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_SLPIN);
			endTransaction();
		#else
			writecommand(CMD_SLPIN);
		#endif
		delay(5);//needed
	} else {
		if (sleep == 0) return; //Already awake
		sleep = 0;
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_last(CMD_SLPOUT);
			endTransaction();
		#else
			writecommand(CMD_SLPOUT);
		#endif
		delay(120);//needed
	}
}

void TFT_ILI9163C::defineScrollArea(uint16_t tfa, uint16_t bfa){
    tfa += __OFFSET;
    int16_t vsa = _GRAMHEIGH - tfa - bfa;
    if (vsa >= 0) {
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			startTransaction();
			writecommand_cont(CMD_VSCLLDEF);
			writedata16_cont(tfa);
			writedata16_cont(vsa);
			writedata16_last(bfa);
			endTransaction();
		#else
			writecommand(CMD_VSCLLDEF);
			writedata16(tfa);
			writedata16(vsa);
			writedata16(bfa);
		#endif
    }
}

void TFT_ILI9163C::scroll(uint16_t adrs) {
	if (adrs <= _GRAMHEIGH) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writecommand_cont(CMD_VSSTADRS);
		writedata16_last(adrs + __OFFSET);
		endTransaction();
	#else
		writecommand(CMD_VSSTADRS);
		writedata16(adrs + __OFFSET);
	#endif
	}
}

//fast
void TFT_ILI9163C::fillScreen(uint16_t color) {
	int px;
	
	startTransaction();
	
	setAddrWindow_cont(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		for (px = 0;px < _GRAMSIZE; px++){
			writedata16_cont(color);
		}
		writecommand_last(CMD_NOP);
	#else
		enableDataStream();
		for (px = 0;px < _GRAMSIZE; px++){ spiwrite16(color); }
	#endif
	
	endTransaction();
}


void TFT_ILI9163C::homeAddress() 
{
	setAddrWindow(0x00,0x00,_GRAMWIDTH,_GRAMHEIGH);
}


void TFT_ILI9163C::setCursor(int16_t x, int16_t y) 
{
	if (boundaryCheck(x,y)) return;
	setAddrWindow(0x00,0x00,x,y);
	cursor_x = x;
	cursor_y = y;
}

void TFT_ILI9163C::getCursor(int16_t &x, int16_t &y) 
{
	x = cursor_x;
	y = cursor_y;
}

//fast
void TFT_ILI9163C::drawPixel(int16_t x, int16_t y, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	if ((x < 0) || (y < 0)) return;
	startTransaction();
	drawPixel_cont(x,y,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


void TFT_ILI9163C::setTextSize(uint8_t s) 
{
	textsize = (s > 0) ? s : 1;
}

void TFT_ILI9163C::setTextColor(uint16_t color) 
{
	textcolor = textbgcolor = color;
}

void TFT_ILI9163C::setTextColor(uint16_t frgrnd, uint16_t bckgnd) 
{
	textcolor = frgrnd;
	textbgcolor = bckgnd;
}

void TFT_ILI9163C::setBackground(uint16_t color) 
{
	_defaultBackground = color;
}


void TFT_ILI9163C::setForeground(uint16_t color) 
{
	_defaultForeground = color;
}

uint16_t TFT_ILI9163C::getBackground(void) 
{
	return _defaultBackground;
}

uint16_t TFT_ILI9163C::getForeground(void) 
{
	return _defaultForeground;
}

void TFT_ILI9163C::setTextWrap(boolean w) 
{
	wrap = w;
}

uint8_t TFT_ILI9163C::getRotation(void)  
{
	return rotation;
}

bool TFT_ILI9163C::boundaryCheck(int16_t x,int16_t y){
	if ((x >= _width) || (y >= _height)) return true;
	return false;
}

/*
draw fast vertical line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ILI9163C::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height - y;
	startTransaction();
	drawFastVLine_cont(x,y,h,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


/*
draw fast horizontal line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void TFT_ILI9163C::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) 
{
	// Rudimentary clipping
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width - x;
	startTransaction();
	drawFastHLine_cont(x,y,w,color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


void TFT_ILI9163C::clearScreen(void) 
{
	fillScreen(_defaultBackground);
}

/*
fill RECT
*/
void TFT_ILI9163C::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width  - x;
	if (((y + h) - 1) >= _height) h = _height - y;
	startTransaction();
	fillRect_cont(x,y,w,h,color);
	endTransaction();
}

void TFT_ILI9163C::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	setAddrWindow_cont(x,y,(x+w)-1,(y+h)-1);
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableDataStream();
	#endif
	for (y = h;y > 0;y--) {
		for (x = w;x > 1;x--) {
			#if defined(__MK20DX128__) || defined(__MK20DX256__)
				writedata16_cont(color);
			#else
				spiwrite16(color);
			#endif
		}
		
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writedata16_last(color);
		#else
			spiwrite16(color);
		#endif
		
	}
}


//80% fast
/*
draw LINE
*/
void TFT_ILI9163C::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	if (y0 == y1) {
		if (x1 > x0) {
			drawFastHLine(x0, y0, x1 - x0 + 1, color);
		} else if (x1 < x0) {
			drawFastHLine(x1, y0, x0 - x1 + 1, color);
		} else {
			drawPixel(x0, y0, color);
		}
		return;
	} else if (x0 == x1) {
		if (y1 > y0) {
			drawFastVLine(x0, y0, y1 - y0 + 1, color);
		} else {
			drawFastVLine(x0, y1, y0 - y1 + 1, color);
		}
		return;
	}

	bool steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	startTransaction();
	
	int16_t xbegin = x0;
	if (steep) {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastVLine_cont(y0, xbegin, len + 1, color);
				} else {
					drawPixel_cont(y0, x0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) drawFastVLine_cont(y0, xbegin, x0 - xbegin, color);
	} else {
		for (; x0<=x1; x0++) {
			err -= dy;
			if (err < 0) {
				int16_t len = x0 - xbegin;
				if (len) {
					drawFastHLine_cont(xbegin, y0, len + 1, color);
				} else {
					drawPixel_cont(x0, y0, color);
				}
				xbegin = x0 + 1;
				y0 += ystep;
				err += dx;
			}
		}
		if (x0 > xbegin + 1) drawFastHLine_cont(xbegin, y0, x0 - xbegin, color);
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}

//fast
/*
draw rect
*/
void TFT_ILI9163C::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
	startTransaction();
		drawFastHLine_cont(x, y, w, color);
		drawFastHLine_cont(x, (y+h)-1, w, color);
		drawFastVLine_cont(x, y, h, color);
		drawFastVLine_cont((x+w)-1, y, h, color);	
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}



void TFT_ILI9163C::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);//bogus command to set HI the CS
	#endif
	endTransaction();
}


void TFT_ILI9163C::setRotation(uint8_t m) {
	rotation = m % 4; // can't be higher than 3
	_Mactrl_Data &= ~(0xF0);//clear bit 4...7
	if (rotation == 0){
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		_portrait = false;
	} else if (rotation == 1){
		bitSet(_Mactrl_Data,6);
		bitSet(_Mactrl_Data,5);
		_width  = _TFTHEIGHT;//-__OFFSET;
		_height = _TFTWIDTH;
		_portrait = true;
	} else if (rotation == 2){
		bitSet(_Mactrl_Data,7);
		bitSet(_Mactrl_Data,6);
		_width  = _TFTWIDTH;
		_height = _TFTHEIGHT;//-__OFFSET;
		_portrait = false;
	} else {
		bitSet(_Mactrl_Data,7);
		bitSet(_Mactrl_Data,5);
		_width  = _TFTHEIGHT;
		_height = _TFTWIDTH;//-__OFFSET;
		_portrait = true;
	}
	colorSpace(_colorspaceData);
	
	startTransaction();
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_cont(CMD_MADCTL);
		writedata8_last(_Mactrl_Data);
	#else
		enableCommandStream();
		spiwrite(CMD_MADCTL);
		enableDataStream();
		spiwrite(_Mactrl_Data);
	#endif
	endTransaction();
}


int16_t TFT_ILI9163C::width(void) const {
	return _width;
}
 
int16_t TFT_ILI9163C::height(void) const {
	return _height;
}

/* --------------------------------------------------------------------------------------------------
										GRAPHIC SUBS
-----------------------------------------------------------------------------------------------------*/

void TFT_ILI9163C::setFont(uint8_t f) 
{
	font = f;
	switch(font) {
		case GFXFONT_GLCD:
			fontData = glcdFont;
			fontKern = 1;
		break;
		case GFXFONT_GLCD_ASCII:
			fontData = glcdFont_ascii;
			fontKern = 1;
		break;
		default:
			font = GFXFONT_GLCD;
			fontData = glcdFont;
			fontKern = 1;
		break;
	}
	fontWidth = pgm_read_byte(fontData+FONT_WIDTH);
	fontHeight = pgm_read_byte(fontData+FONT_HEIGHT);
	fontStart = pgm_read_byte(fontData+FONT_START);
	fontLength = pgm_read_byte(fontData+FONT_LENGTH);
}



//fast
void TFT_ILI9163C::drawArcHelper(uint16_t cx, uint16_t cy, uint16_t radius, uint16_t thickness, float start, float end, uint16_t color) 
{
	int16_t xmin = 65535, xmax = -32767, ymin = 32767, ymax = -32767;
	float cosStart, sinStart, cosEnd, sinEnd;
	float r, t;
	float startAngle, endAngle;

	startAngle = (start / _arcAngleMax) * 360;	// 252
	endAngle = (end / _arcAngleMax) * 360;		// 807

	while (startAngle < 0) startAngle += 360;
	while (endAngle < 0) endAngle += 360;
	while (startAngle > 360) startAngle -= 360;
	while (endAngle > 360) endAngle -= 360;


	if (startAngle > endAngle) {
		drawArcHelper(cx, cy, radius, thickness, ((startAngle) / 360.0) * _arcAngleMax, _arcAngleMax, color);
		drawArcHelper(cx, cy, radius, thickness, 0, ((endAngle) / 360.0) * _arcAngleMax, color);
	} else {
		// Calculate bounding box for the arc to be drawn
		cosStart = cosDegrees(startAngle);
		sinStart = sinDegrees(startAngle);
		cosEnd = cosDegrees(endAngle);
		sinEnd = sinDegrees(endAngle);

		r = radius;
		// Point 1: radius & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 2: radius & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		r = radius - thickness;
		// Point 3: radius-thickness & startAngle
		t = r * cosStart;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinStart;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;

		// Point 4: radius-thickness & endAngle
		t = r * cosEnd;
		if (t < xmin) xmin = t;
		if (t > xmax) xmax = t;
		t = r * sinEnd;
		if (t < ymin) ymin = t;
		if (t > ymax) ymax = t;
		// Corrections if arc crosses X or Y axis
		if ((startAngle < 90) && (endAngle > 90)) ymax = radius;
		if ((startAngle < 180) && (endAngle > 180)) xmin = -radius;
		if ((startAngle < 270) && (endAngle > 270)) ymin = -radius;

		// Slopes for the two sides of the arc
		float sslope = (float)cosStart / (float)sinStart;
		float eslope = (float)cosEnd / (float)sinEnd;
		if (endAngle == 360) eslope = -1000000;
		int ir2 = (radius - thickness) * (radius - thickness);
		int or2 = radius * radius;
		int x,y;
		startTransaction();
		for (x = xmin; x <= xmax; x++) {
			bool y1StartFound = false, y2StartFound = false;
			bool y1EndFound = false, y2EndSearching = false;
			int y1s = 0, y1e = 0, y2s = 0;//, y2e = 0;
			for (y = ymin; y <= ymax; y++) {
				int x2 = x * x;
				int y2 = y * y;

				if (
					(x2 + y2 < or2 && x2 + y2 >= ir2) && (
					(y > 0 && startAngle < 180 && x <= y * sslope) ||
					(y < 0 && startAngle > 180 && x >= y * sslope) ||
					(y < 0 && startAngle <= 180) ||
					(y == 0 && startAngle <= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)
					) && (
					(y > 0 && endAngle < 180 && x >= y * eslope) ||
					(y < 0 && endAngle > 180 && x <= y * eslope) ||
					(y > 0 && endAngle >= 180) ||
					(y == 0 && endAngle >= 180 && x < 0) ||
					(y == 0 && startAngle == 0 && x > 0)))
				{
					if (!y1StartFound) {	//start of the higher line found
						y1StartFound = true;
						y1s = y;
					} else if (y1EndFound && !y2StartFound) {//start of the lower line found
						y2StartFound = true;
						y2s = y;
						y += y1e - y1s - 1;	// calculate the most probable end of the lower line (in most cases the length of lower line is equal to length of upper line), in the next loop we will validate if the end of line is really there
						if (y > ymax - 1) {// the most probable end of line 2 is beyond ymax so line 2 must be shorter, thus continue with pixel by pixel search
							y = y2s;	// reset y and continue with pixel by pixel search
							y2EndSearching = true;
						}
					} else if (y2StartFound && !y2EndSearching) {
						// we validated that the probable end of the lower line has a pixel, continue with pixel by pixel search, in most cases next loop with confirm the end of lower line as it will not find a valid pixel
						y2EndSearching = true;
					}
				} else {
					if (y1StartFound && !y1EndFound) {//higher line end found
						y1EndFound = true;
						y1e = y - 1;
						drawFastVLine_cont(cx + x, cy + y1s, y - y1s, color);
						if (y < 0) {
							y = abs(y); // skip the empty middle
						}
						else
							break;
					} else if (y2StartFound) {
						if (y2EndSearching) {
							// we found the end of the lower line after pixel by pixel search
							drawFastVLine_cont(cx + x, cy + y2s, y - y2s, color);
							y2EndSearching = false;
							break;
						} else {
							// the expected end of the lower line is not there so the lower line must be shorter
							y = y2s;	// put the y back to the lower line start and go pixel by pixel to find the end
							y2EndSearching = true;
						}
					}
				}
			}
			if (y1StartFound && !y1EndFound){
				y1e = ymax;
				drawFastVLine_cont(cx + x, cy + y1s, y1e - y1s + 1, color);
			} else if (y2StartFound && y2EndSearching)	{// we found start of lower line but we are still searching for the end
														// which we haven't found in the loop so the last pixel in a column must be the end
				drawFastVLine_cont(cx + x, cy + y2s, ymax - y2s + 1, color);
			}
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writecommand_last(CMD_NOP);
		#endif
		endTransaction();
	}
}

float TFT_ILI9163C::cosDegrees(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return cos(radians);
}

float TFT_ILI9163C::sinDegrees(float angle)
{
	float radians = angle / (float)360 * 2 * PI;
	return sin(radians);
}

void TFT_ILI9163C::setArcParams(float arcAngleMax, int arcAngleOffset)
{
	_arcAngleMax = arcAngleMax;
	_arcAngleOffset = arcAngleOffset;
}

/*
void TFT_ILI9163C::drawPie(int16_t x, int16_t y, int16_t r, int16_t rs, int16_t re,uint16_t color)
{
	int16_t px, py, cx, cy, d;
	rs -= 90;
	re   -= 90;
	if (rs>re) rs -= 360;
	px = x + cos((rs*3.14)/180) * r;
	py = y + sin((rs*3.14)/180) * r;
	drawLine(x, y, px, py,color);
	for (d=rs+1; d<re+1; d++){
		cx = x + cos((d*3.14)/180) * r;
		cy = y + sin((d*3.14)/180) * r;
		drawLine(px, py, cx, cy,color);
		px = cx;
		py = cy;
	}
	drawLine(x, y, px, py,color);
}
*/

//fast
void TFT_ILI9163C::drawEllipse(int16_t cx,int16_t cy,int16_t radiusW,int16_t radiusH,uint16_t color)
{
	int16_t x,y;
	int16_t twoASquare,twoBSquare;
	int32_t stoppingX,stoppingY;
    int32_t xchange,ychange,ellipseError;
    twoASquare = 2 * (radiusW * radiusW);
    twoBSquare = 2 * (radiusH * radiusH);
    x = radiusW;
    y = 0;
    xchange = (radiusH * radiusH) * (1 - (2 * radiusW));
    ychange = (radiusW * radiusW);
    ellipseError = 0;
    stoppingX = (twoBSquare * radiusW);
    stoppingY = 0;
	startTransaction();
    while (stoppingX >= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		y++;
		stoppingY += twoASquare;
		ellipseError += ychange;
		ychange += twoASquare;
		if ((2 * ellipseError) + xchange > 0) {
			x--;
			stoppingX -= twoBSquare;
			ellipseError += xchange;
			xchange += twoBSquare;
		}
    }
    x = 0;
    y = radiusH;
    xchange = (radiusH * radiusH);
    ychange = (radiusW * radiusW) * (1 - (2 * radiusH));
    ellipseError = 0;
    stoppingX = 0;
    stoppingY = (twoASquare * radiusH);
    while (stoppingX <= stoppingY) {
		plot4points_cont(cx,cy,x,y,color);
		x++;
		stoppingX += twoBSquare;
		ellipseError += xchange;
		xchange += twoBSquare;
		if (((2 * ellipseError) + ychange) > 0) {
			y--;
			stoppingY -= twoASquare;
			ellipseError += ychange;
			ychange += twoASquare;
		}
    }
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


/*
void TFT_ILI9163C::drawBezier(int x0, int y0, int x1, int y1, int x2, int y2,uint16_t color)
{
	int sx = x0 < x2 ? 1 : -1, sy = y0 < y2 ? 1 : -1; // step direction
	int cur = sx * sy * ((x0 - x1) * (y2 - y1) - (x2 - x1) * (y0 - y1)); // curvature
	int x = x0 - 2 * x1 + x2, y = y0 - 2 * y1 + y2, xy = 2 * x * y * sx * sy;
                                // compute error increments of P0
	long dx = (1 - 2 * abs(x0 - x1)) * y * y + abs(y0 - y1) * xy - 2 * cur * abs(y0 - y2);
	long dy = (1 - 2 * abs(y0 - y1)) * x * x + abs(x0 - x1) * xy + 2 * cur * abs(x0 - x2);
                                // compute error increments of P2 
	long ex = (1 - 2 * abs(x2 - x1)) * y * y + abs(y2 - y1) * xy + 2 * cur * abs(y0 - y2);
	long ey = (1 - 2 * abs(y2 - y1)) * x * x + abs(x2 - x1) * xy - 2 * cur * abs(x0 - x2);

	if (cur == 0) { drawLine(x0, y0, x2, y2, color); return; } // straight line 

	x *= 2 * x; y *= 2 * y;
	if (cur < 0) { // negated curvature 
		x = -x; dx = -dx; ex = -ex; xy = -xy;
		y = -y; dy = -dy; ey = -ey;
	}
  // algorithm fails for almost straight line, check error values 
	if (dx >= -y || dy <= -x || ex <= -y || ey >= -x) {
		drawLine(x0, y0, x1, y1, color); // simple approximation 
		drawLine(x1, y1, x2, y2, color);
		return;
	}
	dx -= xy; ex = dx+dy; dy -= xy; // error of 1.step 

	for(;;) { // plot curve
		drawPixel(y0, x0, color);
		ey = 2 * ex - dy; // save value for test of y step 
		if (2 * ex >= dx) { // x step
			if (x0 == x2) break;
			x0 += sx; dy -= xy; ex += dx += y;
		}
		if (ey <= 0) { // y step 
			if (y0 == y2) break;
			y0 += sy; dx -= xy; ex += dy += x;
		}
	}
}
*/


//fast
void TFT_ILI9163C::drawCircle(int16_t cx, int16_t cy, int16_t radius, uint16_t color)
{
	int error = -radius;
	int16_t x = radius;
	int16_t y = 0;
	startTransaction();
	while (x >= y){
		plot4points_cont(cx, cy, x, y, color);
		if (x != y) plot4points_cont(cx, cy, y, x, color);
		error += y;
		++y;
		error += y;
		if (error >= 0){
			--x;
			error -= x;
			error -= x;
		}
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//fast
void TFT_ILI9163C::drawRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t color) 
{
	startTransaction();
	drawFastHLine_cont(x+r  , y    , w-2*r, color); // Top
	drawFastHLine_cont(x+r  , y+h-1, w-2*r, color); // Bottom
	drawFastVLine_cont(x    , y+r  , h-2*r, color); // Left
	drawFastVLine_cont(x+w-1, y+r  , h-2*r, color); // Right
  // draw four corners
	drawCircle_cont(x+r    , y+r    , r, 1, color);
	drawCircle_cont(x+w-r-1, y+r    , r, 2, color);
	drawCircle_cont(x+w-r-1, y+h-r-1, r, 4, color);
	drawCircle_cont(x+r    , y+h-r-1, r, 8, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}


//fast
void TFT_ILI9163C::fillRoundRect(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t color) 
{
	startTransaction();
	fillRect_cont(x+r, y, w-2*r, h, color);
	fillCircle_cont(x+w-r-1, y+r, r, 1, h-2*r-1, color);
	fillCircle_cont(x+r    , y+r, r, 2, h-2*r-1, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ILI9163C::fillCircle(int16_t x0, int16_t y0, int16_t r,uint16_t color) 
{
	startTransaction();//open SPI comm
	drawFastVLine_cont(x0, y0-r, 2*r+1, color);
	fillCircle_cont(x0, y0, r, 3, 0, color);
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}


void TFT_ILI9163C::drawQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2,int16_t x3, int16_t y3, uint16_t color) 
{
	drawLine(x0, y0, x1, y1, color);//low 1
	drawLine(x1, y1, x2, y2, color);//high 1
	drawLine(x2, y2, x3, y3, color);//high 2
	drawLine(x3, y3, x0, y0, color);//low 2
}


void TFT_ILI9163C::fillQuad(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, int16_t x3, int16_t y3, uint16_t color) 
{
    fillTriangle(x0,y0,x1,y1,x2,y2,color);
    fillTriangle(x0,y0,x2,y2,x3,y3,color);
}

void TFT_ILI9163C::drawPolygon(int16_t cx, int16_t cy, uint8_t sides, int16_t diameter, float rot, uint16_t color)
{ 
	sides = (sides > 2? sides : 3);
	float dtr = (PI/180.0) + PI;
	float rads = 360.0 / sides;//points spacd equally
	uint8_t i;
	for (i = 0; i < sides; i++) { 
		drawLine(
			cx + (sin((i*rads + rot) * dtr) * diameter),
			cy + (cos((i*rads + rot) * dtr) * diameter),
			cx + (sin(((i+1)*rads + rot) * dtr) * diameter),
			cy + (cos(((i+1)*rads + rot) * dtr) * diameter),
			color);
	}
}

void TFT_ILI9163C::drawTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	drawLine(x0, y0, x1, y1, color);
	drawLine(x1, y1, x2, y2, color);
	drawLine(x2, y2, x0, y0, color);
}

//85% fast
void TFT_ILI9163C::fillTriangle(int16_t x0, int16_t y0,int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color) 
{
	int16_t a, b, y, last;

	if (y0 > y1) {
		swap(y0, y1); 
		swap(x0, x1);
	}
	if (y1 > y2) {
		swap(y2, y1); 
		swap(x2, x1);
	}
	if (y0 > y1) {
		swap(y0, y1); 
		swap(x0, x1);
	}

	if (y0 == y2) {
		a = b = x0;
		if (x1 < a){
			a = x1;
		} else if (x1 > b) {
			b = x1;
		}
		if (x2 < a){
			a = x2;
		} else if (x2 > b) {
			b = x2;
		}
		drawFastHLine(a, y0, b-a+1, color);
		return;
  }

	int16_t
		dx01 = x1 - x0,
		dy01 = y1 - y0,
		dx02 = x2 - x0,
		dy02 = y2 - y0,
		dx12 = x2 - x1,
		dy12 = y2 - y1;
	int32_t
		sa   = 0,
		sb   = 0;

	if (y1 == y2) {
		last = y1;
	} else { 
		last = y1-1;
	}
	startTransaction();//open SPI comm
	for (y=y0; y<=last; y++) {
		a   = x0 + sa / dy01;
		b   = x0 + sb / dy02;
		sa += dx01;
		sb += dx02;
		if (a > b) swap(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}

	sa = dx12 * (y - y1);
	sb = dx02 * (y - y0);
	for(; y<=y2; y++) {
		a   = x1 + sa / dy12;
		b   = x0 + sb / dy02;
		sa += dx12;
		sb += dx02;
		if (a > b) swap(a,b);
		drawFastHLine_cont(a, y, b-a+1, color);
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)	
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();//close SPI comm
}

//fast
void TFT_ILI9163C::plot4points_cont(uint16_t cx, uint16_t cy, uint16_t x, uint16_t y, uint16_t color)
{
	drawPixel_cont(cx + x, cy + y, color);
	if (x != 0) drawPixel_cont(cx - x, cy + y, color);
	if (y != 0) drawPixel_cont(cx + x, cy - y, color);
	if (x != 0 && y != 0) drawPixel_cont(cx - x, cy - y, color);
}

//fast
void TFT_ILI9163C::drawCircle_cont(int16_t x0,int16_t y0,int16_t r,uint8_t cornername,uint16_t color) 
{
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;
	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;
		if (cornername & 0x4) {
			drawPixel_cont(x0 + x, y0 + y, color);
			drawPixel_cont(x0 + y, y0 + x, color);
		} 
		if (cornername & 0x2) {
			drawPixel_cont(x0 + x, y0 - y, color);
			drawPixel_cont(x0 + y, y0 - x, color);
		}
		if (cornername & 0x8) {
			drawPixel_cont(x0 - y, y0 + x, color);
			drawPixel_cont(x0 - x, y0 + y, color);
		}
		if (cornername & 0x1) {
			drawPixel_cont(x0 - y, y0 - x, color);
			drawPixel_cont(x0 - x, y0 - y, color);
		}
	}
}

//fast
void TFT_ILI9163C::fillCircle_cont(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color) 
{
	int16_t f     = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x     = 0;
	int16_t y     = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f     += ddF_y;
		}
		x++;
		ddF_x += 2;
		f     += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine_cont(x0+x, y0-y, 2*y+1+delta, color);
			drawFastVLine_cont(x0+y, y0-x, 2*x+1+delta, color);
		}
		if (cornername & 0x2) {
			drawFastVLine_cont(x0-x, y0-y, 2*y+1+delta, color);
			drawFastVLine_cont(x0-y, y0-x, 2*x+1+delta, color);
		}
	}
}

//fast
size_t TFT_ILI9163C::write(uint8_t c) 
{
	if (c == '\n') {
		cursor_y += textsize*8;
		cursor_x  = 0;
	} else if (c == '\r') {
		// skip em
	} else {
		startTransaction();
		drawChar_cont(cursor_x,cursor_y,c,textcolor,textbgcolor,textsize);
		if (fontKern > 0 && textcolor != textbgcolor) {
			fillRect_cont(cursor_x+fontWidth*textsize,cursor_y,fontKern*textsize,fontHeight*textsize,textbgcolor);
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)	
			writecommand_last(CMD_NOP);
		#endif
		endTransaction();
		cursor_x += textsize*(fontWidth+fontKern);
		if (wrap && (cursor_x > (_width - textsize*fontWidth))) {
			cursor_y += textsize*fontHeight;
			cursor_x = 0;
		}
	}
  return 1;
}


//fast
void TFT_ILI9163C::drawChar_cont(int16_t x, int16_t y, unsigned char c,uint16_t color, uint16_t bg, uint8_t size) {
	if((x >= _width)             || // Clip right
		(y >= _height)           || // Clip bottom
		((x + 6 * size - 1) < 0) || // Clip left
		((y + 8 * size - 1) < 0))   // Clip top
    return;
	if (c < fontStart || c > fontStart+fontLength) {
		c = 0;
	} else {
		c -= fontStart;
	}
	uint16_t bitCount = 0;
	uint16_t line = 0;
	uint16_t i,j;
	int fontIndex = (c*(fontWidth*fontHeight)/8)+4;
	for (i=0; i<fontHeight; i++ ) {
		//uint8_t line;
		for (j = 0; j<fontWidth; j++) {
			if (bitCount++%8 == 0) line = pgm_read_byte(fontData+fontIndex++);
			if (line & 0x80) {
				if (size > 1) {//big
					fillRect_cont(x+(j*size), y+(i*size), size, size, color);
				} else {  // default size
					drawPixel_cont(x+j, y+i, color);
				} 
			} else if (bg != color) {
				if (size > 1) {// big
					fillRect_cont(x+(j*size), y+(i*size), size, size, bg);
				} else {  // def size
					drawPixel_cont(x+j, y+i, bg);
				}
			}		
			line <<= 1;
		}
	}
}

//fast
void TFT_ILI9163C::startPushData(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1);
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableDataStream();
	#endif
}


//fast
void TFT_ILI9163C::pushData(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writedata16_cont(color);
	#else
		spiwrite16(color);
	#endif
}

//fast
void TFT_ILI9163C::endPushData() {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);
	#endif
	endTransaction();
}

//fast
void TFT_ILI9163C::pushColor(uint16_t color) {
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		startTransaction();
		writedata16_last(color);
		endTransaction();
	#else
		writedata16(color);
	#endif
}


//fast
void TFT_ILI9163C::drawColorBitmap(int16_t x, int16_t y, int16_t w, int16_t h, const uint32_t *bitmap,bool true24) 
{
	int16_t px;
	uint16_t color;
	
	startTransaction();
	setAddrWindow_cont(x,y,w,h);//constrain window
	#if !defined(__MK20DX128__) && !defined(__MK20DX256__)
		enableCommandStream();
		spiwrite(CMD_RAMWR);
		enableDataStream();
	#else
		writecommand_cont(CMD_RAMWR);//ram write
	#endif
	for (px = 0;px < w*h; px++){//loop trough pixels
		if (true24){
			color = Color24To565(bitmap[px]);//24 bit
		} else {
			color = bitmap[px];//18 bit
		}
		#if defined(__MK20DX128__) || defined(__MK20DX256__)
			writedata16_cont(color);
		#else
			spiwrite16(color);
		#endif
	}
	#if defined(__MK20DX128__) || defined(__MK20DX256__)
		writecommand_last(CMD_NOP);//just for set CS hi
	#endif
	endTransaction();
}


void TFT_ILI9163C::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color) 
{
	int16_t i, j, byteWidth = (w + 7) / 8;
	for (j=0; j<h; j++) {
		for (i=0; i<w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) drawPixel(x+i, y+j, color);
		}
	}
}

void TFT_ILI9163C::drawBitmap(int16_t x, int16_t y,const uint8_t *bitmap, int16_t w, int16_t h,uint16_t color, uint16_t bg) 
{
	int16_t i, j, byteWidth = (w + 7) / 8;
	for (j=0; j<h; j++) {
		for (i=0; i<w; i++ ) {
			if (pgm_read_byte(bitmap + j * byteWidth + i / 8) & (128 >> (i & 7))) {
				drawPixel(x+i, y+j, color);
			} else {
				drawPixel(x+i, y+j, bg);
		}
    }
  }
}

