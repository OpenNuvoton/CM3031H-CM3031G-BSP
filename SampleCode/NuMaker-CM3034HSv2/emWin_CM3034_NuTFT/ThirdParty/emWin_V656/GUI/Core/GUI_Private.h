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
File        : GUI_Private.h
Purpose     : GUI internal declarations
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_PRIVATE_H
#define GUI_PRIVATE_H

#include "GUI.h"
#include "LCD_Protected.h"
#include "GUI_Debug.h"
#if GUI_WINSUPPORT
  #include "WM_GUI.h"
#endif

#include <stdarg.h>

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defaults for config switches
*
**********************************************************************

  The config switches below do not affect the interface in GUI.h and
  are therefor not required to be in GUI.h.
*/

/* Short address area.
   For  most compilers, this is "near" or "__near"
   We do not use this except for some CPUs which we know to always have some
   near memory, because the GUI_Context and some other data will be declared
   to be in this short address (near) memory area as it has a major effect
   on performance.
   Please define in GUIConf.h (if you want to use it)
*/
#ifndef GUI_SADDR
  #define GUI_SADDR
#endif

#ifndef GUI_DEFAULT_FONT
  #define GUI_DEFAULT_FONT    &GUI_Font6x8
#endif

#ifndef GUI_DEFAULT_CURSOR
  #define GUI_DEFAULT_CURSOR  &GUI_CursorArrowM
#endif

#ifndef GUI_DEFAULT_BKCOLOR
  #define GUI_DEFAULT_BKCOLOR GUI_BLACK
#endif

#ifndef GUI_DEFAULT_COLOR
  #define GUI_DEFAULT_COLOR   GUI_WHITE
#endif

/*********************************************************************
*
*       Angles
*
**********************************************************************
*/
#define GUI_45DEG  512
#define GUI_90DEG  (2 * GUI_45DEG)
#define GUI_180DEG (4 * GUI_45DEG)
#define GUI_360DEG (8 * GUI_45DEG)

/*********************************************************************
*
*       Locking checks
*
**********************************************************************
*/
#if defined (WIN32) && defined (_DEBUG) && GUI_OS
  #define GUI_ASSERT_LOCK()    GUITASK_AssertLock()
  #define GUI_ASSERT_NO_LOCK() GUITASK_AssertNoLock()
  void GUITASK_AssertLock(void);
  void GUITASK_AssertNoLock(void);
#else
  #define GUI_ASSERT_LOCK()
  #define GUI_ASSERT_NO_LOCK()
#endif

/*********************************************************************
*
*       Division tables
*
**********************************************************************
*/
extern const U8 GUI__aConvert_15_255[(1 << 4)];
extern const U8 GUI__aConvert_31_255[(1 << 5)];
extern const U8 GUI__aConvert_63_255[(1 << 6)];
extern const U8 GUI__aConvert_255_15[(1 << 8)];
extern const U8 GUI__aConvert_255_31[(1 << 8)];
extern const U8 GUI__aConvert_255_63[(1 << 8)];

/*********************************************************************
*
*       Usage internals
*
**********************************************************************
*/
typedef GUI_HMEM GUI_USAGE_Handle;
typedef struct tsUSAGE_APIList tUSAGE_APIList;
typedef struct GUI_Usage GUI_USAGE;
#define GUI_USAGE_h GUI_USAGE_Handle

typedef GUI_USAGE_h tUSAGE_CreateCompatible(GUI_USAGE * p);
typedef void        tUSAGE_AddPixel        (GUI_USAGE * p, int x, int y);
typedef void        tUSAGE_AddHLine        (GUI_USAGE * p, int x0, int y0, int len);
typedef void        tUSAGE_Clear           (GUI_USAGE * p);
typedef void        tUSAGE_Delete          (GUI_USAGE_h h);
typedef int         tUSAGE_GetNextDirty    (GUI_USAGE * p, int * pxOff, int yOff);
#define GUI_USAGE_LOCK_H(h) ((GUI_USAGE *)GUI_LOCK_H(h))

void GUI_USAGE_DecUseCnt(GUI_USAGE_Handle  hUsage);

GUI_USAGE_Handle GUI_USAGE_BM_Create(int x0, int y0, int xsize, int ysize, int Flags);
void    GUI_USAGE_Select(GUI_USAGE_Handle hUsage);
void    GUI_USAGE_AddRect(GUI_USAGE * pUsage, int x0, int y0, int xSize, int ySize);
#define GUI_USAGE_AddPixel(p, x,y)            p->pAPI->pfAddPixel(p,x,y)
#define GUI_USAGE_AddHLine(p,x,y,len)         p->pAPI->pfAddHLine(p,x,y,len)
#define GUI_USAGE_Clear(p)                    p->pAPI->pfClear(p)
#define GUI_USAGE_Delete(p)                   p->pAPI->pfDelete(p)
#define GUI_USAGE_GetNextDirty(p,pxOff, yOff) p->pAPI->pfGetNextDirty(p,pxOff, yOff)

struct tsUSAGE_APIList {
  tUSAGE_AddPixel         * pfAddPixel;
  tUSAGE_AddHLine         * pfAddHLine;
  tUSAGE_Clear            * pfClear;
  tUSAGE_CreateCompatible * pfCreateCompatible;
  tUSAGE_Delete           * pfDelete;
  tUSAGE_GetNextDirty     * pfGetNextDirty;
} ;

struct GUI_Usage {
  I16P x0, y0, XSize, YSize;
  const tUSAGE_APIList * pAPI;
  I16 UseCnt;
};

/*********************************************************************
*
*       GUI_DEVICE
*
**********************************************************************
*/
//
// API for routing to next device
//
void           GUI_DEVICE_NEXT__DrawBitmap   (GUI_DEVICE * pDevice, int x0, int y0, int xsize, int ysize, int BitsPerPixel, int BytesPerLine, const U8 * pData, int Diff, const LCD_PIXELINDEX * pTrans);
void           GUI_DEVICE_NEXT__DrawHLine    (GUI_DEVICE * pDevice, int x0, int y, int x1);
void           GUI_DEVICE_NEXT__DrawVLine    (GUI_DEVICE * pDevice, int x, int y0, int y1);
void           GUI_DEVICE_NEXT__FillRect     (GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1);
LCD_PIXELINDEX GUI_DEVICE_NEXT__GetPixelIndex(GUI_DEVICE * pDevice, int x, int y);
void           GUI_DEVICE_NEXT__SetPixelIndex(GUI_DEVICE * pDevice, int x, int y, LCD_PIXELINDEX PixelIndex);
void           GUI_DEVICE_NEXT__XorPixel     (GUI_DEVICE * pDevice, int x, int y);
void           GUI_DEVICE_NEXT__SetOrg       (GUI_DEVICE * pDevice, int x, int y);
void        (* GUI_DEVICE_NEXT__GetDevFunc   (GUI_DEVICE ** ppDevice, int Index))(void);
I32            GUI_DEVICE_NEXT__GetDevProp   (GUI_DEVICE * pDevice, int Index);
const void   * GUI_DEVICE_NEXT__GetDevData   (GUI_DEVICE * pDevice, int Index);
void           GUI_DEVICE_NEXT__GetRect      (GUI_DEVICE * pDevice, LCD_RECT * pRect);

/*********************************************************************
*
*       GUI_MEMDEV
*
**********************************************************************
*/
#if GUI_SUPPORT_MEMDEV

typedef struct {
  GUI_DEVICE * pDevice;
  I16P                   x0, y0, XSize, YSize;
  unsigned               BytesPerLine;
  unsigned               BitsPerPixel;
  GUI_HMEM               hUsage;
} GUI_MEMDEV;

