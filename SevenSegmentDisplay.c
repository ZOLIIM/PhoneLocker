#include "SevenSegmentDisplay.h"
/*
void sevnSeg_Init(){
	oneNumTypeDef sevSeg= malloc(sizeof(oneNumTypeDef));
}
void sevnSet_DeInit(){
	
*/

void readNumber(uint8_t *buf, uint8_t num, uint8_t x, uint8_t y, uint8_t hight, uint8_t width){
	
	
	oneNumTypeDef sevSeg= malloc(sizeof(oneNumTypeDef));
	
	sevSeg.segment=recSegNum(num);
	sevSeg.hight=hight;
	sevSeg.width=width;
	uint16_t thickness=((hight*width)/100)+1
	sevSeg.thick= (uint8_t) thickness;
	sevSeg.type=1;
	setSegment(&sevSeg);
	buf=&sevSeg.buffer;
	free(&sevSeg);
	
	
}	

void readSepChar(char ch, uint8_t x, uint8_t y, uint8_t hight, uint8_t width){
	
}
//read the given number to fill up the buffer for the display so the sevensegment can visualise the correct number
uint8_t recSegNum(uint8_t num){


	switch (num)
	
	case 0:
	return SEGMENTNUM_0;
	case 1:
	return SEGMENTNUM_1;
	case 2:
	return SEGMENTNUM_2;
	case 3:
	return SEGMENTNUM_3;
	case 4:
	return SEGMENTNUM_4;
	case 5:
	return SEGMENTNUM_5;
	case 6:
	return SEGMENTNUM_6;
	case 7:
	return SEGMENTNUM_7;
	case 8:
	return SEGMENTNUM_8;
	case 9:
	return SEGMENTNUM_9;
	
	default:
	return 0b00000000;

}
//read the seperator character
uint8_t recSegCh(char ch)
{
	if( ch == ':')return 1;
	
	else if (ch==' ') return 0;
	
	else return 255;
}

/*
filles up the buffer of the oneNumTypeDef with the neccesary segments based on the segment code previously given 
Read in the given active segment bitfield and set the location of each horisontal and vertical segment

*/	

void setSegment(oneNumTypeDef *self){//process the segment code and call horisonatal and vertical set functions
	
	uint8_t x=0;
	uint8_t y=0;
	uint8_t newBits=0;
	
	for(uint8_t i=0; i<8 ;i++){
		
		newBits=self.segment>> i;
		
		//Verticals coordinates
		if(i<4&&newBits%2==1){
			
			if(i==3){
			x=self.width-self.thick;//right
			y=(self.hight+self.thick)/2;//bot
			}
			if(i==2){
			x=self.width-self.thick;//right
			y=self.thick;//top
			}
		
			if(i==1){
			x=0;//left
			y=self.thick;//top
			}
		
			if(i==0){
			x=0;//left
			y=(self.hight+self.thick)/2;//bot
			}
			setVertical(&self,x,y);
			}
		//Horisontal coordinates
		if(i>=4&&newBits%2==1){
			x=self.thick;
			
			if(i==6){
			y=0;//Top
			}
			if(i==5){
			y=(self.hight-self.thick/2);//mid
			}
		
			if(i==4){
			y=self.hight-self.thick;//bot
			}
	
			setHorisontal(&self,x,y);
	
		}
	}


}
/*
This fucntion set up all the vertical to the given location
onNumTypeDef is the structure containing the all the data of the SevenSegment display
x= column location in px (bitfield)
y= row location in px(bitfield)

First it for loop, go throug all the column
Secound for loop go through all the pages (build based on the SH1106 Oled)
a page 8bit*picture width px field 

with this setup filles up the buffer of the oneNumTypeDef, with placed vertical oriented segment
*/
void setVertical(oneNumTypeDef *self, uint8_t x, uint8_t y){//build the vertical lines accoring to the place wher it is supossed be
	
	uint8_t page_start = y / 8;
    uint8_t total_pages = (self.height + 7) / 8;
	uint8_t pageOffset=0;
	uint8_t segmentHight=0;
	
    for (uint8_t col = x; col < self.thick+x; col++) {
		segmentHight=((self.hight-(3*self.thick))/2);
		pageOffset= y%8;
        for (uint8_t page = page_start; page <= total_pages; page++) {
		
			self.buffer[col+(page*self.width)] |=fillByte(&segmentHight,&pageOffset);
		}
	}
}
/*
This fucntion set up all the horisontal to the given location
onNumTypeDef is the structure containing the all the data of the SevenSegment display
x= column location in px (bitfield)
y= row location in px(bitfield)

First it for loop, go throug all the column
Secound for loop go through all the pages (build based on the SH1106 Oled)
a page 8bit*picture width px field 

with this setup filles up the buffer of the oneNumTypeDef, with placed horisontal oriented segment

*/
void setHorisontal(oneNumTypeDef *self, uint8_t x, uint8_t y){//build the horisontla lines accoring to the place wher it is supossed be
	
	uint8_t page_start = y / 8;
    uint8_t total_pages = (self.height + 7) / 8;
	uint8_t pageOffset=0;
	uint8_t segmentWidth=self.widht-(2*self.thick);
	uint8_t segmentHight=self.thick;
	
    for (uint8_t col = x; col < segmentWidth+x; col++) {
		pageOffset= y%8;
		segmentHight=self.thick;
        for (uint8_t page = page_start; page <= total_pages; page++) {
			
			self.buffer[col+(page*self.width)] |=fillByte(&segmentHight,&pageOffset);
			
		}
	}
	
/*
This function fill up the 8bit page collum of the screen with the given parameters
px: the actual px (1bit is one pixel)
pageOffset: if the px is not at the top of the page but somewhere else
segmentHight: The segment hight which needs to be set 
the page bits start from the "top"

they need to be referneced by place (pointers) 
if the segment is continues to the next page the segmentHight would not be updated and started from the beging again 

*/
uint8_t fillByte(uint8_t *segmentHight,uint8_t *pageOffset){
	
	uint8_t buf=0;
	for(uint8_t px=0;px <8; px++){
				
				if(*pageOffset>0){// fills the byte with 0 till the placed bits needed (Padding)
					buf |= 0<<px;
					*pageOffset=*pageOffset-1;
				}
				else if(*segmentHight>0){ //fills the byte with 1s till reach the max width
					buf |= 1<<px;
						*segmentHight=*segmentHight-1;
				}
				else buf |= 0<<px;// fill the rest of the byte with 0
	
			}
		return buf;
}
