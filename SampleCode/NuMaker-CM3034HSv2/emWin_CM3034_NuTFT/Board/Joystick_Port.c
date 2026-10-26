#include <stdio.h>
#include "NuMicro.h"
#include "Joystick_Port.h"

/*
 * NuFun right-top JP5 mates with CM3034HS8 right-top JP4.  The male/female
 * connector reverses the odd/even pin within each row.
 * UP      NuFun JP5-14 -> CM3034 JP4-13 -> PG9
 * DOWN    NuFun JP5-16 -> CM3034 JP4-15 -> PG11
 * LEFT    NuFun JP5-20 -> CM3034 JP4-19 -> PG15
 * RIGHT   NuFun JP5-13 -> CM3034 JP4-14 -> PG10
 * CENTRAL NuFun JP5-22 -> CM3034 JP4-21 -> PA12
 *
 * The five contacts have external 10 kOhm pull-ups and are active low.
 */

static uint8_t _LastReported;
static uint16_t _DiagnosticDivider;
static uint8_t _DiagnosticSamplesLeft;

/*
 * Keep this enabled while locating a joystick wiring problem.  DEMOUI_Exec()
 * calls JOYPORT_GetState() every 5 ms, therefore 200 calls is about 1 second.
 */
#define JOY_DIAGNOSTIC_INTERVAL_CALLS  200u

static uint8_t _ReadState(void) {
  uint8_t state = 0u;
  if (!PG9)  state |= JOY_UP;
  if (!PG11) state |= JOY_DOWN;
  if (!PG15) state |= JOY_LEFT;
  if (!PG10) state |= JOY_RIGHT;
  if (!PA12) state |= JOY_CENTER;
  return state;
}

void JOYPORT_Init(void) {
  CLK_EnableModuleClock(GPA_MODULE);
  CLK_EnableModuleClock(GPG_MODULE);

  SET_GPIO_PA12();
  SET_GPIO_PG9();
  SET_GPIO_PG10();
  SET_GPIO_PG11();
  SET_GPIO_PG15();

  GPIO_SetMode(PA, BIT12, GPIO_MODE_INPUT);
  GPIO_SetMode(PG, BIT9 | BIT10 | BIT11 | BIT15, GPIO_MODE_INPUT);
  GPIO_SetPullCtl(PA, BIT12, GPIO_PUSEL_PULL_UP);
  GPIO_SetPullCtl(PG, BIT9 | BIT10 | BIT11 | BIT15, GPIO_PUSEL_PULL_UP);
  GPIO_ENABLE_DIGITAL_PATH(PA, BIT12);
  GPIO_ENABLE_DIGITAL_PATH(PG, BIT9 | BIT10 | BIT11 | BIT15);
  GPIO_ENABLE_SCHMITT_TRIGGER(PA, BIT12);
  GPIO_ENABLE_SCHMITT_TRIGGER(PG, BIT9 | BIT10 | BIT11 | BIT15);

  printf("[JOY] UP=PG9 DOWN=PG11 LEFT=PG15 RIGHT=PG10 CENTER=PA12\n");
  printf("[JOY] initial U/D/L/R/C=%u/%u/%u/%u/%u (released=1)\n",
         (unsigned)PG9, (unsigned)PG11, (unsigned)PG15,
         (unsigned)PG10, (unsigned)PA12);
  printf("[JOY] diagnostic enabled: hold each direction for >=2 seconds\n");
  _LastReported = _ReadState();
  _DiagnosticDivider = 0u;
  _DiagnosticSamplesLeft = 30u;
}

uint8_t JOYPORT_GetState(void) {
  uint8_t state = _ReadState();
  if (state != _LastReported) {
    printf("[JOY] state=0x%02X U/D/L/R/C=%u/%u/%u/%u/%u\n",
           (unsigned)state,
           (unsigned)((state & JOY_UP) != 0u),
           (unsigned)((state & JOY_DOWN) != 0u),
           (unsigned)((state & JOY_LEFT) != 0u),
           (unsigned)((state & JOY_RIGHT) != 0u),
           (unsigned)((state & JOY_CENTER) != 0u));
    _LastReported = state;
  }
  if (_DiagnosticSamplesLeft != 0u) _DiagnosticDivider++;
  if ((_DiagnosticSamplesLeft != 0u) &&
      (_DiagnosticDivider >= JOY_DIAGNOSTIC_INTERVAL_CALLS)) {
    _DiagnosticDivider = 0u;
    _DiagnosticSamplesLeft--;
    printf("[JOY SCAN] U/PG9=%u D/PG11=%u L/PG15=%u R/PG10=%u C/PA12=%u\n",
           (unsigned)PG9, (unsigned)PG11, (unsigned)PG15,
           (unsigned)PG10, (unsigned)PA12);
    if (_DiagnosticSamplesLeft == 0u) {
      printf("[JOY] periodic diagnostic ended; edge reporting remains active\n");
    }
  }
  return state;
}
