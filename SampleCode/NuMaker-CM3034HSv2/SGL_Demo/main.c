/**************************************************************************//**
 * @file     main.c
 * @brief    CM3034 + ILI9341（竖屏 240×320）上的 SGL 样例：仪表 UI、运行参数页、电阻触摸
 *
 * @details  流程：SYS_Init → 触摸 BSP → SGL（日志 / 帧缓冲 / 1ms Tick）→ tft 初始化 →
 *           开机 Logo（nuvoton_logo.c 内嵌位图）→ 仪表根界面（instrument-bag.c 全屏背景 +
 *           车速/压力/电压/Trip 卡片与底部导航）。主循环内 touch_poll、dashboard_update、sgl_task_handler。
 *
 *           位图不经外部 SPI：Logo 用 nuvoton_pixmap，仪表背景用 pic1_pixmap（与 instrument-bag.c 中定义一致）。
 *           LCD 时序与命令见 tft.c 头注释
 ******************************************************************************/

#include "NuMicro.h"
#include "sgl.h"
#include <stdbool.h>

#include "panel_config.h"
#include "tft.h"
#include "touch_adc_bsp.h"

/* sgl_font.h 仅声明了 song23；以下字体在对应 .c 内定义，这里补 extern 声明供本文件使用 */
extern const sgl_font_t myfont23;
extern const sgl_font_t FontAwesome_23;
extern const sgl_font_t iconfont_23;

/* 开机 Logo（nuvoton_logo.c）、仪表全屏背景（instrument-bag.c 的 pic1_pixmap），编译进 Flash */
extern const sgl_pixmap_t nuvoton_pixmap;
extern const sgl_pixmap_t pic1_pixmap;

#ifndef SGL_TOUCH_COORD_DEBUG
#define SGL_TOUCH_COORD_DEBUG  0
#endif
#ifndef SGL_DASH_TOUCH_DEBUG
#define SGL_DASH_TOUCH_DEBUG  0
#endif
#if (SGL_TOUCH_COORD_DEBUG || SGL_DASH_TOUCH_DEBUG)
#include <stdio.h>
#endif

/* 1=串口打印横幅/提示（占用少量 rodata；已用 uart_puts，不拉 printf 库） */
#ifndef SGL_SAMPLE_UART_LOG
#define SGL_SAMPLE_UART_LOG  1
#endif

/* FontAwesome_23 图标：UTF-8 字符串（SGL 直接用字体渲染） */
#define SGL_ICON_F013_USER_NAME "\xEF\x80\x93"
#define SGL_ICON_F015_USER_NAME "\xEF\x80\x95"
#define SGL_ICON_F028_USER_NAME "\xEF\x80\xA8"
#define SGL_ICON_F122_USER_NAME "\xEF\x84\xA2"
#define SGL_ICON_F14D_USER_NAME "\xEF\x85\x8D"

/* iconfont_23（CONFIG_SGL_FONT_ICONFONT_23）：与 sgl/fonts/iconfont_23.c 中 UTF-8 一致 */
#define SGL_ICON_E612_USER_NAME "\xEE\x98\x92"
#define SGL_ICON_E613_USER_NAME "\xEE\x98\x93"

/*----------------------------------------------------------------------------*/
/* SGL 行缓冲（条带高度 = PANEL_STRIP_MAX_LINES，默认 1）
 *
 * - 越大：单次 flush 行数多，边角未刷风险略低，RAM 占用 = PANEL_WIDTH × 行数 × 2（RGB565）越大。
 * - 越小：更省 RAM，全屏脏区分条更多，需依赖 gauge_row_flush / 整屏标脏 等规避接缝伪影。
 * 其它内存占用见 ThirdParty/sgl/sgl_config.h（堆、事件队列、脏区个数等）；用 .map 核对 ZI+Stack。
 */
/*----------------------------------------------------------------------------*/
#ifndef PANEL_STRIP_MAX_LINES
    #define PANEL_STRIP_MAX_LINES  1
#endif
#define PANEL_STRIP_LINES  ((PANEL_HEIGHT) < PANEL_STRIP_MAX_LINES ? (PANEL_HEIGHT) : PANEL_STRIP_MAX_LINES)

static sgl_color_t panel_buffer[PANEL_WIDTH * PANEL_STRIP_LINES];

/**
 * @brief 将字符串通过 UART 发送，供 SGL 日志输出使用
 * @param str 以 '\0' 结尾的 C 字符串
 * @note  阻塞式发送；需在 SYS_Init 中已使能 UART0 并打开（如 115200）
 */
static void uart_puts(const char *str)
{
    while (*str)
    {
        while (UART_IS_TX_FULL(UART0));  /* 等待发送 FIFO 非满 */
        UART_WRITE(UART0, *str++);
    }
}

/** 无圆角、无边框（仪表里大量全平矩形共用） */
static void dash_rect_flat(sgl_obj_t *o)
{
    sgl_rect_set_radius(o, 0);
    sgl_rect_set_border_width(o, 0);
}

static void boot_logo_img_fade_anim(struct sgl_anim *anim, int32_t value)
{
    sgl_rect_set_alpha((sgl_obj_t *)anim->data, (uint8_t)value);
}

static void show_boot_logo_image(void)
{
    const int boot_w = (int)nuvoton_pixmap.width;
    const int boot_h = (int)nuvoton_pixmap.height;

    /* screen background */
    sgl_obj_t *screen = sgl_rect_create(NULL);
    sgl_obj_set_pos(screen, 0, 0);
    sgl_obj_set_size(screen, PANEL_WIDTH, PANEL_HEIGHT);
    dash_rect_flat(screen);
    sgl_rect_set_color(screen, SGL_COLOR_WHITE);
    sgl_obj_set_dirty(screen);

    /* Logo：位图在 nuvoton_logo.c（nuvoton_pixmap），矩形控件直接绘制 pixmap */
    sgl_obj_t *logo_img = sgl_rect_create(screen);
    sgl_obj_set_size(logo_img, boot_w, boot_h);
    sgl_obj_set_pos(logo_img, (PANEL_WIDTH - boot_w) / 2, (PANEL_HEIGHT - boot_h) / 2);
    dash_rect_flat(logo_img);
    sgl_rect_set_pixmap(logo_img, &nuvoton_pixmap);
    sgl_rect_set_alpha(logo_img, SGL_ALPHA_MIN);
    sgl_obj_set_dirty(logo_img);

    sgl_screen_load(screen);
    sgl_task_handler_sync();
    /* Alpha：SGL_ALPHA_MAX → MIN（不透明 → 透明） */
    sgl_anim_t *fade_out = sgl_anim_create();
    if (fade_out == NULL)
    {
        sgl_obj_delete(screen);
        return;
    }
    sgl_anim_set_data(fade_out, logo_img);
    sgl_anim_set_act_duration(fade_out, 1200);
    sgl_anim_set_start_value(fade_out, SGL_ALPHA_MAX);
    sgl_anim_set_end_value(fade_out, SGL_ALPHA_MIN);
    sgl_anim_set_path(fade_out, boot_logo_img_fade_anim, SGL_ANIM_PATH_LINEAR);
    sgl_anim_start(fade_out, SGL_ANIM_REPEAT_ONCE);
    while (!sgl_anim_is_finished(fade_out))
        sgl_task_handler();
    sgl_anim_delete(fade_out);

    sgl_obj_delete(screen);
    sgl_task_handler_sync();
}

/* 主界面动态数值：由主循环周期性调用 dashboard_update() 刷新 */
typedef struct {
    /** 透明全宽条：标脏含 x=0 与车速卡上方边距，缓和行缓冲分条时左上角白点/竖线（见 sgl_config） */
    sgl_obj_t *gauge_row_flush;
    /** Trip 卡行：合并 km/m 标脏，避免两标签各 set_dirty 在中间接缝出竖黑线 */
    sgl_obj_t *trip_row_flush;
    sgl_obj_t *speed_blocks[16];
    sgl_obj_t *lbl_speed;
    sgl_obj_t *lbl_speed_unit;
    sgl_obj_t *lbl_mpa;
    sgl_obj_t *lbl_v;
    sgl_obj_t *lbl_trip_km;
    sgl_obj_t *lbl_trip_m;
} dashboard_widgets_t;

