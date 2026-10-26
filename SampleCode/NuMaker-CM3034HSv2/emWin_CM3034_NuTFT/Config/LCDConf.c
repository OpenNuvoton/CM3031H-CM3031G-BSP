#include <stdio.h>
#include "GUI.h"
#include "GUIDRV_FlexColor.h"
#include "LCD_Port.h"

void LCD_X_Config(void) {
  GUI_DEVICE *device;
  GUI_PORT_API port_api = {0};
  CONFIG_FLEXCOLOR config = {0};

  printf("[GUI 04] LCD_X_Config enter (NuFun 320x240)\n");
  device = GUI_DEVICE_CreateAndLink(GUIDRV_FLEXCOLOR, GUICC_M565, 0, 0);
  if (device == NULL) {
    printf("[GUI ERROR] GUI_DEVICE_CreateAndLink failed\n");
    while (1) { }
  }
  port_api.pfWrite8_A0 = LCDPORT_Write8_A0;
  port_api.pfWrite8_A1 = LCDPORT_Write8_A1;
  port_api.pfWriteM8_A0 = LCDPORT_WriteM8_A0;
  port_api.pfWriteM8_A1 = LCDPORT_WriteM8_A1;
  port_api.pfSetCS = LCDPORT_SetCS;
  GUIDRV_FlexColor_SetFunc(device, &port_api,
                           GUIDRV_FLEXCOLOR_F66709,
                           GUIDRV_FLEXCOLOR_M16C0B8);
  config.Orientation = 0;
  config.RegEntryMode = 0xE8;
  GUIDRV_FlexColor_Config(device, &config);
  LCD_SetSizeEx(0, 320, 240);
  LCD_SetVSizeEx(0, 320, 240);
  printf("[GUI 05] LCD size=%dx%d\n", LCD_GetXSizeEx(0), LCD_GetYSizeEx(0));
}

int LCD_X_DisplayDriver(unsigned layer_index, unsigned cmd, void *p_data) {
  GUI_USE_PARA(layer_index);
  GUI_USE_PARA(p_data);
  if (cmd == LCD_X_INITCONTROLLER) {
    printf("[GUI 06] LCD controller init begin\n");
    LCDPORT_InitController();
    printf("[GUI 07] LCD controller init complete\n");
    return 0;
  }
  return -1;
}