typedef struct {
  GUI_USAGE Public;
  struct {
    int BytesPerLine;
  } Private;
} GUI_USAGE_BM;

#define GUI_MEMDEV_LOCK_H(h) ((GUI_MEMDEV *)GUI_LOCK_H(h))

void              GUI_MEMDEV__SetAlphaCallback   (unsigned(* pcbSetAlpha)(U8));
GUI_MEMDEV_Handle GUI_MEMDEV__CreateFixed        (int x0, int y0, int xSize, int ySize, int Flags, const GUI_DEVICE_API * pDeviceAPI, const LCD_API_COLOR_CONV * pColorConvAPI);
void              GUI_MEMDEV__DrawSizedAt        (GUI_MEMDEV_Handle hMem, int xPos, int yPos, int xSize, int ySize);
GUI_MEMDEV_Handle GUI_MEMDEV__GetEmptyCopy32     (GUI_MEMDEV_Handle hMem, int * pxSize, int * pySize, int * pxPos, int * pyPos);
GUI_MEMDEV_Handle GUI_MEMDEV__GetEmptyCopy       (GUI_MEMDEV_Handle hMem);
void              GUI_MEMDEV__ReadLine           (int x0, int y, int x1, LCD_PIXELINDEX * pBuffer);
void              GUI_MEMDEV__WriteToActiveAlpha (GUI_MEMDEV_Handle hMem,int x, int y);
void              GUI_MEMDEV__WriteToActiveAt    (GUI_MEMDEV_Handle hMem,int x, int y);
void              GUI_MEMDEV__WriteToActiveOpaque(GUI_MEMDEV_Handle hMem,int x, int y);
void            * GUI_MEMDEV__XY2PTR             (int x,int y);
void            * GUI_MEMDEV__XY2PTREx           (GUI_MEMDEV * pDev, int x,int y);
void            * GUI_MEMDEV__XY2PTRStride       (int x, int y, int * pBytesPerLine);
void              GUI_MEMDEV__BlendColor32       (GUI_MEMDEV_Handle hMem, U32 BlendColor, U8 BlendIntens);

unsigned GUI__AlphaPreserveTrans(int OnOff);

extern unsigned GUI_MEMDEV__TimePerFrame;

#define GUI_TIME_PER_FRAME (GUI_TIMER_TIME)GUI_MEMDEV__TimePerFrame

#define GUI_POS_AUTO -4095   /* Position value for auto-pos */

#endif

/*********************************************************************
*
*       LCD_HL_ level defines
*
**********************************************************************
*/
#define LCD_HL_DrawHLine             GUI_pContext->pLCD_HL->pfDrawHLine
#define LCD_HL_DrawPixel             GUI_pContext->pLCD_HL->pfDrawPixel

/*********************************************************************
*
*       Helper functions
*
**********************************************************************
*/
#define GUI_ZEROINIT(Obj) GUI__MEMSET(Obj, 0, sizeof(Obj))
int  GUI_cos(int angle);
int  GUI_sin(int angle);
extern const U32 GUI_Pow10[10];

/* Multi-touch */
void GUI_MTOUCH__ManagePID(int OnOff);

/* Anti-aliased drawing */
int  GUI_AA_Init       (int x0, int x1);
int  GUI_AA_Init_HiRes (int x0, int x1);
void GUI_AA_Exit       (void);
I16  GUI_AA_HiRes2Pixel(int HiRes);

void GL_DrawCircleAA_HiRes(int x0, int y0, int r);
void GL_FillCircleAA_HiRes (int x0, int y0, int r);
void GL_FillEllipseAA_HiRes(int x0, int y0, int rx, int ry);
void GL_FillEllipseAA_HiRes_XL(int x0, int y0, int rx, int ry);
void GL_DrawEllipseAA_HiRes(int x0, int y0, int rx, int ry);

void GUI_AA__DrawCharAA2(int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8 * pData);
void GUI_AA__DrawCharAA4(int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8 * pData);
void GUI_AA__DrawCharAA8(int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8 * pData);

typedef void         GUI_DRAWCHAR_FUNC   (int x0, int y0, int XSize, int YSize, int BytesPerLine, const U8 * pData);
typedef const void * GUI_FINDCHAR_FUNC   (const void * pProp, U16 c);
typedef const void * GUI_GETCHARINFO_FUNC(const void * pProp, U16 c);

typedef struct {
  GUI_FINDCHAR_FUNC    * pfFindChar;
  GUI_GETCHARINFO_FUNC * pfGetCharInfo;
  GUI_DRAWCHAR_FUNC    * pfDraw;
  int                    BitsPerPixel;
  GUI_BOOLEAN            IsExtended : 1;
  GUI_BOOLEAN            IsFramed   : 1;
} GUI_DISPCHAR_API;

typedef void GUI_DRAWCHAR_HL_FUNC(int x0, int y0, int XSize, int YSize, int xDist, int BytesPerLine, const U8 * pData, const GUI_DISPCHAR_API * pAPI);

void GUI__DispChar   (U16 c, const GUI_FONT * pFont, const GUI_DISPCHAR_API * pAPI, GUI_DRAWCHAR_HL_FUNC * pfDrawChar);
void GUI__DrawChar   (int x, int y, int xSize, int ySize, int xDist, int BytesPerLine, const U8 * pData, const GUI_DISPCHAR_API * pAPI);
void GUI_AA__DrawChar(int x, int y, int xSize, int ySize, int xDist, int BytesPerLine, const U8 * pData, const GUI_DISPCHAR_API * pAPI);

/* Default routine for drawing string characters with EXT fonts */
U16 GUI__DrawCharEXT(int RemChars, const char ** ps);

/* Alpha blending helper functions */
#define GUI_ALPHABLENDING_DONE  (1 << 0)

int      GUI__GetAlphaBuffer    (U32 ** ppCurrent, U32 ** ppConvert, U32 ** ppData, int * pVXSizeMax);
int      GUI__AllocAlphaBuffer  (int AllocDataBuffer);
U32    * GUI__DoAlphaBlending   (int x, int y, U32 * pData, int xSize, tLCDDEV_Index2Color * pfIndex2Color_DEV, int * pDone);
unsigned GUI__SetAlphaBufferSize(int xSize);

/* System independent font routines */
int          GUI_SIF__GetCharDistX         (U16P c, int * pSizeX);
void         GUI_SIF__GetFontInfo          (const GUI_FONT * pFont, GUI_FONTINFO * pfi);
char         GUI_SIF__IsInFont             (const GUI_FONT * pFont, U16 c);
const U8   * GUI_SIF__GetpCharInfo         (const GUI_FONT * pFont, U16P c, unsigned SizeOfCharInfo);
int          GUI_SIF__GetNumCharAreas      (const GUI_FONT * pFont);
int          GUI_SIF__GetCharDistX_ExtFrm  (U16P c, int * pSizeX);
void         GUI_SIF__GetFontInfo_ExtFrm   (const GUI_FONT * pFont, GUI_FONTINFO * pfi);
void         GUI_SIF__GetFontInfo_Frm      (const GUI_FONT * pFont, GUI_FONTINFO * pfi);
char         GUI_SIF__IsInFont_ExtFrm      (const GUI_FONT * pFont, U16 c);
int          GUI_SIF__GetCharInfo_ExtFrm   (U16P c, GUI_CHARINFO_EXT * pInfo);
void         GUI_SIF__ClearLine_ExtFrm     (const char * s, int Len);
int          GUI_SIF__GetStringDistX_ExtFrm(const char * s, int Len);
void         GUI_SIF__DispLine_ExtFrm      (const char * s, int Len);
const void * GUI_SIF__FindChar             (const void * pProp, U16 c);
const void * GUI_SIF__FindCharExt          (const void * pProp, U16 c);
const void * GUI_SIF__GetCharInfo          (const void * pCharInfo, U16 c);
const void * GUI_SIF__GetCharInfoExt       (const void * pCharInfo, U16 c);
void         GUI_SIF__SetDispCharAPI       (GUI_DISPCHAR_API * pAPI, GUI_DRAWCHAR_FUNC * pfDraw, int BitsPerPixel);
void         GUI_SIF__SetDispCharExtAPI    (GUI_DISPCHAR_API * pAPI, GUI_DRAWCHAR_FUNC * pfDraw, int BitsPerPixel);