static dashboard_widgets_t s_dash;
static uint32_t s_dash_trip_m;
/** 演示车速：仅在速度区按步进刷新，0~120 每次 +5 */
static unsigned s_dash_speed_demo;
static bool s_blue_mode;
static sgl_obj_t *s_blue_overlay;
/** 音量键：FontAwesome_23 图标；静音红色，非静音白色 */
static sgl_obj_t *s_dash_vol_btn;
static bool       s_dash_vol_muted;
/** 与车速条同步的当前车速演示值（0~120），供运行参数页转速换算 */
static unsigned s_dash_cur_speed;
static sgl_obj_t *s_blue_lbl_title;
static sgl_obj_t *s_blue_lbl_val[5];
/* 数值串最大长度约 10 字符（如 "9999 rpm"），12 足够并可减少 RW */
static char s_blue_val_buf[5][12];

/* sgl_label 只保存指针；各域独立静态缓冲（勿合并为单 buf） */
static char s_dash_buf_speed[8];
static char s_dash_buf_mpa[8];
static char s_dash_buf_v[8];
static char s_dash_buf_trip_km[12];
static char s_dash_buf_trip_m[8];

#define DASH_SPEED_BLOCK_COUNT 16
#ifndef DASH_SPEED_POLL_MS
/** 车速区刷新周期；过小会多次 merge 窄脏区与底层位图背景叠刷，易噪点；可按观感调整 */
#define DASH_SPEED_POLL_MS  20u
#endif
/* 与底部四键对齐：x0=5, w=55, gap=3 => 4*55+3*3=229（右边到 x=233） */
#define DASH_CARD_FULL_W  229

/* 车速条与运行参数页渐变条共用 16 色，避免 rodata 重复一份 */
static const sgl_color_t s_dash_rainbow[DASH_SPEED_BLOCK_COUNT] = {
    sgl_rgb(0xFF, 0x00, 0x00), sgl_rgb(0xFF, 0x5A, 0x00), sgl_rgb(0xFF, 0xA0, 0x00), sgl_rgb(0xFF, 0xD0, 0x00),
    sgl_rgb(0xE8, 0xFF, 0x00), sgl_rgb(0x9C, 0xFF, 0x00), sgl_rgb(0x30, 0xFF, 0x00), sgl_rgb(0x00, 0xFF, 0x66),
    sgl_rgb(0x00, 0xFF, 0xC8), sgl_rgb(0x00, 0xC8, 0xFF), sgl_rgb(0x00, 0x66, 0xFF), sgl_rgb(0x24, 0x20, 0xFF),
    sgl_rgb(0x70, 0x00, 0xFF), sgl_rgb(0xB0, 0x00, 0xFF), sgl_rgb(0xE0, 0x00, 0xE8), sgl_rgb(0xFF, 0x00, 0xA0)
};

/* 触摸轮询周期（ms），与 lv_glue.c 中 CONFIG_TRIGGER_PERIOD 16 对齐，减轻 ADC 切换频率 */
#ifndef SGL_TOUCH_POLL_PERIOD_MS
#define SGL_TOUCH_POLL_PERIOD_MS  16u
#endif

#if SGL_TOUCH_COORD_DEBUG
static sgl_obj_t *s_blue_lbl_touch_dbg;
static char s_blue_touch_dbg_buf[28];
#endif

#if SGL_DASH_TOUCH_DEBUG
static sgl_obj_t *s_dash_touch_dbg_lbl;
static char s_dash_touch_dbg_buf[36];
#endif

/** 打开运行参数全屏页（仅由底部扳手键触发；退出仍在该页内点返回） */
static void dash_open_blue_overlay(void)
{
    if (s_blue_mode || s_blue_overlay == NULL)
        return;
    s_blue_mode = true;
    sgl_obj_move_top(s_blue_overlay);
    sgl_obj_set_visible(s_blue_overlay);
    /* 行缓冲 + 脏区合并有限：切换界面必须整屏标脏，否则底层仪表会透显、运行参数页左侧易缺列/闪 */
    sgl_obj_set_dirty(s_blue_overlay);
    sgl_obj_set_dirty(sgl_screen_act());
}

/** 运行参数页右下角「返回」按钮区域（须与 blue_overlay_install_ui 中控件位置一致） */
#define BLUE_BACK_BTN_W   72
#define BLUE_BACK_BTN_H   44
#define BLUE_BACK_BTN_X   ((int16_t)(PANEL_WIDTH - 8 - BLUE_BACK_BTN_W))
#define BLUE_BACK_BTN_Y   ((int16_t)(PANEL_HEIGHT - 8 - BLUE_BACK_BTN_H))
/** 触摸命中区比可见按钮略大（仅右下角返回区），在原有基础上再放大约 +10px */
#define BLUE_BACK_TOUCH_PAD  26

static bool touch_point_in_blue_back_btn(int16_t px, int16_t py)
{
    int16_t x1 = (int16_t)(BLUE_BACK_BTN_X - BLUE_BACK_TOUCH_PAD);
    int16_t y1 = (int16_t)(BLUE_BACK_BTN_Y - BLUE_BACK_TOUCH_PAD);
    int16_t x2 = (int16_t)(BLUE_BACK_BTN_X + BLUE_BACK_BTN_W + BLUE_BACK_TOUCH_PAD);
    int16_t y2 = (int16_t)(BLUE_BACK_BTN_Y + BLUE_BACK_BTN_H + BLUE_BACK_TOUCH_PAD);
    if (x1 < 0)
        x1 = 0;
    if (y1 < 0)
        y1 = 0;
    if (x2 > PANEL_WIDTH)
        x2 = (int16_t)PANEL_WIDTH;
    if (y2 > PANEL_HEIGHT)
        y2 = (int16_t)PANEL_HEIGHT;
    return (px >= x1 && px < x2 && py >= y1 && py < y2);
}

/* 仅做 XY 交换容错，避免左右整列误判为返回区 */
static bool touch_point_in_blue_back_btn_safe_map(int16_t px, int16_t py)
{
    if (touch_point_in_blue_back_btn(px, py))
        return true;
    if (px >= 0 && px < PANEL_HEIGHT && py >= 0 && py < PANEL_WIDTH) {
        if (touch_point_in_blue_back_btn(py, px))
            return true;
    }
    /* 实机回退热区（仅小范围）：当物理点在右下 Back 附近时，当前触摸读数约落在 x:0~60, y:160~235。 */
    if (px >= 0 && px <= 60 && py >= 160 && py <= 235)
        return true;
    return false;
}

static void touch_blue_exit_to_dashboard(void)
{
    if (!s_blue_mode)
        return;
    s_blue_mode = false;
    sgl_obj_set_hidden(s_blue_overlay);
    sgl_obj_set_dirty(sgl_screen_act());
}

