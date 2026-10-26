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
File        : GUI_GPU_Private.c
Purpose     : Private code of GPU driver devices.
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_PRIVATE_H
#define GUI_GPU_PRIVATE_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"
#include "GUI_VG.h"
#include "GUI_SVG_Private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define LOCK_ABSTRACT(TYPE, HANDLE_VAR, PTR_VAR)    {                     \
                            TYPE * PTR_VAR;                               \
                            if (HANDLE_VAR) {                             \
                              PTR_VAR = (TYPE *)GUI_LOCK_H((GUI_HMEM)HANDLE_VAR);   \
                              if (PTR_VAR) {
#define UNLOCK_ABSTRACT(PTR_VAR) GUI_UNLOCK_H(PTR_VAR);                   \
                              }                                           \
                            }                                             \
                          }

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
//
// GPU context management, needed if multiple modules access it
//
typedef struct {
  void * pCtx;     // Global context of the 3rd party API, may be NULL if there's no pointer to this.
  int    RefCount; // Ensures that above context does not get NULLed to early.
} GUI_GPU_CONTEXT;
//
// Static driver flags
//
typedef struct {
  U8 NeedsNibbleSwap  : 1; // Does A4 bitmap need nibble swap?
  U8 SupportsClipping : 1; // Does the GPU support hardware clipping?
} GUI_GPU_DRIVER_PROPS;

typedef struct {
  U8 FuncDisabled : 1;
} GUI_GPU_FUNCTION_FLAGS;

typedef struct {
  int x, y;
  int w, h;
} GUI_GPU_CLIPRECT;

//
// Internal helper functions.
//
typedef enum {
  GUI_GPU_UTIL_GETPOLYBUFFERSIZE,  // Returns the size of a buffer of polygon points in the driver format for the given number of points.
  GUI_GPU_UTIL_CONVERTPOLYPOINTS,  // Converts the GUI_POINTs into absolute points and into the format the driver uses.
} GUI_GPU_UTIL_FUNCTION_INDEX;

//
// Forward declare the GPU driver API.
//
typedef struct GUI_GPU_DRIVER_API GUI_GPU_DRIVER_API;
//
// Static context for each driver
//
typedef struct {
  const GUI_GPU_DRIVER_API * pDriver;
  GUI_GPU_FUNCTION_FLAGS     aFuncFlags [GUI_GPU_FUNCTION_COUNT];
  GUI_VOID_FUNCTION        * apfUserFunc[GUI_GPU_FUNCTION_COUNT];
  GUI_DYNARRAY_Handle        haThreshold;
  GUI_DYNARRAY_Handle        haHooks;              // Expandable array of hooks, each item is a block of hooks (size = GUI_GPU_HOOK_COUNT)
  GUI_DYNARRAY_Handle        haContexts;           // Expandable array of void** for NULLing GPU contexts. It's handled like a stack (LIFO)
  GUI_DYNARRAY_Handle        haBufferA4;           // Optional expandable buffer for fast swapping of A4 nibbles.
  GUI_GPU_CLIPRECT           ClipRect;
  const void               * pAPI;                 // User supplied pointer to API table.
  GUI_GPU_CONTEXT            GPU;                  // Global context allocated by the API. May be NULL.
  LCD_PIXELINDEX           * pConvTable;           // Last converted table sent to GPU for palette-based bitmaps.
  U32                        Version        : 24;  // Version of the API (MAJOR << 16 | MINOR << 8 | BUGFIX)
  U8                         DriverDisabled : 1;
  U8                         DriverInited   : 1;   // Flag set when driver has been initialised and is ready.
  U8                         OwnsAPIPointer : 1;   // Flag set when pAPI was allocated and is owned by the GPU module and not the user.
  U8                         SwapNibbles    : 1;   // Does the GPU need to swap nibbles for AA4 bitmaps?
} GUI_GPU_DRIVER_CONTEXT;
//
// Driver functions of a GPU
//
struct GUI_GPU_DRIVER_API {
  const char                     * sName;
  GUI_GPU_DRIVER_CONTEXT      * (* pfGetContext)      (void);
  GUI_VOID_FUNCTION           * (* pfGetDrawFunc)     (GUI_GPU_FUNCTION_INDEX Index);
  GUI_VOID_FUNCTION           * (* pfGetIRQHandler)   (int Index);
  GUI_VOID_FUNCTION           * (* pfGetUtilFunc)     (GUI_GPU_UTIL_FUNCTION_INDEX Index);
  GUI_GPU_DRIVER                 * pfGetDriver;
  int                           (* pfInit)            (const void * pConfig);
  void                          (* pfExit)            (void);
  U8                            (* pfLoadExtension)   (void);
  void                        * (* pfCreateGPUContext)(void);
  void                          (* pfDeleteGPUContext)(void * pCtx);
  int                           (* pfHasAPI)          (void);
  void                          (* pfLoadAPI)         (void * pAPI, GUI_GPU_LOAD_API_CALLBACK * cbLoadFunction);
  U32                           (* pfGetAPISize)      (void);
  const GUI_GPU_DRIVER_PROPS     * pDriverProps;
  const GUI_VG_API               * pVG;
};

