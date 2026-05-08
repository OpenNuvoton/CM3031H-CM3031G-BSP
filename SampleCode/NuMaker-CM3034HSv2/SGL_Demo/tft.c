/**************************************************************************//**
 * @file     tft.c
 * @brief    ILI9341 TFT 驱动 - 初始化与 SGL 帧缓冲刷屏回调
 * @details  SPI1：PE0 MOSI、PE1 MISO、PH8 CLK；PH9 GPIO 片选；DC=PH10, EN=PH11, RST=PD14。
 *
 *           本工程 SPI 位宽仍为 8bit 逐字节；参考用 16bit 写坐标/显存，与 ILI9341 大端 16 位参数等价。
 ******************************************************************************/
#include <stddef.h>
#include "NuMicro.h"
#include "panel_config.h"
#include "tft.h"
#include "sgl.h"

/*----------------------------------------------------------------------------*/
/* ILI9341 寄存器地址（与 disp_ili9341.c / 数据手册一致）                      */
/*----------------------------------------------------------------------------*/
#define ILI9341_SLPIN        0x10   /* 进入睡眠 */
#define ILI9341_SLPOUT       0x11   /* 退出睡眠 */
#define ILI9341_DISPON       0x29   /* 显示开 */
#define ILI9341_CASET        0x2A   /* 列地址集（x 起止） */
#define ILI9341_PASET        0x2B   /* 页地址集（y 起止） */
#define ILI9341_RAMWR        0x2C   /* 写显存 */
#define ILI9341_MADCTL       0x36   /* 内存访问控制（旋转/镜像等） */
#define ILI9341_PIXFMT       0x3A   /* 像素格式 */
#define ILI9341_FRMCTR1      0xB1   /* 帧率控制 */
#define ILI9341_DISCTRL      0xB6   /* 显示控制 */
#define ILI9341_PWCTR1       0xC0   /* 电源控制 1 */
#define ILI9341_PWCTR2       0xC1   /* 电源控制 2 */
#define ILI9341_VMCTR1       0xC5   /* VCOM 控制 1 */
#define ILI9341_VMCTR2       0xC7   /* VCOM 控制 2 */
#define ILI9341_PWCTRLA      0xCB   /* 电源控制 A */
#define ILI9341_PWCTRLB      0xCF   /* 电源控制 B */
#define ILI9341_PGAMCTRL     0xE0   /* 正极性伽马 */
#define ILI9341_NGAMCTRL     0xE1   /* 负极性伽马 */
#define ILI9341_DTCTRLA      0xE8   /* 驱动时序 A */
#define ILI9341_DTCTRLB      0xEA   /* 驱动时序 B */
#define ILI9341_PWRSEQ       0xED   /* 上电序列 */
#define ILI9341_EN3GAM       0xF2   /* 3 伽马使能 */
#define ILI9341_PUMPCTRL     0xF7   /* 泵控制 */
#define ILI9341_GAMMASET     0x26   /* 伽马集 */

#define SPI_PORT             SPI1  /* PE0 MOSI, PE1 MISO, PH8 CLK；PH9=GPIO CS */

/* ---------------- SPI1 PDMA (pixel push) ----------------
 * 用 PDMA 把 RGB565 像素流推到 SPI1->TX，可显著减少“可见刷屏”时间。
 * 命令/坐标仍用 8-bit CPU 写；像素阶段切到 16-bit + PDMA_WIDTH_16。
 */
#define TFT_SPI1_PDMA_CH_TX   0

static void tft_spi1_pdma_init_once(void)
{
    static uint8_t inited = 0;
    if (inited) return;
    inited = 1;

    CLK_EnableModuleClock(PDMA0_MODULE);
    /* 不要 SYS_ResetModule(PDMA0_RST)：其它外设若共用 PDMA0 通道，全模块 reset 会打断/清掉对方 */
    PDMA_Open(PDMA0, 1u << TFT_SPI1_PDMA_CH_TX);
}

