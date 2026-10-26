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
File        : GUIDRV_Lin_Opt_16.h
Purpose     : Optimized routines, included by GUIDRV_Lin_..._16.c
---------------------------END-OF-HEADER------------------------------
*/

/*********************************************************************
*
*       Static functions
*
**********************************************************************
*/
/*********************************************************************
*
*       _DrawHLineOpt16
*/
static void _DrawHLineOpt16(GUI_DEVICE * pDevice, int y, int x0, int x1) {
  DRIVER_CONTEXT * pContext;
  U32 * pDest;
  U32 Off;
  int RemPixels, RemItems;
  U32 Data, ColorMask;
  LCD_PIXELINDEX ColorIndex;

  pContext   = (DRIVER_CONTEXT *)pDevice->u.pContext;
  ColorIndex = LCD__GetColorIndex();
  Off        = XY2OFF32(pContext->vxSizePhys, x0, y);
  RemPixels  = x1 - x0 + 1;
  pDest      = OFF2PTR32(pContext->VRAMAddr, Off);
  if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
    //
    // First DWORD
    //
    if (x0 & 1) {
      Data = READ_MEM32P(pDest);
      #if (LCD_ENDIAN_BIG == 0)
        Data ^= 0xFFFF0000;
      #else
        Data ^= 0xFFFF;
      #endif
      WRITE_MEM32P(pDest, Data);
      pDest++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    if (RemPixels >= 2) {
      RemItems = RemPixels;
      do {
        Data  = READ_MEM32P(pDest);
        Data ^= 0xFFFFFFFF;
        WRITE_MEM32P(pDest, Data);
        pDest++;
        RemItems -= 2;
      } while (RemItems >= 2);
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      Data = READ_MEM32P(pDest);
      #if (LCD_ENDIAN_BIG == 0)
        Data ^= 0xFFFF;
      #else
        Data ^= 0xFFFF0000;
      #endif
      WRITE_MEM32P(pDest, Data);
    }
  } else {
    //
    // First DWORD
    //
    if (x0 & 1) {
      Data = READ_MEM32P(pDest);
      #if (LCD_ENDIAN_BIG == 0)
        Data &= 0xFFFF;
        Data |= (((U32)ColorIndex) << 16);
      #else
        Data &= 0xFFFF0000;
        Data |= ColorIndex;
      #endif
      WRITE_MEM32P(pDest, Data);
      pDest++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    ColorMask = ColorIndex * 0x00010001;
    if (RemPixels >= 16) {
      RemPixels -= 16;
      RemItems = RemPixels;
      do {
        WRITE_MEM32P(pDest,     ColorMask);
        WRITE_MEM32P(pDest + 1, ColorMask);
        WRITE_MEM32P(pDest + 2, ColorMask);
        WRITE_MEM32P(pDest + 3, ColorMask);
        WRITE_MEM32P(pDest + 4, ColorMask);
        WRITE_MEM32P(pDest + 5, ColorMask);
        WRITE_MEM32P(pDest + 6, ColorMask);
        WRITE_MEM32P(pDest + 7, ColorMask);
        pDest    += 8;
        RemItems -= 16;
      } while (RemItems >= 0);
      RemPixels += 16;
      RemPixels -= (RemPixels >> 4) << 4;
    }
    if (RemPixels >= 2) {
      RemItems = RemPixels;
      do {
        WRITE_MEM32P(pDest, ColorMask);
        pDest++;
        RemItems -= 2;
      } while (RemItems >= 2);
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      Data = READ_MEM32P(pDest);
      #if (LCD_ENDIAN_BIG == 0)
        Data &= 0xFFFF0000;
        Data |= ColorIndex;
      #else
        Data &= 0xFFFF;
        Data |= (((U32)ColorIndex) << 16);
      #endif
      WRITE_MEM32P(pDest, Data);
    }
  }
}

/*********************************************************************
*
*       _DrawVLineOpt16
*/
static void _DrawVLineOpt16(GUI_DEVICE * pDevice, int x0, int y0, int y1) {
  DRIVER_CONTEXT * pContext;
  U32 * pDest;
  U32 Off, OffLine;
  int NumLines;
  U32 Data;
  LCD_PIXELINDEX ColorIndex;

  pContext   = (DRIVER_CONTEXT *)pDevice->u.pContext;
  ColorIndex = LCD__GetColorIndex();
  Off        = XY2OFF32(pContext->vxSizePhys, x0, y0);
  pDest      = OFF2PTR32(pContext->VRAMAddr, Off);
  NumLines   = y1 - y0 + 1;
  OffLine    = pContext->vxSizePhys >> 1;
  if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
    if (x0 & 1) {
      do {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF0000;
        #else
          Data ^= 0xFFFF;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      } while (--NumLines);
    } else {
      do {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF;
        #else
          Data ^= 0xFFFF0000;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      } while (--NumLines);
    }
  } else {
    if (x0 & 1) {
      do {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #else
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      } while (--NumLines);
    } else {
      do {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #else
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      } while (--NumLines);
    }
  }
}

