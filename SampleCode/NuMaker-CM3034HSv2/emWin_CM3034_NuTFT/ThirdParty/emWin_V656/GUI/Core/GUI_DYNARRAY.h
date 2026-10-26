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
File        : GUI_DYNARRAY.h
Purpose     : Dynamic array of items with the same size.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_DYNARRAY_H
#define GUI_DYNARRAY_H

#include "GUI.h"

typedef GUI_HMEM GUI_DYNARRAY_Handle;

typedef struct {
  void             ** ppItem;
  unsigned            i;
  GUI_DYNARRAY_Handle hArray;
} GUI_DYNARRAY_ITERATOR;

typedef int (GUI_DYNARRAY_EXPAND_FUNC)(int NumNewItems, int MaxItems);
typedef int (GUI_DYNARRAY_COMPARE_FUNC)(void * pItem0, void * pItem1);

void                GUI_DYNARRAY_Append       (GUI_DYNARRAY_Handle hArray, const void * pNewData, unsigned NumNewItems);
void                GUI_DYNARRAY_AppendItem   (GUI_DYNARRAY_Handle hArray, const void * pNewData);
void                GUI_DYNARRAY_Prepend      (GUI_DYNARRAY_Handle hArray, const void * pNewData, unsigned NumNewItems);
void                GUI_DYNARRAY_PrependItem  (GUI_DYNARRAY_Handle hArray, const void * pNewData);
GUI_DYNARRAY_Handle GUI_DYNARRAY_Create       (unsigned SizeOfItem, unsigned NumItems);
GUI_DYNARRAY_Handle GUI_DYNARRAY_CreateEx     (unsigned SizeOfItem, unsigned NumItems, GUI_DYNARRAY_EXPAND_FUNC * pfExpand);
void                GUI_DYNARRAY_Clear        (GUI_DYNARRAY_Handle hArray);
void                GUI_DYNARRAY_Delete       (GUI_DYNARRAY_Handle hArray);
int                 GUI_DYNARRAY_FindItem     (GUI_DYNARRAY_Handle hArray, const void * pItem);
void                GUI_DYNARRAY_Resize       (GUI_DYNARRAY_Handle hArray, unsigned NumBytesNew);
void                GUI_DYNARRAY_RemoveItemAt (GUI_DYNARRAY_Handle hArray, unsigned Index);
void                GUI_DYNARRAY_RemoveItem   (GUI_DYNARRAY_Handle hArray, const void * pItem);
void              * GUI_DYNARRAY_GetItemAt    (GUI_DYNARRAY_Handle hArray, unsigned Index);
void              * GUI_DYNARRAY_GetDataPtr   (GUI_DYNARRAY_Handle hArray);
unsigned            GUI_DYNARRAY_GetNumItems  (GUI_DYNARRAY_Handle hArray);
unsigned            GUI_DYNARRAY_GetNumBytes  (GUI_DYNARRAY_Handle hArray);
unsigned            GUI_DYNARRAY_GetSize      (GUI_DYNARRAY_Handle hArray);
unsigned            GUI_DYNARRAY_GetSizeOfItem(GUI_DYNARRAY_Handle hArray);
void                GUI_DYNARRAY_MoveData     (GUI_DYNARRAY_Handle hArray, unsigned IndexFrom, unsigned IndexTo);
void                GUI_DYNARRAY_BeginIterator(GUI_DYNARRAY_Handle hArray, void ** ppItem, GUI_DYNARRAY_ITERATOR * pIterator);
void              * GUI_DYNARRAY_GetNextItem  (GUI_DYNARRAY_ITERATOR * pIterator);
void                GUI_DYNARRAY_Sort         (GUI_DYNARRAY_Handle hArray, GUI_DYNARRAY_COMPARE_FUNC * pfCompare);
//
// Expand strategies
//
int GUI_DYNARRAY_EXPAND_Double(int NumNewItems, int MaxItems);
int GUI_DYNARRAY_EXPAND_Add   (int NumNewItems, int MaxItems);

//
// Macros for more type-safety 
//
#define GUI_DYNARRAY_HANDLE(TYPE)   GUI_DYNARRAY_Handle

#endif /* GUI_DYNARRAY_H */

/*************************** End of file ****************************/