typedef struct {
  U32 Id;
  U32 Version;
  U32 NumThresholds;
  U32 : 32;
} GUI_GPU_THRESHOLD_FILE_HEADER;

typedef struct {
  const void ** ppSrc;
  void       ** ppDst;
  const int   * pxSize;
  const int   * pySize;
  const int   * pBytesPerLine;
} GUI_GPU_COPYBUFFERRECT_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * px1;
  const int * py1;
  const int * pxSize;
  const int * pySize;
} GUI_GPU_COPYRECT_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * px1;
  const int * py1;
} GUI_GPU_RECT_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * px1;
  const int * py1;
} GUI_GPU_LINE_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * px1;
  const int * py1;
  const int * pr;
} GUI_GPU_ROUNDRECT_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * pr;
} GUI_GPU_CIRCLE_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * prx;
  const int * pry;
} GUI_GPU_ELLIPSE_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * prx;
  const int * pry;
  const I32 * pa0;
  const I32 * pa1;
} GUI_GPU_ARC_PARAMETERS;

typedef struct {
  const int * px0;
  const int * py0;
  const int * pr;
  const I32 * pa0;
  const I32 * pa1;
} GUI_GPU_PIE_PARAMETERS;

typedef struct {
  const int       * px0;
  const int       * py0;
  const int       * px1;
  const int       * py1;
  const GUI_COLOR * pColor0;
  const GUI_COLOR * pColor1;
  const int       * pAxis;
  const int       * pRadius;
} GUI_GPU_GRADIENT_PARAMETERS;

typedef struct {
  const int                * px0;
  const int                * py0;
  const int                * px1;
  const int                * py1;
  const GUI_GRADIENT_INFO ** ppGradientInfo;
  const int                * pNumColors;
  const int                * pAxis;
  const int                * pRadius;
} GUI_GPU_MULTIGRADIENT_PARAMETERS;

typedef struct {
  const int   * px;
  const int   * py;
  const void ** pp;
  const int   * pxSize;
  const int   * pySize;
  const int   * pBytesPerLine;
} GUI_GPU_BITMAP_PARAMETERS;

typedef struct {
  const int   * px;
  const int   * py;
  const void ** pp;
  const int   * pxSize;
  const int   * pySize;
  const int   * pBytesPerLine;
  const int   * pxMag;
  const int   * pyMag;
} GUI_GPU_BITMAPEX_PARAMETERS;

typedef struct {
  LCD_COLOR ** ppColor;
  void      ** ppIndex;
  const U32  * pNumItems;
  const U8   * pSizeOfIndex;
} GUI_GPU_COLORTOINDEX_PARAMETERS;

typedef struct {
  void      ** ppIndex;
  LCD_COLOR ** ppColor;
  const U32  * pNumItems;
  const U8   * pSizeOfIndex;
} GUI_GPU_INDEXTOCOLOR_PARAMETERS;

typedef struct {
  const GUI_POINT ** ppSrc;
  const int        * pNumPoints;
  const int        * pThickness;
  const int        * px;
  const int        * py;
} GUI_GPU_DRAWPOLYGON_PARAMETERS;

typedef struct {
  const GUI_POINT ** ppSrc;
  const int        * pNumPoints;
  const int        * px;
  const int        * py;
} GUI_GPU_FILLPOLYGON_PARAMETERS;

