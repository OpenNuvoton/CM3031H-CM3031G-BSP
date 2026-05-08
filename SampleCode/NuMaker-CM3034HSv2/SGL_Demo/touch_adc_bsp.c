/**************************************************************************//**
 * @file     touch_adc_bsp.c
 * @brief    四线电阻触摸（ADC）BSP 适配层
 *
 * @details
 * 本文件实现了：
 * 1) 四线电阻屏 X/Y 分时采样（通过 GPIO 驱动 + EADC 采样）
 * 2) 原始 ADC 坐标到屏幕坐标的校准矩阵映射
 * 3) 统一上层读取接口 bsp_touch_read()
 *
 * 使用约束：
 * - 需先调用 adc_res_touch_hw_init() 完成 EADC 与引脚初始化
 * - 建议在 SYS_UnlockReg 期间配置（main.c 已在 SYS_LockReg 前调用）
 *
 * SPDX-License-Identifier: Apache-2.0
 * @copyright (C) 2020 Nuvoton Technology Corp. All rights reserved.
 ******************************************************************************/

#include "touch_adc_bsp.h"
#include "panel_config.h"

#if defined(CONFIG_NG_MFP)

#define NU_MFP_POS(PIN)   ((PIN % 4) * 8)
#define NU_MFP_MSK(PIN)   (0x1ful << NU_MFP_POS(PIN))

/*
 * 设置引脚多功能（MFP）：
 * - data=0：回到 GPIO 数字功能
 * - data=1<<slot：切到模拟输入（对应 ADC 通道）
 *
 * 说明：不同芯片族 MFP 寄存器布局不同，故分 CONFIG_NG_MFP 与旧布局两套。
 */
static void nu_pin_func(uint32_t pin, int data)
{
    uint32_t GPx_MFPx_org;
    uint32_t pin_index      = NU_GET_PIN(pin);
    uint32_t port_index     = NU_GET_PORT(pin);
    __IO uint32_t *GPx_MFPx = ((__IO uint32_t *)&SYS->GPA_MFP0 + (port_index * 4) + (pin_index / 4));
    uint32_t MFP_Msk        = NU_MFP_MSK(pin_index);

    GPx_MFPx_org = *GPx_MFPx;
    *GPx_MFPx    = (GPx_MFPx_org & (~MFP_Msk)) | (uint32_t)data;
}

#else

#define NU_MFP_POS(PIN)                ((PIN % 8) * 4)
#define NU_MFP_MSK(PIN)                (0xful << NU_MFP_POS(PIN))

/* 旧版 MFP 布局（GPA_MFPL/MFPH） */
static void nu_pin_func(uint32_t pin, int data)
{
    uint32_t pin_index      = NU_GET_PIN(pin);
    uint32_t port_index     = NU_GET_PORT(pin);
    __IO uint32_t *GPx_MFPx = ((__IO uint32_t *)&SYS->GPA_MFPL + (port_index * 2) + (pin_index / 8));
    uint32_t MFP_Msk        = NU_MFP_MSK(pin_index);

    *GPx_MFPx  = (*GPx_MFPx & (~MFP_Msk)) | (uint32_t)data;
}

#endif

static void tp_switch_to_analog(uint32_t pin)
{
    GPIO_T *port = (GPIO_T *)(GPIOA_BASE + PORT_OFFSET * NU_GET_PORT(pin));

    /*
     * 切到模拟输入并关闭数字路径：
     * - 避免数字输入缓冲引入漏电流/串扰，提升 ADC 采样稳定性
     */
    nu_pin_func(pin, (int)(1u << NU_MFP_POS(NU_GET_PIN(pin))));

    GPIO_DISABLE_DIGITAL_PATH(port, NU_GET_PIN_MASK(NU_GET_PIN(pin)));
}

static void tp_switch_to_digital(uint32_t pin)
{
    GPIO_T *port = (GPIO_T *)(GPIOA_BASE + PORT_OFFSET * NU_GET_PORT(pin));

    /* 回到 GPIO 数字功能（用于驱动上拉/下拉与输入检测） */
    nu_pin_func(pin, 0);

    GPIO_ENABLE_DIGITAL_PATH(port, NU_GET_PIN_MASK(NU_GET_PIN(pin)));
}

/*
 * 读取“X 方向”坐标（四线电阻屏经典接法）：
 * - XR 输出高，XL 输出低，在 X 轴形成电压梯度
 * - YU 作为 ADC 输入采样梯度电压 => 对应 X 位置
 * - YD 置输入，避免形成额外驱动
 */
