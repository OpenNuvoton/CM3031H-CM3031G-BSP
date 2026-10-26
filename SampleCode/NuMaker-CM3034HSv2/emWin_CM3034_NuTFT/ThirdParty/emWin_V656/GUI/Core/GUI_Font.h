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
File        : GUI_Font.h
Purpose     : Standard fonts
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_FONT_H
#define  GUI_FONT_H

#include "GUI_Type.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Externals
*
**********************************************************************
*/
//
// Proportional fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10S_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10S_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font10_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13HB_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13HB_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13H_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13H_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font13_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_1HK;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_HK;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font20B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font20B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font20_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font20_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_ASCII;

//
// Proportional fonts, framed
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font20F_ASCII;

//
// Monospaced fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font4x6;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font6x8_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font6x8_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font6x9_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font6x9_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x8_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x8_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x9_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x9_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x10_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x12_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x13_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x13_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x15B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x15B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x1x2_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x1x2_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x2x2_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x2x2_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x3x3_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16x3x3_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x17_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x17_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x18_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x18_ASCII;

//
// Digit fonts, proportional
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD64;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD80;

//
// Digit fonts, monospaced
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD24x32;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD36x48;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD48x64;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD60x80;

//
// Comic fonts
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic18B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic18B_ASCII;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic24B_1;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontComic24B_ASCII;

//
// Proportional fonts, antialiased
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font12_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font12_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font48_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font48_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font64_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font64_ASCII_AA4;

//
// Monospaced fonts, antialiased
//
extern GUI_CONST_STORAGE GUI_FONT GUI_Font5x12_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font5x12_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font8x16_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font12x24_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font12x24_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16x32_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16x32_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24x48_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font24x48_ASCII_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32x64_1_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_Font32x64_ASCII_AA4;

//
// Digit fonts, proportional, antialiased
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD32_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD48_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD64_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD80_AA4;

//
// Digit fonts, monospaced, antialiased
//
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD29x32_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD42x48_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD56x64_AA4;
extern GUI_CONST_STORAGE GUI_FONT GUI_FontD71x80_AA4;

/*********************************************************************
*
*       Kerning
*/
extern GUI_CONST_STORAGE U16 aF16_AA4_Kerning[];
extern GUI_CONST_STORAGE U16 aF24_AA4_Kerning[];
extern GUI_CONST_STORAGE U16 aF32_AA4_Kerning[];
extern GUI_CONST_STORAGE U16 aF48_AA4_Kerning[];
extern GUI_CONST_STORAGE U16 aF64_AA4_Kerning[];

extern U32 F16_AA4_Kerning_NumPairs;
extern U32 F24_AA4_Kerning_NumPairs;
extern U32 F32_AA4_Kerning_NumPairs;
extern U32 F48_AA4_Kerning_NumPairs;
extern U32 F64_AA4_Kerning_NumPairs;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// Proportional fonts
//
#define GUI_FONT_8_1             &GUI_Font8_1
#define GUI_FONT_8_ASCII         &GUI_Font8_ASCII
#define GUI_FONT_10S_1           &GUI_Font10S_1
#define GUI_FONT_10S_ASCII       &GUI_Font10S_ASCII
#define GUI_FONT_10_1            &GUI_Font10_1
#define GUI_FONT_10_ASCII        &GUI_Font10_ASCII
#define GUI_FONT_13B_1           &GUI_Font13B_1
#define GUI_FONT_13B_ASCII       &GUI_Font13B_ASCII
#define GUI_FONT_13HB_1          &GUI_Font13HB_1
#define GUI_FONT_13HB_ASCII      &GUI_Font13HB_ASCII
#define GUI_FONT_13H_1           &GUI_Font13H_1
#define GUI_FONT_13H_ASCII       &GUI_Font13H_ASCII
#define GUI_FONT_13_1            &GUI_Font13_1
#define GUI_FONT_13_ASCII        &GUI_Font13_ASCII
#define GUI_FONT_16B_1           &GUI_Font16B_1
#define GUI_FONT_16B_ASCII       &GUI_Font16B_ASCII
#define GUI_FONT_16_1            &GUI_Font16_1
#define GUI_FONT_16_1HK          &GUI_Font16_1HK
#define GUI_FONT_16_ASCII        &GUI_Font16_ASCII
#define GUI_FONT_16_HK           &GUI_Font16_HK
#define GUI_FONT_20B_1           &GUI_Font20B_1
#define GUI_FONT_20B_ASCII       &GUI_Font20B_ASCII
#define GUI_FONT_20_1            &GUI_Font20_1
#define GUI_FONT_20_ASCII        &GUI_Font20_ASCII
#define GUI_FONT_24B_1           &GUI_Font24B_1
#define GUI_FONT_24B_ASCII       &GUI_Font24B_ASCII
#define GUI_FONT_24_1            &GUI_Font24_1
#define GUI_FONT_24_ASCII        &GUI_Font24_ASCII
#define GUI_FONT_32B_1           &GUI_Font32B_1
#define GUI_FONT_32B_ASCII       &GUI_Font32B_ASCII
#define GUI_FONT_32_1            &GUI_Font32_1
#define GUI_FONT_32_ASCII        &GUI_Font32_ASCII