typedef struct {
  GUI_MEMDEV_Handle * phSrc;
  GUI_MEMDEV_Handle * phDst;
  float             * pdx;
  float             * pdy;
  int               * pa;
  int               * pMag;
} GUI_GPU_ROTATEMEMDEV_PARAMETERS;

typedef struct {
  GUI_MEMDEV_Handle * phMem;
  GUI_COLOR         * pColor;
  U8                * pBlendIntens;
} GUI_GPU_BLENDMEMDEV_PARAMETERS;

typedef struct {
  GUI_MEMDEV_Handle * phMemSrc;
  GUI_MEMDEV_Handle * phMemDst;
  U8                * pBlurDepth;
  U8                * pHighQuality;
} GUI_GPU_BLURMEMDEV_PARAMETERS;

typedef struct {
  LCD_COLOR ** ppFG;
  LCD_COLOR ** ppBG;
  LCD_COLOR ** ppDest;
  U32        * pNumItems;
} GUI_GPU_ALPHABLENDING_PARAMETERS;

typedef struct {
  U32           ** ppFG;
  U32           ** ppBG;
  U32           ** ppDst;
  const unsigned * pOffFG;
  const unsigned * pOffBG;
  const unsigned * pOffDest;
  const unsigned * pxSize;
  const unsigned * pySize;
  const U8       * pIntens;
} GUI_GPU_MIXCOLOR_PARAMETERS;

typedef struct {
  const LCD_LOGPALETTE ** ppLogPal;
  const GUI_BITMAP     ** ppBitmap;
  LCD_PIXELINDEX      *** pppReturn;
} GUI_GPU_GETPALCONVTABLE_PARAMETERS;

#define GPU_FUNC_MAX_ARGS   12
typedef void * GUI_GPU_FUNCTION_ARGS[GPU_FUNC_MAX_ARGS];

typedef union {
  //
  // Abstract args
  //
  GUI_GPU_FUNCTION_ARGS              Args;
  //
  // Arguments mapped to different function prototypes.
  //
  GUI_GPU_COPYBUFFERRECT_PARAMETERS  CopyBufferRect;
  GUI_GPU_COPYRECT_PARAMETERS        CopyRect;
  GUI_GPU_LINE_PARAMETERS            DrawLine;
  GUI_GPU_RECT_PARAMETERS            DrawRect;
  GUI_GPU_ROUNDRECT_PARAMETERS       DrawRoundRect;
  GUI_GPU_CIRCLE_PARAMETERS          DrawCircle;
  GUI_GPU_ELLIPSE_PARAMETERS         DrawEllipse;
  GUI_GPU_ARC_PARAMETERS             DrawArc;
  GUI_GPU_PIE_PARAMETERS             DrawPie;
  GUI_GPU_BITMAP_PARAMETERS          DrawBitmap;
  GUI_GPU_BITMAPEX_PARAMETERS        DrawBitmapEx;
  GUI_GPU_DRAWPOLYGON_PARAMETERS     DrawPolygon;
  GUI_GPU_FILLPOLYGON_PARAMETERS     FillPolygon;
  GUI_GPU_COLORTOINDEX_PARAMETERS    ColorToIndex;
  GUI_GPU_INDEXTOCOLOR_PARAMETERS    IndexToColor;
  GUI_GPU_ROTATEMEMDEV_PARAMETERS    RotateMemdev;
  GUI_GPU_BLENDMEMDEV_PARAMETERS     BlendMemdev;
  GUI_GPU_BLURMEMDEV_PARAMETERS      BlurMemdev;
  GUI_GPU_ALPHABLENDING_PARAMETERS   BlendAlpha;
  GUI_GPU_MIXCOLOR_PARAMETERS        MixColors;
  GUI_GPU_GETPALCONVTABLE_PARAMETERS GetPalConvTable;
  GUI_GPU_GRADIENT_PARAMETERS        DrawGradient;
  GUI_GPU_MULTIGRADIENT_PARAMETERS   DrawMultiGradient;
} GUI_GPU_FUNCTION_PARAMETERS;

typedef struct {
  GUI_GPU_FUNCTION_INDEX Func;
  int                    Threshold;
} GUI_GPU_THRESHOLD;

typedef struct {
  GUI_GPU_FUNCTION_INDEX      Func;
  GUI_GPU_FUNCTION_PARAMETERS Paras;
} GUI_GPU_FUNCTION;