extern const tGUI_ENC_APIList GUI_SIF__APIList_ExtFrm;

/* External binary font routines */
int        GUI_XBF__GetOff        (const GUI_XBF_DATA * pXBF_Data, unsigned c, U32 * pOff);
int        GUI_XBF__GetOffAndSize (const GUI_XBF_DATA * pXBF_Data, unsigned c, U32 * pOff, U16 * pSize);
int        GUI_XBF__GetCharDistX  (U16P c, int * pSizeX);
void       GUI_XBF__GetFontInfo   (const GUI_FONT * pFont, GUI_FONTINFO * pInfo);
void       GUI_XBF__GetFontInfoFrm(const GUI_FONT * pFont, GUI_FONTINFO * pInfo);
char       GUI_XBF__IsInFont      (const GUI_FONT * pFont, U16 c);
int        GUI_XBF__GetCharInfo   (U16P c, GUI_CHARINFO_EXT * pInfo);
void       GUI_XBF__ClearLine     (const char * s, int Len);
int        GUI_XBF__GetStringDistX(const char * s, int Len);
void       GUI_XBF__DispLine      (const char * s, int Len);

extern const tGUI_ENC_APIList GUI_XBF__APIList;

void GUI_Swap(void * p0, void * p1, size_t Size);

/* Conversion routines */
void GUI_AddHex     (U32 v, U8 Len, char ** ps);
void GUI_AddBin     (U32 v, U8 Len, char ** ps);
void GUI_AddDecMin  (I32 v, char ** ps);
void GUI_AddDecShift(I32 v, U8 Len, U8 Shift, char ** ps);
long GUI_AddSign    (long v, char ** ps);
int  GUI_Long2Len   (I32 v);

#define GUI__P2V(p)    ((U32)((PTR_ADDR)p))
#define GUI__V2P(v)    ((void *)(PTR_ADDR)v)
#define GUI__C2V(p)    ((U32)((PTR_ADDR)p))

void * GUI__C2D(const void * c);
void * GUI__F2D(void (* pFunc)(void));
volatile void * GUI__C2VD(const void * c);

#define GUI_UC__GetCharSize(sText)  GUI_pUC_API->pfGetCharSize(sText)
#define GUI_UC__GetCharCode(sText)  GUI_pUC_API->pfGetCharCode(sText)

int   GUI_UC__CalcSizeOfChar    (U16 Char);
U16   GUI_UC__GetCharCodeInc    (const char ** ps);
U16   GUI_UC__GetNextChar       (void);
int   GUI_UC__NumBytes2NumChars (const char * s, int NumBytes);
int   GUI_UC__NumBytes2NumGlyphs(const char * s, int NumChars);
int   GUI_UC__NumChars2NumBytes (const char * s, int NumChars);
int   GUI_UC__NumGlyphs2NumBytes(const char * s, int NumChars);
int   GUI_UC__GetLigature       (U16 Char, U16 Next, int PrevAffectsJoining);
void  GUI_UC__EnableLigatures   (void);
void  GUI_UC__DisableLigatures  (void);
U16   GUI_UC__GetGlyphCode      (const char * s);
int   GUI_UC__GetGlyphSize      (const char * s);
void  GUI_UC__Warn              (const char * s);

void (* GUI_UC__SetpfWarn(void(* pFuncNew)(const char *)))(const char *);

int  GUI__GetLineNumChars   (const char * s, int MaxNumChars);
int  GUI__GetNumChars       (const char * s);
int  GUI__GetOverlap        (U16 Char);
int  GUI__GetLineDistX      (const char * s, int Len);
int  GUI__GetFontSizeY      (void);
void GUI__GetTextSize       (GUI_SIZE * pSize, const char * s);
int  GUI__HandleEOLine      (const char ** ps);
void GUI__InvertRectColors  (int x0, int y0, int x1, int y1);
void GUI__InvertRectColorsEx(GUI_RECT * pRect);
void GUI__DispLine          (const char * s, int Len, const GUI_RECT * pr);
void GUI__AddSpaceHex       (U32 v, U8 Len, char ** ps);
void GUI__CalcTextRect      (const char * pText, const GUI_RECT * pTextRectIn, GUI_RECT * pTextRectOut, int TextAlign);
int  GUI__IsPointInRect     (GUI_RECT * pRect, int x, int y);

void GUI__DrawNonExistingCharacter(U16 c, LCD_DRAWMODE DrawMode);
int  GUI__GetNonExistingCharWidth (U16 c, int * pSizeX);

void GUI__ClearTextBackground(int xDist, int yDist);

int  GUI__WrapGetNumCharsDisp       (const char * pText, int xSize, GUI_WRAPMODE WrapMode);
int  GUI__WrapGetNumCharsToNextLine (const char * pText, int xSize, GUI_WRAPMODE WrapMode);
int  GUI__WrapGetNumBytesToNextLine (const char * pText, int xSize, GUI_WRAPMODE WrapMode);
void GUI__memset16   (U16 * p, U16 Fill, int NumWords);
int  GUI__strlen     (const char * s);
int  GUI__strcmp     (const char * s0, const char * s1);
int  GUI__strcmp_hp  (GUI_HMEM hs0, const char * s1);
int  GUI__strncasecmp(const char * s1, const char * s2, size_t Length);


/* Get cursor position */
int  GUI__GetCursorPosX     (const char * s, int Index, int MaxNumChars);
int  GUI__GetCursorPosChar  (const char * s, int x, int NumCharsToNextLine);
U16  GUI__GetCursorCharacter(const char * s, int Index, int MaxNumChars, int * pIsRTL);
int  GUI__GetCursorWidth    (const char * s, int Index, int NumChars, int MaxNumChars, int * pIsRTL);

/* Arabic support (tbd) */
U16  GUI__GetPresentationForm     (U16 Char, U16 Next, U16 Prev, int * pIgnoreNext, const char * s);
int  GUI__IsArabicCharacter       (U16 c);

/* BiDi support */
const char * GUI__NOBIDI_Log2VisBuffered   (const char * s, int * pMaxNumChars, int Mode);
int          GUI__NOBIDI_GetCursorPosX     (const char * s, int MaxNumChars, int Index);
int          GUI__NOBIDI_GetCursorPosChar  (const char * s, int MaxNumChars, int x);
U16          GUI__NOBIDI_GetCursorCharacter(const char * s, int Index, int MaxNumChars, int * pIsRTL);
int          GUI__NOBIDI_GetCursorWidth    (const char * s, int Index, int NumChars, int MaxNumChars, int * pIsRTL);
int          GUI__NOBIDI_GetWordWrap       (const char * s, int xSize, int * pxDist);
int          GUI__NOBIDI_GetCharWrap       (const char * s, int xSize);
int          GUI__BIDI2_Log2Vis            (const char * s, int NumChars, char * pBuffer, int BufferSize);
int          GUI__BIDI2_GetCursorPosX      (const char * s, int NumChars, int Index);
int          GUI__BIDI2_GetCursorPosChar   (const char * s, int NumChars, int x);
U16          GUI__BIDI2_GetLogChar         (const char * s, int NumChars, int Index);
int          GUI__BIDI2_GetCharDir         (const char * s, int NumChars, int Index);
int          GUI__BIDI2_IsNSM              (U16 Char);
U16          GUI__BIDI2_GetCursorCharacter (const char * s, int Index, int MaxNumChars, int * pIsRTL);
int          GUI__BIDI2_GetWordWrap        (const char * s, int xSize, int * pxDist);
int          GUI__BIDI2_GetCharWrap        (const char * s, int xSize);
void         GUI__BIDI2_SetBaseDir         (int Dir);
int          GUI__BIDI2_GetBaseDir         (void);
U16          GUI__BIDI2_GetGlyph           (const char * s, int CursorPosByte, int * pByteSize);
const char * GUI__BIDI_Log2VisBuffered     (const char * s, int * pMaxNumChars, int Mode);

