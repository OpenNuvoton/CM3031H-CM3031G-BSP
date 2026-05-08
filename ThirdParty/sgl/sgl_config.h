//********************************************************************
//* SGL Configuration File                                           *
//* You can modify the following parameters according to your needs. *
//********************************************************************

#ifndef  __SGL_CONFIG_H__
#define  __SGL_CONFIG_H__



#define  CONFIG_SGL_FBDEV_PIXEL_DEPTH                      16 
#define  CONFIG_SGL_FBDEV_ROTATION                         0
#define  CONFIG_SGL_FBDEV_RUNTIME_ROTATION                 0  
#define  CONFIG_SGL_USE_FBDEV_VRAM                         0
#define  CONFIG_SGL_SYSTICK_MS                             10 
/* 事件队列：须为 2 的幂；减小可省 RAM，过小可能丢事件 */
#define  CONFIG_SGL_EVENT_QUEUE_SIZE                       16
/* 仪表+蓝屏控件多，过小易合并溢出；行缓冲局部脏区不含 x=0 时会残留边带 */
#define  CONFIG_SGL_DIRTY_AREA_NUM_MAX                     16
#define  CONFIG_SGL_COLOR16_SWAP                           0   
#define  CONFIG_SGL_ANIMATION                              1  
/* 发布版可关调试，省少量 RAM + 减代码体积 */
#define  CONFIG_SGL_DEBUG                                  1 
#define  CONFIG_SGL_LOG_COLOR                              0  
#define  CONFIG_SGL_LOG_LEVEL                              0  
#define  CONFIG_SGL_OBJ_USE_NAME                           0  
/* 字形表用 uint16/uint8 存 bitmap_index 等，减小各字体 font_table 的 RO（本工程字模偏移均 < 64K） */
#define  CONFIG_SGL_FONT_SMALL_TABLE                       1
#define  CONFIG_SGL_FONT_COMPRESSED                        0  
#define  CONFIG_SGL_BOOT_LOGO                              0  
#define  CONFIG_SGL_THEME_DARK                             0      
#define  CONFIG_SGL_HEAP_ALGO                              lwmem  
/* lwmem 池：见 sgl_core.c 静态数组；过小会导致控件/动画创建失败 */
#define  CONFIG_SGL_HEAP_MEMORY_SIZE                       10240  //6912

/* ext_img 可选片上读缓冲；0=不分配 flash_buffer[]，外部 SPI 读走用户回调缓冲 */
#define  CONFIG_SGL_EXT_IMG_BUFFER                         0
#define  CONFIG_SGL_LABEL_ROTATION                         0
#define  CONFIG_SGL_FONT_SONG23                            1      
#define  CONFIG_SGL_FONT_CONSOLAS14                        0      
#define  CONFIG_SGL_FONT_CONSOLAS23                        0      
#define  CONFIG_SGL_FONT_CONSOLAS24                        0      
#define  CONFIG_SGL_FONT_CONSOLAS32                        0      
#define  CONFIG_SGL_FONT_CONSOLAS24_COMPRESS               0   
#define  CONFIG_SGL_FONT_SOURCEHANMONOTC_MEDIUM_24         0
#define  CONFIG_SGL_FONT_MYFONT23                  1
#define  CONFIG_SGL_FONT_SOURCEHANMONOTC_MEDIUM_23         0
/* 未使用：勿编入，否则 MiSans_Medium_23.c 内默认 #define 为 1 会整库进 Flash */
#define  CONFIG_SGL_FONT_MISANS_MEDIUM_23                  0

/* FontAwesome 图标字体（底部按钮、电池、蓝屏返回等） */
#define  CONFIG_SGL_FONT_FONTAWESOME_23                  1
#define  CONFIG_SGL_FONT_ICONFONT_23    1



#endif  //!__SGL_CONFIG_H__
