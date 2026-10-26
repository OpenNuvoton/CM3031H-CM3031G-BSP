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
File        : GUI_GPU_IPA.h
Purpose     : Header of GigaDevice IPA driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_IPA_H
#define GUI_GPU_IPA_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_IPA_CONFIG
* 
*  Description
*    Configuration structure used for GigaDevice IPA (Image Processing
*    Accelerator) driver during creation with GUI_GPU_Create().
*/
typedef struct {
  void              * pIPA;                    // IPA peripheral base address.
  GUI_VOID_FUNCTION * pfPreExecHook;           // Hook to be called before the execution of one action.
  GUI_VOID_FUNCTION * pfPostExecHook;          // Hook to be called after the execution of one action.
  GUI_VOID_FUNCTION * pfTransferCompleteHook;  // Hook to be called from within the IRQ handler after
                                               // the transfer has been completed.
  U8                  FillAlpha : 1;           // Some ChromART devices are able to fill rectangles with
                                               // alpha blending while other devices are not able to do this.
                                               // If this flag is true, the routine for filling rectangles
                                               // will make use of this feature.
} GUI_GPU_IPA_CONFIG;

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_IPA_H */

/*************************** End of file ****************************/