// Compatibility with old BiDi code
#define GUI__BIDI_Log2Vis            GUI__BIDI2_Log2Vis
#define GUI__BIDI_GetCursorPosX      GUI__BIDI2_GetCursorPosX
#define GUI__BIDI_GetCursorPosChar   GUI__BIDI2_GetCursorPosChar
#define GUI__BIDI_GetLogChar         GUI__BIDI2_GetLogChar
#define GUI__BIDI_GetCharDir         GUI__BIDI2_GetCharDir
#define GUI__BIDI_IsNSM              GUI__BIDI2_IsNSM
#define GUI__BIDI_GetCursorCharacter GUI__BIDI2_GetCursorCharacter
#define GUI__BIDI_GetWordWrap        GUI__BIDI2_GetWordWrap
#define GUI__BIDI_GetCharWrap        GUI__BIDI2_GetCharWrap
#define GUI__BIDI_SetBaseDir         GUI__BIDI2_SetBaseDir
#define GUI__BIDI_GetBaseDir         GUI__BIDI2_GetBaseDir
#define GUI__BIDI_GetGlyph           GUI__BIDI2_GetGlyph
#define GUI__BIDI_GetCursorWidth     GUI__NOBIDI_GetCursorWidth  // Same logic

extern int GUI__BIDI_Enabled;

/* BiDi-related function pointers */
extern int          (* _pfGUI__BIDI_GetCursorPosX)      (const char * s, int NumChars, int Index);
extern int          (* _pfGUI__BIDI_GetCursorPosChar)   (const char * s, int NumChars, int x);
extern U16          (* _pfGUI__BIDI_GetLogChar)         (const char * s, int NumChars, int Index);
extern int          (* _pfGUI__BIDI_GetCharDir)         (const char * s, int NumChars, int Index);
extern int          (* _pfGUI__BIDI_IsNSM)              (U16 Char);
extern const char * (* GUI_CharLine_pfLog2Vis)          (const char * s, int * pMaxNumChars, int Mode);
extern int          (* GUI__GetCursorPos_pfGetPosX)     (const char * s, int MaxNumChars, int Index);
extern int          (* GUI__GetCursorPos_pfGetPosChar)  (const char * s, int MaxNumChars, int x);
extern U16          (* GUI__GetCursorPos_pfGetCharacter)(const char * s, int MaxNumChars, int Index, int * pIsRTL);
extern int          (* GUI__GetCursorPos_pfGetWidth)    (const char * s, int Index, int NumChars, int MaxNumChars, int * pIsRTL);
extern int          (* GUI__Wrap_pfGetWordWrap)         (const char * s, int xSize, int * pxDist);
extern int          (* GUI__Wrap_pfGetCharWrap)         (const char * s, int xSize);

GUI_BOOLEAN GUI__IsHarfBuzzActive(void);

/* Proportional  font support */
const GUI_FONT_PROP * GUIPROP__FindChar    (const GUI_FONT_PROP * pProp, U16P c);
const GUI_CHARINFO  * GUIPROP__GetpCharInfo(const GUI_FONT_PROP * pProp, U16P c);

/* Extended proportional font support */
extern int GUIPROP_EXT__Index;

//
// Wrapping separators
//
extern const U16 * GUI_Wrap__pSep;
extern int         GUI_Wrap__NumSeps;

const GUI_CHARINFO_EXT  * GUIPROP_EXT__GetpCharInfo(const GUI_FONT_PROP_EXT * pPropExt, U16P c);
const GUI_FONT_PROP_EXT * GUIPROP_EXT__FindChar    (const GUI_FONT_PROP_EXT * pPropExt, U16P c);

void  GUIPROP_EXT__DispLine           (const char * s, int Len);
void  GUIPROP_EXT__ClearLine          (const char * s, int Len);
int   GUIPROP_EXT__GetStringDistX     (const char * s, int Len);
void  GUIPROP_EXT__SetfpClearLine     (void (* fpClearLine)(const char * s, int Len));

/* Reading data routines */
U16 GUI__Read16(const U8 ** ppData);
U32 GUI__Read32(const U8 ** ppData);

/* Virtual screen support */
void GUI__GetOrg(int * px, int * py);

/* Timer support */
int              GUI_TIMER__IsActive       (void);
GUI_TIMER_TIME   GUI_TIMER__GetPeriod      (void);
GUI_TIMER_HANDLE GUI_TIMER__GetFirstTimer  (PTR_ADDR * pContext);
GUI_TIMER_HANDLE GUI_TIMER__GetNextTimerLin(GUI_TIMER_HANDLE hTimer, PTR_ADDR * pContext);
int              GUI_TIMER__IsTimer        (GUI_TIMER_HANDLE hObj);

/* Get function pointers for color conversion */
tLCDDEV_Index2Color * GUI_GetpfIndex2ColorEx(int LayerIndex);
tLCDDEV_Color2Index * GUI_GetpfColor2IndexEx(int LayerIndex);

int GUI_GetBitsPerPixelEx(int LayerIndex);

/*********************************************************************
*
*       Format definitions used by streamed bitmaps
*
*   IMPORTANT: DO NOT CHANGE THESE VALUES!
*   THEY HAVE TO CORRESPOND TO THE DEFINITIONS WITHIN THE CODE OF THE BITMAPCONVERTER!
*/
#define GUI_STREAM_FORMAT_INDEXED    100 /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLE1       32  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLE4       6   /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLE8       7   /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_565        8   /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M565       9   /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_555        10  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M555       11  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLE16      12  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLEM16     13  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_8888       16  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLE32      15  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_24         17  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_A8         33  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_RLEALPHA   18  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_444_12     19  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M444_12    20  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_444_12_1   21  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M444_12_1  22  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_444_16     23  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M444_16    24  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_A555       25  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_AM555      26  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_A565       27  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_AM565      28  /* DO NOT CHANGE */
#define GUI_STREAM_FORMAT_M8888I     29  /* DO NOT CHANGE */

void GUI__ReadHeaderFromStream  (GUI_BITMAP_STREAM * pBitmapHeader, const U8 * pData);
void GUI__CreateBitmapFromStream(const GUI_BITMAP_STREAM * pBitmapHeader, const void * pData, GUI_BITMAP * pBMP, GUI_LOGPALETTE * pPAL, const GUI_BITMAP_METHODS * pMethods);

/* Cache management */
int GUI__ManageCache  (int Cmd);
int GUI__ManageCacheEx(int LayerIndex, int Cmd);

