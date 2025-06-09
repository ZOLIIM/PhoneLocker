
#include "main.h"
#include "sh1106.h"

extern I2C_HandleTypeDef hi2c2;

sh1106_TypeDef* sh1106_init(void){
	
	sh1106_TypeDef* self= (sh1106_TypeDef*)malloc(sizeof(sh1106_TypeDef));
	
	self->clearScreen=&sh1106_clearScreen;
	self->sendCommand=&sh1106_sendCommand;
	self->sendData=&sh1106_sendData;
	self->draw=&sh1106_drawImage;
	self->sleep=&sh1106_Sleep;
	self->wakeUp=&sh1106_WakeUp;
	
uint8_t cmds[] = {
			0x80, 0xAE,// command, Display off
			0x80, 0x20,// command, Display off
			0x80, 0x00,// command, Horizontal Addressing Mode??
			0x80, 0xB0,// command, Page Start Address
			0x80, 0xC8,// command, COM Output Scan Direction
			0x80, 0x00, // Lower column address = 0
			0x80, 0x10,  // Higher column address = 0
			0x80, 0x40,// command, Start line address // Start at the first line0x40-0x7F
			0x80, 0x81,// command, Contrast// Contrast modification entered
			0x80, 0x7F,// command, Contrast Value //Min:0x00-Max:0xFF
			0x80, 0xA1,// command, Segment remap 0xA0-0xA1 (Now revers direction)
			0x80, 0xA6,// command, Normal/Revers display 0xA6-0xA7
			0x80, 0xA8,// command, Multiplex (Enter multiplex setup mode)
			0x80, 0x3F,// command, Multiplex//0x00-0x3F (64multiplexer mode) I have no idea what it is doing
			0x80, 0xA4,// command, Display all on resume (7.punkt)
			0x80, 0xD3,// command, Display offset Enters setup mode
			0x80, 0x00,// command, Display offset 0x00-0x3F
			0x80, 0xD5,// command, Display clock Enters setup mode
			0x80, 0x80,// command, Display clock 0x00-0xFF
			0x80, 0xD9,// command, Pre-charge Enters setup mode
			0x80, 0xF1,// command, Pre-charge 0x00-0xFF
			0x80, 0xDA,// command, COM pins Enters setup mode
			0x80, 0x12,// command, COM pins 0x02-0x12
			0x80, 0xDB,// command, VCOM detect Enters setup mode
			0x80, 0x40,// command, VCOM detect 0x00-0xFF
			0x80, 0xAF,// command, Display on
	    };
	    HAL_I2C_Master_Transmit(&hi2c2, 0x3C << 1, cmds, sizeof(cmds), HAL_MAX_DELAY);
		
		return self;
}
void sh1106_clearScreen(void){

	    uint8_t data_buf[131];
	    data_buf[0] = 0x40; // Control byte: Co=0, D/C#=1 (data)
		
	    // Fill all 128 data bytes with 0x00 (pixel off)
	    for (int i = 1; i <= sizeof(data_buf); i++) {
	        data_buf[i] = 0x00;
	    }
	    // Clear all 8 pages (page 0–7)
	    for (int page = 0; page < 8; page++) {
	        // Set page address
	      
			sh1106_sendCommand(0xB0 + page);
			sh1106_sendCommand(0x00);
			sh1106_sendCommand(0x10);

	        // Send 128 bytes of 0x00
	        HAL_I2C_Master_Transmit(&hi2c2, 0x3C << 1, data_buf, sizeof(data_buf), HAL_MAX_DELAY);
	    }
}

void sh1106_sendCommand(uint8_t command){
	uint8_t cmd_buf [2]={0x80,0x00};
	cmd_buf[1]=command;
	
	HAL_I2C_Master_Transmit(&hi2c2, SH1106_I2C_ADDR, cmd_buf, 2, HAL_MAX_DELAY);
}


void sh1106_sendData(uint8_t data){
	uint8_t data_buf [2]={0x40,0x00};
	data_buf[1]=data;
	
	HAL_I2C_Master_Transmit(&hi2c2, SH1106_I2C_ADDR, data_buf, 2, HAL_MAX_DELAY);
}

//void sh1106_updateScreen();

void sh1106_Sleep(void){
	
	sh1106_sendCommand(DISPLAYSLEEP(0));//Send sleep command
}

void sh1106_WakeUp(void){
	sh1106_sendCommand(DISPLAYSLEEP(1));//Send wakeUp command
}
void sh1106_drawImage(uint8_t *buffer, uint8_t width, uint8_t height, uint8_t x, uint8_t y) {
    uint8_t page_start = y / 8;
    uint8_t y_offset = y % 8;
    uint8_t total_pages = (height + 7) / 8;

    for (uint8_t col = 0; col < width; col++) {
        for (uint8_t page = 0; page <= total_pages; page++) {

            uint8_t upper = 0;
            uint8_t lower = 0;

            // Index in the image buffer (1-bit packed, vertical bytes)
            uint16_t byte_index = col + page * width;

            if (page < total_pages) {
                upper = buffer[byte_index];
            }
            if (page > 0 && byte_index - width < width * total_pages) {
                lower = buffer[byte_index - width];
            }

            // Shift based on y_offset to position the bits correctly
            uint8_t result = (upper << y_offset) | (lower >> (8 - y_offset)); 
            // Set page
            sh1106_sendCommand(0xB0 + page_start + page);

            // Set column
            sh1106_sendCommand(0x00 | ((x + col) & 0x0F));
            sh1106_sendCommand(0x10 | ((x + col) >> 4));

            // Send data
            sh1106_sendData(result);
        }
    }
}