//
// Proportional fonts, framed
//
#define GUI_FONT_20F_ASCII       &GUI_Font20F_ASCII

//
// Monospaced fonts
//
#define GUI_FONT_4X6             &GUI_Font4x6
#define GUI_FONT_6X8             &GUI_Font6x8_ASCII
#define GUI_FONT_6X8_1           &GUI_Font6x8_1
#define GUI_FONT_6X8_ASCII       &GUI_Font6x8_ASCII
#define GUI_FONT_6X9             &GUI_Font6x9_ASCII
#define GUI_FONT_8X8             &GUI_Font8x8_ASCII
#define GUI_FONT_8X8_1           &GUI_Font8x8_1
#define GUI_FONT_8X8_ASCII       &GUI_Font8x8_ASCII
#define GUI_FONT_8X9             &GUI_Font8x9_ASCII
#define GUI_FONT_8X10_ASCII      &GUI_Font8x10_ASCII
#define GUI_FONT_8X12_ASCII      &GUI_Font8x12_ASCII
#define GUI_FONT_8X13_1          &GUI_Font8x13_1
#define GUI_FONT_8X13_ASCII      &GUI_Font8x13_ASCII
#define GUI_FONT_8X15B_1         &GUI_Font8x15B_1
#define GUI_FONT_8X15B_ASCII     &GUI_Font8x15B_ASCI
#define GUI_FONT_8X16            &GUI_Font8x16_ASCII
#define GUI_FONT_8X16_1          &GUI_Font8x16_1
#define GUI_FONT_8X16_ASCII      &GUI_Font8x16_ASCII
#define GUI_FONT_8X16X1X2_1      &GUI_Font8x16x1x2_1
#define GUI_FONT_8X16X1X2_ASCII  &GUI_Font8x16x1x2_ASCII
#define GUI_FONT_8X16X2X2_1      &GUI_Font8x16x2x2_1
#define GUI_FONT_8X16X2X2_ASCII  &GUI_Font8x16x2x2_ASCII
#define GUI_FONT_8X16X3X3_1      &GUI_Font8x16x3x3_1
#define GUI_FONT_8X16X3X3_ASCII  &GUI_Font8x16x3x3_ASCII
#define GUI_FONT_8X17_1          &GUI_Font8x17_1
#define GUI_FONT_8X17_ASCII      &GUI_Font8x17_ASCII
#define GUI_FONT_8X18_1          &GUI_Font8x18_1
#define GUI_FONT_8X18_ASCII      &GUI_Font8x18_ASCII

//
// Digit fonts, proportional
//
#define GUI_FONT_D32             &GUI_FontD32
#define GUI_FONT_D48             &GUI_FontD48
#define GUI_FONT_D64             &GUI_FontD64
#define GUI_FONT_D80             &GUI_FontD80