/*********************************************************************
*
*       2d - GL
*
**********************************************************************
*/
void GL_DispChar         (U16 c);
void GL_DrawArc          (int x0, int y0, int rx, int ry, int a0, int a1);
void GL_DrawArcHR        (int x0, int y0, int rx, int ry, I32 a0, I32 a1);
void GL_DrawBitmap       (const GUI_BITMAP * pBM, int x0, int y0);
void GL_DrawCircle       (int x0, int y0, int r);
void GL_DrawEllipse      (int x0, int y0, int rx, int ry, int w);
void GL_DrawEllipseXL    (int xm, int ym, int rx, int ry, int w);
void GL_DrawPolygon      (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GL_DrawPoint        (int x,  int y);
void GL_DrawLine1        (int x0, int y0, int x1, int y1);
void GL_DrawLineTo       (int x,  int y);
void GL_DrawLineToEx     (int x,  int y, unsigned * pPixelCnt);
void GL_DrawLine         (int x0, int y0, int x1, int y1);
void GL_DrawLineEx       (int x0, int y0, int x1, int y1, unsigned * pPixelCnt);
void GL_MoveTo           (int x,  int y);
void GL_FillCircle       (int x0, int y0, int r);
void GL_FillEllipse      (int x0, int y0, int rx, int ry);
void GL_FillEllipseXL    (int x0, int y0, int rx, int ry);
void GL_FillPolygon      (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GL_SetDefault       (void);

/*********************************************************************
*
*       C Standard Library
*
**********************************************************************
*/
extern GUI_STDLIB_INTERFACE GUI__StdLib;
//
// Internal defines
//
#define GUI_memset       GUI__StdLib.pfMemset
#define GUI_memcpy       GUI__StdLib.pfMemcpy
#define GUI_strcmp       GUI__StdLib.pfStrcmp
#define GUI_strlen       GUI__StdLib.pfStrlen
#define GUI_strcpy       GUI__StdLib.pfStrcpy
#define GUI_vsnprintf    GUI__StdLib.pfVsnprintf
#define GUI_strchr       GUI__StdLib.pfStrchr
#define GUI_strrchr      GUI__StdLib.pfStrrchr
#define GUI_strtoul      GUI__StdLib.pfStrtoul
#define GUI_strtol       GUI__StdLib.pfStrtol
#define GUI_strtod       GUI__StdLib.pfStrtod
#define GUI_rand         GUI__StdLib.pfRand
#define GUI_srand        GUI__StdLib.pfSrand
#define GUI_memmove      GUI__StdLib.pfMemmove
#define GUI_memcmp       GUI__StdLib.pfMemcmp
#define GUI_strncpy      GUI__StdLib.pfStrncpy
#define GUI_strncmp      GUI__StdLib.pfStrncmp
#define GUI_sprintf      GUI__StdLib.pfSprintf
#define GUI_snprintf     GUI__StdLib.pfSnprintf
#define GUI_strcat       GUI__StdLib.pfStrcat

//
// Macros for typesave use of function pointers
//
#define GUI__MEMSET(pDest, Fill, Cnt) GUI_memset((void *)(pDest), (int)(Fill), (size_t)(Cnt))
#define GUI__MEMCPY(pDest, pSrc, Cnt) GUI_memcpy((void *)(pDest), (const void *)(pSrc), (size_t)(Cnt))

/*********************************************************************
*
*       Callback pointers for dynamic linkage
*
**********************************************************************
Dynamic linkage pointers reduces configuration hassles.
*/
typedef int  GUI_tfTimer(void);
typedef int  WM_tfHandlePID(void);

/*********************************************************************
*
*       Text rotation
*
**********************************************************************
*/
extern GUI_RECT  GUI_RectDispString; /* Used by LCD_Rotate...() and GUI_DispStringInRect() */

/*********************************************************************
*
*       Flag for setting transparency for 'EXT' fonts
*
**********************************************************************
*/
extern U8 GUI__CharHasTrans;

/*********************************************************************
*
*       Multitasking support
*
**********************************************************************
*/
extern int GUITASK__EntranceCnt;

/*********************************************************************
*
*       Bitmap related functions
*
**********************************************************************
*/
int              GUI_GetBitmapPixelIndex  (const GUI_BITMAP * pBMP, unsigned x, unsigned y);
GUI_COLOR        GUI_GetBitmapPixelColor  (const GUI_BITMAP * pBMP, unsigned x, unsigned y);
int              GUI_GetBitmapPixelIndexEx(int BitsPerPixel, int BytesPerLine, const U8 * pData, unsigned x, unsigned y);
LCD_PIXELINDEX * GUI__GetpPalConvTable    (const LCD_LOGPALETTE * pLogPal, const GUI_BITMAP * pBitmap);
void             GUI__DrawBitmap16bpp     (int x0, int y0, int xsize, int ysize, const U8 * pPixel, const LCD_LOGPALETTE * pLogPal, int xMag, int yMag, tLCDDEV_Index2Color * pfIndex2Color, const LCD_API_COLOR_CONV * pColorConvAPI);
void             GUI__DrawBitmapA16bpp    (int x0, int y0, int xSize, int ySize, const U8 * pPixel, const LCD_LOGPALETTE * pLogPal, int xMag, int yMag, tLCDDEV_Index2Color * pfIndex2Color);
void             GUI__SetPixelAlpha       (int x, int y, U8 Alpha, LCD_COLOR Color);
LCD_COLOR        GUI__MixColors           (LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
void             GUI__MixColorsBulk       (U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens);

extern const GUI_UC_ENC_APILIST GUI_UC_None;

/*********************************************************************
*
*       Instrumentation via SystemView
*
**********************************************************************
*/
/*********************************************************************
*
*       Profile event identifiers
*/
enum {
  //
  // GUI_..
  //
  GUI_EVTID_GUI_DRAWBITMAP = 0,
  GUI_EVTID_GUI_DRAWCIRCLE,
  GUI_EVTID_GUI_DRAWROUNDEDRECT,
  GUI_EVTID_GUI_FILLCIRCLE,
  GUI_EVTID_GUI_FILLROUNDEDRECT,
  GUI_EVTID_GUI_INIT,
  GUI_EVTID_GUI_MEMDEV_DRAW,
  GUI_EVTID_GUI_MULTIBUF_BEGINEX,
  GUI_EVTID_GUI_MULTIBUF_CONFIRMEX,
  GUI_EVTID_GUI_MULTIBUF_ENDEX,
  //
  // GUI_AA_...
  //
  GUI_EVTID_GUI_AA__DRAWCHARAA4,
  GUI_EVTID_GUI_AA__DRAWCHARAA8,
  GUI_EVTID_GUI_AA_DRAWARCHR,
  GUI_EVTID_GUI_AA_DRAWCIRCLE,
  GUI_EVTID_GUI_AA_DRAWLINE,
  GUI_EVTID_GUI_AA_DRAWPOLYOUTLINE,
  GUI_EVTID_GUI_AA_FILLCIRCLE,
  GUI_EVTID_GUI_AA_FILLPOLYGON,
  //
  // GUI__...
  //
  GUI_EVTID_GUI__DISPLINE,
  GUI_EVTID_GUI__DRAWCHAREXT,
  //
  // GL_...
  //
  GUI_EVTID_GL_DRAWBITMAP,
  GUI_EVTID_GL_DRAWLINE,
  //
  // LCD_...
  //
  GUI_EVTID_LCD_DRAWBITMAP,
  GUI_EVTID_LCD_DRAWHLINE,
  GUI_EVTID_LCD_DRAWPIXEL,
  GUI_EVTID_LCD_DRAWVLINE,
  GUI_EVTID_LCD_FILLRECT,
  //
  // WM_...
  //
  GUI_EVTID_WM_EXEC,
  GUI_EVTID_WM__PAINT,
  GUI_EVTID_WM__PAINTWINANDOVERLAYS,
  //
  // Last entry, number of ids.
  //
  GUI_NUM_EVTIDS
};

#define GUI_PROFILE_GET_EVENT_ID(EvtId) ((unsigned)(EvtId) + (unsigned)GUI_pContext->Profile.IdOffset)

/*********************************************************************
*
*       GUI_PROFILE_END_CALL
*/
#if (GUI_SUPPORT_PROFILE != 0) && (GUI_SUPPORT_PROFILE_END_CALL != 0)
  #define GUI_PROFILE_END_CALL(EventId)                                                \
    if (GUI_pContext->Profile.pAPI != NULL) {                                          \
      GUI_pContext->Profile.pAPI->pfRecordEndCall(GUI_PROFILE_GET_EVENT_ID(EventId));  \
    }
#else
  #define GUI_PROFILE_END_CALL(EventId)
#endif

/*********************************************************************
*
*       GUI_PROFILE_END_CALL_U32
*/
#if (GUI_SUPPORT_PROFILE != 0) && (GUI_SUPPORT_PROFILE_END_CALL != 0)
  #define GUI_PROFILE_END_CALL_U32(EventId, Para0)                                                      \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                           \
      GUI_pContext->Profile.pAPI->pfRecordEndCallU32(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0));  \
    }
#else
  #define GUI_PROFILE_END_CALL_U32(EventId, ReturnValue)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_VOID
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_VOID(EventId)                                            \
    if (GUI_pContext->Profile.pAPI != NULL) {                                       \
      GUI_pContext->Profile.pAPI->pfRecordVoid(GUI_PROFILE_GET_EVENT_ID(EventId));  \
    }
#else
  #define GUI_PROFILE_CALL_VOID(EventId)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32(EventId, Para0)                                                   \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                    \
      GUI_pContext->Profile.pAPI->pfRecordU32(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0));  \
    }