uint32_t indev_touch_get_x(void)
{
    GPIO_T *PORT;

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_XR) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_XR)), GPIO_MODE_OUTPUT);

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_YD) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_YD)), GPIO_MODE_INPUT);

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_XL) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_XL)), GPIO_MODE_OUTPUT);

    GPIO_PIN_DATA(NU_GET_PORT(CONFIG_AD_PIN_XR), NU_GET_PIN(CONFIG_AD_PIN_XR)) = 1;
    GPIO_PIN_DATA(NU_GET_PORT(CONFIG_AD_PIN_XL), NU_GET_PIN(CONFIG_AD_PIN_XL)) = 0;

    tp_switch_to_digital(CONFIG_AD_PIN_XR);
    tp_switch_to_digital(CONFIG_AD_PIN_YD);
    tp_switch_to_digital(CONFIG_AD_PIN_XL);

    tp_switch_to_analog(CONFIG_AD_PIN_YU);

    /* 在 YU 通道采样，得到原始 X ADC 值（0~4095） */
    return nu_adc_sampling(NU_GET_PIN(CONFIG_AD_PIN_YU));
}

/*
 * 读取“Y 方向”坐标：
 * - YU 输出高，YD 输出低，在 Y 轴形成电压梯度
 * - XR 作为 ADC 输入采样梯度电压 => 对应 Y 位置
 * - XL 置输入，避免形成额外驱动
 */
uint32_t indev_touch_get_y(void)
{
    GPIO_T *PORT;

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_YU) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_YU)), GPIO_MODE_OUTPUT);

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_YD) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_YD)), GPIO_MODE_OUTPUT);

    PORT    = (GPIO_T *)(GPIOA_BASE + (NU_GET_PORT(CONFIG_AD_PIN_XL) * PORT_OFFSET));
    GPIO_SetMode(PORT, NU_GET_PIN_MASK(NU_GET_PIN(CONFIG_AD_PIN_XL)), GPIO_MODE_INPUT);

    GPIO_PIN_DATA(NU_GET_PORT(CONFIG_AD_PIN_YU), NU_GET_PIN(CONFIG_AD_PIN_YU)) = 1;
    GPIO_PIN_DATA(NU_GET_PORT(CONFIG_AD_PIN_YD), NU_GET_PIN(CONFIG_AD_PIN_YD)) = 0;

    tp_switch_to_digital(CONFIG_AD_PIN_YU);
    tp_switch_to_digital(CONFIG_AD_PIN_YD);
    tp_switch_to_digital(CONFIG_AD_PIN_XL);

    tp_switch_to_analog(CONFIG_AD_PIN_XR);

    /* 在 XR 通道采样，得到原始 Y ADC 值（0~4095） */
    return nu_adc_sampling(NU_GET_PIN(CONFIG_AD_PIN_XR));
}

/*
 * 单次 ADC 采样（轮询式）：
 * - 配置 Sample Module 0 到目标通道
 * - 软件触发转换
 * - 轮询 ADINT0 完成标志
 * - 返回 12-bit 结果
 */
uint32_t nu_adc_sampling(uint32_t channel)
{
    EADC_ConfigSampleModule(CONFIG_AD_RES_TOUCH, 0, EADC_SOFTWARE_TRIGGER, channel);

    EADC_CLR_INT_FLAG(CONFIG_AD_RES_TOUCH, EADC_STATUS2_ADIF0_Msk);

    EADC_ENABLE_INT(CONFIG_AD_RES_TOUCH, BIT0);
    EADC_ENABLE_SAMPLE_MODULE_INT(CONFIG_AD_RES_TOUCH, 0, BIT0);

    EADC_START_CONV(CONFIG_AD_RES_TOUCH, BIT0);

    while (EADC_GET_INT_FLAG(CONFIG_AD_RES_TOUCH, BIT0) == 0) {
        ;
    }

    return EADC_GET_CONV_DATA(CONFIG_AD_RES_TOUCH, 0) & 0x0FFFu;
}

/*
 * 默认校准矩阵（仿射变换）：
 *   x' = (c + a*x + b*y) / div
 *   y' = (f + d*x + e*y) / div
 *
 * 当前矩阵来源：numaker-hmi-m3334（__320×240__）默认值；竖屏 240×320 时见
 * panel_config.h 中 PANEL_TOUCH_REMAP_CAL_TO_240X320（默认 1）做输出重映射。
 * 若已用五点校准得到本屏专用矩阵，可将该宏改为 0。
 */
S_CALIBRATION_MATRIX g_sCalMat = {
    -105, 6354, -3362552,
    5086, -24, -2489744,
    65536
};

int ad_touch_map(int32_t *sumx, int32_t *sumy)
{
    int32_t xtemp;
    int32_t ytemp;

    /*
     * 历史兼容约定：
     * - div==1 作为“无有效校准矩阵”标记，返回失败
     * - 其它值执行仿射映射
     */
    if (g_sCalMat.div == 1) {
        return -1;
    }

    xtemp = *sumx; 
    ytemp = *sumy;
    *sumx = (g_sCalMat.c +
             g_sCalMat.a * xtemp +
             g_sCalMat.b * ytemp) / g_sCalMat.div;
    *sumy = (g_sCalMat.f +
             g_sCalMat.d * xtemp +
             g_sCalMat.e * ytemp) / g_sCalMat.div;

    return 0;
}

