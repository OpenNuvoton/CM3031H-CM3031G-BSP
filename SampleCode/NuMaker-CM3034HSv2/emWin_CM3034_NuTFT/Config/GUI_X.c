#include "GUI.h"
#include "NuMicro.h"

volatile GUI_TIMER_TIME OS_TimeMS;

GUI_TIMER_TIME GUI_X_GetTime(void) {
  return OS_TimeMS;
}

void GUI_X_Delay(int ms) {
  GUI_TIMER_TIME end = OS_TimeMS + (GUI_TIMER_TIME)ms;
  while ((I32)(end - OS_TimeMS) > 0) {
    __NOP();
  }
}

void GUI_X_Init(void) {
}

void GUI_X_ExecIdle(void) {
  __NOP();
}

void GUI_X_Log(const char *s) {
  while (*s) {
    while (UART_IS_TX_FULL(UART0)) {
    }
    UART_WRITE(UART0, *s++);
  }
}

void GUI_X_Warn(const char *s) {
  GUI_X_Log(s);
}

void GUI_X_ErrorOut(const char *s) {
  GUI_X_Log(s);
}
