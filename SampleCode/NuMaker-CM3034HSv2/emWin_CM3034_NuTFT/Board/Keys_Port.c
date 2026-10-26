#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"
#include "Keys_Port.h"

/*
 * NuFun lower-left JP4 mates with CM3034HS8 lower-left JP5.  The male/female
 * connector reverses the odd/even pin within each row.
 * NuFun silk -> NuFun net/pin -> CM3034 JP5 pin -> MCU pin -> UI action
 * SW2          SW1 / 2        1                  PH4        1 (CHIP)
 * SW3          SW2 / 1        2                  PH5        2 (FLASH)
 * SW4          SW3 / 4        3                  PH6        3 (SRAM)
 * SW5          SW4 / 3        4                  PH7        4 (WELCOME)
 *
 * All four switches have external 4.7 kOhm pull-ups and are active low.
 * PH4..PH7 are independent GPIOs, so the debugger can remain connected.
 */
#ifndef KEYPORT_PERIODIC_DIAGNOSTICS
#define KEYPORT_PERIODIC_DIAGNOSTICS 0
#endif

#define KEY_RELEASED_MASK 0x0Fu
#define KEY_DEBOUNCE_SAMPLES 3u

static uint8_t _LastRaw = KEY_RELEASED_MASK;
static uint8_t _Stable = KEY_RELEASED_MASK;
static uint8_t _DebounceCount;
static uint16_t _DiagnosticDivider;

static uint8_t _ReadRaw(void) {
  uint8_t raw = 0u;

  if (PH4) raw |= 0x01u; /* physical SW2 */
  if (PH5) raw |= 0x02u; /* physical SW3 */
  if (PH6) raw |= 0x04u; /* physical SW4 */
  if (PH7) raw |= 0x08u; /* physical SW5 */
  return raw;
}

static void _PrintLevels(const char *event) {
  printf("[KEY] %s SW2/PH4=%u SW3/PH5=%u SW4/PH6=%u SW5/PH7=%u\n",
         event,
         (unsigned)PH4, (unsigned)PH5,
         (unsigned)PH6, (unsigned)PH7);
}

void KEYPORT_Init(void) {
  CLK_EnableModuleClock(GPH_MODULE);

  SET_GPIO_PH4();
  SET_GPIO_PH5();
  SET_GPIO_PH6();
  SET_GPIO_PH7();
  GPIO_SetMode(PH, BIT4 | BIT5 | BIT6 | BIT7, GPIO_MODE_INPUT);
  GPIO_SetPullCtl(PH, BIT4 | BIT5 | BIT6 | BIT7, GPIO_PUSEL_PULL_UP);
  GPIO_ENABLE_DIGITAL_PATH(PH, BIT4 | BIT5 | BIT6 | BIT7);
  GPIO_ENABLE_SCHMITT_TRIGGER(PH, BIT4 | BIT5 | BIT6 | BIT7);

  _LastRaw = _ReadRaw();
  _Stable = _LastRaw;
  _DebounceCount = 0u;
  _DiagnosticDivider = 0u;
  printf("[KEY] map SW2/SW3/SW4/SW5 -> actions 1/2/3/4\n");
  _PrintLevels("initial");
  printf("[KEY] MFP PH4..PH7=0x%08X, DINOFF PH=0x%08X; SWD remains enabled\n",
         (unsigned)SYS->GPH_MFP1, (unsigned)PH->DINOFF);
}

int KEYPORT_Exec(void) {
  uint8_t raw = _ReadRaw();

#if KEYPORT_PERIODIC_DIAGNOSTICS
  _DiagnosticDivider++;
  if (_DiagnosticDivider >= 200u) {
    _DiagnosticDivider = 0u;
    _PrintLevels("SCAN");
  }
#endif

  if (raw != _LastRaw) {
    _LastRaw = raw;
    _DebounceCount = 0u;
    _PrintLevels("edge");
    return 0;
  }
  if (_DebounceCount < KEY_DEBOUNCE_SAMPLES) {
    _DebounceCount++;
    return 0;
  }
  if (raw != _Stable) {
    uint8_t falling = (uint8_t)(_Stable & (uint8_t)~raw);
    _Stable = raw;

    if (falling & 0x01u) {
      printf("[KEY] SW2 -> action 1 (CHIP)\n");
      return 1;
    }
    if (falling & 0x02u) {
      printf("[KEY] SW3 -> action 2 (FLASH)\n");
      return 2;
    }
    if (falling & 0x04u) {
      printf("[KEY] SW4 -> action 3 (SRAM)\n");
      return 3;
    }
    if (falling & 0x08u) {
      printf("[KEY] SW5 -> action 4 (WELCOME)\n");
      return 4;
    }
  }
  return 0;
}