static void touch_poll_and_feed_sgl(void)
{
    /*
     * 轮询状态变量
     * - s_next_touch_ms: 触摸采样节拍（16ms），避免每帧都切换 ADC 引脚模式
     * - s_prev_pressed: 上一拍是否按下，用于识别“抬起沿”
     * - s_gesture_toggled: 一次按压手势内只触发一次动作，防止长按连跳
     * - s_last_x/y: 抬起补判用最后按下坐标
     */
    static uint32_t s_next_touch_ms;
    static bool     s_prev_pressed;
    static bool     s_gesture_toggled;
    static bool     s_back_press_candidate;
    static int16_t  s_last_x;
    static int16_t  s_last_y;
    uint32_t        t = sgl_tick_get();

    if (t < s_next_touch_ms) {
        return;
    }
    s_next_touch_ms = t + SGL_TOUCH_POLL_PERIOD_MS;

    int16_t x = 0;
    int16_t y = 0;
    bool pressed = false;

    if (bsp_touch_read(&x, &y, &pressed) != 0) {
        /* 读触摸失败：本拍直接退出，不上报事件 */
        return;
    }

    /* 坐标钳位，避免越界干扰事件系统 */
    if (x < 0) x = 0;
    if (x >= PANEL_WIDTH) x = (PANEL_WIDTH - 1);
    if (y < 0) y = 0;
    if (y >= PANEL_HEIGHT) y = (PANEL_HEIGHT - 1);

#if SGL_TOUCH_COORD_DEBUG
    if (s_blue_lbl_touch_dbg != NULL && s_blue_mode) {
        snprintf(s_blue_touch_dbg_buf, sizeof(s_blue_touch_dbg_buf), "x:%3d y:%3d %c", (int)x, (int)y, pressed ? 'D' : 'U');
        sgl_label_set_text(s_blue_lbl_touch_dbg, s_blue_touch_dbg_buf);
        sgl_obj_clear_dirty(s_blue_lbl_touch_dbg);
        sgl_obj_set_dirty(s_blue_overlay);
    }
#endif

#if SGL_DASH_TOUCH_DEBUG
    if (s_dash_touch_dbg_lbl != NULL) {
        snprintf(s_dash_touch_dbg_buf, sizeof(s_dash_touch_dbg_buf), "x:%3d y:%3d %c", (int)x, (int)y, pressed ? 'D' : 'U');
        sgl_label_set_text(s_dash_touch_dbg_lbl, s_dash_touch_dbg_buf);
        sgl_obj_set_dirty(s_dash_touch_dbg_lbl);
    }
#endif

    if (pressed) {
        s_last_x = x;
        s_last_y = y;
        if (!s_gesture_toggled) {
            /* 仅运行参数页需要记录「按下时是否在返回热区」；仪表进入运行参数页走底部扳手键回调，此处不设候选 */
            s_back_press_candidate = s_blue_mode && touch_point_in_blue_back_btn_safe_map(x, y);
            s_gesture_toggled = true;
        }
    } else {
        /* 运行参数页：仅在抬起时落在右下角返回区才退出 */
        if (s_prev_pressed && s_blue_mode) {
            if (s_back_press_candidate || touch_point_in_blue_back_btn_safe_map(s_last_x, s_last_y)) {
                touch_blue_exit_to_dashboard();
            }
        }
        s_back_press_candidate = false;
        s_gesture_toggled = false;
    }
    s_prev_pressed = pressed;

    /* 把触摸状态喂给 SGL 事件系统 */
    sgl_event_pos_input(x, y, pressed);
}

/* 单色矩形条（卡片边框用四条拼成框） */
static void dash_rect_band(sgl_obj_t *parent, int16_t x, int16_t y, int16_t w, int16_t h, sgl_color_t color)
{
    sgl_obj_t *r = sgl_rect_create(parent);
    sgl_obj_set_pos(r, x, y);
    sgl_obj_set_size(r, w, h);
    dash_rect_flat(r);
    sgl_rect_set_color(r, color);
    sgl_rect_set_alpha(r, SGL_ALPHA_MAX);
    sgl_obj_set_dirty(r);
}

static void dash_draw_box_border(sgl_obj_t *parent, int16_t x, int16_t y, int16_t w, int16_t h,
                                 uint8_t border_w, sgl_color_t color)
{
    int16_t bw = (border_w == 0u) ? 1 : (int16_t)border_w;

    dash_rect_band(parent, x, y, w, bw, color);
    dash_rect_band(parent, x, (int16_t)(y + h - bw), w, bw, color);
    dash_rect_band(parent, x, y, bw, h, color);
    dash_rect_band(parent, (int16_t)(x + w - bw), y, bw, h, color);
}

/* card 左上角 iconfont 小图标（机油 / 电池等） */
static void dash_card_iconfont(sgl_obj_t *parent, const char *utf8, sgl_color_t color)
{
    sgl_obj_t *lbl = sgl_label_create(parent);
    sgl_obj_set_pos(lbl, 6, 6);
    sgl_obj_set_size(lbl, 38, 26);
    sgl_label_set_text(lbl, (char *)(uintptr_t)utf8);
    sgl_label_set_text_color(lbl, color);
    sgl_label_set_text_align(lbl, SGL_ALIGN_LEFT_MID);
    sgl_label_set_font(lbl, &iconfont_23);
    sgl_obj_set_dirty(lbl);
}

/* 与 ThirdParty/sgl/fonts/iconfont_23.c 中 U+E612 / U+E613 一致（该文件内宏未对外暴露，此处用 UTF-8 字面量） */
static void dash_draw_oil_icon(sgl_obj_t *parent)
{
    dash_card_iconfont(parent, "\xEE\x98\x92", SGL_COLOR_GOLD);
}

static void dash_draw_battery_icon(sgl_obj_t *parent)
{
    dash_card_iconfont(parent, "\xEE\x98\x93", SGL_COLOR_YELLOW_GREEN);
}

static void dash_nav_btn_set_fa_icon(sgl_obj_t *p, const char *icon_utf8)
{
    sgl_button_set_text(p, icon_utf8);
    sgl_button_set_text_color(p, SGL_COLOR_WHITE);
    sgl_button_set_text_align(p, SGL_ALIGN_CENTER);
    sgl_button_set_font(p, &FontAwesome_23);
    sgl_obj_set_dirty(p);
}

static void dash_vol_icon_apply_state(bool muted)
{
    if (s_dash_vol_btn == NULL)
        return;
    sgl_button_set_text_color(s_dash_vol_btn, muted ? SGL_COLOR_RED : SGL_COLOR_WHITE);
    sgl_obj_set_dirty(s_dash_vol_btn);
}

static void dash_nav_vol_event_cb(sgl_event_t *e)
{
    if (e->type != SGL_EVENT_RELEASED)
        return;
    if (s_dash_vol_btn == NULL)
        return;
    s_dash_vol_muted = !s_dash_vol_muted;
    dash_vol_icon_apply_state(s_dash_vol_muted);
}

/** 底部扳手键（运行参数入口）：松开进入运行参数页 */
static void dash_nav_info_event_cb(sgl_event_t *e)
{
    if (e->type != SGL_EVENT_RELEASED)
        return;
    dash_open_blue_overlay();
}

/** 底部导航：主页 / 轮胎 / 音量 / 运行参数（扳手图标，点击进入运行参数页）
 *  @param trip_y   里程卡片左上角 Y（与 card_trip 一致）
 *  @param trip_h   里程卡片高度；按钮行垂直居中于「卡片下沿 ~ 屏幕底」之间 */
static void dash_create_bottom_nav_buttons(sgl_obj_t *parent, int16_t trip_y, int16_t trip_h)
{
    const int16_t trip_bottom = (int16_t)(trip_y + trip_h);
    const int16_t h = 34;
    const int16_t y = (int16_t)(trip_bottom + (PANEL_HEIGHT - trip_bottom - h) / 2);
    const int16_t w = 55;
    const int16_t gap = 3;
    const int16_t x0 = 5;
    unsigned i;

    for (i = 0u; i < 4u; i++) {
        int16_t x = (int16_t)(x0 + (int16_t)i * (w + gap));
        sgl_obj_t *btn = sgl_button_create(parent);
        /* 按下时禁止弹性缩放，避免未释放阶段在按钮外产生额外线条重绘 */
        sgl_obj_set_unflexible(btn);
        sgl_obj_set_pos(btn, x, y);
        sgl_obj_set_size(btn, w, h);
        sgl_button_set_radius(btn, 4);
        sgl_button_set_border_width(btn, 1);
        sgl_button_set_color(btn, sgl_rgb(0x18, 0x28, 0x48));
        sgl_button_set_border_color(btn, SGL_COLOR_BLUE);
        sgl_button_set_alpha(btn, SGL_ALPHA_MAX);
        sgl_button_set_text(btn, NULL);
        sgl_obj_set_dirty(btn);
        switch (i) {
        case 0u:
            dash_nav_btn_set_fa_icon(btn, SGL_ICON_F015_USER_NAME);
            break;
        case 1u:
            dash_nav_btn_set_fa_icon(btn, SGL_ICON_F013_USER_NAME);
            break;
        case 2u:
            s_dash_vol_btn = btn;
            dash_nav_btn_set_fa_icon(btn, SGL_ICON_F028_USER_NAME);
            s_dash_vol_muted = false;
            dash_vol_icon_apply_state(false);
            sgl_obj_set_event_cb(btn, dash_nav_vol_event_cb, NULL);
            break;
        default:
            dash_nav_btn_set_fa_icon(btn, SGL_ICON_F14D_USER_NAME);
            sgl_obj_set_event_cb(btn, dash_nav_info_event_cb, NULL);
            break;
        }
    }
}

