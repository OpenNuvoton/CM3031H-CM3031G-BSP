#include <stdio.h>
#include <stdint.h>
#include "NuMicro.h"
#include "GUI.h"
#include "Touch_Port.h"

/* NuFun 4-wire resistive panel: YD=PB2, XR=PB3, YU=PB4, XL=PB5. */
#define TP_YD 18u
#define TP_XR 19u
#define TP_YU 20u
#define TP_XL 21u
#define TP_PORT_OFFSET 0x40u
#define TP_PORT(pin) ((uint32_t)(pin) >> 4)
#define TP_PIN(pin)  ((uint32_t)(pin) & 0x0Fu)
#define TP_MASK(pin) (1ul << TP_PIN(pin))
#define TP_MFP_POS(pin) ((TP_PIN(pin) % 4u) * 8u)
#define TP_MFP_MASK(pin) (0x1ful << TP_MFP_POS(pin))

typedef struct {
  int32_t a, b, c, d, e, f, div;
} TP_CAL_MATRIX;

/* Nuvoton 320x240 NuFun reference calibration. */
static const TP_CAL_MATRIX _Cal = {
  -105, 6354, -3362552,
  5086, -24, -2489744,
  65536
};
static int _LastX;
static int _LastY;
static int _LastPressed;

static GPIO_T *_Port(uint32_t pin) {
  return (GPIO_T *)(GPIOA_BASE + TP_PORT(pin) * TP_PORT_OFFSET);
}

static void _SetMfp(uint32_t pin, uint32_t value) {
  uint32_t port_index = TP_PORT(pin);
  uint32_t pin_index = TP_PIN(pin);
  __IO uint32_t *reg = ((__IO uint32_t *)&SYS->GPA_MFP0) +
                       port_index * 4u + pin_index / 4u;
  *reg = (*reg & ~TP_MFP_MASK(pin)) | value;
}

static void _Digital(uint32_t pin) {
  _SetMfp(pin, 0u);
  GPIO_ENABLE_DIGITAL_PATH(_Port(pin), TP_MASK(pin));
}

static void _Analog(uint32_t pin) {
  _SetMfp(pin, 1u << TP_MFP_POS(pin));
  GPIO_DISABLE_DIGITAL_PATH(_Port(pin), TP_MASK(pin));
}

static uint32_t _Sample(uint32_t channel) {
  EADC_ConfigSampleModule(EADC0, 0, EADC_SOFTWARE_TRIGGER, channel);
  EADC_CLR_INT_FLAG(EADC0, EADC_STATUS2_ADIF0_Msk);
  EADC_ENABLE_INT(EADC0, BIT0);
  EADC_ENABLE_SAMPLE_MODULE_INT(EADC0, 0, BIT0);
  EADC_START_CONV(EADC0, BIT0);
  while (EADC_GET_INT_FLAG(EADC0, BIT0) == 0u) { }
  return EADC_GET_CONV_DATA(EADC0, 0) & 0x0FFFu;
}

static uint32_t _ReadRawX(void) {
  GPIO_SetMode(_Port(TP_XR), TP_MASK(TP_XR), GPIO_MODE_OUTPUT);
  GPIO_SetMode(_Port(TP_YD), TP_MASK(TP_YD), GPIO_MODE_INPUT);
  GPIO_SetMode(_Port(TP_XL), TP_MASK(TP_XL), GPIO_MODE_OUTPUT);
  _Digital(TP_XR); _Digital(TP_YD); _Digital(TP_XL);
  GPIO_PIN_DATA_S(TP_PORT(TP_XR), TP_PIN(TP_XR)) = 1;
  GPIO_PIN_DATA_S(TP_PORT(TP_XL), TP_PIN(TP_XL)) = 0;
  _Analog(TP_YU);
  return _Sample(TP_PIN(TP_YU));
}

static uint32_t _ReadRawY(void) {
  GPIO_SetMode(_Port(TP_YU), TP_MASK(TP_YU), GPIO_MODE_OUTPUT);
  GPIO_SetMode(_Port(TP_YD), TP_MASK(TP_YD), GPIO_MODE_OUTPUT);
  GPIO_SetMode(_Port(TP_XL), TP_MASK(TP_XL), GPIO_MODE_INPUT);
  _Digital(TP_YU); _Digital(TP_YD); _Digital(TP_XL);
  GPIO_PIN_DATA_S(TP_PORT(TP_YU), TP_PIN(TP_YU)) = 1;
  GPIO_PIN_DATA_S(TP_PORT(TP_YD), TP_PIN(TP_YD)) = 0;
  _Analog(TP_XR);
  return _Sample(TP_PIN(TP_XR));
}

int TOUCHPORT_Init(void) {
  CLK_EnableModuleClock(GPB_MODULE);
  CLK_EnableModuleClock(EADC0_MODULE);
  CLK_SetModuleClock(EADC0_MODULE,
                     CLK_CLKSEL0_EADC0SEL_PLL_DIV2,
                     CLK_CLKDIV0_EADC0(8));
  SET_EADC0_CH2_PB2();
  SET_EADC0_CH3_PB3();
  SET_EADC0_CH4_PB4();
  SET_EADC0_CH5_PB5();
  GPIO_DISABLE_DIGITAL_PATH(PB, BIT2 | BIT3 | BIT4 | BIT5);
  SYS_SetVRef(SYS_VREFCTL_VREF_PIN);
  if (EADC_Open(EADC0, EADC_CTL_DIFFEN_SINGLE_END) != 0) {
    printf("[TOUCH ERROR] EADC0 open failed\n");
    return 0;
  }
  printf("[TOUCH] NuFun resistive ADC PB2..PB5 initialized\n");
  return 1;
}

int TOUCHPORT_Exec(int *x, int *y) {
  GUI_PID_STATE state;
  uint32_t raw_x = _ReadRawX();
  uint32_t raw_y = _ReadRawY();
  int pressed = (raw_x < 3900u) && (raw_y < 3900u);

  if (pressed) {
    int32_t mapped_x = (_Cal.c + _Cal.a * (int32_t)raw_x +
                        _Cal.b * (int32_t)raw_y) / _Cal.div;
    int32_t mapped_y = (_Cal.f + _Cal.d * (int32_t)raw_x +
                        _Cal.e * (int32_t)raw_y) / _Cal.div;
    if (mapped_x < 0) mapped_x = 0;
    if (mapped_x > 319) mapped_x = 319;
    if (mapped_y < 0) mapped_y = 0;
    if (mapped_y > 239) mapped_y = 239;
    *x = (int)mapped_x;
    *y = (int)mapped_y;
  } else {
    *x = _LastX;
    *y = _LastY;
  }

  if ((pressed != _LastPressed) ||
      (pressed && ((*x != _LastX) || (*y != _LastY)))) {
    state.x = *x;
    state.y = *y;
    state.Pressed = (U8)pressed;
    state.Layer = 0;
    GUI_PID_StoreState(&state);
    if (pressed != _LastPressed) {
      printf("[TOUCH] %s raw=%u,%u gui=%d,%d\n",
             pressed ? "DOWN" : "UP",
             (unsigned)raw_x, (unsigned)raw_y, *x, *y);
    }
  }
  _LastX = *x;
  _LastY = *y;
  _LastPressed = pressed;
  return pressed;
}
