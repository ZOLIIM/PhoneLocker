#define SEGMENTNUM_0 0b01011111
#define SEGMENTNUM_1 0b00001100
#define SEGMENTNUM_2 0b01110101
#define SEGMENTNUM_3 0b01111100
#define SEGMENTNUM_4 0b00101110
#define SEGMENTNUM_5 0b01111010
#define SEGMENTNUM_6 0b01111101
#define SEGMENTNUM_7 0b01001100
#define SEGMENTNUM_8 0b01111111
#define SEGMENTNUM_9 0b01111110


typedef struct {
	uint8_t hight;
	uint8_t width;
	uint8_t thick;
	uint8_t type; //if space =0 if num =1
	uint8_t buffer [];
	uint8_t segment
	
}oneNumTypeDef;


void sevseg_init(void);

void sevseg_deinit(void);

void readNumber(uint8_t num, uint8_t x, uint8_t y, uint8_t hight, uint8_t width);

void readSepChar(char ch, uint8_t x, uint8_t y, uint8_t hight, uint8_t width);

uint8_t recNum(uint8_t num); 
/*Return segment structure
- if 1 means active
- MSB dont care, 
Binar system struct:
  0b0 + Top + Mid + Bot + RightBot + RightTop + LeftTop+ LeftBot
Example: the recived number is 7
the segment value =0b01001100
Example: the recived num is 0
the segment value =0b0101111
*/



uint8_t recSegCh(char ch);

void setVertical()
void setHorisontal(uint8_t horisontal, uint8_t buffer[],uint8_t hight, uint8_t width) 