/*********************************************************************
*
*       _FillRectOpt16
*
* Purpose:
*   Optimized filling routine for 16 bpp
*/
static void _FillRectOpt16(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1) {
  DRIVER_CONTEXT * pContext;
  U32 * pDest;
  U32 * pDest0;
  U32 Off, OffLine;
  int RemPixels, NumLines, RemLines, RemItems;
  U32 Data, ColorMask;
  LCD_PIXELINDEX ColorIndex;

  pContext   = (DRIVER_CONTEXT *)pDevice->u.pContext;
  ColorIndex = LCD__GetColorIndex();
  Off        = XY2OFF32(pContext->vxSizePhys, x0, y0);
  pDest0     = OFF2PTR32(pContext->VRAMAddr, Off);
  RemPixels  = x1 - x0 + 1;
  NumLines   = y1 - y0 + 1;
  OffLine    = pContext->vxSizePhys >> 1;
  pDest      = NULL;
  if (GUI_pContext->DrawMode & LCD_DRAWMODE_XOR) {
    //
    // First DWORD
    //
    if (x0 & 1) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF0000;
        #else
          Data ^= 0xFFFF;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
      pDest0++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    if (RemPixels >= 2) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          Data  = READ_MEM32P(pDest);
          Data ^= 0xFFFFFFFF;
          WRITE_MEM32P(pDest, Data);
          pDest++;
          RemItems -= 2;
        } while (RemItems >= 2);
      }
      pDest0 = pDest;
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data ^= 0xFFFF;
        #else
          Data ^= 0xFFFF0000;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
    }
  } else {
    //
    // First DWORD
    //
    if (x0 & 1) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #else
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
      pDest0++;
      RemPixels--;
    }
    //
    // Complete DWORDS
    //
    ColorMask = ColorIndex * 0x00010001;
    if (RemPixels >= 16) {
      RemPixels -= 16;
      
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          WRITE_MEM32P(pDest,     ColorMask);
          WRITE_MEM32P(pDest + 1, ColorMask);
          WRITE_MEM32P(pDest + 2, ColorMask);
          WRITE_MEM32P(pDest + 3, ColorMask);
          WRITE_MEM32P(pDest + 4, ColorMask);
          WRITE_MEM32P(pDest + 5, ColorMask);
          WRITE_MEM32P(pDest + 6, ColorMask);
          WRITE_MEM32P(pDest + 7, ColorMask);
          pDest    += 8;
          RemItems -= 16;
        } while (RemItems >= 0);
      }
      pDest0 = pDest;

      RemPixels += 16;
      RemPixels -= (RemPixels >> 4) << 4;
    }
    if (RemPixels >= 2) {
      for (RemLines = NumLines; RemLines; RemLines--) {
        RemItems = RemPixels;
        pDest    = pDest0 + OffLine * (RemLines - 1);
        do {
          WRITE_MEM32P(pDest, ColorMask);
          pDest++;
          RemItems -= 2;
        } while (RemItems >= 2);
      }
      pDest0 = pDest;
      RemPixels -= (RemPixels >> 1) << 1;
    }
    //
    // Last DWORD
    //
    if (RemPixels > 0) {
      for (RemLines = NumLines, pDest = pDest0; RemLines; RemLines--) {
        Data = READ_MEM32P(pDest);
        #if (LCD_ENDIAN_BIG == 0)
          Data &= 0xFFFF0000;
          Data |= ColorIndex;
        #else
          Data &= 0xFFFF;
          Data |= (((U32)ColorIndex) << 16);
        #endif
        WRITE_MEM32P(pDest, Data);
        pDest += OffLine;
      }
    }
  }
}

/*************************** End of file ****************************/