/**
 * @brief 无后缀十进制转换（用于 speed/km/rm 等整数显示）
 * @note 只需要轻量实现，避免 snprintf 带来的大体积 printf formatting 代码
 */
static void u_to_dec_str(unsigned v, char *buf, unsigned buf_sz)
{
    if (buf_sz == 0u)
        return;
    if (v == 0u) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    char tmp[10];
    unsigned n = 0u;
    while (v != 0u && n < (sizeof(tmp) / sizeof(tmp[0])))
    {
        tmp[n++] = (char)('0' + (v % 10u));
        v /= 10u;
    }

    /* 反向写回 */
    unsigned i = 0u;
    while (n != 0u && i + 1u < buf_sz) {
        buf[i++] = tmp[--n];
    }
    buf[i] = '\0';
}

static void str_append_ascii(char *buf, unsigned buf_sz, const char *suf)
{
    unsigned i = 0u;
    while (i + 1u < buf_sz && buf[i] != '\0')
        i++;
    while (suf != NULL && *suf != '\0' && i + 1u < buf_sz)
        buf[i++] = *suf++;
    if (i < buf_sz)
        buf[i] = '\0';
}

/* 横向彩色渐变条：多段矩形拼成（与车速条同色环）；seg 与 s_dash_rainbow 项数一致 */
#define BLUE_GRAD_SEG_N DASH_SPEED_BLOCK_COUNT
#define BLUE_GRAD_H       4
#define BLUE_GRAD_MARGIN  8

static void blue_overlay_add_gradient_bar(sgl_obj_t *parent, int16_t y)
{
    const int16_t x0 = BLUE_GRAD_MARGIN;
    const int16_t total_w = (int16_t)(PANEL_WIDTH - 2 * BLUE_GRAD_MARGIN);
    const unsigned seg = BLUE_GRAD_SEG_N;
    unsigned i;

    for (i = 0; i < seg; i++) {
        int32_t x1 = (int32_t)x0 + ((int32_t)total_w * (int32_t)i) / (int32_t)seg;
        int32_t x2 = (int32_t)x0 + ((int32_t)total_w * (int32_t)(i + 1)) / (int32_t)seg;
        int16_t w = (int16_t)(x2 - x1);
        if (w < 1)
            w = 1;
        sgl_obj_t *r = sgl_rect_create(parent);
        sgl_obj_set_pos(r, (int16_t)x1, y);
        sgl_obj_set_size(r, w, BLUE_GRAD_H);
        dash_rect_flat(r);
        sgl_rect_set_color(r, s_dash_rainbow[i % BLUE_GRAD_SEG_N]);
        sgl_rect_set_alpha(r, SGL_ALPHA_MAX);
        sgl_obj_set_dirty(r);
    }
}

/** 运行参数页：标题「运行参数」+ 五行；左列 &myfont23（字库仅含本函数内出现的汉字），右列 &song23 */
static void blue_overlay_install_ui(void)
{
    sgl_obj_t *p = s_blue_overlay;

    /* 标题下渐变线：标题区 y=4、h=40 → 线从 y=44 起 */
    blue_overlay_add_gradient_bar(p, 44);
    /* 机油压力行下渐变线：第 5 行 y=48+4*40=208，行高 36 → 线从 y=244 起 */
    blue_overlay_add_gradient_bar(p, 244);

    /* 与 sgl/fonts/myfont23.c 中 20 字去重集一致；改文案须重生成 myfont23 */
    static const char *const names[5] = {
        "冷却水温",
        "燃油油位",
        "电源电压",
        "发动机转速",
        "机油压力",
    };
    for (unsigned i = 0u; i < 5u; i++) {
        int16_t y = (int16_t)(48 + (int16_t)i * 40);
        sgl_obj_t *ln = sgl_label_create(p);
        sgl_obj_set_pos(ln, 4, y);
        sgl_obj_set_size(ln, 118, 36);
        sgl_label_set_text(ln, (char *)names[i]);
        sgl_label_set_text_color(ln, SGL_COLOR_WHITE);
        sgl_label_set_font(ln, &myfont23);
        sgl_label_set_text_align(ln, SGL_ALIGN_LEFT_MID);
        sgl_obj_set_dirty(ln);

        s_blue_lbl_val[i] = sgl_label_create(p);
        sgl_obj_set_pos(s_blue_lbl_val[i], 122, y);
        sgl_obj_set_size(s_blue_lbl_val[i], 114, 36);
        sgl_label_set_text(s_blue_lbl_val[i], "--");
        sgl_label_set_text_color(s_blue_lbl_val[i], SGL_COLOR_WHITE);
        sgl_label_set_font(s_blue_lbl_val[i], &song23);
        sgl_label_set_text_align(s_blue_lbl_val[i], SGL_ALIGN_RIGHT_MID);
        sgl_obj_set_dirty(s_blue_lbl_val[i]);
    }
    /* 标题「运行参数」：同上，属 myfont23 字集；最后创建保证置顶 */
    s_blue_lbl_title = sgl_label_create(p);
    sgl_obj_set_pos(s_blue_lbl_title, 0, 4);
    sgl_obj_set_size(s_blue_lbl_title, PANEL_WIDTH, 40);
    sgl_label_set_text(s_blue_lbl_title, "运行参数");
    sgl_label_set_text_color(s_blue_lbl_title, SGL_COLOR_GREEN);
    sgl_label_set_font(s_blue_lbl_title, &myfont23);
    sgl_label_set_text_align(s_blue_lbl_title, SGL_ALIGN_CENTER);
    sgl_obj_set_dirty(s_blue_lbl_title);
    /* 右下角返回（视觉 + 与 touch_point_in_blue_back_btn 坐标一致） */
    sgl_obj_t *btn_back = sgl_button_create(p);
    /* 禁止按下弹性放大，避免缩放时边框短时超出区域产生多一条边线 */
    sgl_obj_set_unflexible(btn_back);
    sgl_obj_set_pos(btn_back, BLUE_BACK_BTN_X, BLUE_BACK_BTN_Y);
    sgl_obj_set_size(btn_back, BLUE_BACK_BTN_W, BLUE_BACK_BTN_H);
    sgl_button_set_radius(btn_back, 6);
    sgl_button_set_border_width(btn_back, 2);
    sgl_button_set_color(btn_back, SGL_COLOR_BLACK);
    sgl_button_set_border_color(btn_back, SGL_COLOR_WHITE);
    sgl_button_set_alpha(btn_back, SGL_ALPHA_MAX);
    sgl_button_set_text(btn_back, SGL_ICON_F122_USER_NAME);
    sgl_button_set_text_color(btn_back, SGL_COLOR_WHITE);
    sgl_button_set_text_align(btn_back, SGL_ALIGN_CENTER);
    sgl_button_set_font(btn_back, &FontAwesome_23);
    sgl_obj_set_dirty(btn_back);

#if SGL_TOUCH_COORD_DEBUG
    s_blue_lbl_touch_dbg = sgl_label_create(p);
    sgl_obj_set_pos(s_blue_lbl_touch_dbg, 4, 292);
    sgl_obj_set_size(s_blue_lbl_touch_dbg, 160, 24);
    sgl_label_set_text(s_blue_lbl_touch_dbg, "x:--- y:--- U");
    sgl_label_set_text_color(s_blue_lbl_touch_dbg, SGL_COLOR_WHITE);
    sgl_label_set_font(s_blue_lbl_touch_dbg, &song23);
    sgl_label_set_text_align(s_blue_lbl_touch_dbg, SGL_ALIGN_LEFT_MID);
    sgl_obj_set_dirty(s_blue_lbl_touch_dbg);
#endif
}

