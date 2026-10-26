#include <stdio.h>
#include "NuMicro.h"
#include "LCD_Port.h"

/* NuFun schematic: SPI1 PE0/PE1/PH8/PH9, DC PH10, EN PH11, RST PD14. */
#define LCD_SPI SPI1

static void _DelayMs(uint32_t ms) {
  volatile uint32_t count = (SystemCoreClock / 2000u) * ms;
  while (count-- != 0u) { __NOP(); }
}

static void _WaitIdle(void) {
  while (SPI_IS_BUSY(LCD_SPI)) { }
}

static void _WriteBytes(const uint8_t *data, int count) {
  while (count-- > 0) {
    while (SPI_GET_TX_FIFO_FULL_FLAG(LCD_SPI)) { }
    SPI_WRITE_TX(LCD_SPI, *data++);
  }
  _WaitIdle();
}

static void _CommandData(uint8_t command, const uint8_t *data, int count) {
  PH9 = 0;
  PH10 = 0;
  _WriteBytes(&command, 1);
  if (count > 0) {
    PH10 = 1;
    _WriteBytes(data, count);
  }
  PH9 = 1;
}

void LCDPORT_Write8_A0(uint8_t data) { PH10 = 0; _WriteBytes(&data, 1); }
void LCDPORT_Write8_A1(uint8_t data) { PH10 = 1; _WriteBytes(&data, 1); }
void LCDPORT_WriteM8_A0(uint8_t *data, int count) { PH10 = 0; _WriteBytes(data, count); }
void LCDPORT_WriteM8_A1(uint8_t *data, int count) { PH10 = 1; _WriteBytes(data, count); }
void LCDPORT_SetCS(uint8_t not_active) { PH9 = not_active ? 1 : 0; }

void LCDPORT_BlitRGB565(int x0, int y0, int width, int height,
                       const uint16_t *pixels, int source_stride) {
  uint8_t address[4];
  uint8_t line[320 * 2];
  int x;
  int y;

  if ((pixels == 0) || (width <= 0) || (height <= 0) ||
      (width > 320) || (source_stride < width)) {
    return;
  }

  address[0] = (uint8_t)((uint32_t)x0 >> 8);
  address[1] = (uint8_t)x0;
  address[2] = (uint8_t)((uint32_t)(x0 + width - 1) >> 8);
  address[3] = (uint8_t)(x0 + width - 1);
  _CommandData(0x2Au, address, 4);
  address[0] = (uint8_t)((uint32_t)y0 >> 8);
  address[1] = (uint8_t)y0;
  address[2] = (uint8_t)((uint32_t)(y0 + height - 1) >> 8);
  address[3] = (uint8_t)(y0 + height - 1);
  _CommandData(0x2Bu, address, 4);
  _CommandData(0x2Cu, 0, 0);

  /* GUI_MEMDEV_APILIST_16 + GUICC_M565 stores native U16 RGB565 pixels.
     ILI9341 expects each pixel on SPI in big-endian byte order. */
  PH10 = 1;
  for (y = 0; y < height; y++) {
    for (x = 0; x < width; x++) {
      uint16_t color = pixels[x];
      line[x * 2]     = (uint8_t)(color >> 8);
      line[x * 2 + 1] = (uint8_t)color;
    }
    _WriteBytes(line, width * 2);
    pixels += source_stride;
  }
}

void LCDPORT_SetLandscape(void) {
  const uint8_t madctl = 0xE8u;
  _CommandData(0x36u, &madctl, 1);
  printf("[LCD 06] ILI9341 landscape MADCTL=0xE8\n");
}

void LCDPORT_SetBacklight(int On) {
  PH11 = On ? 1 : 0;
}

