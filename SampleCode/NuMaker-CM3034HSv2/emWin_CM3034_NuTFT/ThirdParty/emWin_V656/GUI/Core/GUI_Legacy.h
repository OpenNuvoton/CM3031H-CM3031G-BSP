/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2026  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.56 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws.  Knowledge of the
source code may not be used to write a similar product. This file may
only be used in accordance with the following terms:

The  software has  been  licensed by  SEGGER Software GmbH to Nuvoton
Technology  Corporation   at  the  address:   No. 4, Creation Rd. III,
Hsinchu Science Park,  Taiwan for  the purposes of  creating libraries
for its Arm Cortex-A,  Arm Cortex-M and  Arm9 32-bit microcontrollers,
commercialized and distributed by Nuvoton Technology Corporation under
the  terms and  conditions of an  End User License Agreement  supplied
with the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M, Cortex-A, and ARM9 32-bit series microcontroller designed and manufactured by Nuvoton Technology Corporation.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2026-09-04
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUI_Legacy.h
Purpose     : Legacy code left for compatibility purpose
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_LEGACY_H
#define  GUI_LEGACY_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

#define GUI_DispString_UC                                                     GUI_UC_DispString
#define TOUCH_X_ActivateX                                                     GUI_TOUCH_X_ActivateX
#define TOUCH_X_ActivateY                                                     GUI_TOUCH_X_ActivateY
#define TOUCH_X_Disable                                                       GUI_TOUCH_X_Disable
#define TOUCH_X_MeasureX                                                      GUI_TOUCH_X_MeasureX
#define TOUCH_X_MeasureY                                                      GUI_TOUCH_X_MeasureY
#define GUI_SelLayer                                                          GUI_SelectLayer
#define GUI_MEMDEV_FadeDevices                                                GUI_MEMDEV_FadeInDevices
#define GUI_GCACHE_1_Create(pColorConvAPI)                                    GUI_GCACHE_Create  (GUI_GCACHE_1, pColorConvAPI)
#define GUI_GCACHE_1_CreateEx(LayerIndex, pColorConvAPI)                      GUI_GCACHE_CreateEx(GUI_GCACHE_1, LayerIndex, pColorConvAPI)
#define GUI_GCACHE_4_Create(pColorConvAPI)                                    GUI_GCACHE_Create  (GUI_GCACHE_4, pColorConvAPI)
#define GUI_GCACHE_4_CreateEx(LayerIndex, pColorConvAPI)                      GUI_GCACHE_CreateEx(GUI_GCACHE_4, LayerIndex, pColorConvAPI)
#define GUI_PID_SetInitFunc(x)                                                GUI_SetAfterInitHook(x)
#define GUI_TTF_DestroyCache()                                                GUI_FT_DestroyCache()
#define GUI_TTF_Done()                                                        GUI_FT_Done()
#define GUI_TTF_SetCacheSize(MaxFaces, MaxSizes, MaxBytes)                    GUI_FT_SetCacheSize(MaxFaces, MaxSizes, MaxBytes)
#define GUI_ALLOC_SetAvBlockSize(BlockSize)                                   
#define GUI_Font6x8                                                           GUI_Font6x8_ASCII
#define GUI_Font6x9                                                           GUI_Font6x9_ASCII
#define GUI_Font8x8                                                           GUI_Font8x8_ASCII
#define GUI_Font8x9                                                           GUI_Font8x9_ASCII
#define GUI_Font8x16                                                          GUI_Font8x16_ASCII
#define GUI_Font8x16x1x2                                                      GUI_Font8x16x1x2_ASCII
#define GUI_Font8x16x2x2                                                      GUI_Font8x16x2x2_ASCII
#define GUI_Font8x16x3x3                                                      GUI_Font8x16x3x3_ASCII
#define GUI_Font8x17                                                          GUI_Font8x17_ASCII
#define GUI_Font8x18                                                          GUI_Font8x18_ASCII
#define GUI_AA_DrawPolyOutline                                                GUI_AA_DrawPolygonEx
#define GUI_AA_PreserveTrans(OnOff)                                           GUI_PreserveTrans(OnOff)
#define GUI_AA_DrawPolyOutlineEx(pSrc, NumPoints, Thickness, x, y, pBuffer)   GUI_AA_DrawPolyOutline(pSrc, NumPoints, Thickness, x, y)
#define GUI_MEMDEV_Fill32                                                     GUI_MEMDEV_Fill


/*********************************************************************
*
*       Logging (for debugging primarily)
*/
void GUI__LegacyLogAbstract(void (*pfLog)(const char * s), const char * s, unsigned NumArgs, ...);
void GUI_Log               (const char * s);
void GUI_Log1              (const char * s, I32 p0);
void GUI_Log2              (const char * s, I32 p0, I32 p1);
void GUI_Log3              (const char * s, I32 p0, I32 p1, I32 p2);
void GUI_Log4              (const char * s, I32 p0, I32 p1, I32 p2,I32 p3);
void GUI_Warn              (const char * s);
void GUI_Warn1             (const char * s, I32 p0);
void GUI_Warn2             (const char * s, I32 p0, I32 p1);
void GUI_Warn3             (const char * s, I32 p0, I32 p1, I32 p2);
void GUI_Warn4             (const char * s, I32 p0, I32 p1, I32 p2, I32 p3);
void GUI_ErrorOut          (const char * s);
void GUI_ErrorOut1         (const char * s, I32 p0);
void GUI_ErrorOut2         (const char * s, I32 p0, I32 p1);
void GUI_ErrorOut3         (const char * s, I32 p0, I32 p1, I32 p2);
void GUI_ErrorOut4         (const char * s, I32 p0, I32 p1, I32 p2, I32 p3);


