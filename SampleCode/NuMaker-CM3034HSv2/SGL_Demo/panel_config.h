/**************************************************************************//**
 * @file     panel_config.h
 * @brief    屏分辨率（须与 ILI9341 方向一致）
 *
 * @details  main.c 与 tft.c 都必须在包含 tft.h 之前包含本文件。
 *           若 tft.c 未定义 TFT_LCD_HOR_RES_MAX，tft.h 会默认 240，ILI9341 仍按竖屏
 *           配 MADCTL(0x48)，而 SGL 按 320×240 写显存，会出现约 240 列正常、右侧花屏。
 ******************************************************************************/
#ifndef PANEL_CONFIG_H
#define PANEL_CONFIG_H

/* 竖屏 240×320：水平 240、垂直 320
 *
 * 与 NuMaker-M3334KI LVGL_WidgetsDemo 的差异（同 ILI9341、不同方向宏）：
 * - 该工程 Keil 常定义 __320x240__ → LV_HOR_RES_MAX=320、VER=240，disp_ili9341.c 里 MADCTL=0xE8；
 * - 本 BSP 按物理竖屏使用 240×320，tft.c 中 MADCTL=0x48（见 TFT_LCD_HOR_RES_MAX==240 分支）。
 * 若把本工程改成 320×240 横屏，须同步 panel_config、tft MADCTL 与 SGL 坐标，勿混用。
 */
#define PANEL_WIDTH            240
#define PANEL_HEIGHT           320

#define TFT_LCD_HOR_RES_MAX    PANEL_WIDTH
#define TFT_LCD_VER_RES_MAX    PANEL_HEIGHT

/*
 * 触摸：`touch_adc_bsp.c` 默认 g_sCalMat 来自 numaker 的 320×240 参考值时，
 * 仿射输出轴向与本机竖屏 240×320 不一致（表现为点右下角却得到 x≈20、y≈150）。
 * 置 1：映射成功后做 sx=原y、sy=(H-1)-原x，与实机右下角命中一致。
 * 若你改用五点校准且矩阵已针对本屏，可改为 0。
 */
#ifndef PANEL_TOUCH_REMAP_CAL_TO_240X320
#define PANEL_TOUCH_REMAP_CAL_TO_240X320  1
#endif

#endif /* PANEL_CONFIG_H */
