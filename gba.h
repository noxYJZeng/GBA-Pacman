#ifndef GBA_H
#define GBA_H

// ---------------------------------------------------------------------------
//                       USEFUL TYPEDEFS
// ---------------------------------------------------------------------------

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;

typedef unsigned short Color; 
typedef Color (*GradientFunc)(int index, int totalChars);

// ---------------------------------------------------------------------------
//                       MODE3 MACROS
// ---------------------------------------------------------------------------
#define OFFSET(row, col, width) ((col)+(width)*(row))

#define REG_DISPCNT (*(volatile unsigned short *) 0x4000000)
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define RGB(R, G, B) (((R) & 31) | ((G) & 31) << 5 | ((B) & 31) << 10)
#define BLACK   RGB(0, 0, 0)
#define WHITE   RGB(31, 31, 31)
#define GRAY    RGB(15, 15, 15)
#define RED     RGB(31, 0, 0)
#define GREEN   RGB(0, 31, 0)
#define BLUE    RGB(0, 0, 31)
#define CYAN    RGB(0, 31, 31)
#define MAGENTA RGB(31, 0, 31)
#define YELLOW  RGB(31, 31, 0)
#define BROWN   RGB(20, 10, 5)
#define TEAL RGB(0,16,16)
#define ORANGE  RGB(31, 15, 0)


// The size of the GBA Screen
#define WIDTH 240
#define HEIGHT 160

// This is initialized in gba.c
extern volatile unsigned short *videoBuffer;

// ---------------------------------------------------------------------------
//                       BUTTON INPUT
// ---------------------------------------------------------------------------
#define BUTTON_A        (1<<0)
#define BUTTON_B        (1<<1)
#define BUTTON_SELECT   (1<<2)
#define BUTTON_START    (1<<3)
#define BUTTON_RIGHT    (1<<4)
#define BUTTON_LEFT     (1<<5)
#define BUTTON_UP       (1<<6)
#define BUTTON_DOWN     (1<<7)
#define BUTTON_R        (1<<8)
#define BUTTON_L        (1<<9)

#define BUTTONS (*(volatile u32 *) 0x4000130)
#define KEY_DOWN(key, buttons) (~(buttons) & (key))

// TODO: COMPLETE THIS MACRO.
// Remember that a button is recently pressed if it wasn't pressed in the last
// input (oldButtons) but is pressed in the current input. Use the KEY_DOWN
// macro to check if the button was pressed in the inputs.
#define KEY_JUST_PRESSED(key, buttons, oldbuttons) (~(buttons) & (oldbuttons)) & (key)

// ---------------------------------------------------------------------------
//                       DMA
// ---------------------------------------------------------------------------
typedef struct
{
  const volatile void *src;
  const volatile void *dst;
  u32                  cnt;
} DMA_CONTROLLER;

#define DMA ((volatile DMA_CONTROLLER *) 0x040000B0)

// Defines
#define DMA_CHANNEL_0 0
#define DMA_CHANNEL_1 1
#define DMA_CHANNEL_2 2
#define DMA_CHANNEL_3 3

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)



// ---------------------------------------------------------------------------
//                       VBLANK
// ---------------------------------------------------------------------------
#define SCANLINECOUNTER (*(volatile unsigned short *) 0x4000006)

// Use this variable to count vBlanks. Initialized in gba.c and to be
// manipulated by waitForVBlank()
extern u32 vBlankCounter;

/*
 * Runs a blocking loop until the start of next VBlank.
 */
void waitForVBlank(void);

// ---------------------------------------------------------------------------
//                       MISC
// ---------------------------------------------------------------------------
#define UNUSED(param) ((void)((param)))

/*
 * Generates a pseudo-random number between min and max.
 *
 * @param  min bottom end of range (inclusive).
 * @param  max top end of range (exclusive).
 * @return random number in the given range.
 */
int randint(int min, int max);

// ---------------------------------------------------------------------------
//                       DRAWING
// ---------------------------------------------------------------------------
void setPixel(int row, int col, u16 color);
void drawRectDMA(int row, int col, int width, int height, volatile u16 color);
void drawFullScreenImageDMA(const u16 *image);
void drawImageDMA(int row, int col, int width, int height, const u16 *image);
void undrawImageDMA(int row, int col, int width, int height, const u16 *image);
void fillScreenDMA(volatile u16 color);
void drawChar(int row, int col, char ch, u16 color);
void drawString(int row, int col, char *str, u16 color);
void drawCenteredString(int row, int col, int width, int height, char *str, u16 color);
void drawTransparentImageDMA(int row, int col, int width, int height, const u16 *image);
void drawLargeChar(int x, int y, char ch, Color color, int scale);


/* Contains the pixels of each character from a 6x8 font */
// This is in the font.c file. You can replace the font if you want.
extern const unsigned char fontdata_6x8[12288];

#endif