static int tft_spi1_write_pixels_pdma16(const sgl_color_t *src, uint32_t len)
{
    /* len 是像素个数（16-bit RGB565）。 */
    tft_spi1_pdma_init_once();

    /* 等 SPI 空闲后再切换数据宽度 */
    while (SPI_IS_BUSY(SPI_PORT));

    /* 清 TX/RX FIFO，避免残留数据影响 PDMA 请求 */
    SPI_ClearRxFIFO(SPI_PORT);
    SPI_ClearTxFIFO(SPI_PORT);

    /* 像素阶段改用 16-bit 发送：MSB first 时可直接送 src[i].full */
    SPI_SET_DATA_WIDTH(SPI_PORT, 16);

    PDMA_SetTransferCnt(PDMA0, TFT_SPI1_PDMA_CH_TX, PDMA_WIDTH_16, len);
    PDMA_SetTransferAddr(PDMA0, TFT_SPI1_PDMA_CH_TX,
                         (uint32_t)src, PDMA_SAR_INC,
                         (uint32_t)&SPI_PORT->TX, PDMA_DAR_FIX);
    PDMA_SetTransferMode(PDMA0, TFT_SPI1_PDMA_CH_TX, PDMA_SPI1_TX, FALSE, 0);
    PDMA_SetBurstType(PDMA0, TFT_SPI1_PDMA_CH_TX, PDMA_REQ_SINGLE, 0);
    PDMA0->DSCT[TFT_SPI1_PDMA_CH_TX].CTL |= PDMA_DSCT_CTL_TBINTDIS_Msk;

    /* 不用中断，轮询 TD 标志 */
    PDMA_CLR_TD_FLAG(PDMA0, 1u << TFT_SPI1_PDMA_CH_TX);

    SPI_TRIGGER_TX_PDMA(SPI_PORT);

    uint32_t timeout = SystemCoreClock; /* ~1s */
    while (((PDMA_GET_TD_STS(PDMA0) & (1u << TFT_SPI1_PDMA_CH_TX)) == 0u) && timeout) {
        timeout--;
    }

    SPI_DISABLE_TX_PDMA(SPI_PORT);
    PDMA_CLR_TD_FLAG(PDMA0, 1u << TFT_SPI1_PDMA_CH_TX);

    /* 等待 SPI 真正把最后数据移出 */
    while (SPI_IS_BUSY(SPI_PORT));

    /* 恢复 8-bit，后续命令/坐标写入用 */
    SPI_SET_DATA_WIDTH(SPI_PORT, 8);

    return timeout ? 0 : -1;
}

/**
 * @brief 毫秒级阻塞延时（用于上电、复位、命令间隔）
 * @param ms 延时毫秒数
 * @note  简单循环实现，不占用定时器；若系统有 os_delay 可替换
 */
static void Delay_ms(uint32_t ms)
{
    uint32_t i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1000; j++) {}
}

/*-------- ILI9341：片选 PH9 = GPIO（main.c 已配置为输出，空闲高）----------------*/
static void ILI9341_CS_Low(void)   { PH9 = 0; }   /* 片选有效 */
static void ILI9341_CS_High(void)  { PH9 = 1; }   /* 片选释放 */
static void ILI9341_DC_Command(void){ PH10 = 0; }  /* RS/DC=0 命令 */
static void ILI9341_DC_Data(void)   { PH10 = 1; }  /* RS/DC=1 数据 */
static void ILI9341_RST_Low(void)  { PD14 = 0; }  /* 复位拉低 */
static void ILI9341_RST_High(void)  { PD14 = 1; }  /* 复位释放 */

/**
 * @brief 仅发送命令字节（无参数），一次 CS 事务
 */
static void ILI9341_WriteCmd8(uint8_t cmd)
{
    ILI9341_DC_Command();
    ILI9341_CS_Low();
    SPI_WRITE_TX(SPI_PORT, cmd);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_CS_High();
}