#else
  #define GUI_PROFILE_CALL_U32(EventId, Para0)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x2
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x2(EventId, Para0, Para1)                                                          \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                    \
      GUI_pContext->Profile.pAPI->pfRecordU32x2(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x2(Id, Para0, Para1)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x3
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x3(EventId, Para0, Para1, Para2)                                                                 \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                  \
      GUI_pContext->Profile.pAPI->pfRecordU32x3(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x3(EventId, Para0, Para1, Para2)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x4
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x4(EventId, Para0, Para1, Para2, Para3)                                                                        \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                                \
      GUI_pContext->Profile.pAPI->pfRecordU32x4(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x4(EventId, Para0, Para1, Para2, Para3)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x5
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x5(EventId, Para0, Para1, Para2, Para3, Para4)                                                                               \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                                              \
      GUI_pContext->Profile.pAPI->pfRecordU32x5(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x5(EventId, Para0, Para1, Para2, Para3, Para4)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x6
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)                                                                                      \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                                                            \
      GUI_pContext->Profile.pAPI->pfRecordU32x6(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4), (U32)(Para5));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x6(EventId, Para0, Para1, Para2, Para3, Para4, Para5)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_U32x7
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_U32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)                                                                                             \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                                                                          \
      GUI_pContext->Profile.pAPI->pfRecordU32x7(GUI_PROFILE_GET_EVENT_ID(EventId), (U32)(Para0), (U32)(Para1), (U32)(Para2), (U32)(Para3), (U32)(Para4), (U32)(Para5), (U32)(Para6));  \
    }
#else
  #define GUI_PROFILE_CALL_U32x7(EventId, Para0, Para1, Para2, Para3, Para4, Para5, Para6)
#endif

/*********************************************************************
*
*       GUI_PROFILE_CALL_STRING
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_STRING(EventId, pPara0)                                                            \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                 \
      GUI_pContext->Profile.pAPI->pfRecordString(GUI_PROFILE_GET_EVENT_ID(EventId), (const char *)(pPara0));  \
    }
#else
  #define GUI_PROFILE_CALL_STRING(EventId, pPara0)
#endif

/*********************************************************************
*
*       GUI_PROFILE_RECORD_API_STRINGx2
*/
#if GUI_SUPPORT_PROFILE
  #define GUI_PROFILE_CALL_STRINGx2(EventId, pPara0, pPara1)                                                                            \
    if (GUI_pContext->Profile.pAPI != NULL) {                                                                                           \
      GUI_pContext->Profile.pAPI->pfRecordStringx2(GUI_PROFILE_GET_EVENT_ID(EventId), (const char *)(pPara0), (const char *)(pPara1));  \
    }
#else
  #define GUI_PROFILE_CALL_STRINGx2(EventId, pPara0, pPara1)
#endif

/*********************************************************************
*
*       LCDDEV_L0_xxx
*
**********************************************************************
*/
#define LCDDEV_L0_Color2Index         GUI__apDevice[GUI_pContext->SelLayer]->pColorConvAPI->pfColor2Index
#define LCDDEV_L0_Index2Color         GUI__apDevice[GUI_pContext->SelLayer]->pColorConvAPI->pfIndex2Color

#define LCDDEV_L0_DrawBitmap          GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfDrawBitmap
#define LCDDEV_L0_DrawHLine           GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfDrawHLine
#define LCDDEV_L0_DrawVLine           GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfDrawVLine
#define LCDDEV_L0_DrawPixel           GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfDrawPixel
#define LCDDEV_L0_FillRect            GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfFillRect
#define LCDDEV_L0_GetPixel            GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfGetPixel
#define LCDDEV_L0_GetRect             GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfGetRect
#define LCDDEV_L0_GetPixelIndex       GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfGetPixelIndex
#define LCDDEV_L0_SetPixelIndex       GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfSetPixelIndex
#define LCDDEV_L0_XorPixel            GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfXorPixel
#define LCDDEV_L0_GetDevFunc          GUI__apDevice[GUI_pContext->SelLayer]->pDeviceAPI->pfGetDevFunc

void LCD_ReadRect  (int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer, GUI_DEVICE * pDevice);
void GUI_ReadRect  (int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer, GUI_DEVICE * pDevice);
void GUI_ReadRectEx(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer, GUI_DEVICE * pDevice);

void LCD_ReadRectNoClip(int x0, int y0, int x1, int y1, LCD_PIXELINDEX * pBuffer, GUI_DEVICE * pDevice);


/*********************************************************************
*
*       GPU related
*
**********************************************************************
*/
#if (GUI_SUPPORT_GPU)

typedef struct {
  GUI_ERROR   (* pfCallFunc)     (GUI_GPU_FUNCTION_INDEX FuncIndex, va_list Args);
  GUI_BOOLEAN (* pfHasFunc)      (GUI_GPU_FUNCTION_INDEX FuncIndex);
  void        (* pfInvalidateAll)(void);
} GUI_GPU_CORE_API;

extern const GUI_GPU_CORE_API * GUI_GPU__pAPI;

GUI_ERROR   GUI_GPU__CallFunction (int FuncIndex, ...); // FuncIndex needs to be int, not enum to avoid promotion
GUI_BOOLEAN GUI_GPU__HasFunction  (GUI_GPU_FUNCTION_INDEX FuncIndex);
void        GUI_GPU__InvalidateAll(void);