static void blue_params_values_refresh(uint32_t t)
{
    if (s_blue_lbl_val[0] == NULL)
        return;

    unsigned wt = 65u + (unsigned)((t / 200u) % 22u);
    u_to_dec_str(wt, s_blue_val_buf[0], sizeof(s_blue_val_buf[0]));
    str_append_ascii(s_blue_val_buf[0], sizeof(s_blue_val_buf[0]), " C");
    sgl_label_set_text(s_blue_lbl_val[0], s_blue_val_buf[0]);

    unsigned fuel = (unsigned)((t / 300u) % 101u);
    u_to_dec_str(fuel, s_blue_val_buf[1], sizeof(s_blue_val_buf[1]));
    str_append_ascii(s_blue_val_buf[1], sizeof(s_blue_val_buf[1]), " %");
    sgl_label_set_text(s_blue_lbl_val[1], s_blue_val_buf[1]);

    unsigned n = 0u;
    while (n + 1u < sizeof(s_blue_val_buf[2]) && s_dash_buf_v[n] != '\0') {
        s_blue_val_buf[2][n] = s_dash_buf_v[n];
        n++;
    }
    s_blue_val_buf[2][n] = '\0';
    str_append_ascii(s_blue_val_buf[2], sizeof(s_blue_val_buf[2]), " V");
    sgl_label_set_text(s_blue_lbl_val[2], s_blue_val_buf[2]);

    unsigned v100 = (unsigned)((t / 90u) % 100u);
    unsigned rpm = s_dash_cur_speed * 9u + 128u;
    if (rpm > 9999u)
        rpm = 9999u;
    u_to_dec_str(rpm, s_blue_val_buf[3], sizeof(s_blue_val_buf[3]));
    str_append_ascii(s_blue_val_buf[3], sizeof(s_blue_val_buf[3]), " rpm");
    sgl_label_set_text(s_blue_lbl_val[3], s_blue_val_buf[3]);

    unsigned kpa = v100 * 8u;
    if (kpa > 999u)
        kpa = 999u;
    u_to_dec_str(kpa, s_blue_val_buf[4], sizeof(s_blue_val_buf[4]));
    str_append_ascii(s_blue_val_buf[4], sizeof(s_blue_val_buf[4]), " Kpa");
    sgl_label_set_text(s_blue_lbl_val[4], s_blue_val_buf[4]);

    /* sgl_label_set_text 会逐个 set_dirty；数值区在 x≈122 起，不含 x=0，行缓冲分条刷时易错位（见 sgl_config 注释）。
     * 去掉子控件脏标记后只整层标脏，脏矩形为全幅宽，避免右对齐字形“断到中间”、与底层边线重叠发花。 */
    {
        unsigned i;
        for (i = 0u; i < 5u; i++) {
            sgl_obj_clear_dirty(s_blue_lbl_val[i]);
        }
        sgl_obj_set_dirty(s_blue_overlay);
    }
}

/** 只改矩形像素色，不 set_dirty（配合 gauge_row_flush 整幅标脏） */
static void dash_rect_color_only(sgl_obj_t *obj, sgl_color_t c)
{
    sgl_rectangle_t *r = sgl_container_of(obj, sgl_rectangle_t, obj);
    r->color = c;
}

/** 只改标签文本指针，不 set_dirty */
static void dash_label_text_only(sgl_obj_t *obj, char *txt)
{
    sgl_label_t *lb = sgl_container_of(obj, sgl_label_t, obj);
    lb->text = txt;
}

/**
 * @brief 演示用动态数据（后续可改为 ADC/CAN/寄存器）
 *        车速区见 DASH_SPEED_POLL_MS；压力/电压/里程约 150ms 更新一次，减轻对 ILI9341 SPI 刷屏的压力
 */
static void dashboard_update(void)
{
    static uint32_t s_last_ms;
    static uint32_t s_last_speed_ms;
    uint32_t        t = sgl_tick_get();

    /* 车速与彩虹条：静默改色后只对全宽 gauge_row_flush 标脏一次，并降频，减轻与底层位图叠刷噪点 */
    if (s_last_speed_ms == 0u || (t - s_last_speed_ms) >= DASH_SPEED_POLL_MS)
    {
        s_last_speed_ms = t;

        unsigned speed = s_dash_speed_demo;
        s_dash_cur_speed = speed;
        s_dash_speed_demo += 5u;
        if (s_dash_speed_demo > 120u)
            s_dash_speed_demo = 0u;
        if (!s_blue_mode) {
            unsigned active = (speed * DASH_SPEED_BLOCK_COUNT + 119u) / 120u;
            unsigned phase = (unsigned)((t / 50u) % DASH_SPEED_BLOCK_COUNT);
            unsigned i;
            for (i = 0; i < DASH_SPEED_BLOCK_COUNT; i++)
            {
                sgl_color_t c = sgl_rgb(0x00, 0x18, 0x38);
                if (i < active) {
                    c = s_dash_rainbow[(i + phase) % DASH_SPEED_BLOCK_COUNT];
                }
                dash_rect_color_only(s_dash.speed_blocks[i], c);
            }
            u_to_dec_str(speed, s_dash_buf_speed, sizeof(s_dash_buf_speed));
            dash_label_text_only(s_dash.lbl_speed, s_dash_buf_speed);
            if (s_dash.gauge_row_flush != NULL) {
                sgl_obj_set_dirty(s_dash.gauge_row_flush);
            }
        }
    }

    if (s_last_ms != 0u && (t - s_last_ms) < 150u)
        return;

    uint32_t dt = (s_last_ms == 0u) ? 0u : (t - s_last_ms);
    s_last_ms   = t;
    s_dash_trip_m += (dt > 500u) ? 1u : (dt / 5u); /* 约 200m/s 量级演示 */

    /* 压力 0.0~9.9 MPa */
    {
        unsigned v100 = (unsigned)((t / 90u) % 100u);
        unsigned d0 = v100 / 10u; /* 0~9 */
        unsigned d1 = v100 % 10u; /* 0~9 */
        s_dash_buf_mpa[0] = (char)('0' + d0);
        s_dash_buf_mpa[1] = '.';
        s_dash_buf_mpa[2] = (char)('0' + d1);
        s_dash_buf_mpa[3] = '\0';
        if (!s_blue_mode) {
            sgl_label_set_text(s_dash.lbl_mpa, s_dash_buf_mpa);
            sgl_obj_set_dirty(s_dash.lbl_mpa);
        }
    }

    /* 电压 10.0~15.0 V */
    {
        unsigned idx = (unsigned)((t / 110u) % 51u); /* 0~50 */
        unsigned v10 = 100u + idx;                   /* 100~150 => 10.0~15.0 */
        unsigned ip = v10 / 10u; /* 10~15 */
        unsigned fp = v10 % 10u; /* 0~9 */
        s_dash_buf_v[0] = (char)('0' + (ip / 10u));
        s_dash_buf_v[1] = (char)('0' + (ip % 10u));
        s_dash_buf_v[2] = '.';
        s_dash_buf_v[3] = (char)('0' + fp);
        s_dash_buf_v[4] = '\0';
        if (!s_blue_mode) {
            sgl_label_set_text(s_dash.lbl_v, s_dash_buf_v);
            sgl_obj_set_dirty(s_dash.lbl_v);
        }
    }

    /* 里程：演示累加总米数，拆成 km + m */
    {
        uint32_t m = s_dash_trip_m;
        unsigned km = (unsigned)(m / 1000u);
        unsigned rm = (unsigned)(m % 1000u);
        /* km 后缀：Km */
        u_to_dec_str(km, s_dash_buf_trip_km, sizeof(s_dash_buf_trip_km));
        str_append_ascii(s_dash_buf_trip_km, sizeof(s_dash_buf_trip_km), "Km");
        u_to_dec_str(rm, s_dash_buf_trip_m, sizeof(s_dash_buf_trip_m));
        str_append_ascii(s_dash_buf_trip_m, sizeof(s_dash_buf_trip_m), "m");
        if (!s_blue_mode) {
            dash_label_text_only(s_dash.lbl_trip_km, s_dash_buf_trip_km);
            dash_label_text_only(s_dash.lbl_trip_m, s_dash_buf_trip_m);
            if (s_dash.trip_row_flush != NULL) {
                sgl_obj_set_dirty(s_dash.trip_row_flush);
            }
        }
    }

    if (s_blue_mode)
        blue_params_values_refresh(t);
}

