#ifndef LCD_PORT_H
#define LCD_PORT_H
#include <stdint.h>
void LCDPORT_InitController(void);
void LCDPORT_SetLandscape(void);
void LCDPORT_SetBacklight(int On);
void LCDPORT_Write8_A0(uint8_t data);
void LCDPORT_Write8_A1(uint8_t data);
void LCDPORT_WriteM8_A0(uint8_t *data, int count);
void LCDPORT_WriteM8_A1(uint8_t *data, int count);
void LCDPORT_SetCS(uint8_t not_active);
void LCDPORT_BlitRGB565(int x0, int y0, int width, int height,
                       const uint16_t *pixels, int source_stride);
#endif