/**
 * @brief 命令 + 连续多字节参数，在一次 CS 拉低期间发完
 * @note ILI9341 规定：同一条命令的所有参数必须在 CS 保持有效时连续送出。
 *       若每发一字节就 CS 拉高，CASET/PASET 等只会收到部分数据，导致写窗口错误 → 横条花屏。
 */
static void ILI9341_WriteCmdData8(uint8_t cmd, const uint8_t *data, size_t len)
{
    ILI9341_DC_Command();
    ILI9341_CS_Low();
    SPI_WRITE_TX(SPI_PORT, cmd);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_DC_Data();
    for (size_t i = 0; i < len; i++) {
        while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
        SPI_WRITE_TX(SPI_PORT, data[i]);
    }
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_CS_High();
}

/**
 * @brief 发送“命令 + 多字节参数”（len 可为 0，仅发命令）
 */
static void ILI9341_WriteCommandData(uint8_t cmd, uint8_t *data, uint8_t len)
{
    if (len == 0) {
        ILI9341_WriteCmd8(cmd);
    } else {
        ILI9341_WriteCmdData8(cmd, data, (size_t)len);
    }
}

/**
 * @brief 只设置列/页地址窗口，不发 RAMWR（由后续与像素同一次 CS 发出）
 */
static void ILI9341_SetAddrWindowOnly(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t b[4];
    b[0] = (uint8_t)(x1 >> 8);
    b[1] = (uint8_t)(x1 & 0xFF);
    b[2] = (uint8_t)(x2 >> 8);
    b[3] = (uint8_t)(x2 & 0xFF);
    ILI9341_WriteCmdData8(ILI9341_CASET, b, 4);
    b[0] = (uint8_t)(y1 >> 8);
    b[1] = (uint8_t)(y1 & 0xFF);
    b[2] = (uint8_t)(y2 >> 8);
    b[3] = (uint8_t)(y2 & 0xFF);
    ILI9341_WriteCmdData8(ILI9341_PASET, b, 4);
}

/**
 * @brief 一次 CS 事务内完成：CASET + PASET + RAMWR + RGB565 像素（PDMA 推 SPI1 TX）
 * @note 原先分两次 CS 设窗口再第三次发像素，片选开销大、易产生可见“分段刷新感”。
 */
static void ILI9341_FlushAreaOneCS(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,
                                   const sgl_color_t *src, uint32_t len)
{
    uint8_t b[4];

    ILI9341_CS_Low();

    /* CASET */
    ILI9341_DC_Command();
    SPI_WRITE_TX(SPI_PORT, ILI9341_CASET);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_DC_Data();
    b[0] = (uint8_t)(x1 >> 8);
    b[1] = (uint8_t)(x1 & 0xFF);
    b[2] = (uint8_t)(x2 >> 8);
    b[3] = (uint8_t)(x2 & 0xFF);
    for (size_t i = 0; i < 4; i++) {
        while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
        SPI_WRITE_TX(SPI_PORT, b[i]);
    }
    while (SPI_IS_BUSY(SPI_PORT));

    /* PASET */
    ILI9341_DC_Command();
    SPI_WRITE_TX(SPI_PORT, ILI9341_PASET);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_DC_Data();
    b[0] = (uint8_t)(y1 >> 8);
    b[1] = (uint8_t)(y1 & 0xFF);
    b[2] = (uint8_t)(y2 >> 8);
    b[3] = (uint8_t)(y2 & 0xFF);
    for (size_t i = 0; i < 4; i++) {
        while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
        SPI_WRITE_TX(SPI_PORT, b[i]);
    }
    while (SPI_IS_BUSY(SPI_PORT));

    /* RAMWR + 像素（PDMA；失败回退 CPU 双字节） */
    ILI9341_DC_Command();
    SPI_WRITE_TX(SPI_PORT, ILI9341_RAMWR);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_DC_Data();
    if (tft_spi1_write_pixels_pdma16(src, len) != 0) {
        for (uint32_t i = 0; i < len; i++) {
            uint16_t c = src[i].full;
            while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
            SPI_WRITE_TX(SPI_PORT, (uint8_t)(c >> 8));
            while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
            SPI_WRITE_TX(SPI_PORT, (uint8_t)(c & 0xFF));
        }
        while (SPI_IS_BUSY(SPI_PORT));
    }
    ILI9341_CS_High();
}