void LCDPORT_InitController(void) {
  static uint8_t initialized;
  uint8_t data[15];
  static const uint8_t gamma_p[15] = {
    0x0F,0x31,0x2B,0x0C,0x0E,0x08,0x4E,0xF1,0x37,0x07,0x10,0x03,0x0E,0x09,0x00
  };
  static const uint8_t gamma_n[15] = {
    0x00,0x0E,0x14,0x03,0x11,0x07,0x31,0xC1,0x48,0x08,0x0F,0x0C,0x31,0x36,0x0F
  };

  if (initialized) { return; }
  initialized = 1u;
  printf("[LCD 01] NuFun GPIO/SPI1 configuration begin\n");

  CLK_EnableModuleClock(GPD_MODULE);
  CLK_EnableModuleClock(GPE_MODULE);
  CLK_EnableModuleClock(GPH_MODULE);
  CLK_EnableModuleClock(SPI1_MODULE);
  CLK_SetModuleClock(SPI1_MODULE, CLK_CLKSEL2_SPI1SEL_PCLK0, MODULE_NoMsk);
  SET_SPI1_MOSI_PE0();
  SET_SPI1_MISO_PE1();
  SET_SPI1_CLK_PH8();
  SET_SPI1_SS_PH9();
  SET_GPIO_PH10();
  SET_GPIO_PH11();
  SET_GPIO_PD14();
  GPIO_SetMode(PH, BIT10 | BIT11, GPIO_MODE_OUTPUT);
  GPIO_SetMode(PD, BIT14, GPIO_MODE_OUTPUT);
  PH->SMTEN |= GPIO_SMTEN_SMTEN8_Msk;
  GPIO_SetSlewCtl(PE, BIT0 | BIT1, GPIO_SLEWCTL_HIGH);
  GPIO_SetSlewCtl(PH, BIT8 | BIT9 | BIT10 | BIT11, GPIO_SLEWCTL_HIGH);
  PH9 = 1; PH10 = 0; PH11 = 0; PD14 = 1;

  SPI_Open(LCD_SPI, SPI_MASTER, SPI_MODE_0, 8, 48000000);
  SPI_SET_MSB_FIRST(LCD_SPI);
  SPI_EnableAutoSS(LCD_SPI, SPI_SS, SPI_SS_ACTIVE_LOW);
  printf("[LCD 02] SPI1 opened at 48 MHz\n");

  PD14 = 0; _DelayMs(20); PD14 = 1; _DelayMs(120);
  printf("[LCD 03] ILI9341 hardware reset complete\n");

  data[0]=0x00; data[1]=0xC1; data[2]=0x30; _CommandData(0xCF,data,3);
  data[0]=0x64; data[1]=0x03; data[2]=0x12; data[3]=0x81; _CommandData(0xED,data,4);
  data[0]=0x85; data[1]=0x00; data[2]=0x78; _CommandData(0xE8,data,3);
  data[0]=0x39; data[1]=0x2C; data[2]=0x00; data[3]=0x34; data[4]=0x02; _CommandData(0xCB,data,5);
  data[0]=0x20; _CommandData(0xF7,data,1);
  data[0]=0x00; data[1]=0x00; _CommandData(0xEA,data,2);
  data[0]=0x23; _CommandData(0xC0,data,1);
  data[0]=0x10; _CommandData(0xC1,data,1);
  data[0]=0x3E; data[1]=0x28; _CommandData(0xC5,data,2);
  data[0]=0x86; _CommandData(0xC7,data,1);
  data[0]=0xE8; _CommandData(0x36,data,1);
  data[0]=0x55; _CommandData(0x3A,data,1);
  data[0]=0x00; data[1]=0x18; _CommandData(0xB1,data,2);
  data[0]=0x08; data[1]=0x82; data[2]=0x27; _CommandData(0xB6,data,3);
  data[0]=0x00; _CommandData(0xF2,data,1);
  data[0]=0x01; _CommandData(0x26,data,1);
  _CommandData(0xE0,gamma_p,15);
  _CommandData(0xE1,gamma_n,15);
  printf("[LCD 04] ILI9341 320x240 sequence complete\n");

  _CommandData(0x11,0,0); _DelayMs(120);
  _CommandData(0x29,0,0); _DelayMs(20);
  /* Keep the panel dark until the first complete boot frame has reached
     GRAM. This prevents the controller's reset/clear contents from flashing
     before the animation becomes visible. */
  PH11 = 0;
  printf("[LCD 05] Sleep-out/display-on complete; backlight deferred\n");
}
