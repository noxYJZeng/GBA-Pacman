#include "gba.h"

volatile unsigned short *videoBuffer = (volatile unsigned short *) 0x6000000;
u32 vBlankCounter = 0;




/*
  Wait until the start of the next VBlank. This is useful to avoid tearing.
  Completing this function is required.
*/
void waitForVBlank(void) {
  // TODO: IMPLEMENT

  // (1)
  // Write a while loop that loops until we're NOT in vBlank anymore:
  // (This prevents counting one VBlank more than once if your app is too fast)
  while (SCANLINECOUNTER >= 160);
  // (2)
  // Write a while loop that keeps going until we're in vBlank:
  while(SCANLINECOUNTER < 160);
  // (3)
  // Finally, increment the vBlank counter:
  vBlankCounter++;
}

static int __qran_seed = 42;
static int qran(void) {
  __qran_seed = 1664525 * __qran_seed + 1013904223;
  return (__qran_seed >> 16) & 0x7FFF;
}

int randint(int min, int max) { return (qran() * (max - min) >> 15) + min; }

/*
  Sets a pixel in the video buffer to a given color.
  Using DMA is NOT recommended. (In fact, using DMA with this function would be really slow!)
*/
void setPixel(int row, int col, u16 color) {
  // TODO: IMPLEMENT
  *(videoBuffer + OFFSET(row, col, WIDTH)) = color;
}

/*
  Draws a rectangle of a given color to the video buffer.
  The width and height, as well as the top left corner of the rectangle, are passed as parameters.
  This function can be completed using `height` DMA calls. 
*/
void drawRectDMA(int row, int col, int width, int height, volatile u16 color) {
  // TODO: IMPLEMENT
  for (int i = 0; i < height; i++) {
    volatile unsigned short *dst = videoBuffer + OFFSET(i + row, col, WIDTH);

    DMA[3].src = &color;
    DMA[3].dst = dst;
    DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
  }
}

/*
  Draws a fullscreen image to the video buffer.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using a single DMA call.
*/
void drawFullScreenImageDMA(const u16 *image) {
  // TODO: IMPLEMENT
  DMA[3].src = image;//不变
  DMA[3].dst = videoBuffer;//不变
  DMA[3].cnt = DMA_ON | (240 * 160) | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;//上面俩都要变
}

/*
  Draws an image to the video buffer.
  The width and height, as well as the top left corner of the image, are passed as parameters.
  The image passed in must be of size width * height.
  Completing this function is required.
  This function can be completed using `height` DMA calls. Solutions that use more DMA calls will not get credit.
*/
void drawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  for (int i = 0; i < height; i++) {
    volatile unsigned short* dst = videoBuffer + OFFSET(i + row, col, WIDTH);
    DMA[3].src = image + i * width;
    DMA[3].dst = dst;
    DMA[3].cnt = DMA_ON | width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
  }
}

/*
  Draws a rectangular chunk of a fullscreen image to the video buffer.
  The width and height, as well as the top left corner of the chunk to be drawn, are passed as parameters.
  The image passed in must be of size WIDTH * HEIGHT.
  This function can be completed using `height` DMA calls.
*/
void undrawImageDMA(int row, int col, int width, int height, const u16 *image) {
  // TODO: IMPLEMENT
  for (int i = 0; i < height; i++) {
    volatile unsigned short* dst = videoBuffer + OFFSET(i + row, col, WIDTH);
    const volatile unsigned short* src = image + OFFSET(i + row, col, WIDTH );

    DMA[3].src = src;
    DMA[3].dst = dst;
    DMA[3].cnt = DMA_ON | width | DMA_SOURCE_INCREMENT | DMA_DESTINATION_INCREMENT;
  }
}

/*
  Fills the video buffer with a given color.
  This function can be completed using a single DMA call.
*/
void fillScreenDMA(volatile u16 color) {
  // TODO: IMPLEMENT
  DMA[3].src = &color;          
  DMA[3].dst = videoBuffer;
  DMA[3].cnt = DMA_ON | DMA_SOURCE_FIXED | (240 * 160);
}

/* STRING-DRAWING FUNCTIONS (provided) */
void drawChar(int row, int col, char ch, u16 color) {
  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      if (fontdata_6x8[OFFSET(j, i, 6) + ch * 48]) {
        setPixel(row + j, col + i, color);
      }
    }
  }
}

void drawLargeChar(int row, int col, char ch, u16 color, int scale) {
  for (int dy = 0; dy < 8; dy++) {  // 8 行字体
      for (int dx = 0; dx < 6; dx++) {  // 6 列字体
          if (fontdata_6x8[OFFSET(dy, dx, 6) + ch * 48]) {
              for (int sy = 0; sy < scale; sy++) {
                  for (int sx = 0; sx < scale; sx++) {
                      setPixel(row + dy * scale + sy, col + dx * scale + sx, color);
                  }
              }
          }
      }
  }
}


void drawString(int row, int col, char *str, u16 color) {
  while (*str) {
    drawChar(row, col, *str++, color);
    col += 6;
  }
}

void drawOutlinedChar(int x, int y, char ch, Color outlineColor, Color textColor) {
  for (int dx = -1; dx <= 1; dx++) {
      for (int dy = -1; dy <= 1; dy++) {
          if (dx != 0 || dy != 0) {
              drawChar(x + dx, y + dy, ch, outlineColor);
          }
      }
  }
  drawChar(x, y, ch, textColor);
}

void drawGradientString(int x, int y, char *str, Color outlineColor, GradientFunc gradientFunc) {
  int i = 0;
  int length = 0;
  
  while (str[length] != '\0') {
      length++;
  }

  while (str[i] != '\0') {
      Color textColor = gradientFunc(i, length);
      drawOutlinedChar(x + (i * 6), y, str[i], outlineColor, textColor);
      i++;
  }
}


void drawCenteredString(int row, int col, int width, int height, char *str, u16 color) {
  u32 len = 0;
  char *strCpy = str;
  while (*strCpy) {
    len++;
    strCpy++;
  }

  u32 strWidth = 6 * len;
  u32 strHeight = 8;

  int new_row = row + ((height - strHeight) >> 1);
  int new_col = col + ((width - strWidth) >> 1);
  drawString(new_row, new_col, str, color);
}

void drawTransparentImageDMA(int row, int col, int width, int height, const u16 *image) {
  for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
          u16 pixel = image[y * width + x];
          if (pixel != 0x0000) {
              videoBuffer[OFFSET(row + y, col + x, WIDTH)] = pixel;
          }
      }
  }
}