/**
 * @brief SGL 刷屏回调：把 area 矩形内的像素从 src 写入 ILI9341
 * @param area 脏区矩形（x1,y1,x2,y2）
 * @param src  该矩形对应的行优先像素缓冲
 * @note  完成后必须调用 sgl_fbdev_flush_ready()，否则 SGL 会一直等待
 */
void demo_panel_flush_area(sgl_area_t *area, sgl_color_t *src)
{
    uint32_t len = (uint32_t)(area->x2 - area->x1 + 1) * (uint32_t)(area->y2 - area->y1 + 1);
    ILI9341_FlushAreaOneCS((uint16_t)area->x1, (uint16_t)area->y1,
                           (uint16_t)area->x2, (uint16_t)area->y2, src, len);
    sgl_fbdev_flush_ready();  /* 通知 SGL 本块刷新完成 */
}

/**
 * @brief 整屏填充 RGB565（调试用：在 tft_init 后调用可确认 SPI/窗口是否正常）
 */
void tft_fill_screen_rgb565(uint16_t rgb565)
{
    ILI9341_SetAddrWindowOnly(0, 0,
                              (uint16_t)(TFT_LCD_HOR_RES_MAX - 1),
                              (uint16_t)(TFT_LCD_VER_RES_MAX - 1));
    ILI9341_DC_Command();
    ILI9341_CS_Low();
    SPI_WRITE_TX(SPI_PORT, ILI9341_RAMWR);
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_DC_Data();
    for (uint32_t n = 0; n < (uint32_t)TFT_LCD_HOR_RES_MAX * (uint32_t)TFT_LCD_VER_RES_MAX; n++) {
        while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
        SPI_WRITE_TX(SPI_PORT, (uint8_t)(rgb565 >> 8));
        while (SPI_GET_TX_FIFO_FULL_FLAG(SPI_PORT));
        SPI_WRITE_TX(SPI_PORT, (uint8_t)(rgb565 & 0xFF));
    }
    while (SPI_IS_BUSY(SPI_PORT));
    ILI9341_CS_High();
}

/**
 * @brief ILI9341 上电与寄存器初始化
 * @note  与 Nuvoton LVGL 参考实现逐条对齐：
 *        lv_port_nuvoton-lvgl_v9/common/drv_disp/disp_ili9341.c
 *        （无 0x01 软复位、无 0x28 DISPOFF、无 0xB7；0xB6 仅 3 字节，与参考一致）
 */
