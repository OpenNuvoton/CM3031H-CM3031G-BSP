/**************************************************************************//**
 * @file     tft.h
 * @brief    TFT/ILI9341 驱动头文件 - SGL 在 M3331 上的显示层接口声明
 * @details  声明屏初始化与 SGL 刷屏回调，供 main.c 注册帧缓冲时使用。
 *           本板：SPI1 PE0/PE1/PH8，PH9=GPIO CS，DC=PH10，背光 EN=PH11，RST=PD14。
 *           PH9 作 CS 时请用 GPIO，不要用 SPI1_SS 复用（见 main.c 注释）。
 *           SPI：命令+参数连续 CS；RAMWR 与像素流在同一次 CS 内发送（见 tft.c）。
 ******************************************************************************/
#ifndef TFT_H
#define TFT_H

#include "sgl.h"

/**
 * 水平/垂直像素：须在包含本头文件前由 panel_config.h（或工程配置）定义。
 * disp_ili9341：HOR==240 → MADCTL 0x48（竖屏 240×320），否则 0xE8（横屏 320×240）。
 */
#ifndef TFT_LCD_HOR_RES_MAX
#define TFT_LCD_HOR_RES_MAX  240
#endif
#ifndef TFT_LCD_VER_RES_MAX
#define TFT_LCD_VER_RES_MAX  320
#endif

/**
 * @brief 初始化 TFT 屏（SPI 与 ILI9341 寄存器）
 * @note  在 sgl_fbdev_register 之后、sgl_init 之前调用
 */
void tft_init(void);

/**
 * @brief SGL 刷屏回调：将指定矩形区域的像素数据通过 SPI 写入 ILI9341
 * @param area 要刷新的矩形区域（x1,y1,x2,y2，闭区间）
 * @param src  该矩形对应的像素缓冲区（行优先，sgl_color_t 即 RGB565）
 * @note  实现中必须在写完像素后调用 sgl_fbdev_flush_ready()，否则 SGL 会阻塞
 */
void demo_panel_flush_area(sgl_area_t *area, sgl_color_t *src);

/**
 * @brief 整屏填充 RGB565（调试用，例如 0xF800 红、0x001F 蓝）
 * @note 若此函数能均匀铺满一色而 SGL 仍花屏，再查 SGL；若仍分条，查接线/MADCTL/驱动 IC。
 */
void tft_fill_screen_rgb565(uint16_t rgb565);

/** 毫秒延时（与 tft 内一致），供 main 调试填屏后观察画面 */
void tft_delay_ms(uint32_t ms);

#endif /* TFT_H */