/*********************************************************************
*
*       Function pointers
*/
void GUI_SetFuncFillCircle     (int (* pfFillCircle)(int x0, int y0, int r));
void GUI_SetFuncDrawCircle     (int (* pfDrawCircle)(int x0, int y0, int r));
void GUI_SetFuncDrawLine       (int (* pfDrawLine)(int x0, int y0, int x1, int y1));
void GUI_SetFuncDrawRect       (int (* pfDrawRect)(int x0, int y0, int x1, int y1));
void GUI_SetFuncFillRoundedRect(int (* pfFillRoundedRect)(int x0, int y0, int x1, int y1, int r));
void GUI_SetFuncDrawRoundedRect(int (* pfDrawRoundedRect)(int x0, int y0, int x1, int y1, int r));
void GUI_SetFuncDrawBitmapEx   (int (* pfDrawBitmapEx)(const GUI_BITMAP * pBitmap, int x0, int y0, int xMag, int yMag));

typedef void GUI_DRAWMEMDEV_16BPP_FUNC(void * pDst, const void * pSrc, int xSize, int ySize, int BytesPerLineDst, int BytesPerLineSrc);
typedef void GUI_DRAWBITMAP_FUNC      (int LayerIndex, int x, int y, const void * p, int xSize, int ySize, int BytesPerLine);

#define GUI_DRAWMEMDEV_FUNC GUI_DRAWMEMDEV_16BPP_FUNC

int GUI_SetFuncDrawAlpha(GUI_DRAWMEMDEV_FUNC * pfDrawMemdevFunc, GUI_DRAWBITMAP_FUNC * pfDrawBitmapFunc);
int GUI_SetFuncDrawM565 (GUI_DRAWMEMDEV_FUNC * pfDrawMemdevFunc, GUI_DRAWBITMAP_FUNC * pfDrawBitmapFunc);
int GUI_SetFuncDrawA8   (GUI_DRAWMEMDEV_FUNC * pfDrawMemdevFunc, GUI_DRAWBITMAP_FUNC * pfDrawBitmapFunc);
int GUI_SetFuncDrawAlphaMemdevFunc(GUI_DRAWMEMDEV_FUNC * pfDrawAlphaMemdevFunc);
int GUI_SetFuncDrawAlphaMemdevM565(GUI_DRAWMEMDEV_FUNC * pfDrawAlphaMemdevM565);
int GUI_SetFuncDrawAlphaBitmapFunc(GUI_DRAWBITMAP_FUNC * pfDrawAlphaBitmapFunc);

void GUI_MEMDEV_SetDrawMemdev16bppFunc(GUI_DRAWMEMDEV_16BPP_FUNC * pfDrawMemdev16bppFunc);
void GUI_MEMDEV_SetRotateFuncLR       (int (* pfRotate)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag));
void GUI_MEMDEV_SetRotateFuncHR       (int (* pfRotate)(GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag));
void GUI_MEMDEV_SetBlendFunc          (int (* pfBlend)(GUI_MEMDEV_Handle hMem, GUI_COLOR Color, U8 BlendIntens));

void (* GUI_SetFuncAlphaBlending (void (* pfAlphaBlending)(LCD_COLOR *, LCD_COLOR *, LCD_COLOR *, U32)))
                                                          (LCD_COLOR *, LCD_COLOR *, LCD_COLOR *, U32);
void (* GUI_SetFuncMixColorsBulk (void (* pFunc)(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens)))
                                                (U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens);

void GUI_AA_SetFuncDrawArc        (int (* pfDrawArc)    (int x0, int y0, int rx, int ry, I32 a0, I32 a1));
void GUI_AA_SetFuncDrawCircle     (int (* pfDrawCircle) (int x0, int y0, int r));
void GUI_AA_SetFuncDrawLine       (int (* pfDrawLine)   (int x0, int y0, int x1, int y1));
void GUI_AA_SetFuncDrawPolyOutline(int (* pfDrawPolyOutline)(const GUI_POINT * pSrc, int NumPoints, int Thickness, int x, int y));
void GUI_AA_SetFuncFillCircle     (int (* pfFillCircle) (int x0, int y0, int r));
void GUI_AA_SetFuncFillPolygon    (int (* pfFillPolygon)(const GUI_POINT * pPoints, int NumPoints, int x0, int y0));

// Setting a function for converting a color palette to an array of index values
void GUI_SetFuncGetpPalConvTable(LCD_PIXELINDEX * (* pFunc)(const LCD_LOGPALETTE * pLogPal, const GUI_BITMAP * pBitmap, int LayerIndex));

#if defined(__cplusplus)
}
#endif

#endif   /* ifdef GUI_LEGACY_H */

/*************************** End of file ****************************/