static void ILI9341_Init(void)
{
    uint8_t data[15];

    /* disp_init：RST 高、关背光，再按 5ms / 20ms / 40ms 做硬件复位 */
    ILI9341_RST_High();
    PH11 = 0;

    ILI9341_RST_High();
    Delay_ms(5);
    ILI9341_RST_Low();
    Delay_ms(20);
    ILI9341_RST_High();
    Delay_ms(40);

    /* --- 以下寄存器顺序与数值同 disp_ili9341.c --- */
    data[0] = 0x39; data[1] = 0x2C; data[2] = 0x00; data[3] = 0x34; data[4] = 0x02;
    ILI9341_WriteCommandData(ILI9341_PWCTRLA, data, 5);   /* 0xCB */

    data[0] = 0x00; data[1] = 0xC1; data[2] = 0x30;
    ILI9341_WriteCommandData(ILI9341_PWCTRLB, data, 3);   /* 0xCF */

    data[0] = 0x85; data[1] = 0x00; data[2] = 0x78;
    ILI9341_WriteCommandData(ILI9341_DTCTRLA, data, 3);   /* 0xE8 */

    data[0] = 0x00; data[1] = 0x00;
    ILI9341_WriteCommandData(ILI9341_DTCTRLB, data, 2);   /* 0xEA */

    data[0] = 0x64; data[1] = 0x03; data[2] = 0x12; data[3] = 0x81;
    ILI9341_WriteCommandData(ILI9341_PWRSEQ, data, 4);    /* 0xED */

    data[0] = 0x20;
    ILI9341_WriteCmdData8(ILI9341_PUMPCTRL, data, 1);    /* 0xF7 */

    data[0] = 0x23;
    ILI9341_WriteCmdData8(ILI9341_PWCTR1, data, 1);       /* 0xC0 */

    data[0] = 0x10;
    ILI9341_WriteCmdData8(ILI9341_PWCTR2, data, 1);      /* 0xC1 */

    data[0] = 0x3E; data[1] = 0x28;
    ILI9341_WriteCmdData8(ILI9341_VMCTR1, data, 2);      /* 0xC5 */

    data[0] = 0x86;
    ILI9341_WriteCmdData8(ILI9341_VMCTR2, data, 1);      /* 0xC7 */

#if (TFT_LCD_HOR_RES_MAX == 240)
    data[0] = 0x48;   /* 240×320 竖屏，同 disp_ili9341 if (LV_HOR_RES_MAX == 240) */
#else
    data[0] = 0xE8;   /* 320×240 横屏 */
#endif
    ILI9341_WriteCmdData8(ILI9341_MADCTL, data, 1);       /* 0x36 */

    data[0] = 0x55;
    ILI9341_WriteCmdData8(ILI9341_PIXFMT, data, 1);      /* 0x3A */

    data[0] = 0x00; data[1] = 0x18;
    ILI9341_WriteCmdData8(ILI9341_FRMCTR1, data, 2);     /* 0xB1 */

    data[0] = 0x08; data[1] = 0x82; data[2] = 0x27;
    ILI9341_WriteCommandData(ILI9341_DISCTRL, data, 3);  /* 0xB6，参考工程仅 3 字节 */

    data[0] = 0x00;
    ILI9341_WriteCmdData8(ILI9341_EN3GAM, data, 1);     /* 0xF2 */

    data[0] = 0x01;
    ILI9341_WriteCmdData8(ILI9341_GAMMASET, data, 1);    /* 0x26 */

    data[0] = 0x0F; data[1] = 0x31; data[2] = 0x2B; data[3] = 0x0C; data[4] = 0x0E;
    data[5] = 0x08; data[6] = 0x4E; data[7] = 0xF1; data[8] = 0x37; data[9] = 0x07;
    data[10] = 0x10; data[11] = 0x03; data[12] = 0x0E; data[13] = 0x09; data[14] = 0x00;
    ILI9341_WriteCommandData(ILI9341_PGAMCTRL, data, 15); /* 0xE0 */

    data[0] = 0x00; data[1] = 0x0E; data[2] = 0x14; data[3] = 0x03; data[4] = 0x11;
    data[5] = 0x07; data[6] = 0x31; data[7] = 0xC1; data[8] = 0x48; data[9] = 0x08;
    data[10] = 0x0F; data[11] = 0x0C; data[12] = 0x31; data[13] = 0x36; data[14] = 0x0F;
    ILI9341_WriteCommandData(ILI9341_NGAMCTRL, data, 15); /* 0xE1 */

    ILI9341_WriteCmd8(ILI9341_SLPOUT);                  /* 0x11 */
    Delay_ms(120);

    ILI9341_WriteCmd8(ILI9341_DISPON);                  /* 0x29 */

    PH11 = 1;   /* DISP_SET_BACKLIGHT */
}

/**
 * @brief TFT 驱动对外入口：完成 ILI9341 上电与初始化
 */
void tft_init(void)
{
    ILI9341_Init();
}

void tft_delay_ms(uint32_t ms)
{
    Delay_ms(ms);
}
