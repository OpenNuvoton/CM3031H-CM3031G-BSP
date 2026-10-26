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
File        : GUIDRV_UC1698G_Private.h
Purpose     : Private interface of GUIDRV_UC1698G driver
---------------------------END-OF-HEADER------------------------------
*/

#include "GUIDRV_UC1698G.h"
#include "GUIDRV_NoOpt_1_8.h"

#ifndef GUIDRV_UC1698G_PRIVATE_H
#define GUIDRV_UC1698G_PRIVATE_H

#if defined(__cplusplus)
  extern "C" {        // Make sure we have C-declarations in C++ programs
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define UC1698G_MAX_NUM_DUMMY_READS 5
#define UC1698G_NUM_DUMMY_READS     1

#ifndef   LCD_WRITE_BUFFER_SIZE
  #define LCD_WRITE_BUFFER_SIZE 320
#endif

//
// Use unique context identified
//
#define DRIVER_CONTEXT DRIVER_CONTEXT_UC1698G

/*********************************************************************
*
*       SFR definitions
*
**********************************************************************
*/
#define REG_SET_COL_ADDR_LSB      0x00 // Set Column Address LSB
#define REG_SET_COL_ADDR_MSB      0x10 // Set Column Address MSB
#define REG_SET_ROW_ADDR_LSB      0x60 // Set Row    Address LSB
#define REG_SET_ROW_ADDR_MSB      0x70 // Set Row    Address MSB

#define REG_SET_START_COL         0xF4 // Set start column
#define REG_SET_START_ROW         0xF5 // Set start row
#define REG_SET_END_COL           0xF6 // Set end column
#define REG_SET_END_ROW           0xF7 // Set end row

#define REG_DISPLAY_ENABLE        0xA8 // Set display mode

#define REG_DISPLAY_ENABLE_DISPLAY_ON            (1 << 0)
#define REG_DISPLAY_ENABLE_GRAY_SHADE_MODULATION (1 << 1)
#define REG_DISPLAY_ENABLE_GREEN_ENHANCE_DISABLE (1 << 2)

#define REG_WINDOW_MODE           0xF8 // Window mode

#define REG_WINDOW_MODE_OUTSIDE                  (1 << 0)
#define REG_WINDOW_MODE_INSIDE                   (0 << 0)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct DRIVER_CONTEXT DRIVER_CONTEXT;

/*********************************************************************
*
*       DRIVER_CONTEXT_UC1698G
*/
struct DRIVER_CONTEXT {
  //
  // Common data
  //
  U8  aData  [UC1698G_MAX_NUM_DUMMY_READS + 2];
  U16 aData16[UC1698G_MAX_NUM_DUMMY_READS + 2];
  void * pWriteBuffer;
  int xSize, ySize;
  int vxSize, vySize;
  int NumDummyReads;
  int BitsPerPixel;
  int WordsPerLine;
  int IsBigEndian;
  int NoOutput;       // If 1 _SetPixelIndex() of X_5C1 should manage cache only
  //
  // Cache
  //
  U16 * pVRAM;
  int CacheLocked;
  int IsDirty;
  GUI_RECT DirtyRect;
  //
  // Driver specific data
  //
  int FirstSEG;
  int FirstCOM;
  //
  // Hardware routines
  //
  GUI_PORT_API HW_API;
  U16  (* pfReadData)  (DRIVER_CONTEXT * pContext);
  void (* pfWriteData) (DRIVER_CONTEXT * pContext, U16 Data);
  void (* pfWriteReg)  (DRIVER_CONTEXT * pContext, U16 Data);
  void (* pfWriteDataM)(DRIVER_CONTEXT * pContext, U16 Data, U32 NumItems);
  void (* pfWriteDataP)(DRIVER_CONTEXT * pContext, U16 * pData, U32 NumItems);
};

/*********************************************************************
*
*       LOG2PHYS_xxx
*/
#define LOG2PHYS_X      (                  x    )
#define LOG2PHYS_X_OX   (pContext->xSize - x - 1)
#define LOG2PHYS_X_OY   (                  x    )
#define LOG2PHYS_X_OXY  (pContext->xSize - x - 1)
#define LOG2PHYS_X_OS   (                  y    )
#define LOG2PHYS_X_OSX  (pContext->ySize - y - 1)
#define LOG2PHYS_X_OSY  (                  y    )
#define LOG2PHYS_X_OSXY (pContext->ySize - y - 1)

#define LOG2PHYS_Y      (                  y    )
#define LOG2PHYS_Y_OX   (                  y    )
#define LOG2PHYS_Y_OY   (pContext->ySize - y - 1)
#define LOG2PHYS_Y_OXY  (pContext->ySize - y - 1)
#define LOG2PHYS_Y_OS   (                  x    )
#define LOG2PHYS_Y_OSX  (                  x    )
#define LOG2PHYS_Y_OSY  (pContext->xSize - x - 1)
#define LOG2PHYS_Y_OSXY (pContext->xSize - x - 1)

/*********************************************************************
*
*       _SetPixelIndex_##EXT
*/
#define DEFINE_SETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                                                 \
static void _SetPixelIndex_##EXT(GUI_DEVICE * pDevice, int x, int y, LCD_PIXELINDEX PixelIndex) { \
  DRIVER_CONTEXT_UC1698G * pContext;                                                              \
                                                                                                  \
  pContext = (DRIVER_CONTEXT_UC1698G *)pDevice->u.pContext;                                       \
  _SetPixelIndex(pContext, X_PHYS, Y_PHYS, PixelIndex);                                           \
}

