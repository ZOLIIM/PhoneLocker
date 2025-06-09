/*
 * SevSeg.c
 *
 * Created on: Jun 8, 2025
 * Author: Jay
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "SevSeg.h" // Include the corrected header file

// --- Static Function Prototypes (for internal use within this C file) ---
// These are not exposed in the header file, but declared here for the compiler.
static uint8_t recSegNum_static(uint8_t num); // Renamed to avoid conflict with public prototype if not intended as static
static uint8_t recSegCh_static(char ch);     // Renamed to avoid conflict with public prototype if not intended as static
static void setSegment_static(oneNumTypeDef *self,uint8_t segment,uint8_t *buffer);
static void setVertical_static(oneNumTypeDef *self, uint8_t x, uint8_t y, uint8_t *buffer);
static void setHorisontal_static(oneNumTypeDef *self, uint8_t x, uint8_t y, uint8_t *buffer);
static uint8_t fillByte_static(uint8_t *segmentHight,uint8_t *pageOffset);
static void setSpace_static(oneNumTypeDef *self, uint8_t* buffer);
static void setDdott_static(oneNumTypeDef *self, uint8_t* buffer);


oneNumTypeDef* sevnSeg_Init(uint8_t height, uint8_t width, uint8_t thick){

	oneNumTypeDef* self = (oneNumTypeDef*)malloc(sizeof(oneNumTypeDef));
	if (self == NULL) {
	        return NULL; // Always check malloc!
	    }
	self->height = height; // Corrected typo: hight -> height
	self->width = width;
	self->thick = thick;

	self->readChar = sevnSeg_readValue; // Re-enabled function pointer assignment

	return self;
}

void sevnSet_DeInit(oneNumTypeDef* self){
	if (self != NULL) {
		free(self);
	}
}

// Function definition for sevnSeg_readValue, which is part of oneNumTypeDef's interface
void sevnSeg_readValue(uint8_t *buf, char value, oneNumTypeDef *self){

	uint8_t segment = 0;
	uint8_t number = 0; // Initialize to avoid issues if value is not a digit

	// Determine if value is a digit and convert, or a special character
	if (value >= '0' && value <= '9') {
	    number = (uint8_t)(value - '0');
	    segment = recSegNum_static(number); // Call the static version
	} else if (value == ' ' || value == ':'){
		segment = recSegCh_static(value); // Call the static version
	} else {
	    return; // If not a recognized character or number, do nothing
	}

	setSegment_static(self, segment, buf); // Call the static version
}

//read the given number to fill up the buffer for the display so the sevensegment can visualise the correct number
static uint8_t recSegNum_static(uint8_t num){ // Made static

	switch (num){
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
	return 0b00000000; // Return an empty segment for unknown numbers
	}
}

//read the seperator character
static uint8_t recSegCh_static(char ch) // Made static
{
	if( ch == ':') return 1; // Return 1 for ':' (mapped to double dot)
	else if (ch == ' ') return 0; // Return 0 for ' ' (mapped to space)
	else return 255; // Indicate an unknown character
}

/*
Fills up the buffer of the oneNumTypeDef with the necessary segments based on the segment code previously given
Reads in the given active segment bitfield and sets the location of each horizontal and vertical segment
*/
static void setSegment_static(oneNumTypeDef *self,uint8_t segment,uint8_t *buffer){ // Made static
	setSpace_static(self,buffer);
	uint8_t x=0;
	uint8_t y=0;
	uint8_t bit_value=0; // Renamed newBits to bit_value for clarity

	if(segment == 0) {
	    setSpace_static(self,buffer); // Call static version
	} else if (segment == 1) {
	    setDdott_static(self,buffer); // Call static version
	} else {
		for(uint8_t i = 0; i < 8; i++){
				bit_value = (segment >> i) & 0x01; // Get the i-th bit

				//Verticals coordinates
				if(i < 4 && bit_value == 1){ // Check if the bit is set and it's a vertical segment index
					if(i == 3){ // Right-bottom vertical
						x = self->width - self->thick;
						y = (self->height + self->thick) / 2; // Corrected typo: hight -> height
					}
					else if(i == 2){ // Right-top vertical
						x = self->width - self->thick;
						y = self->thick;
					}
					else if(i == 1){ // Left-top vertical
						x = 0;
						y = self->thick;
					}
					else if(i == 0){ // Left-bottom vertical
						x = 0;
						y = (self->height + self->thick) / 2; // Corrected typo: hight -> height
					}
					setVertical_static(self,x,y,buffer); // Call static version
				}
				//Horisontal coordinates
				else if(i >= 4 && bit_value == 1){ // Check if the bit is set and it's a horizontal segment index
					x = self->thick; // Common x-start for horizontal segments

					if(i == 6){ // Top horizontal
						y = 0;
					}
					else if(i == 5){ // Mid horizontal
						y = (self->height - self->thick) / 2; // Corrected typo: hight -> height, adjusted calculation
					}
					else if(i == 4){ // Bottom horizontal
						y = self->height - self->thick; // Corrected typo: hight -> height
					}
					setHorisontal_static(self,x,y,buffer); // Call static version
				}
			}
	}
}