#define GUI_GPU__CallFunction_1(FX,  ARG0)                                                                      \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0)                                                                 
#define GUI_GPU__CallFunction_2(FX,  ARG0, ARG1)                                                                \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1)                                                  
#define GUI_GPU__CallFunction_3(FX,  ARG0, ARG1, ARG2)                                                          \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2)                                   
#define GUI_GPU__CallFunction_4(FX,  ARG0, ARG1, ARG2, ARG3)                                                    \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3)                    
#define GUI_GPU__CallFunction_5(FX,  ARG0, ARG1, ARG2, ARG3, ARG4)                                              \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4)                                                                 
#define GUI_GPU__CallFunction_6(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5)                                        \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5)                                                  
#define GUI_GPU__CallFunction_7(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6)                                  \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6)                                   
#define GUI_GPU__CallFunction_8(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7)                            \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6, (void *)&ARG7)                    
#define GUI_GPU__CallFunction_9(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8)                      \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6, (void *)&ARG7,                    \
                                 (void *)&ARG8)                                                               
#define GUI_GPU__CallFunction_10(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9)               \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6, (void *)&ARG7,                    \
                                 (void *)&ARG8, (void *)&ARG9)                                                
#define GUI_GPU__CallFunction_11(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10)        \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2, (void *)&ARG3,                    \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6, (void *)&ARG7,                    \
                                 (void *)&ARG8, (void *)&ARG9, (void *)&ARG10)
#define GUI_GPU__CallFunction_12(FX,  ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6, ARG7, ARG8, ARG9, ARG10, ARG11) \
  GUI_GPU__CallFunction((int)FX, (void *)&ARG0, (void *)&ARG1, (void *)&ARG2,  (void *)&ARG3,                   \
                                 (void *)&ARG4, (void *)&ARG5, (void *)&ARG6,  (void *)&ARG7,                   \
                                 (void *)&ARG8, (void *)&ARG9, (void *)&ARG10, (void *)&ARG11)

#define _RESOLVE(x)               x
#define _PRIMITIVE_CONCAT(a, b)   a##b
#define _CONCAT_2(a, b)           _PRIMITIVE_CONCAT(a, b)

//
// Dispatch macros, depends on the number of arguments for the given function
//
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COPYRECT               GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COPYBUFFERRECT         GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLCIRCLE             GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWCIRCLE             GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWLINE               GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLRECT               GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLALPHARECT          GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWRECT               GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLROUNDEDRECT        GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWROUNDEDRECT        GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWBITMAPEX_16        GUI_GPU__CallFunction_8
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWBITMAPEX_32        GUI_GPU__CallFunction_8
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWBITMAP_8           GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWBITMAP_16          GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWBITMAP_24          GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWALPHABITMAP_4      GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWALPHABITMAP_8      GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWALPHABITMAP_32     GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_ALPHABLENDING          GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_MIXCOLORSBULK          GUI_GPU__CallFunction_9
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWARCAA              GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWPIEAA              GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLCIRCLEAA           GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWCIRCLEAA           GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWELLIPSEAA          GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLELLIPSEAA          GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWLINEAA             GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWPOLYGONAA          GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLPOLYGONAA          GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWROUNDEDRECTAA      GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_FILLROUNDEDRECTAA      GUI_GPU__CallFunction_5
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_MEMDEV_BLEND           GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_MEMDEV_ROTATE          GUI_GPU__CallFunction_6
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_MEMDEV_BLUR            GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COLORTOINDEX_M1555I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COLORTOINDEX_M565      GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COLORTOINDEX_M4444I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COLORTOINDEX_M888      GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_COLORTOINDEX_M8888I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_INDEXTOCOLOR_M1555I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_INDEXTOCOLOR_M565      GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_INDEXTOCOLOR_M4444I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_INDEXTOCOLOR_M888      GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_INDEXTOCOLOR_M8888I    GUI_GPU__CallFunction_4
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_GETPALCONVTABLE        GUI_GPU__CallFunction_3
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWGRADIENT           GUI_GPU__CallFunction_8
#define _GPU_DISPATCH_GUI_GPU_FUNCTION_DRAWMULTIGRADIENT      GUI_GPU__CallFunction_8

//
// Internal GPU API.
//
#define GUI_GPU_HAS_FUNCTION(FX)                        GUI_GPU__HasFunction(FX)
#define GUI_GPU_CALL_FUNCTION(FX, ...)                  _RESOLVE(_CONCAT_2(_GPU_DISPATCH_, FX) (FX, __VA_ARGS__))
#define GUI_GPU_INVALIDATE_ALL()                        GUI_GPU__InvalidateAll()

#else  // #if (GUI_SUPPORT_GPU)

#define GUI_GPU_HAS_FUNCTION(FX)                        (GUI_FALSE)
#define GUI_GPU_CALL_FUNCTION(FX, ...)                  (GUI_ERROR_NOT_OK)
#define GUI_GPU_INVALIDATE_ALL()

#endif  // #if (GUI_SUPPORT_GPU)

/*********************************************************************
*
*       Generating static initializers (up to 16, expand if required)
*
**********************************************************************
*/
#define _INIT_ARRAY_1(VALUE)                            VALUE
#define _INIT_ARRAY_2(VALUE)     _INIT_ARRAY_1(VALUE),  VALUE
#define _INIT_ARRAY_3(VALUE)     _INIT_ARRAY_2(VALUE),  VALUE
#define _INIT_ARRAY_4(VALUE)     _INIT_ARRAY_3(VALUE),  VALUE
#define _INIT_ARRAY_5(VALUE)     _INIT_ARRAY_4(VALUE),  VALUE
#define _INIT_ARRAY_6(VALUE)     _INIT_ARRAY_5(VALUE),  VALUE
#define _INIT_ARRAY_7(VALUE)     _INIT_ARRAY_6(VALUE),  VALUE
#define _INIT_ARRAY_8(VALUE)     _INIT_ARRAY_7(VALUE),  VALUE
#define _INIT_ARRAY_9(VALUE)     _INIT_ARRAY_8(VALUE),  VALUE
#define _INIT_ARRAY_10(VALUE)    _INIT_ARRAY_9(VALUE),  VALUE
#define _INIT_ARRAY_11(VALUE)    _INIT_ARRAY_10(VALUE), VALUE
#define _INIT_ARRAY_12(VALUE)    _INIT_ARRAY_11(VALUE), VALUE
#define _INIT_ARRAY_13(VALUE)    _INIT_ARRAY_12(VALUE), VALUE
#define _INIT_ARRAY_14(VALUE)    _INIT_ARRAY_13(VALUE), VALUE
#define _INIT_ARRAY_15(VALUE)    _INIT_ARRAY_14(VALUE), VALUE
#define _INIT_ARRAY_16(VALUE)    _INIT_ARRAY_15(VALUE), VALUE

#define GUI_INIT_ARRAY(NUM_ITEMS, VALUE) _CONCAT_2(_INIT_ARRAY_, NUM_ITEMS)(VALUE)

/*********************************************************************
*
*       GUI_DIRTY
*/
typedef struct GUI_DIRTY_DEVICE GUI_DIRTY_DEVICE;

struct GUI_DIRTY_DEVICE {
  GUI_DEVICE       * pDevice;
  void            (* pfAddRect)(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1);
  GUI_DIRTY_DEVICE * pNext;
};

extern void (* GUI__pfAddDirtyRect)(int x0, int y0, int x1, int y1);
extern void (* GUI__pfAddDirtyPoly)(const GUI_POINT * pPoints, int NumPoints, int x, int y);

void GUI_DIRTY__AddDevice   (GUI_DEVICE * pDevice, GUI_DIRTY_DEVICE * pDirty, void (* pfAddRect)(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1));
void GUI_DIRTY__RemoveDevice(GUI_DEVICE * pDevice);

/*********************************************************************
*
*       GUI_DIRTYTILES module
*
**********************************************************************
*/
typedef struct {
  int  (* pfFetch)  (GUI_RECT ** ppRects, U8 ** ppBytes, int LayerIndex);
  void (* pfCleanUp)(int LayerIndex);
  int  (* pfExists) (int LayerIndex);
} GUI_DIRTYTILES_CORE_API;