/*********************************************************************
*
*       _GetPixelIndex_##EXT
*/
#define DEFINE_GETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                                \
static LCD_PIXELINDEX _GetPixelIndex_##EXT(GUI_DEVICE * pDevice, int x, int y) { \
  DRIVER_CONTEXT_UC1698G * pContext;                                             \
  LCD_PIXELINDEX PixelIndex;                                                     \
                                                                                 \
  pContext = (DRIVER_CONTEXT_UC1698G *)pDevice->u.pContext;                      \
  PixelIndex = _GetPixelIndex(pContext, X_PHYS, Y_PHYS);                         \
  return PixelIndex;                                                             \
}

/*********************************************************************
*
*       _GetDevProp_##EXT
*/
#define DEFINE_GETDEVPROP(EXT, MX, MY, SWAP)                    \
static I32 _GetDevProp_##EXT(GUI_DEVICE * pDevice, int Index) { \
  switch (Index) {                                              \
  case LCD_DEVCAP_MIRROR_X: return MX;                          \
  case LCD_DEVCAP_MIRROR_Y: return MY;                          \
  case LCD_DEVCAP_SWAP_XY:  return SWAP;                        \
  default:                  break;                              \
  }                                                             \
  return _GetDevProp(pDevice, Index);                           \
}

/*********************************************************************
*
*       DEFINE_FUNCTIONS
*/
#define DEFINE_FUNCTIONS(EXT, X_PHYS, Y_PHYS, MX, MY, SWAP) \
  DEFINE_SETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                 \
  DEFINE_GETPIXELINDEX(EXT, X_PHYS, Y_PHYS)                 \
  DEFINE_GETDEVPROP(EXT, MX, MY, SWAP)                      \
  DEFINE_GUI_DEVICE_API(EXT)

/*********************************************************************
*
*       Private functions, common
*
**********************************************************************
*/
void (* GUIDRV_UC1698G__GetDevFunc   (GUI_DEVICE ** ppDevice, int Index))(void);
void    GUIDRV_UC1698G__SetOrg       (GUI_DEVICE *  pDevice,  int x, int y);
I32     GUIDRV_UC1698G__GetDevProp   (GUI_DEVICE *  pDevice,  int Index);
void    GUIDRV_UC1698G__GetRect      (GUI_DEVICE *  pDevice,  LCD_RECT * pRect);
int     GUIDRV_UC1698G__Init         (GUI_DEVICE *  pDevice);
void    GUIDRV_UC1698G__SetAddr      (DRIVER_CONTEXT * pContext, int Col,  int Row);
void    GUIDRV_UC1698G__SetRect      (DRIVER_CONTEXT * pContext, int Col0, int Row0, int Col1, int Row1);
void    GUIDRV_UC1698G__AddDirtyRect (DRIVER_CONTEXT * pContext, int Col0, int Row0, int Col1, int Row1);
void    GUIDRV_UC1698G__AddDirtyPixel(DRIVER_CONTEXT * pContext, int Col,  int Row);
void    GUIDRV_UC1698G__FlushCache   (DRIVER_CONTEXT * pContext);
int     GUIDRV_UC1698G__ControlCache (GUI_DEVICE * pDevice, int Cmd);
void    GUIDRV_UC1698G__Refresh      (GUI_DEVICE * pDevice);

/*********************************************************************
*
*       Simulation (Segger internal use only)
*
**********************************************************************
*/
#if defined(WIN32) && defined(LCD_SIMCONTROLLER)

  extern GUI_PORT_API SIM_UC1698G_HW_API;
  void SIM_UC1698G_Init(GUI_DEVICE * pDevice);

#endif

#if defined(__cplusplus)
  }
#endif

#endif   // GUIDRV_UC1698G_PRIVATE_H

/*************************** End of file ****************************/
