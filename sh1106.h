
#ifndef INC_SH1106_H_ // Changed to INC_SEVSEG_H_ to avoid potential conflicts with SEVSEG_H_
#define INC_SH1106_H_

#include <stdint.h> // For uint8_t, uint16_

#define SH1106_I2C_ADDR (0x3C << 1)

#define SCREENWIDTH 132
#define SCREENHEIGHT 64
#define PAGENUMS 8


#define COLUMNADRESSLOW(adr) (0x00 | adr)
#define COLUMNADRESSHIGH(adr) (0x10 | adr)
#define PUMPVOLTAGE(val) (0x30 |val)
#define CONTRASTMODE 0x81
#define CONTRASTDATA(val) (0x00 |val)
#define STARTLINE(val) (0x40 |val)
#define SEGMENTREMAP(val) (0xA0 |val)
#define DISPLAYONOFFE(val) (0xA4 |val)
#define DISPLAYREVERS(val) (0xA6 |val)
#define MULTOPLEXRATIOMODE 0xA8
#define MULTOPLEXRATIOSET(val) (0x00 |val)
#define DCDCONOFFMODE 0xA8
#define DCDCONOFFSET(val) (0x8A |val)
#define DISPLAYONOFF(val) (0xAE |val)
#define PAGEADDRESS(val) (0xB0 |val)
#define DISPLAYSLEEP(val) (0xAE |val)
#define COMMONOUTSCAN(val) (0xC0 | val<<3)
#define DISPLAYOFSETMODE 0xD3
#define DISPLAYOFSET(val) (0x00 |val)
#define OSCIALLATORFREQMODE 0xD5
#define OSCIALLATORFREQSET(val) (0x00 |val)
#define PREDISCHARGMODE 0xD9
#define PREDISCHARGSET(val) (0x00 |val)
#define PADCONFIGMODE 0xDA
#define PADCONFIGSET(val) (0x20 |val<<4)
#define VCOMMODE 0xDB
#define VCOMSET(val) (0x00 |val)
#define READMODIFYWRITE 0xE0
#define ENDING 0xEE


typedef struct
{

	void (*clearScreen)(void);
	void (*sendCommand)(uint8_t command);
	void (*sendData)(uint8_t data);
	void (*draw)(uint8_t *buffer, uint8_t image_width, uint8_t image_height, uint8_t x, uint8_t y);
	void (*sleep)(void);
	void (*wakeUp)(void);


} sh1106_TypeDef;



sh1106_TypeDef* sh1106_init(void);

void sh1106_clearScreen(void);

void sh1106_sendCommand(uint8_t command);

void sh1106_sendData(uint8_t data);

//void sh1106_updateScreen();

void sh1106_Sleep(void);

void sh1106_WakeUp(void);

void sh1106_drawImage(uint8_t *buffer, uint8_t image_width, uint8_t image_height, uint8_t x, uint8_t y);


#endif