/**
 * @brief 系统级初始化：时钟、GPIO、UART0、SPI1 及 ILI9341 控制引脚
 * @note  SPI1：PE0 MOSI, PE1 MISO, PH8 CLK；PH9 为 GPIO 片选（勿用 SPI1_SS 复用，见下方注释）；
 *        DC=PH10, EN(背光)=PH11, RST=PD14。开机图与仪表背景使用 nuvoton_logo.c / instrument-bag.c 内置位图，不再初始化 QSPI 外部 Flash。
 */
void SYS_Init(void)
{
    /*-------- 总线时钟 --------*/
    /* PCLK0/PCLK1 为 HCLK 的 1/2，供外设使用 */
    CLK->PCLKDIV = (CLK_PCLKDIV_APB0DIV_DIV2 | CLK_PCLKDIV_APB1DIV_DIV2);

    /* 内核时钟 180MHz（与 CLK 驱动约定一致；若改板级晶振/PLL 请同步） */
    CLK_SetCoreClock(FREQ_180MHZ);

    /*-------- GPIO 时钟 --------*/
    /* 使能所有 GPIO 组时钟，以便使用 PA、PB、PH 等引脚 */
    CLK->AHBCLK0 |= CLK_AHBCLK0_GPACKEN_Msk | CLK_AHBCLK0_GPBCKEN_Msk | CLK_AHBCLK0_GPCCKEN_Msk | CLK_AHBCLK0_GPDCKEN_Msk |
                    CLK_AHBCLK0_GPECKEN_Msk | CLK_AHBCLK0_GPFCKEN_Msk | CLK_AHBCLK0_GPGCKEN_Msk | CLK_AHBCLK0_GPHCKEN_Msk;

    /*-------- UART0：SGL 日志与（可选）样例横幅 --------*/
    CLK_EnableModuleClock(UART0_MODULE);
    /* 时钟源 HIRC，分频 1 */
    CLK_SetModuleClock(UART0_MODULE, CLK_CLKSEL1_UART0SEL_HIRC, CLK_CLKDIV0_UART0(1));

    /*-------- SPI1：用于 ILI9341 数据/命令 --------*/
    CLK_EnableModuleClock(SPI1_MODULE);
    CLK_SetModuleClock(SPI1_MODULE, CLK_CLKSEL2_SPI1SEL_PCLK0, MODULE_NoMsk);

    SET_UART0_RXD_PB12();
    SET_UART0_TXD_PB13();
    SET_SPI1_MOSI_PE0();
    SET_SPI1_MISO_PE1();
    SET_SPI1_CLK_PH8();
    /* 片选必须用 GPIO 手动控制，勿复用为 SPI1_SS（见 tft.c 注释） */
    GPIO_SetMode(PH, BIT9, GPIO_MODE_OUTPUT);
    PH9 = 1;  /* CS 空闲高 */

    GPIO_SetMode(PH, BIT10, GPIO_MODE_OUTPUT); /* DC */
    GPIO_SetMode(PH, BIT11, GPIO_MODE_OUTPUT); /* 背光 EN */
    GPIO_SetMode(PD, BIT14, GPIO_MODE_OUTPUT); /* RST */
    PH11 = 1;  /* 背光默认打开 */

    /* PH8 clock 施密特，提升高速边沿 */
    PH->SMTEN |= GPIO_SMTEN_SMTEN8_Msk;

    /* SPI1：主机、模式 0、8bit；MSB 先出；关闭自动 SS，手动拉片选 */
    SPI_Open(SPI1, SPI_MASTER, SPI_MODE_0, 8, 60000000);
    SPI_SET_MSB_FIRST(SPI1);
    SPI_DisableAutoSS(SPI1);
}

/**
 * @brief 使能 SysTick 为 1ms 周期中断，用于驱动 SGL 的 sgl_tick_inc(1)
 * @note  SystemCoreClock 在 CLK_SetCoreClock 后由 BSP 维护，单位 Hz
 */
static void enable_systick_for_sgl(void)
{
    /* 重载值 = 1ms 对应的周期数 - 1；VAL 清 0 从 0 开始计数 */
    SysTick->LOAD = SystemCoreClock / 1000 - 1;
    SysTick->VAL  = 0;
    /* 使用内核时钟、使能中断、使能计数器 */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                    SysTick_CTRL_TICKINT_Msk   |
                    SysTick_CTRL_ENABLE_Msk;

    /* 确保全局中断打开，否则 SysTick 中断可能不会进入，导致 SGL 动画不刷新 */
    __enable_irq();
}

/**
 * @brief SysTick 中断服务函数：每 1ms 调用一次，给 SGL 提供时间基准
 * @note  动画、定时等依赖 sgl_system.tick_ms，必须定期调用 sgl_tick_inc
 */
void SysTick_Handler(void)
{
    /* 读 CTRL 可清除 COUNTFLAG，避免重复进入的误判 */
    (void)SysTick->CTRL;
    sgl_tick_inc(1);
}

/**
 * @brief 程序入口
 */