//
// Digit fonts, monospaced
//
#define GUI_FONT_D24X32          &GUI_FontD24x32
#define GUI_FONT_D36X48          &GUI_FontD36x48
#define GUI_FONT_D48X64          &GUI_FontD48x64
#define GUI_FONT_D60X80          &GUI_FontD60x80

//
// Comic fonts
//
#define GUI_FONT_COMIC18B_1      &GUI_FontComic18B_1
#define GUI_FONT_COMIC18B_ASCII  &GUI_FontComic18B_A
#define GUI_FONT_COMIC24B_1      &GUI_FontComic24B_1
#define GUI_FONT_COMIC24B_ASCII  &GUI_FontComic24B_A

//
// Proportional fonts, antialiased
//
#define GUI_FONT_12_1_AA4        &GUI_Font12_1_AA4
#define GUI_FONT_12_ASCII_AA4    &GUI_Font12_ASCII_AA4
#define GUI_FONT_16_1_AA4        &GUI_Font16_1_AA4
#define GUI_FONT_16_ASCII_AA4    &GUI_Font16_ASCII_AA4
#define GUI_FONT_24_1_AA4        &GUI_Font24_1_AA4
#define GUI_FONT_24_ASCII_AA4    &GUI_Font24_ASCII_AA4
#define GUI_FONT_32_1_AA4        &GUI_Font32_1_AA4
#define GUI_FONT_32_ASCII_AA4    &GUI_Font32_ASCII_AA4
#define GUI_FONT_48_1_AA4        &GUI_Font48_1_AA4
#define GUI_FONT_48_ASCII_AA4    &GUI_Font48_ASCII_AA4
#define GUI_FONT_64_1_AA4        &GUI_Font64_1_AA4
#define GUI_FONT_64_ASCII_AA4    &GUI_Font64_ASCII_AA4

//
// Monospaced fonts, antialiased
//
#define GUI_FONT_5X12_1_AA4      &GUI_Font5x12_1_AA4
#define GUI_FONT_5X12_ASCII_AA4  &GUI_Font5x12_ASCII
#define GUI_FONT_8X16_1_AA4      &GUI_Font8x16_1_AA4
#define GUI_FONT_8X16_ASCII_AA4  &GUI_Font8x16_ASCII
#define GUI_FONT_12X24_1_AA4     &GUI_Font12x24_1_AA
#define GUI_FONT_12X24_ASCII_AA4 &GUI_Font12x24_ASCI
#define GUI_FONT_16X32_1_AA4     &GUI_Font16x32_1_AA
#define GUI_FONT_16X32_ASCII_AA4 &GUI_Font16x32_ASCI
#define GUI_FONT_24X48_1_AA4     &GUI_Font24x48_1_AA
#define GUI_FONT_24X48_ASCII_AA4 &GUI_Font24x48_ASCI
#define GUI_FONT_32X64_1_AA4     &GUI_Font32x64_1_AA
#define GUI_FONT_32X64_ASCII_AA4 &GUI_Font32x64_ASCI

//
// Digit fonts, proportional, antialiased
//
#define GUI_FONT_D32_AA4         &GUI_FontD32_AA4
#define GUI_FONT_D48_AA4         &GUI_FontD48_AA4
#define GUI_FONT_D64_AA4         &GUI_FontD64_AA4
#define GUI_FONT_D80_AA4         &GUI_FontD80_AA4

//
// Digit fonts, monospaced, antialiased
//
#define GUI_FONT_D29X32_AA4      &GUI_FontD29x32_AA4
#define GUI_FONT_D42X48_AA4      &GUI_FontD42x48_AA4
#define GUI_FONT_D56X64_AA4      &GUI_FontD56x64_AA4
#define GUI_FONT_D71X80_AA4      &GUI_FontD71x80_AA4


#if defined(__cplusplus)
}
#endif

#endif   /* ifdef GUI_FONT_H */

/*************************** End of file ****************************/