/*
This function sets up all the vertical segments at the given location
oneNumTypeDef is the structure containing all the data of the SevenSegment display
x = column location in px (bitfield)
y = row location in px (bitfield)

First it for loop, goes through all the columns
Second for loop goes through all the pages (built based on the SH1106 Oled)
A page is an 8bit*picture width px field

With this setup, it fills up the buffer of the oneNumTypeDef, with placed vertical oriented segment
*/
static void setVertical_static(oneNumTypeDef *self, uint8_t x, uint8_t y, uint8_t *buffer){ // Made static

	uint8_t page_start = y / 8;
    uint8_t total_pages = (self->height + 7) / 8; // Corrected typo: hight -> height
	uint8_t pageOffset = 0;
	uint8_t segmentHeight_remaining = 0; // Renamed for clarity

    for (uint8_t col = x; col < self->thick + x; col++) {
		segmentHeight_remaining = ((self->height - (3 * self->thick)) / 2); // Corrected typo: hight -> height
		pageOffset = y % 8;
        for (uint8_t page = page_start; page < total_pages; page++) { // Loop up to total_pages, not <= total_pages as pages are 0-indexed
            if (col + (page * self->width) < (self->width * total_pages)) { // Prevent out-of-bounds access
			    buffer[col + (page * self->width)] |= fillByte_static(&segmentHeight_remaining, &pageOffset); // Call static version
            }
		}
	}
}

/*
This function sets up all the horizontal segments at the given location
oneNumTypeDef is the structure containing all the data of the SevenSegment display
x = column location in px (bitfield)
y = row location in px (bitfield)

First it for loop, goes through all the columns
Second for loop goes through all the pages (built based on the SH1106 Oled)
A page is an 8bit*picture width px field

With this setup, it fills up the buffer of the oneNumTypeDef, with placed horizontal oriented segment
*/
static void setHorisontal_static(oneNumTypeDef *self, uint8_t x, uint8_t y,uint8_t *buffer){ // Made static

	uint8_t page_start = y / 8;
    uint8_t total_pages = (self->height + 7) / 8; // Corrected typo: hight -> height
	uint8_t pageOffset = 0;
	uint8_t segmentWidth = self->width - (2 * self->thick);
	uint8_t segmentHeight_remaining = 0; // Renamed for clarity

    for (uint8_t col = x; col < segmentWidth + x; col++) {
		pageOffset = y % 8;
		segmentHeight_remaining = self->thick; // Thickness for horizontal segment height
        for (uint8_t page = page_start; page < total_pages; page++) { // Loop up to total_pages, not <= total_pages
            if (col + (page * self->width) < (self->width * total_pages)) { // Prevent out-of-bounds access
			    buffer[col + (page * self->width)] |= fillByte_static(&segmentHeight_remaining, &pageOffset); // Call static version
            }
		}
	}
}

