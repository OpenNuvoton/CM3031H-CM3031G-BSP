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
File        : GUI_CACHE.h
Purpose     : Abstract LRU cache implementation
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_CACHE_H
#define GUI_CACHE_H

#include "GUI.h"

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef GUI_HMEM GUI_CACHE_Handle;

typedef GUI_BOOLEAN (GUI_CACHE_CHECKITEM_FUNC)(void * pKey, void * pValue, void * pData);

typedef enum {
  GUI_CACHE_ITEM_OWN_DATA,   // Full data is owned, that means item is copied.
  GUI_CACHE_ITEM_OWN_PTR,    // User creates object on heap, ownership is transferred to cache.
} GUI_CACHE_ITEM_OWNERSHIP;

typedef struct {
  unsigned                 SizeOfKey;
  unsigned                 SizeOfValue;
  GUI_CACHE_ITEM_OWNERSHIP KeyOwnership;
  GUI_CACHE_ITEM_OWNERSHIP ValueOwnership;
  U32                   (* pfHash)               (const void * pKey);
  GUI_BOOLEAN           (* pfCompareKeys)        (const void * p0, const void * p1);
  void                  (* pfFreeKey)            (void * pKey);
  void                  (* pfFreeValue)          (void * pValue);
  U32                   (* pfGetKeyPayloadSize)  (void * pKey);
  U32                   (* pfGetValuePayloadSize)(void * pKey);
} GUI_CACHE_CONFIG;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void             GUI_CACHE_Cleanup    (GUI_CACHE_Handle hCache);
void             GUI_CACHE_CleanupEx  (GUI_CACHE_Handle hCache, GUI_CACHE_CHECKITEM_FUNC * cbCheckItem, void * p);
GUI_CACHE_Handle GUI_CACHE_Create     (unsigned MaxItems, unsigned MaxBytes, const GUI_CACHE_CONFIG * pConfig);
void             GUI_CACHE_Delete     (GUI_CACHE_Handle hCache);
void           * GUI_CACHE_GetItem    (GUI_CACHE_Handle hCache, void * pKey);
void           * GUI_CACHE_GetItemRef (GUI_CACHE_Handle hCache, void * pKey);
unsigned         GUI_CACHE_GetNumItems(GUI_CACHE_Handle hCache);
unsigned         GUI_CACHE_GetNumBytes(GUI_CACHE_Handle hCache);
unsigned         GUI_CACHE_GetMaxItems(GUI_CACHE_Handle hCache);
unsigned         GUI_CACHE_GetMaxBytes(GUI_CACHE_Handle hCache);
void             GUI_CACHE_SetMaxItems(GUI_CACHE_Handle hCache, unsigned MaxItems);
void             GUI_CACHE_SetMaxBytes(GUI_CACHE_Handle hCache, unsigned MaxBytes);
void             GUI_CACHE_ReleaseItem(GUI_CACHE_Handle hCache, const void * pValue);
GUI_BOOLEAN      GUI_CACHE_AddItem    (GUI_CACHE_Handle hCache, void * pKey, void * pValue);

#endif /* GUI_CACHE_H */

/*************************** End of file ****************************/