extern const GUI_DIRTYTILES_CORE_API * GUI_DIRTYTILES__pAPI;

/*********************************************************************
*
*       Internal color management
*
**********************************************************************
*/
typedef struct {
  void         (* pfSetColor)   (LCD_COLOR Index);
  void         (* pfSetBkColor) (LCD_COLOR Index);
  LCD_DRAWMODE (* pfSetDrawMode)(LCD_DRAWMODE dm);
} LCD_SET_COLOR_API;

extern const LCD_SET_COLOR_API * LCD__pSetColorAPI;

#define LCD__SetBkColorIndex(Index) (*GUI_pContext->LCD_pBkColorIndex = Index)
#define LCD__SetColorIndex(Index)   (*GUI_pContext->LCD_pColorIndex   = Index)
#define LCD__GetBkColorIndex()      (*GUI_pContext->LCD_pBkColorIndex)
#define LCD__GetColorIndex()        (*GUI_pContext->LCD_pColorIndex)

/* The following 2 defines are only required for compatibility to older versions of the TTF library */
#define LCD_BKCOLORINDEX (*GUI_pContext->LCD_pBkColorIndex)
#define LCD_COLORINDEX   (*GUI_pContext->LCD_pColorIndex)

//
// Make public on demand
//
typedef enum {
  GUI_COLORFORMAT_1,
  GUI_COLORFORMAT_1_2,
  GUI_COLORFORMAT_1_4,
  GUI_COLORFORMAT_1_5,
  GUI_COLORFORMAT_1_8,
  GUI_COLORFORMAT_1_16,
  GUI_COLORFORMAT_1_24,
  GUI_COLORFORMAT_2,
  GUI_COLORFORMAT_4,
  GUI_COLORFORMAT_5,
  GUI_COLORFORMAT_6,
  GUI_COLORFORMAT_8,
  GUI_COLORFORMAT_16,
  GUI_COLORFORMAT_1616I,
  GUI_COLORFORMAT_111,
  GUI_COLORFORMAT_222,
  GUI_COLORFORMAT_2222,
  GUI_COLORFORMAT_233,
  GUI_COLORFORMAT_323,
  GUI_COLORFORMAT_332,
  GUI_COLORFORMAT_M4444I,
  GUI_COLORFORMAT_444_12,
  GUI_COLORFORMAT_444_12_1,
  GUI_COLORFORMAT_444_16,
  GUI_COLORFORMAT_555,
  GUI_COLORFORMAT_565,
  GUI_COLORFORMAT_666,
  GUI_COLORFORMAT_666_9,
  GUI_COLORFORMAT_666_18,
  GUI_COLORFORMAT_822216,
  GUI_COLORFORMAT_84444,
  GUI_COLORFORMAT_8666,
  GUI_COLORFORMAT_8666_1,
  GUI_COLORFORMAT_88666I,
  GUI_COLORFORMAT_888,
  GUI_COLORFORMAT_8888,
  GUI_COLORFORMAT_8888I,
  GUI_COLORFORMAT_M111,
  GUI_COLORFORMAT_M1555I,
  GUI_COLORFORMAT_M222,
  GUI_COLORFORMAT_M2222I,
  GUI_COLORFORMAT_M233,
  GUI_COLORFORMAT_M323,
  GUI_COLORFORMAT_M332,
  GUI_COLORFORMAT_M444_12,
  GUI_COLORFORMAT_M444_12_1,
  GUI_COLORFORMAT_M444_16,
  GUI_COLORFORMAT_M555,
  GUI_COLORFORMAT_M565,
  GUI_COLORFORMAT_M666,
  GUI_COLORFORMAT_M666_9,
  GUI_COLORFORMAT_M666_18,
  GUI_COLORFORMAT_M8565,
  GUI_COLORFORMAT_M888,
  GUI_COLORFORMAT_M8888,
  GUI_COLORFORMAT_M8888I,
  GUI_COLORFORMAT_INVALID,
} GUI_COLORFORMAT;

GUI_COLORFORMAT GUI_GetColorFormat(const LCD_API_COLOR_CONV * pCC);

/*********************************************************************
*
*       EXTERNs for GL_CORE
*
**********************************************************************
*/
extern const GUI_FONT * GUI__pFontDefault;
extern GUI_COLOR        GUI__ColorDefault;
extern GUI_COLOR        GUI__BkColorDefault;

extern GUI_SADDR GUI_CONTEXT * GUI_pContext;

extern GUI_DEVICE * GUI__apDevice[GUI_NUM_LAYERS];

//
// Function pointer for drawing string characters with EXT fonts
//
extern U16 (* GUI__pfDrawCharEXT)(int RemChars, const char ** ps);
extern int (* GUI__pfGetShiftY)(U16 Code);

//
// Function pointer for mixing up 2 colors
//
extern LCD_COLOR (* LCD__pfMixColors)(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
extern LCD_COLOR (* GUI__pfMixColors)(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);

//
// Function pointer for mixing color and gamma values
//
extern LCD_COLOR (* LCD_AA_pfMixColors16)(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);

extern U8 GUI__DrawStreamedBitmap;

//
// API list to be used for MultiBuffering
//
extern const GUI_MULTIBUF_API    GUI_MULTIBUF_APIList;
extern const GUI_MULTIBUF_API    GUI_MULTIBUF_APIListMasked;
extern const GUI_MULTIBUF_API_EX GUI_MULTIBUF_APIListEx;

extern void (* GUI_pfExecAnimations)(void);
extern int  (* GUI_pfUpdateSoftLayer)(void);

#if GUI_SUPPORT_SIMULATION
  extern void (* GUI_pfSoftlayerGetPixel)(int x, int y, void * p);
#endif

extern void (* GUI_pfHookMTOUCH)(const GUI_MTOUCH_STATE * pState);

extern void (* GUI_pfManageCursor)(int Layer, int OnOff);

extern tGUI_GetGlyph * GUI_UC_pfGetGlyph;

extern const GUI_UC_ENC_APILIST * GUI_pUC_API; /* Unicode encoding API */

extern GUI_SADDR char             GUI_DecChar;
extern           GUI_tfTimer    * GUI_pfTimerExec;
extern           WM_tfHandlePID * WM_pfHandlePID;
extern   void (* GUI_pfDispCharStyle)(U16 Char);
extern   void (* GUI_pfDispCharLine)(int x0);

extern   void (* GUI_pfSetFont)(const GUI_FONT * pOldFont, const GUI_FONT * pNewFont);

extern           int GUI_AA__BufferSize;  // Required buffer size in pixels for alpha blending and/or antialiasing
extern           int GUI_AA__ClipX0;      // x0-clipping value for AA module

extern           I8  GUI__aNumBuffers[GUI_NUM_LAYERS]; // Number of buffers used per layer
extern           U8  GUI__PreserveTrans;
extern           U8  GUI__IsInitialized;

extern           U8  GUI__NumLayersInUse;
extern           U32 GUI__LayerMask;

#if GUI_SUPPORT_ROTATION
  extern const tLCD_APIList * GUI_pLCD_APIList; /* Used for rotating text */
#endif

extern I16 GUI_OrgX, GUI_OrgY;

//
// GUI_DCACHE_...
//
extern void                (* GUI_DCACHE__pfClearCacheHook)(U32 LayerMask);
extern const GUI_DCACHE_API * GUI_DCACHE__pAPI;

#if defined(__cplusplus)
}
#endif

#endif /* GUI_PRIVATE_H */

/*************************** End of file ****************************/