/*
This function fills up the 8-bit page column of the screen with the given parameters
px: the actual pixel (1 bit is one pixel)
pageOffset: if the pixel is not at the top of the page but somewhere else
segmentHeight: The segment height which needs to be set
the page bits start from the "top"

They need to be referenced by place (pointers)
If the segment continues to the next page, the segmentHeight would not be updated and started from the beginning again
*/
static uint8_t fillByte_static(uint8_t *segmentHeight,uint8_t *pageOffset){ // Made static and renamed parameter

	uint8_t buf = 0;
	for(uint8_t px = 0; px < 8; px++){
		if(*pageOffset > 0){ // fills the byte with 0 till the placed bits needed (Padding)
			buf |= (0 << px); // Explicitly set bit to 0
			*pageOffset = *pageOffset - 1;
		}
		else if(*segmentHeight > 0){ //fills the byte with 1s till reach the max width
			buf |= (1 << px);
			*segmentHeight = *segmentHeight - 1;
		}
		else {
			buf |= (0 << px); // fill the rest of the byte with 0
		}
	}
	return buf;
}


static void setSpace_static(oneNumTypeDef *self, uint8_t *buffer){ // Made static

	uint8_t total_pages = (self->height + 7) / 8; // Corrected typo: hight -> height

	for (uint8_t col = 0; col < self->width; col++){
		for(uint8_t page = 0; page < total_pages; page++){
            if (col + (page * self->width) < (self->width * total_pages)) { // Prevent out-of-bounds access
			    buffer[col + page * self->width] = 0x00; // Clear the entire buffer area for this character
            }
		}
	}
}

static void setDdott_static(oneNumTypeDef *self,uint8_t *buffer){ // Made static

	uint8_t total_pages = (self->height + 7) / 8; // Corrected typo: hight -> height
	uint8_t dotWidth = self->width / 3;
	uint8_t dotHeight = self->height / 6; // Adjusted dot height for better appearance, was (self->height / (self->width / 2))
	uint8_t pageOffset = 0;
	uint8_t segmentHeight_remaining = 0;

    // Dot 1 (top dot)
    uint8_t dot1_y_start = self->height / 4 - dotHeight / 2; // Centered in the top quarter
    uint8_t dot1_page_start = dot1_y_start / 8;
    uint8_t dot1_pageOffset_initial = dot1_y_start % 8;

    for (uint8_t col = (self->width - dotWidth) / 2; col < (self->width + dotWidth) / 2; col++){
        segmentHeight_remaining = dotHeight;
        pageOffset = dot1_pageOffset_initial;
        for(uint8_t page = dot1_page_start; page < total_pages; page++){
            if (segmentHeight_remaining == 0 && pageOffset == 0 && page > dot1_page_start) break; // Optimization
            if (col + (page * self->width) < (self->width * total_pages)) { // Prevent out-of-bounds access
                buffer[col + page * self->width] |= fillByte_static(&segmentHeight_remaining, &pageOffset); // Call static version
            }
        }
    }

    // Dot 2 (bottom dot)
    uint8_t dot2_y_start = (self->height * 3) / 4 - dotHeight / 2; // Centered in the bottom quarter
    uint8_t dot2_page_start = dot2_y_start / 8;
    uint8_t dot2_pageOffset_initial = dot2_y_start % 8;

    for (uint8_t col = (self->width - dotWidth) / 2; col < (self->width + dotWidth) / 2; col++){
        segmentHeight_remaining = dotHeight;
        pageOffset = dot2_pageOffset_initial;
        for(uint8_t page = dot2_page_start; page < total_pages; page++){
            if (segmentHeight_remaining == 0 && pageOffset == 0 && page > dot2_page_start) break; // Optimization
            if (col + (page * self->width) < (self->width * total_pages)) { // Prevent out-of-bounds access
                buffer[col + page * self->width] |= fillByte_static(&segmentHeight_remaining, &pageOffset); // Call static version
            }
        }
    }
}
