#include <stdio.h>
#include "NuMicro.h"
#include "GUI.h"
#include "LCD_Port.h"
#include "Touch_Port.h"
#include "Keys_Port.h"
#include "Joystick_Port.h"
#include "ARGB_Port.h"
#include "Application/Demo_UI.h"
#include "Application/Boot_Screen.h"

extern volatile GUI_TIMER_TIME OS_TimeMS;

static void Fault_Stop(const char *name) {
  __disable_irq();
  printf("\n[FAULT] %s HFSR=%08X CFSR=%08X BFAR=%08X\n",
         name, (unsigned)SCB->HFSR, (unsigned)SCB->CFSR,
         (unsigned)SCB->BFAR);
  while (1) { __NOP(); }
}

void HardFault_Handler(void) { Fault_Stop("HardFault"); }
void MemManage_Handler(void) { Fault_Stop("MemManage"); }
void BusFault_Handler(void) { Fault_Stop("BusFault"); }
void UsageFault_Handler(void) { Fault_Stop("UsageFault"); }

void TMR0_IRQHandler(void) {
  OS_TimeMS++;
  TIMER_ClearIntFlag(TIMER0);
}

static void SYS_Init(void) {
  CLK_EnableXtalRC(CLK_PWRCTL_HIRCEN_Msk);
  CLK_WaitClockReady(CLK_STATUS_HIRCSTB_Msk);
  CLK->PCLKDIV = CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2;
  CLK_SetCoreClock(160000000UL);

  CLK->AHBCLK0 |= CLK_AHBCLK0_GPACKEN_Msk |
                  CLK_AHBCLK0_GPBCKEN_Msk |
                  CLK_AHBCLK0_GPCCKEN_Msk |
                  CLK_AHBCLK0_GPDCKEN_Msk |
                  CLK_AHBCLK0_GPECKEN_Msk |
                  CLK_AHBCLK0_GPFCKEN_Msk |
                  CLK_AHBCLK0_GPGCKEN_Msk |
                  CLK_AHBCLK0_GPHCKEN_Msk;
  CLK_EnableModuleClock(UART0_MODULE);
  CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC,
                     CLK_CLKDIV0_UART0(1));
  SET_UART0_RXD_PB12();
  SET_UART0_TXD_PB13();
  CLK_EnableModuleClock(TMR0_MODULE);
  CLK_SetModuleClock(TMR0_MODULE, CLK_CLKSEL1_TMR0SEL_HIRC, MODULE_NoMsk);
}

static void Timer_Init(void) {
  TIMER_Open(TIMER0, TIMER_PERIODIC_MODE, 1000);
  TIMER_EnableInt(TIMER0);
  NVIC_SetPriority(TMR0_IRQn, 1);
  NVIC_EnableIRQ(TMR0_IRQn);
  TIMER_Start(TIMER0);
}

int main(void) {
  uint32_t reset_source;

  SYS_UnlockReg();
  reset_source = SYS_GetResetSrc();
  SYS_Init();
  UART_Open(UART0, 115200);
  Timer_Init();

  printf("\n[BOOT] CM3034HSB + NuFun emWin V6.56\n");
  printf("[BOOT] Reset source=0x%08X, CPU=%u Hz\n",
         (unsigned)reset_source, (unsigned)SystemCoreClock);
  SYS_ClearResetSrc(reset_source);
  printf("[BOOT] GUI_Init begin\n");
  GUI_Init();
  LCDPORT_SetLandscape();
  printf("[BOOT] GUI_Init OK, LCD=%dx%d\n", LCD_GetXSize(), LCD_GetYSize());

  printf("[BOOT] low-brightness ARGB boot accent begin\n");
  ARGBPORT_Init();
  ARGBPORT_Exec((uint32_t)OS_TimeMS);
  BOOTSCREEN_Show();
  ARGBPORT_Off();

  (void)TOUCHPORT_Init();
  KEYPORT_Init();
  JOYPORT_Init();
  printf("[BOOT] UI draw begin\n");
  DEMOUI_Init();
  printf("[BOOT] UI draw OK\n");
  SYS_LockReg();
  printf("[BOOT] product UI running; ARGB remains off\n");

  while (1) {
    int x;
    int y;
    int pressed = TOUCHPORT_Exec(&x, &y);
    DEMOUI_Exec(OS_TimeMS, x, y, pressed);
    GUI_Exec();
    GUI_Delay(5);
  }
}
