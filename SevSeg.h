/*
 * SevSeg.h
 *
 * Created on: Jun 8, 2025
 * Author: Jay
 */

#ifndef INC_SEVSEG_H_ // Changed to INC_SEVSEG_H_ to avoid potential conflicts with SEVSEG_H_
#define INC_SEVSEG_H_

#include <stdint.h> // For uint8_t, uint16_t

// --- Forward declaration of the structure ---
// This is needed for the function pointer inside the struct
typedef struct oneNumTypeDef oneNumTypeDef;

// --- Function pointer type definition ---
// This defines the signature for the readChar function pointer
typedef void (*readCharFunction)(uint8_t *buf, char value, oneNumTypeDef *self);

// --- Structure Definition ---
// This struct holds the configuration and buffer for a single 7-segment digit
struct oneNumTypeDef {
    uint8_t height;     // Corrected typo: hight -> height
    uint8_t width;      // Width of the 7-segment digit in pixels
    uint8_t thick;      // Thickness of the segments in pixels

    // Function pointer to read a character and update the internal buffer
    readCharFunction readChar; // Added back the function pointer
};

// --- Public Function Prototypes ---
// Initializes a new oneNumTypeDef object
oneNumTypeDef* sevnSeg_Init(uint8_t height, uint8_t width, uint8_t thick);

// Deinitializes (frees memory) a oneNumTypeDef object
void sevnSet_DeInit(oneNumTypeDef* self);

// Public function that can be assigned to the readChar function pointer
// This function reads a character and updates the display buffer
void sevnSeg_readValue(uint8_t *buf, char value, oneNumTypeDef *self);


// --- Function prototypes for internal (static) functions ---
// These are included in the header for the compiler to see them
// when compiling SevSeg.c, even if they are static within SevSeg.c.
// This is a common practice for internal helper functions.
uint8_t recSegNum(uint8_t num);
uint8_t recSegCh(char ch);
void setSegment(oneNumTypeDef *self,uint8_t segment,uint8_t *buffer);
void setVertical(oneNumTypeDef *self, uint8_t x, uint8_t y, uint8_t *buffer);
void setHorisontal(oneNumTypeDef *self, uint8_t x, uint8_t y, uint8_t *buffer);
uint8_t fillByte(uint8_t *segmentHight,uint8_t *pageOffset);
void setSpace(oneNumTypeDef *self, uint8_t* buffer);
void setDdott(oneNumTypeDef *self, uint8_t* buffer);


// --- Segment Bitmasks ---
// These macros define the bit patterns for each number/character on a 7-segment display.
// The bit assignment is inferred from the `setSegment` function's logic:
// Bit 0: Left-Bottom Vertical
// Bit 1: Left-Top Vertical
// Bit 2: Right-Top Vertical
// Bit 3: Right-Bottom Vertical
// Bit 4: Bottom Horizontal
// Bit 5: Middle Horizontal
// Bit 6: Top Horizontal
// Bit 7: Unused (can be for decimal point if needed)

#define SEGMENTNUM_0 0b01011111
#define SEGMENTNUM_1 0b00001100
#define SEGMENTNUM_2 0b01110101
#define SEGMENTNUM_3 0b01111100
#define SEGMENTNUM_4 0b00101110
#define SEGMENTNUM_5 0b01111010
#define SEGMENTNUM_6 0b01111011
#define SEGMENTNUM_7 0b01001100
#define SEGMENTNUM_8 0b01111111
#define SEGMENTNUM_9 0b01111110

#endif /* INC_SEVSEG_H_ */