void adc_res_touch_hw_init(void)
{
    int32_t err;

    /* 1) 使能 EADC0 时钟并设置分频（与参考工程一致） */
    CLK_EnableModuleClock(EADC0_MODULE);
    CLK_SetModuleClock(EADC0_MODULE, CLK_CLKSEL0_EADC0SEL_PLL_DIV2, CLK_CLKDIV0_EADC0(8));

    /* 2) 绑定 PB2~PB5 为 EADC 通道（YD/XR/YU/XL，与 touch_adc_bsp.h 一致） */
    SET_EADC0_CH2_PB2();
    SET_EADC0_CH3_PB3();
    SET_EADC0_CH4_PB4();
    SET_EADC0_CH5_PB5();

    /* 3) 关闭对应数字输入路径，降低模拟采样干扰 */
    GPIO_DISABLE_DIGITAL_PATH(PB, BIT2 | BIT3 | BIT4 | BIT5);

    /* 4) Vref 使用芯片内部基准（与 lv_port 工程配置一致） */
    SYS_SetVRef(SYS_VREFCTL_VREF_PIN);

    /* 5) 打开 EADC0（单端模式） */
    err = EADC_Open(EADC0, EADC_CTL_DIFFEN_SINGLE_END);
    /* 当前示例忽略错误码；若需更严格可在此处打印/断言 */
    (void)err;
}

/*
 * 统一触摸读取接口（供 main.c 轮询调用）：
 * 输出：
 * - *pressed: 是否按下
 * - *x, *y  : 屏幕坐标（已映射、钳位）
 *
 * 判定逻辑：
 * - 先读两路原始 ADC（X/Y）
 * - 两路都低于阈值（3900）视为按下
 * - 按下时优先用校准矩阵映射；映射失败则退化为线性缩放
 */
#if PANEL_TOUCH_REMAP_CAL_TO_240X320
/* 将「320×240 风格」矩阵输出对齐到竖屏 240×320 的 SGL 坐标 */
static void touch_remap_cal_output_to_portrait(int32_t *mx, int32_t *my)
{
    int32_t sx = *my;
    int32_t sy = (int32_t)PANEL_HEIGHT - 1 - *mx;
    *mx = sx;
    *my = sy;
}
#endif

int bsp_touch_read(int16_t *x, int16_t *y, bool *pressed)
{
    uint32_t adc_x;
    uint32_t adc_y;
    int32_t  mx;
    int32_t  my;

    adc_x = indev_touch_get_x();
    adc_y = indev_touch_get_y();

    /*
     * 与 lv_glue.c touchpad_device_read 一致：
     * 两路 ADC 均低于约 95% 满量程（3900/4095）视为按下。
     * 直观上：未触摸时电压常接近满量程，触摸后分压落入中低范围。
     */
    if ((adc_x < 3900u) && (adc_y < 3900u)) {
        mx = (int32_t)adc_x;
        my = (int32_t)adc_y;

        /* 优先走校准矩阵（精度更好） */
        if (ad_touch_map(&mx, &my) == 0) {
#if PANEL_TOUCH_REMAP_CAL_TO_240X320
            touch_remap_cal_output_to_portrait(&mx, &my);
#endif
            /* 坐标钳位到显示范围 */
            if (mx < 0) {
                mx = 0;
            }
            if (mx >= (int32_t)PANEL_WIDTH) {
                mx = (int32_t)PANEL_WIDTH - 1;
            }
            if (my < 0) {
                my = 0;
            }
            if (my >= (int32_t)PANEL_HEIGHT) {
                my = (int32_t)PANEL_HEIGHT - 1;
            }
            *x = (int16_t)mx;
            *y = (int16_t)my;
        } else {
            /*
             * 无有效校准矩阵时的后备路径：
             * 直接按 12-bit ADC 比例线性映射到屏幕分辨率。
             * 精度一般，但可保证“有触摸反馈”。
             */
            *x = (int16_t)((adc_x * (uint32_t)PANEL_WIDTH) / 4096u);
            if (*x >= PANEL_WIDTH) {
                *x = (int16_t)(PANEL_WIDTH - 1);
            }
            *y = (int16_t)((adc_y * (uint32_t)PANEL_HEIGHT) / 4096u);
            if (*y >= PANEL_HEIGHT) {
                *y = (int16_t)(PANEL_HEIGHT - 1);
            }
        }
        *pressed = true;
    } else {
        /* 未按下：仅更新状态，不强制改写 x/y（上层可按需忽略） */
        *pressed = false;
    }

    /* 返回 0 表示本次读取流程正常执行 */
    return 0;
}
