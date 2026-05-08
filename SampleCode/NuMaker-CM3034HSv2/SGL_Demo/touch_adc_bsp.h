/**************************************************************************//**
 * @file     touch_adc_bsp.h
 * @brief    电阻式 ADC 触摸：与 numaker-hmi-m3334（lv_port/lv_glue.h，NUFUN!=1）引脚与算法对齐
 *
 * @details  本板：YD=PB2、XR=PB3、YU=PB4、XL=PB5（EADC0_CH2~CH5）；校准矩阵 g_sCalMat 可按屏实测替换。
 ******************************************************************************/
#ifndef TOUCH_ADC_BSP_H
#define TOUCH_ADC_BSP_H

#include <stdint.h>
#include <stdbool.h>
#include "NuMicro.h"

#ifndef GPIO_PIN_DATA
#define GPIO_PIN_DATA   GPIO_PIN_DATA_S
#endif

#if !defined(PORT_OFFSET)
#define PORT_OFFSET     0x40u
#endif

/* 与 lv_port 一致：使用 GPA_MFP0 风格（CONFIG_NG_MFP） */
#define CONFIG_NG_MFP   1

#define CONFIG_AD_RES_TOUCH     EADC0

/* NU_GET_PININDEX: port B=1 => 16 + pin */
#define CONFIG_AD_PIN_XL        21u     /* PB5 */
#define CONFIG_AD_PIN_YU        20u     /* PB4 */
#define CONFIG_AD_PIN_XR        19u     /* PB3 */
#define CONFIG_AD_PIN_YD        18u     /* PB2 */

#define NU_GET_PORT(pinidx)     ((uint32_t)((pinidx) >> 4))
#define NU_GET_PIN(pinidx)      ((uint32_t)((pinidx) & 0xFu))
#define NU_GET_PIN_MASK(pin)    (1ul << (pin))

typedef struct {
    int32_t a;
    int32_t b;
    int32_t c;
    int32_t d;
    int32_t e;
    int32_t f;
    int32_t div;
} S_CALIBRATION_MATRIX;

extern S_CALIBRATION_MATRIX g_sCalMat;

void adc_res_touch_hw_init(void);

uint32_t nu_adc_sampling(uint32_t channel);
uint32_t indev_touch_get_x(void);
uint32_t indev_touch_get_y(void);

int ad_touch_map(int32_t *sumx, int32_t *sumy);

int bsp_touch_read(int16_t *x, int16_t *y, bool *pressed);

#endif /* TOUCH_ADC_BSP_H */
