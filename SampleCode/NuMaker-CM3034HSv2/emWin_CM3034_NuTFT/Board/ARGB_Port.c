#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"
#include "ARGB_Port.h"

#define ARGB_LED_COUNT       8u
#define ARGB_BYTES_PER_LED   3u
#define ARGB_FRAME_BYTES     (ARGB_LED_COUNT * ARGB_BYTES_PER_LED)
#define ARGB_FRAME_WORDS     (ARGB_FRAME_BYTES / 4u)
#define ARGB_STEP_MS         100u
#define ARGB_FIFO_TIMEOUT    100000u

/* Low brightness: the LEDs are used only as a short boot accent. */
static const uint32_t _Palette[ARGB_LED_COUNT] = {
  0x200000u, 0x201000u, 0x202000u, 0x002000u,
  0x002020u, 0x000020u, 0x100020u, 0x200010u
};

static volatile uint32_t _FrameWords[ARGB_FRAME_WORDS];
static uint8_t _Phase;
static uint32_t _NextStep;

static void _BuildFrame(uint8_t phase) {
  uint8_t bytes[ARGB_FRAME_BYTES];
  uint32_t led;
  uint32_t word;

  for (led = 0u; led < ARGB_LED_COUNT; led++) {
    uint32_t rgb = _Palette[(led + phase) & 7u];
    bytes[led * 3u + 0u] = (uint8_t)(rgb >> 16); /* R */
    bytes[led * 3u + 1u] = (uint8_t)(rgb >> 8);  /* G */
    bytes[led * 3u + 2u] = (uint8_t)rgb;         /* B */
  }
  for (word = 0u; word < ARGB_FRAME_WORDS; word++) {
    uint32_t offset = word * 4u;
    _FrameWords[word] =
      ((uint32_t)bytes[offset + 0u]      ) |
      ((uint32_t)bytes[offset + 1u] <<  8) |
      ((uint32_t)bytes[offset + 2u] << 16) |
      ((uint32_t)bytes[offset + 3u] << 24);
  }
}

static int _SendFrame(void) {
  uint32_t word;

  /* Six words do not justify an interrupt-driven state machine.  Polling is
     bounded and takes less than one millisecond for an eight-pixel frame. */
  for (word = 0u; word < ARGB_FRAME_WORDS; word++) {
    uint32_t timeout = ARGB_FIFO_TIMEOUT;
    while (LLSI_GET_TX_FIFO_FULL_FLAG(LLSI0)) {
      if (--timeout == 0u) {
        return 0;
      }
    }
    if (word == (ARGB_FRAME_WORDS - 1u)) {
      LLSI_SET_LAST_DATA(LLSI0);
    }
    LLSI_WRITE_DATA(LLSI0, _FrameWords[word]);
  }
  return 1;
}

void ARGBPORT_Init(void) {
  CLK_EnableModuleClock(LLSI0_MODULE);
  CLK_EnableModuleClock(GPB_MODULE);
  SET_LLSI0_OUT_PB15();

  LLSI_Open(LLSI0, LLSI_MODE_SW, LLSI_FORMAT_GRB,
            SystemCoreClock, 1250u, 400u, 850u, 50000u,
            ARGB_LED_COUNT, LLSI_IDLE_LOW);
  LLSI_SetFIFO(LLSI0, 2u);
  LLSI_ENABLE_RESET_COMMAND(LLSI0);

  /* LLSI0 interrupt number 128 caused INVSTATE on this board/startup image.
     Keep the peripheral interrupt disabled and use bounded polling instead. */
  LLSI_DisableInt(LLSI0, LLSI_TXTH_INT_MASK | LLSI_FEND_INT_MASK |
                         LLSI_RSTC_INT_MASK | LLSI_UNDFL_INT_MASK);
  NVIC_DisableIRQ(LLSI0_IRQn);
  NVIC_ClearPendingIRQ(LLSI0_IRQn);

  _Phase = 0u;
  _NextStep = 0u;
  printf("[ARGB] LLSI0 PB15, 8 LEDs, GRB polling mode enabled\n");
}

void ARGBPORT_Exec(uint32_t now_ms) {
  static uint8_t timeout_reported;

  if ((int32_t)(now_ms - _NextStep) < 0) return;

  _BuildFrame(_Phase);
  if (_SendFrame()) {
    _Phase = (uint8_t)((_Phase + 1u) & 7u);
    timeout_reported = 0u;
  } else if (!timeout_reported) {
    printf("[ARGB ERROR] TX FIFO timeout, chase paused\n");
    timeout_reported = 1u;
  }
  _NextStep = now_ms + ARGB_STEP_MS;
}

void ARGBPORT_Off(void) {
  uint32_t word;

  for (word = 0u; word < ARGB_FRAME_WORDS; word++) {
    _FrameWords[word] = 0u;
  }
  (void)_SendFrame();
  printf("[ARGB] boot accent stopped, LEDs off\n");
}