//
// Utility functions
//
typedef U32  (GUI_GPU_UTIL_GETPOLYBUFFERSIZE_FUNC)(int NumPoints);
typedef void (GUI_GPU_UTIL_CONVERTPOLYPOINTS_FUNC)(const GUI_POINT * pPoints, int NumPoints, int x, int y, void * pResult);

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void                     GUI_GPU__CopyBufferRect        (void * pSrc, void * pDst, int xSize, int ySize, const GUI_RENDERBUFFER * pRT);
GUI_GPU_DRIVER_CONTEXT * GUI_GPU__GetDriverContext      (GUI_GPU_DRIVER * pDriver);
GUI_GPU_THRESHOLD      * GUI_GPU__GetThreshold          (GUI_GPU_DRIVER_CONTEXT * pCtx, GUI_GPU_FUNCTION_INDEX Func);
int                      GUI_GPU__BeginDraw             (GUI_GPU_DRIVER * pDriver, const GUI_GPU_FUNCTION * pFunc);
void                     GUI_GPU__EndDraw               (GUI_GPU_DRIVER * pDriver, const GUI_GPU_FUNCTION * pFunc);
void                     GUI_GPU__GetMemdevTarget       (GUI_MEMDEV_Handle hMem, GUI_RENDERBUFFER * pRT);
const GUI_RENDERBUFFER * GUI_GPU__GetRenderTarget       (void);
void                   * GUI_GPU__GetBufferPtr          (const GUI_RENDERBUFFER * pBuffer, int x, int y);
void                   * GUI_GPU__GetBufferPtrEx        (const GUI_RENDERBUFFER * pBuffer, int x, int y, U8 LogicalCoords);
U32                      GUI_GPU__GetBufferSize         (const GUI_RENDERBUFFER * pBuffer);
GUI_DEVICE             * GUI_GPU__GetOutputDevice       (void);
int                      GUI_GPU__GetOutputDevClassIndex(void);
void                     GUI_GPU__GetOutputMatrix       (GUI_MATRIX * pMat, float x, float y, U8 IgnoreMemdev);
U8                       GUI_GPU__CallHooks             (GUI_GPU_DRIVER * pDriver, GUI_GPU_HOOK_INDEX Index);
const GUI_VG_API       * GUI_GPU__GetVectorAPI          (GUI_GPU_DRIVER * pDriver);
U8                       GUI_GPU__LoadExtension         (GUI_GPU_DRIVER * pDriver);
void                     GUI_GPU__LoadRenderBackend     (void);

/*********************************************************************
*
*       DMA2D module
* 
*  Private functions shared between GUI_GPU_CHROMART and GUI_GPU_IPA.
*
**********************************************************************
*/
GUI_GPU_DRIVER_CONTEXT * GUI_GPU_DMA2D__GetContext   (void * pContext);
GUI_VOID_FUNCTION      * GUI_GPU_DMA2D__GetDrawFunc  (void * pContext, GUI_GPU_FUNCTION_INDEX Func);
GUI_VOID_FUNCTION      * GUI_GPU_DMA2D__GetIRQHandler(void * pContext);
int                      GUI_GPU_DMA2D__Init         (void * pContext, const void * pConfig);
void                     GUI_GPU_DMA2D__Exit         (void * pContext);
//
extern const GUI_GPU_DRIVER_PROPS GUI_GPU_DMA2D__Props;

//
// Has to stay the same as GUI_GPU_CHROMART_CONFIG and GUI_GPU_IPA_CONFIG.
//
typedef struct {
  void              * pDMA2D;
  GUI_VOID_FUNCTION * pfPreExecHook;
  GUI_VOID_FUNCTION * pfPostExecHook;
  GUI_VOID_FUNCTION * pfTransferCompleteHook;
  U8                  FillAlpha : 1;                                             
} GUI_GPU_DMA2D_CONFIG;

typedef struct {
  GUI_GPU_DRIVER_CONTEXT  Driver;
  GUI_GPU_DMA2D_CONFIG    Config;
  GUI_HMEM                hBufferCopyRect;
  U8                      UseCopyRect : 1;
} GUI_GPU_DMA2D_CONTEXT;

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_PRIVATE_H */

/*************************** End of file ****************************/
