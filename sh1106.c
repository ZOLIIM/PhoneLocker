#define ScreenWidth 132
#define ScreenHeight 64
#define PageNum 8

uint8_t buffer[ ScreenHeight*ScreenWidth/PageNum];
uint8_t currentPage;


typedef struct{

}SH1106_SCREEN;



void sh1106_init(void);

void sh1106_clearScreen(void);

void sh1106_sendCommand();

void sh1106_sendData();

void sh1106_updateScreen();

void sh1106_Sleep();

void sh1106_drawPixel();