int main(void)
{
    /* 写时钟等受保护寄存器前需解锁 */
    SYS_UnlockReg();

    /* 完成时钟、GPIO、UART0、SPI1 及 ILI9341 引脚配置 */
    SYS_Init();

    /* 电阻式 ADC 触摸：EADC0 + PB2~PB5（YD/XR/YU/XL） */
    adc_res_touch_hw_init();

    /* 可选：重新上锁以增强安全性 */
    SYS_LockReg();

    /* 打开 UART0：SGL 日志用 uart_puts；勿依赖 printf，以免链接 printf 格式化库（约 1kB+） */
    UART_Open(UART0, 115200);

#if SGL_SAMPLE_UART_LOG
    uart_puts("\n\n+-----------------------------------------------------+\n");
    uart_puts("|               SGL on CM3034 + ILI9341              |\n");
    uart_puts("+-----------------------------------------------------+\n");
#endif

    /*-------- SGL 接口 1：注册日志输出 --------*/
    /* SGL 内部 log 会通过 uart_puts 发到串口，便于调试 */
    sgl_logdev_register(uart_puts);

    /*-------- SGL 接口 2：注册帧缓冲与刷屏回调 --------*/
    /* 分辨率、缓冲指针与大小、以及“把某块区域像素刷到屏”的回调 */
    sgl_fbinfo_t fbinfo = {
        .xres        = PANEL_WIDTH,
        .yres        = PANEL_HEIGHT,
        .flush_area  = demo_panel_flush_area,   /* 在 tft.c 中实现 */
        .buffer      = { panel_buffer, },      /* 单缓冲，仅用 buffer[0] */
        .buffer_size = SGL_ARRAY_SIZE(panel_buffer),
    };
    sgl_fbdev_register(&fbinfo);

    /* 初始化 LCD 控制器（SPI + ILI9341 上电与寄存器配置） */
    tft_init();
    /* 立刻清屏到白色：去掉上电后 LCD RAM 的随机内容闪烁 */
    tft_fill_screen_rgb565(0xFFFF);

    /*-------- SGL 接口 3：1ms Tick --------*/
    /* 启动 SysTick，之后 SysTick_Handler 中会持续调用 sgl_tick_inc(1) */
    enable_systick_for_sgl();

    /* 初始化 SGL 内部状态并显示启动 Logo；必须在上述三个接口就绪后调用 */
    if (sgl_init() != 0)
    {
        uart_puts("sgl_init failed!\n");
        while (1);
    }

    /* 开机 logo：显示 nuvoton_logo.c 里的图片，然后淡出 */
    show_boot_logo_image();
    /* logo 树删除后 SGL 帧缓冲与物理屏可能短暂不一致；整屏填白再搭仪表根，减轻复位后偶发左边竖线 */
    tft_fill_screen_rgb565(0xFFFF);

#if SGL_SAMPLE_UART_LOG
    uart_puts("SGL init done, entering main loop.\n");
#endif

    /*-------- 竖屏仪表：数值主循环中 dashboard_update() 动态刷新 --------*/
    sgl_obj_t *root = sgl_rect_create(NULL);
    sgl_obj_set_pos(root, 0, 0);
    sgl_obj_set_size(root, PANEL_WIDTH, PANEL_HEIGHT);
    dash_rect_flat(root);
    sgl_obj_set_dirty(root);

    sgl_obj_t *dash_bg = sgl_rect_create(root);
    sgl_obj_set_pos(dash_bg, 0, 0);
    sgl_obj_set_size(dash_bg, PANEL_WIDTH, PANEL_HEIGHT);
    dash_rect_flat(dash_bg);
    sgl_rect_set_pixmap(dash_bg, &pic1_pixmap);
    sgl_obj_set_dirty(dash_bg);

    /* 透明全宽条：覆盖车速卡区并含其上 0~5 行边距；标脏含 x=0 与顶行，避免行缓冲分条时 (0,0) 白点/左缘错位 */
    sgl_obj_t *gauge_row_flush = sgl_rect_create(root);
    sgl_obj_set_pos(gauge_row_flush, 0, 0);
    sgl_obj_set_size(gauge_row_flush, PANEL_WIDTH, 124);
    dash_rect_flat(gauge_row_flush);
    sgl_rect_set_color(gauge_row_flush, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(gauge_row_flush, SGL_ALPHA_MIN);
    s_dash.gauge_row_flush = gauge_row_flush;

    sgl_obj_t *gauge_card = sgl_rect_create(root);
    sgl_obj_set_pos(gauge_card, 5, 6);
    sgl_obj_set_size(gauge_card, DASH_CARD_FULL_W, 118);
    sgl_rect_set_radius(gauge_card, 14);
    sgl_rect_set_border_width(gauge_card, 0);
    sgl_rect_set_color(gauge_card, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(gauge_card, SGL_ALPHA_MIN); /* 底色透明，显示背景图 */
    sgl_obj_set_dirty(gauge_card);
    dash_draw_box_border(root, 5, 6, DASH_CARD_FULL_W, 118, 2, SGL_COLOR_BLUE);

    /* 速度条：长方形进度块；总宽 174=16*9+15*2，在 DASH_CARD_FULL_W 内水平居中 */
    {
        const int16_t block_w = 9;
        const int16_t gap = 2;
        const int16_t bars_total_w = (int16_t)(DASH_SPEED_BLOCK_COUNT * block_w + (DASH_SPEED_BLOCK_COUNT - 1) * gap);
        const int16_t x0 = (int16_t)((DASH_CARD_FULL_W - bars_total_w) / 2);
        const int16_t y_base = 48;         /* 所有柱子的底边对齐 */
        const int16_t h_min = 8;
        const int16_t h_step = 1;          /* 每个块比左侧高 1px */
        unsigned i;
        for (i = 0; i < DASH_SPEED_BLOCK_COUNT; i++)
        {
            int16_t h = (int16_t)(h_min + (int16_t)i * h_step);
            sgl_obj_t *blk = sgl_rect_create(gauge_card);
            sgl_obj_set_pos(blk, (int16_t)(x0 + (int16_t)i * (block_w + gap)), (int16_t)(y_base - h));
            sgl_obj_set_size(blk, block_w, h);
            sgl_rect_set_radius(blk, 2);
            sgl_rect_set_border_width(blk, 0);
            sgl_rect_set_color(blk, sgl_rgb(0x00, 0x18, 0x38));
            sgl_rect_set_alpha(blk, SGL_ALPHA_MAX);
            sgl_obj_set_dirty(blk);
            s_dash.speed_blocks[i] = blk;
        }
    }

    /* 数据与单位：放到进度条下方中间（与柱区收紧间距） */
    sgl_obj_t *lbl_speed_value = sgl_label_create(gauge_card);
    sgl_obj_set_pos(lbl_speed_value, 0, 52);
    sgl_obj_set_size(lbl_speed_value, DASH_CARD_FULL_W, 28);
    sgl_label_set_text(lbl_speed_value, "0");
    sgl_label_set_text_color(lbl_speed_value, SGL_COLOR_WHITE);
    sgl_label_set_text_align(lbl_speed_value, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_speed_value, &song23);
    sgl_obj_set_dirty(lbl_speed_value);
    s_dash.lbl_speed = lbl_speed_value;

    sgl_obj_t *lbl_speed_unit = sgl_label_create(gauge_card);
    sgl_obj_set_pos(lbl_speed_unit, 0, 78);
    sgl_obj_set_size(lbl_speed_unit, DASH_CARD_FULL_W, 22);
    sgl_label_set_text(lbl_speed_unit, "Km/h");
    sgl_label_set_text_color(lbl_speed_unit, SGL_COLOR_DEEP_PINK);
    sgl_label_set_text_align(lbl_speed_unit, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_speed_unit, &song23);
    sgl_obj_set_dirty(lbl_speed_unit);
    s_dash.lbl_speed_unit = lbl_speed_unit;

    sgl_obj_t *card_mpa = sgl_rect_create(root);
    sgl_obj_set_pos(card_mpa, 5, 126);
    sgl_obj_set_size(card_mpa, 113, 76);
    sgl_rect_set_radius(card_mpa, 10);
    sgl_rect_set_border_width(card_mpa, 0);
    sgl_rect_set_color(card_mpa, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(card_mpa, SGL_ALPHA_MIN);   /* 底色透明，显示背景图 */
    sgl_obj_set_dirty(card_mpa);
    dash_draw_box_border(root, 5, 126, 113, 76, 2, SGL_COLOR_BLUE);
    dash_draw_oil_icon(card_mpa);

    sgl_obj_t *lbl_mpa_unit = sgl_label_create(card_mpa);
    sgl_obj_set_pos(lbl_mpa_unit, 44, 6);
    sgl_obj_set_size(lbl_mpa_unit, 60, 26);
    sgl_label_set_text(lbl_mpa_unit, "MPa");
    sgl_label_set_text_color(lbl_mpa_unit, SGL_COLOR_WHITE);
    sgl_label_set_text_align(lbl_mpa_unit, SGL_ALIGN_LEFT_MID);
    sgl_label_set_font(lbl_mpa_unit, &song23);
    sgl_obj_set_dirty(lbl_mpa_unit);

    sgl_obj_t *lbl_mpa_value = sgl_label_create(card_mpa);
    sgl_obj_set_pos(lbl_mpa_value, 5, 36);
    sgl_obj_set_size(lbl_mpa_value, 100, 34);
    sgl_label_set_text(lbl_mpa_value, "0.0");
    sgl_label_set_text_color(lbl_mpa_value, SGL_COLOR_GOLD);
    sgl_label_set_text_align(lbl_mpa_value, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_mpa_value, &song23);
    sgl_obj_set_dirty(lbl_mpa_value);
    s_dash.lbl_mpa = lbl_mpa_value;

    sgl_obj_t *card_v = sgl_rect_create(root);
    sgl_obj_set_pos(card_v, 121, 126);
    sgl_obj_set_size(card_v, 113, 76);
    sgl_rect_set_radius(card_v, 10);
    sgl_rect_set_border_width(card_v, 0);
    sgl_rect_set_color(card_v, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(card_v, SGL_ALPHA_MIN);     /* 底色透明，显示背景图 */
    sgl_obj_set_dirty(card_v);
    dash_draw_box_border(root, 121, 126, 113, 76, 2, SGL_COLOR_BLUE);
    dash_draw_battery_icon(card_v);

    sgl_obj_t *lbl_v_unit = sgl_label_create(card_v);
    sgl_obj_set_pos(lbl_v_unit, 58, 6);
    sgl_obj_set_size(lbl_v_unit, 38, 26);
    sgl_label_set_text(lbl_v_unit, "V");
    sgl_label_set_text_color(lbl_v_unit, SGL_COLOR_WHITE);
    sgl_label_set_text_align(lbl_v_unit, SGL_ALIGN_LEFT_MID);
    sgl_label_set_font(lbl_v_unit, &song23);
    sgl_obj_set_dirty(lbl_v_unit);

    sgl_obj_t *lbl_v_value = sgl_label_create(card_v);
    sgl_obj_set_pos(lbl_v_value, 5, 36);
    sgl_obj_set_size(lbl_v_value, 100, 34);
    sgl_label_set_text(lbl_v_value, "12.0");
    sgl_label_set_text_color(lbl_v_value, SGL_COLOR_YELLOW_GREEN);
    sgl_label_set_text_align(lbl_v_value, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_v_value, &song23);
    sgl_obj_set_dirty(lbl_v_value);
    s_dash.lbl_v = lbl_v_value;

    /* Trip 行全幅透明标脏条：与 card_trip 同 y/h，含 x=0，减轻 km/m 两标签分块脏区接缝竖线 */
    sgl_obj_t *trip_row_flush = sgl_rect_create(root);
    sgl_obj_set_pos(trip_row_flush, 0, 204);
    sgl_obj_set_size(trip_row_flush, PANEL_WIDTH, 60);
    dash_rect_flat(trip_row_flush);
    sgl_rect_set_color(trip_row_flush, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(trip_row_flush, SGL_ALPHA_MIN);
    s_dash.trip_row_flush = trip_row_flush;

    sgl_obj_t *card_trip = sgl_rect_create(root);
    sgl_obj_set_pos(card_trip, 5, 204);
    sgl_obj_set_size(card_trip, DASH_CARD_FULL_W, 60);
    sgl_rect_set_radius(card_trip, 10);
    sgl_rect_set_border_width(card_trip, 0);
    sgl_rect_set_color(card_trip, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(card_trip, SGL_ALPHA_MIN);  /* 底色透明，显示背景图 */
    sgl_obj_set_dirty(card_trip);
    dash_draw_box_border(root, 5, 204, DASH_CARD_FULL_W, 60, 2, SGL_COLOR_BLUE);

    /* 英文标题：ASCII 字库即可 */
    sgl_obj_t *lbl_trip_title = sgl_label_create(card_trip);
    sgl_obj_set_pos(lbl_trip_title, 8, 4);
    sgl_obj_set_size(lbl_trip_title, 96, 22);
    sgl_label_set_text(lbl_trip_title, "Trip");
    sgl_label_set_text_color(lbl_trip_title, SGL_COLOR_SAND);
    sgl_label_set_text_align(lbl_trip_title, SGL_ALIGN_TOP_LEFT);
    sgl_label_set_font(lbl_trip_title, &song23);
    sgl_obj_set_dirty(lbl_trip_title);

    sgl_obj_t *lbl_trip_km = sgl_label_create(card_trip);
    /* Km 与 m 分区拉开，避免两标签区域重叠（原 88~160 与 140~212 重叠） */
    sgl_obj_set_pos(lbl_trip_km, 80, 30);
    sgl_obj_set_size(lbl_trip_km, 80, 28);
    sgl_label_set_text(lbl_trip_km, "0Km");
    sgl_label_set_text_color(lbl_trip_km, SGL_COLOR_WHITE);
    sgl_label_set_text_align(lbl_trip_km, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_trip_km, &song23);
    sgl_obj_set_dirty(lbl_trip_km);
    s_dash.lbl_trip_km = lbl_trip_km;

    sgl_obj_t *lbl_trip_m = sgl_label_create(card_trip);
    sgl_obj_set_pos(lbl_trip_m, 160, 30);
    sgl_obj_set_size(lbl_trip_m, 60, 28);
    sgl_label_set_text(lbl_trip_m, "0m");
    sgl_label_set_text_color(lbl_trip_m, SGL_COLOR_WHITE);
    sgl_label_set_text_align(lbl_trip_m, SGL_ALIGN_CENTER);
    sgl_label_set_font(lbl_trip_m, &song23);
    sgl_obj_set_dirty(lbl_trip_m);
    s_dash.lbl_trip_m = lbl_trip_m;

    s_dash_trip_m = 0u;
    s_dash_speed_demo = 0u;

    dash_create_bottom_nav_buttons(root, 204, 60);

    /* 全屏运行参数遮罩（叠在仪表上）；隐藏时退回仪表 */
    s_blue_overlay = sgl_rect_create(root);
    sgl_obj_set_pos(s_blue_overlay, 0, 0);
    sgl_obj_set_size(s_blue_overlay, PANEL_WIDTH, PANEL_HEIGHT);
    dash_rect_flat(s_blue_overlay);
    sgl_rect_set_color(s_blue_overlay, SGL_COLOR_BLACK);
    sgl_rect_set_alpha(s_blue_overlay, SGL_ALPHA_MAX);
    sgl_obj_set_hidden(s_blue_overlay);
    sgl_obj_set_dirty(s_blue_overlay);
    blue_overlay_install_ui();

    /* 运行参数页由底部扳手键松开触发；退出在该页右下角返回 */

#if SGL_DASH_TOUCH_DEBUG
    /* 右上角触摸调试：最后创建并置顶，避免被运行参数页挡住；勿设 clickable，以免挡命中 */
    s_dash_touch_dbg_lbl = sgl_label_create(root);
    sgl_obj_set_pos(s_dash_touch_dbg_lbl, (int16_t)(PANEL_WIDTH - 124), 2);
    sgl_obj_set_size(s_dash_touch_dbg_lbl, 120, 22);
    sgl_label_set_text(s_dash_touch_dbg_lbl, "x:--- y:--- U");
    sgl_label_set_text_color(s_dash_touch_dbg_lbl, SGL_COLOR_LIME);
    sgl_label_set_font(s_dash_touch_dbg_lbl, &song23);
    sgl_label_set_text_align(s_dash_touch_dbg_lbl, SGL_ALIGN_TOP_RIGHT);
    sgl_obj_set_dirty(s_dash_touch_dbg_lbl);
    sgl_obj_move_top(s_dash_touch_dbg_lbl);
#endif

    sgl_screen_load(root);
    /* 先更新首帧动态数据，再一次性刷整屏，避免“先静态再补刷”造成的叠刷线感 */
    dashboard_update(); /* 首帧即显示动态初值 */
    sgl_task_handler_sync();
    sgl_task_handler_sync(); /* 再跑一帧，让首屏脏区合并与 SPI 刷完，降低首帧残影/竖线 */
    /* 与 touch_blue_exit_to_dashboard 一致再标整屏脏，避免复位后首帧局部脏区未覆盖 (0,0) 残留白点 */
    sgl_obj_set_dirty(sgl_screen_act());
    sgl_task_handler_sync();

    /*-------- 主循环：持续处理 SGL 任务（动画、事件、脏区刷新等） --------*/
    while (1)
    {
        touch_poll_and_feed_sgl();
        dashboard_update();
        sgl_task_handler();
    }
}
