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
File        : GUI.h
Purpose     : GUI API include file
---------------------------END-OF-HEADER------------------------------
*/

#ifndef  GUI_H
#define  GUI_H

#include <stddef.h>
#include <string.h>   // for memset()

#include "GUI_ConfDefaults.h"
#include "GUI_Type.h"
#include "GUI_Version.h"
#include "GUI_StdLib.h"
#include "GUI_BinaryConstants.h"
#include "GUI_Font.h"
#include "GUI_DialogID.h"

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Disable string function warning with newer MSVC versions
*/
#if defined (_MSC_VER)
  #if (_MSC_VER > 1200)
    #pragma warning( disable : 4996)
  #endif
#endif

/*********************************************************************
*
*       Numeric limits
*/
#define I32_MIN (-2147483647L - 1)
#define I32_MAX   2147483647L

/*********************************************************************
*
*       Macros, function replacement
*/
#define GUI_ABS(a)              (((a) < 0) ? (-(a)) : (a))
#define GUI_COUNTOF(a)          (sizeof(a) / sizeof(a[0]))
#define GUI_MIN(a,b)            (((a) < (b)) ? (a) : (b))
#define GUI_MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define GUI_ZEROFILL(p, Size)   (memset(p, 0, Size))
#define GUI_ZEROFILL_VAR(var)   (memset(&var,    0, sizeof(var)))
#define GUI_ZEROFILL_ARR(arr)   (memset(&arr[0], 0, sizeof(arr)))
#define GUI_ZEROFILL_PTR(ptr)   (memset(ptr,     0, sizeof(*ptr)))

#define GUI_OFFSET_IN_STRUCT(TYPE, STRUCT_MEMBER)   (PTR_ADDR)(&((TYPE *)NULL)->STRUCT_MEMBER)

/*********************************************************************
*
*       Support for multitasking systems (locking)
*/
typedef struct GUI_CONTEXT GUI_CONTEXT;

#if !GUI_OS
  #define GUI_LOCK()
  #define GUI_UNLOCK()
  #define GUITASK_INIT()
  #define GUITASK_COPY_CONTEXT()
#else
  void GUI_Lock(void);
  void GUI_Unlock(void);
  void GUITASK_Init(void);
  void GUITASK_CopyContext(void);
  void GUITASK_SetMaxTask(int MaxTask);
  int  GUITASK_GetMaxTask(void);
  GUI_CONTEXT * GUITASK_GetpContext(int Index);
  #define GUI_LOCK()             GUI_Lock()
  #define GUI_UNLOCK()           GUI_Unlock()
  #define GUITASK_INIT()         GUITASK_Init()
  #define GUITASK_COPY_CONTEXT() GUITASK_CopyContext()
#endif

/*********************************************************************
*
*       API table of a display driver
*/
struct GUI_DEVICE_API {
  //
  // Data
  //
  int DeviceClassIndex;
  //
  // Drawing functions
  //
  void           (* pfDrawBitmap   )(GUI_DEVICE *  pDevice,  int x0, int y0, int xsize, int ysize, int BitsPerPixel, int BytesPerLine, const U8 * pData, int Diff, const LCD_PIXELINDEX * pTrans);
  void           (* pfDrawHLine    )(GUI_DEVICE *  pDevice,  int x0, int y0,  int x1);
  void           (* pfDrawVLine    )(GUI_DEVICE *  pDevice,  int x , int y0,  int y1);
  void           (* pfFillRect     )(GUI_DEVICE *  pDevice,  int x0, int y0, int x1, int y1);
  LCD_PIXELINDEX (* pfGetPixelIndex)(GUI_DEVICE *  pDevice,  int x, int y);
  void           (* pfSetPixelIndex)(GUI_DEVICE *  pDevice,  int x, int y, LCD_PIXELINDEX ColorIndex);
  void           (* pfXorPixel     )(GUI_DEVICE *  pDevice,  int x, int y);
  //
  // Set origin
  //
  void           (* pfSetOrg       )(GUI_DEVICE *  pDevice,  int x, int y);
  //
  // Request information
  //
  void         (*(* pfGetDevFunc)   (GUI_DEVICE ** ppDevice, int Index))(void);
  I32            (* pfGetDevProp   )(GUI_DEVICE *  pDevice,  int Index);
  const void    *(* pfGetDevData   )(GUI_DEVICE *  pDevice,  int Index);
  void           (* pfGetRect      )(GUI_DEVICE *  pDevice,  LCD_RECT * pRect);
};

/*********************************************************************
*
*       Device classes
*/
typedef enum {
  DEVICE_CLASS_DRIVER = 0,
  DEVICE_CLASS_DRIVER_MODIFIER,   // Zoom or delta-pixel modifier
  DEVICE_CLASS_VNC,
  DEVICE_CLASS_DIRTY,
  DEVICE_CLASS_SPRITE,
  DEVICE_CLASS_MEMDEV,
  DEVICE_CLASS_ALPHA,
  DEVICE_CLASS_GPU,
  DEVICE_CLASS_AUTOALPHA,
  DEVICE_CLASS_MEASDEV,
} DEVICE_CLASS;

#define GUI_DEVICE_STAYONTOP 1

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_Win_API;
//
// Macros to be used in configuration files
//
#ifndef   GUIDRV_WIN32
  #define GUIDRV_WIN32        &GUIDRV_Win_API
#endif

/*********************************************************************
*
*       Definition of GUI_DEVICE structure
*/
struct GUI_DEVICE {
  //
  // Linking
  //
  GUI_DEVICE * pNext;
  GUI_DEVICE * pPrev;
  //
  // Data
  //
  union {
    GUI_HMEM hContext; // Handle of payload data like sprite- or memory device context
    void   * pContext; // Pointer for context data in a fixed block
  } u;
  //
  // API pointers
  //
  const GUI_DEVICE_API     * pDeviceAPI;
  const LCD_API_COLOR_CONV * pColorConvAPI;
  U16 Flags;
  int LayerIndex;
};

extern const GUI_DEVICE_API GUI_MEMDEV_DEVICE_1;
extern const GUI_DEVICE_API GUI_MEMDEV_DEVICE_8;
extern const GUI_DEVICE_API GUI_MEMDEV_DEVICE_16;
extern const GUI_DEVICE_API GUI_MEMDEV_DEVICE_32;

#if GUI_SUPPORT_PROFILE

/*********************************************************************
*
*       Instrumentation
*/
/*********************************************************************
*
*       GUI_PROFILE_API
*/
typedef struct {
  void (*pfRecordEndCall)   (unsigned EventId);
  void (*pfRecordEndCallU32)(unsigned EventId, U32 Para0);
  void (*pfRecordVoid)      (unsigned EventId);
  void (*pfRecordU32)       (unsigned EventId, U32 Para0);
  void (*pfRecordU32x2)     (unsigned EventId, U32 Para0, U32 Para1);
  void (*pfRecordU32x3)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2);
  void (*pfRecordU32x4)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3);
  void (*pfRecordU32x5)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4);
  void (*pfRecordU32x6)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4, U32 Para5);
  void (*pfRecordU32x7)     (unsigned EventId, U32 Para0, U32 Para1, U32 Para2, U32 Para3, U32 Para4, U32 Para5, U32 Para6);
  void (*pfRecordString)    (unsigned EventId, const char * pPara0);
  void (*pfRecordStringx2)  (unsigned EventId, const char * pPara0, const char * pPara1);
} GUI_PROFILE_API;

U32  GUI_PROFILE_GetAPIDesc(const char ** psDesc);
void GUI_PROFILE_SetAPI    (const GUI_PROFILE_API * pAPI, U32 IdOffset);
void GUI_SYSVIEW_Init(void);

/*********************************************************************
*
*       GUI_PROFILE
*/
typedef struct {
  U32                    IdOffset;
  const GUI_PROFILE_API * pAPI;
} GUI_PROFILE;

#endif

/*********************************************************************
*
*       GUI_CONTEXT
*
*  This structure is public for one reason only:
*  To allow the application to save and restore the context.
*/
typedef union {
  U8  aColorIndex8[2];
  U16 aColorIndex16[2];
  U32 aColorIndex32[2];
} LCD_COLORINDEX_UNION;

struct GUI_CONTEXT {
  //
  // Variables in LCD module
  //
  LCD_COLORINDEX_UNION uLCD;
  LCD_RECT       ClipRect;
  U8             DrawMode;
  U8             SelLayer;
  U8             TextStyle;
  //
  // Variables in GL module
  //
  GUI_RECT * pClipRect_HL;                 // High level clip rectangle ... Speed optimization so drawing routines can optimize
  U16        PenSize;
  U8         PenShape;
  U8         LineStyle;
  U8         StrikeWidth;
  //
  // Variables in GUICHAR module
  //
  const GUI_FONT * pAFont;
  I16P LBorder;
  I16P DispPosX, DispPosY;
  I16P DrawPosX, DrawPosY;
  I16P TextMode, TextAlign;
  GUI_COLOR Color, BkColor;                // Required only when changing devices and for speed opt (caching)
  //
  // Pointer to  color indices
  //
  LCD_PIXELINDEX * LCD_pBkColorIndex;
  LCD_PIXELINDEX * LCD_pColorIndex;
  //
  // Variables in WM module
  //
  #if GUI_WINSUPPORT
    const GUI_RECT * WM__pUserClipRect;
    GUI_HWIN hAWin;
    int xOff, yOff;
    U8 WM_IsActive;
    U8 DisableCliprect;
    U8 ClipBKActive;
  #endif
  #if GUI_SUPPORT_PROFILE
    GUI_PROFILE Profile;
  #endif
  //
  // Array of pointers to device chains
  //
  GUI_DEVICE * apDriver[GUI_NUM_LAYERS];
  //
  // Variables in MEMDEV module (with memory devices only)
  //
  GUI_HMEM    hDevData;
  //
  // Variables in Antialiasing module
  //
  const tLCD_HL_APIList * pLCD_HL;       // Required to reroute drawing (HLine & Pixel) to the AA module
  U8 AA_Factor;
  U8 AA_HiResEnable;
  void (* AA_pfSetPixelAA)(int x, int y, U8 Intens); // Function to be used for drawing a single pixel
  //
  // Used to reference and link in the copyright string for libraries.
  //
  const char * sCopyright;
};

/* Rename GUI_SaveContext in order to avoid crashes if wrong GUIConf is used */
#if (GUI_WINSUPPORT == 1)
  #define GUI_SaveContext GUI_SaveContext_W
#else
  #define GUI_SaveContext GUI_SaveContext_
#endif

/*********************************************************************
*
*       Device management
*/
GUI_DEVICE * GUI_DEVICE_Create          (const GUI_DEVICE_API * pDeviceAPI, const LCD_API_COLOR_CONV * pColorConvAPI, U16 Flags, int LayerIndex);
GUI_DEVICE * GUI_DEVICE_CreateAndLink   (const GUI_DEVICE_API * pDeviceAPI, const LCD_API_COLOR_CONV * pColorConvAPI, U16 Flags, int LayerIndex);
void         GUI_DEVICE_Delete          (GUI_DEVICE * pDevice);
int          GUI_DEVICE_GetDeviceClassEx(int LayerIndex);
int          GUI_DEVICE_GetDeviceClass  (void);
int          GUI_DEVICE_Link            (GUI_DEVICE * pDevice);
void         GUI_DEVICE_Unlink          (GUI_DEVICE * pDevice);
GUI_DEVICE * GUI_DEVICE__GetpDriver     (int LayerIndex);
GUI_DEVICE * GUI_DEVICE__GetpDevice     (int LayerIndex, int DeviceClass);

GUI_DEVICE * GUI_DEVICE_UnlinkTaskDevices(void);
void         GUI_DEVICE_LinkDevices      (GUI_DEVICE * pDevice);

/*********************************************************************
*
*       GUI_DIRTYDEVICE_INFO
*
*   Description
*     Information about the dirty device.
*/
typedef struct {
  void * pData;         // Pointer to the first changed pixel.\sup{*1}
  int    x0;            // Leftmost position of changed area.
  int    y0;            // Topmost position of changed area.
  int    xSize;         // Size in X of changed area.
  int    ySize;         // Size in Y of changed area.
  int    LineOff;       // Number of pixels (stride) from one line to the next line.\sup{*1}
  int    BytesPerPixel; // Number of bytes required per pixel.
  int    IsDirty;       // Indicates if dirty pixels exist.
} GUI_DIRTYDEVICE_INFO;

int GUI_DIRTYDEVICE_Create      (void);
int GUI_DIRTYDEVICE_CreateEx    (int LayerIndex);
int GUI_DIRTYDEVICE_CreateExInfo(GUI_DIRTYDEVICE_INFO * pInfo, int LayerIndex);
int GUI_DIRTYDEVICE_Delete      (void);
int GUI_DIRTYDEVICE_DeleteEx    (int LayerIndex);
int GUI_DIRTYDEVICE_Fetch       (GUI_DIRTYDEVICE_INFO * pInfo);
int GUI_DIRTYDEVICE_FetchEx     (GUI_DIRTYDEVICE_INFO * pInfo, int LayerIndex);

/*********************************************************************
*
*       GUI_DIRTYTILES
*/
int  GUI_DIRTYTILES_CreateEx (int NumCols, int NumRows,  int LayerIndex);
int  GUI_DIRTYTILES_Create   (int NumCols, int NumRows);
int  GUI_DIRTYTILES_FetchEx  (GUI_RECT ** ppRects, U8 ** ppBytes, int LayerIndex);
int  GUI_DIRTYTILES_Fetch    (GUI_RECT ** ppRects, U8 ** ppBytes);
void GUI_DIRTYTILES_CleanUpEx(int LayerIndex);
void GUI_DIRTYTILES_CleanUp  (void);
int  GUI_DIRTYTILES_Exists   (void);
int  GUI_DIRTYTILES_ExistsEx (int LayerIndex);
//
// DirtyTiles, internal use only
//
void * GUI_DIRTYTILES__CreateContext(int NumCols, int NumRows, int LayerIndex);
int    GUI_DIRTYTILES__FetchContext (void * pVoid, GUI_RECT ** ppRects, U8 ** ppBytes);
void   GUI_DIRTYTILES__AddDirtyRect (void * pVoid, int x0, int y0, int x1, int y1);
void   GUI_DIRTYTILES__AddDirtyPixel(void * pVoid, int x, int y);
void   GUI_DIRTYTILES__CleanUp      (void * pVoid);
void   GUI_DIRTYTILES__MarkDirty    (void * pVoid);
int    GUI_DIRTYTILES__GetNumCols   (void);
int    GUI_DIRTYTILES__GetNumRows   (void);
void   GUI_DIRTYTILES__SetNumCols   (int NumCols);
void   GUI_DIRTYTILES__SetNumRows   (int NumRows);

void         GUI_DIRTYTILES__LinkDevice    (GUI_DEVICE * pDevice);
void         GUI_DIRTYTILES__UnlinkDevice  (GUI_DEVICE * pDevice);
GUI_DEVICE * GUI_DIRTYTILES__CreateDeviceEx(int NumCols, int NumRows, int LayerIndex);
GUI_DEVICE * GUI_DIRTYTILES__CreateDevice  (int NumCols, int NumRows);
void         GUI_DIRTYTILES__CleanUpDevice (GUI_DEVICE * pDevice);
int          GUI_DIRTYTILES__FetchDevice   (GUI_DEVICE * pDevice, GUI_RECT ** ppRects, U8 ** ppBytes);

/*********************************************************************
*
*       GUI_GCACHE
*/
//
// GCache modules, do not call directly!
//
int GUI_GCACHE_SetMode1bpp (GUI_DEVICE * pDevice);
int GUI_GCACHE_SetMode4bpp (GUI_DEVICE * pDevice);
int GUI_GCACHE_SetMode16bpp(GUI_DEVICE * pDevice);

/*********************************************************************
*
*       GUI_GCACHE modes
* 
*  Description
*    GUI_GCACHE bit depths to be used for GUI_GCACHE_Create() and the
*    other creation functions.
*/
#define GUI_GCACHE_1   (&GUI_GCACHE_SetMode1bpp)     // Use global cache with 1bpp  color depth.
#define GUI_GCACHE_4   (&GUI_GCACHE_SetMode4bpp)     // Use global cache with 4bpp  color depth.
#define GUI_GCACHE_16  (&GUI_GCACHE_SetMode16bpp)    // Use global cache with 16bpp color depth.
/* emDoc stop mark */

//
// Public functions
//
int GUI_GCACHE_Create      (int (* pfMode)(GUI_DEVICE *), const LCD_API_COLOR_CONV * pColorConvAPI);
int GUI_GCACHE_CreateEx    (int (* pfMode)(GUI_DEVICE *), const LCD_API_COLOR_CONV * pColorConvAPI, int LayerIndex);
int GUI_GCACHE_CreateUserEx(int (* pfMode)(GUI_DEVICE *), const LCD_API_COLOR_CONV * pColorConvAPI, int LayerIndex, void * pUserBuffer);

/*********************************************************************
*
*       GUI_DCACHE_API
* 
*  Description
*    API functions required for GUI_DCACHE_ClearEx() and GUI_DCACHE_ClearExAddr().
*    
*    Please note that all functions need to be set.
*/
typedef struct {
  void (* pfCleanDCache)               (void);
  void (* pfInvalidateDCache)          (void);
  void (* pfCleanAndInvalidateDCache)  (void);
  void (* pfCleanDCacheEx)             (volatile void * pAddr, int NumBytes);
  void (* pfInvalidateDCacheEx)        (volatile void * pAddr, int NumBytes);
  void (* pfCleanAndInvalidateDCacheEx)(volatile void * pAddr, int NumBytes);
} GUI_DCACHE_API;

/*********************************************************************
*
*       GUI_DCACHE_CLEAR_MODE
* 
*  Description
*    Parameter passed to GUI_DCACHE_ClearEx() and GUI_DCACHE_ClearExAddr()
*    to determine the cache clearing mode.
*/
typedef enum {
  GUI_DCACHE_CLEAN,                 // Clean data cache of CPU.
  GUI_DCACHE_INVALIDATE,            // Invalidate data cache of CPU.
  GUI_DCACHE_CLEAN_AND_INVALIDATE,  // Clean and invalidate data cache of CPU.
} GUI_DCACHE_CLEAR_MODE;

void GUI_DCACHE_SetClearCacheHook(void (* pFunc)(U32 LayerMask));
void GUI_DCACHE_SetClearCacheAPI (const GUI_DCACHE_API * pAPI);
void GUI_DCACHE_Clear            (U32 LayerMask);
void GUI_DCACHE_ClearEx          (GUI_DCACHE_CLEAR_MODE ClearMode);
void GUI_DCACHE_ClearExAddr      (GUI_DCACHE_CLEAR_MODE ClearMode, volatile void * pAddr, U32 NumBytes);

/*********************************************************************
*
*       GUI_SOFTLAYER_CONFIG
*
*  Description
*    Data structure used by GUI_SOFTLAYER_Enable() to configurate a
*    soft layer.
*/
typedef struct {
  int xPos;      // X-position.
  int yPos;      // Y-position.
  int xSize;     // X-size.
  int ySize;     // Y-size.
  int Visible;   // 1 = visible, 0 = not visible.
} GUI_SOFTLAYER_CONFIG;

int  GUI_SOFTLAYER_Enable           (GUI_SOFTLAYER_CONFIG * pConfig, int NumLayers, GUI_COLOR CompositeColor);
int  GUI_SOFTLAYER_Refresh          (void);
void GUI_SOFTLAYER_SetCompositeColor(U32 Color);
int  GUI_SOFTLAYER_MULTIBUF_Enable  (int OnOff);

/*********************************************************************
*
*       General routines
*/
void             GUI_Exit                 (void);
const GUI_RECT * GUI_GetClipRect          (void);
GUI_COLOR        GUI_GetDefaultBkColor    (void);
GUI_COLOR        GUI_GetDefaultColor      (void);
const GUI_FONT * GUI_GetDefaultFont       (void);
int              GUI_GetScreenSizeX       (void);
int              GUI_GetScreenSizeY       (void);
const char *     GUI_GetVersionString     (void);
int              GUI_Init                 (void);
int              GUI_IsInitialized        (void);
void             GUI_SetAfterExitHook     (void (* pFunc)(void));
void             GUI_SetAfterInitHook     (void (* pFunc)(void));
void             GUI_SetPreInitHook       (void (* pFunc)(void));
void             GUI_RegisterAfterInitHook(void (* pFunc)(void), GUI_REGISTER_INIT * pRegisterInit);
void             GUI_RestoreContext       (const GUI_CONTEXT * pContext);
void             GUI_SaveContext          (GUI_CONTEXT * pContext);
const GUI_RECT * GUI_SetClipRect          (const GUI_RECT * pRect);
void             GUI_SetControlHook       (void (* pFunc)(int LayerIndex, int Cmd));
void             GUI_SetDefault           (void);
void             GUI_SetDefaultBkColor    (GUI_COLOR Color);
void             GUI_SetDefaultColor      (GUI_COLOR Color);
void             GUI_SetDefaultFont       (const GUI_FONT * pFont);
GUI_DRAWMODE     GUI_SetDrawMode          (GUI_DRAWMODE dm);
void             GUI_SetScreenSizeX       (int xSize);
void             GUI_SetScreenSizeY       (int ySize);
void             GUI_SetRefreshHook       (void (* pFunc)(void));
void             MainTask                 (void);

/*********************************************************************
*
*       GUI_RECT functions
*/
int  GUI_RECT_ContainsPoint        (const GUI_RECT * pRect, int x, int y);
int  GUI_RECT_ContainsPointInside  (const GUI_RECT * pRect, int x, int y);
int  GUI_RECT_ContainsPointEx      (const GUI_RECT * pRect, const GUI_POINT * pPoint);
int  GUI_RECT_ContainsPointInsideEx(const GUI_RECT * pRect, const GUI_POINT * pPoint);
int  GUI_RECT_ContainsRect         (const GUI_RECT * pRect, const GUI_RECT * pOther);
int  GUI_RECT_ContainsRectInside   (const GUI_RECT * pRect, const GUI_RECT * pOther);
void GUI_RECT_GetBottomLeft        (const GUI_RECT * pRect, int * px, int * py);
void GUI_RECT_GetBottomLeftEx      (const GUI_RECT * pRect, GUI_POINT * pPoint);
void GUI_RECT_GetBottomRight       (const GUI_RECT * pRect, int * px, int * py);
void GUI_RECT_GetBottomRightEx     (const GUI_RECT * pRect, GUI_POINT * pPoint);
void GUI_RECT_GetCenter            (const GUI_RECT * pRect, int * px, int * py);
void GUI_RECT_GetCenterEx          (const GUI_RECT * pRect, GUI_POINT * pCenter);
int  GUI_RECT_GetHeight            (const GUI_RECT * pRect);
int  GUI_RECT_GetIntersection      (const GUI_RECT * pRect0, const GUI_RECT * pRect1, GUI_RECT * pIntersect);
void GUI_RECT_GetSize              (const GUI_RECT * pRect, int * pWidth, int * pHeight);
void GUI_RECT_GetTopLeft           (const GUI_RECT * pRect, int * px, int * py);
void GUI_RECT_GetTopLeftEx         (const GUI_RECT * pRect, GUI_POINT * pPoint);
void GUI_RECT_GetTopRight          (const GUI_RECT * pRect, int * px, int * py);
void GUI_RECT_GetTopRightEx        (const GUI_RECT * pRect, GUI_POINT * pPoint);
void GUI_RECT_GetUnion             (const GUI_RECT * pRect0, const GUI_RECT * pRect1, GUI_RECT * pUnion);
int  GUI_RECT_GetWidth             (const GUI_RECT * pRect);
void GUI_RECT_Grow                 (GUI_RECT * pRect, int d);
int  GUI_RECT_Intersecting         (const GUI_RECT * pRect, const GUI_RECT * pOther);
void GUI_RECT_Move                 (GUI_RECT * pRect, int dx, int dy);
void GUI_RECT_Set                  (GUI_RECT * pRect, int x0, int y0, int x1, int y1);
void GUI_RECT_SetBottomLeft        (GUI_RECT * pRect, int x, int y);
void GUI_RECT_SetBottomLeftEx      (GUI_RECT * pRect, const GUI_POINT * pPoint);
void GUI_RECT_SetBottomRight       (GUI_RECT * pRect, int x, int y);
void GUI_RECT_SetBottomRightEx     (GUI_RECT * pRect, const GUI_POINT * pPoint);
void GUI_RECT_SetHeight            (GUI_RECT * pRect, int Height);
void GUI_RECT_SetPos               (GUI_RECT * pRect, int x, int y);
void GUI_RECT_SetSize              (GUI_RECT * pRect, int Width, int Height);
void GUI_RECT_SetTopLeft           (GUI_RECT * pRect, int x, int y);
void GUI_RECT_SetTopLeftEx         (GUI_RECT * pRect, const GUI_POINT * pPoint);
void GUI_RECT_SetTopRight          (GUI_RECT * pRect, int x, int y);
void GUI_RECT_SetTopRightEx        (GUI_RECT * pRect, const GUI_POINT * pPoint);
void GUI_RECT_SetWidth             (GUI_RECT * pRect, int Width);
void GUI_RECT_Shrink               (GUI_RECT * pRect, int d);

//
// Left for compatibility
//
void GUI_AddRect        (GUI_RECT * pDest, const GUI_RECT * pRect, int Dist);
int  GUI_RectsIntersect (const GUI_RECT * pr0, const GUI_RECT * pr1);
void GUI_MoveRect       (GUI_RECT * pRect, int x, int y);
void GUI_MergeRect      (GUI_RECT * pDest, const GUI_RECT * pr0, const GUI_RECT * pr1);
int  GUI__IntersectRects(GUI_RECT * pDest, const GUI_RECT * pr0, const GUI_RECT * pr1);
void GUI__IntersectRect (GUI_RECT * pDest, const GUI_RECT * pr0);
void GUI__ReduceRect    (GUI_RECT * pDest, const GUI_RECT * pRect, int Dist);

/*********************************************************************
*
*       Misc helper functions
*/
I32              GUI__ATan2                (I32 x, I32 y, I32 * ph);
I32              GUI__ASinHQ               (I32 SinHQ);
int              GUI__CompactPixelIndices  (LCD_PIXELINDEX * pBuffer, int NumPixels, int BitsPerPixel);
int              GUI__CompactPixelIndicesEx(LCD_PIXELINDEX * pBuffer, int NumPixels, int BitsPerPixel, const LCD_API_COLOR_CONV * pColorConvAPI);
int              GUI__ConvertColor2Index   (LCD_PIXELINDEX * pBuffer, int NumPixels, int BitsPerPixel, const LCD_API_COLOR_CONV * pColorConvAPI, void * pResult);
void             GUI__Config               (void);
GUI_HMEM         GUI__CopyText             (GUI_HMEM hItem, const char * pItem);
I32              GUI__CosHQ                (I32 Ang1000);
int              GUI__DivideRound          (int a, int b);
I32              GUI__DivideRound32        (I32 a, I32 b);
void             GUI__DrawTwinArc2         (int xl, int xr, int y0,         int r, GUI_COLOR ColorR0, GUI_COLOR ColorR1, GUI_COLOR ColorFill);
void             GUI__DrawTwinArc4         (int x0, int y0, int x1, int y1, int r, GUI_COLOR ColorR0, GUI_COLOR ColorR1, GUI_COLOR ColorFill);
void             GUI__ExpandPixelIndices   (void * pBuffer, int NumPixels, int BitsPerPixel);
void             GUI__ExpandPixelIndicesEx (void * pBufferSRC, void * pBufferDST, int NumPixels, int BitsPerPixel, const LCD_API_COLOR_CONV * pColorConvAPI);
void             GUI__FillTrippleArc       (int x0, int y0, int Size, GUI_COLOR ColorR0, GUI_COLOR ColorR1, GUI_COLOR ColorR2, GUI_COLOR ColorFill);
const GUI_RECT * GUI__GetClipRect          (void);
void             GUI__RegisterExit         (GUI_REGISTER_EXIT * pRegisterExit);
void             GUI__RegisterInit         (GUI_REGISTER_INIT * pRegisterInit);
int              GUI__SetText              (GUI_HMEM * phText, const char * s);
I32              GUI__SinHQ                (I32 Ang1000);
I32              GUI__sqrt32               (I32 Square);
I64              GUI__sqrt64               (I64 Square);
char           * GUI__itoa                 (int Value, char * s, int Base);


/*********************************************************************
*
*       Optional function replacement
*/
void * GUI__memcpy(void * pDest, const void * pSrc, size_t NumBytes);
void * GUI__memset(void * pDest, int c, size_t Cnt);

/*********************************************************************
*
*       Get / Set Attributes
*/
GUI_COLOR GUI_GetBkColor     (void);
int       GUI_GetBkColorIndex(void);
GUI_COLOR GUI_GetColor       (void);
int       GUI_GetColorIndex  (void);
U8        GUI_GetLineStyle   (void);
U16       GUI_GetPenSize     (void);
U8        GUI_GetPenShape    (void);
unsigned  GUI_GetPixelIndex  (int x, int y);

void      GUI_SetBkColor     (GUI_COLOR);
void      GUI_SetColor       (GUI_COLOR);
void      GUI_SetBkColorIndex(LCD_PIXELINDEX Index);
void      GUI_SetColorIndex  (LCD_PIXELINDEX Index);

GUI_COLOR GUI_SetBlendColorEx  (GUI_COLOR Color0, GUI_COLOR Color1, U16 Intens, U16 IMax);
GUI_COLOR GUI_SetBlendColor    (GUI_COLOR Color0, GUI_COLOR Color1, U8 Intens);
GUI_COLOR GUI_SetBlendBkColorEx(GUI_COLOR Color0, GUI_COLOR Color1, U16 Intens, U16 IMax);
GUI_COLOR GUI_SetBlendBkColor  (GUI_COLOR Color0, GUI_COLOR Color1, U8 Intens);

U16       GUI_SetPenSize  (U16 Size);
U8        GUI_SetPenShape (U8 Shape);
U8        GUI_SetLineStyle(U8 Style);

/* Get/Set Character used as decimal point (usually '.' or ',') */
char      GUI_GetDecChar(void);
char      GUI_SetDecChar(char c);

/*********************************************************************
*
*       Color / Index related functions
*/
GUI_COLOR GUI_BlendColors      (GUI_COLOR Color0, GUI_COLOR Color1, U16 Intens, U16 IMax);
int       GUI_Color2Index      (GUI_COLOR color);
GUI_COLOR GUI_Color2VisColor   (GUI_COLOR color);
char      GUI_ColorIsAvailable (GUI_COLOR color);
int       GUI_ColorIsOpaque    (GUI_COLOR Color);
GUI_COLOR GUI_Index2Color      (int Index);
U32       GUI_CalcColorDist    (GUI_COLOR Color0, GUI_COLOR  Color1);
U32       GUI_CalcVisColorError(GUI_COLOR color);

/*********************************************************************
*
*       Error handler
*/
void GUI_SetOnLogFunc  (void (* pFunc)(const char * s));
void GUI_SetOnWarnFunc (void (* pFunc)(const char * s));
void GUI_SetOnErrorFunc(void (* pFunc)(const char * s));

/*********************************************************************
*
*       2d - GL
*/
void GUI_Clear                 (void);
void GUI_ClearRect             (int x0, int y0, int x1, int y1);
void GUI_ClearRectEx           (const GUI_RECT * pRect);
void GUI_CopyRect              (int x0, int y0, int x1, int y1, int dx, int dy);
void GUI_DrawArc               (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_DrawArcHR             (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_DrawArcHREx           (int x0, int y0, int r, int a0, int a1, int c0, int c1);
void GUI_DrawBitmap            (const GUI_BITMAP * pBM, int x0, int y0);
void GUI_DrawBitmapMag         (const GUI_BITMAP * pBM, int x0, int y0, int XMul, int YMul);
void GUI_DrawBitmapEx          (const GUI_BITMAP * pBM, int x0, int y0, int xCenter, int yCenter, int xMag, int yMag);
void GUI_DrawBitmapExp         (int x0, int y0, int XSize, int YSize, int XMul,  int YMul, int BitsPerPixel, int BytesPerLine, const U8 * pData, const GUI_LOGPALETTE * pPal);
void GUI_DrawBitmapHWAlpha     (const GUI_BITMAP * pBM, int x0, int y0);
void GUI_DrawCircle            (int x0, int y0, int r);
void GUI_DrawEllipse           (int x0, int y0, int rx, int ry);
void GUI_DrawEllipseXL         (int x0, int y0, int rx, int ry);
void GUI_DrawGradientH         (int x0, int y0, int x1, int y1, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientHEx       (const GUI_RECT * pRect, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientMH        (int x0, int y0, int y1, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientMHEx      (const GUI_RECT * pRect, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientMV        (int x0, int y0, int x1, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientMVEx      (const GUI_RECT * pRect, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientV         (int x0, int y0, int x1, int y1, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientVEx       (const GUI_RECT * pRect, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientRoundedH  (int x0, int y0, int x1, int y1, int rd, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientRoundedMH (int x0, int y0, int y1, int rd, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientRoundedHEx(const GUI_RECT * pRect, int rd, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientRoundedMHEx(const GUI_RECT * pRect, int rd, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientRoundedV  (int x0, int y0, int x1, int y1, int rd, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientRoundedMV (int x0, int y0, int x1, int rd, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGradientRoundedVEx(const GUI_RECT * pRect, int rd, GUI_COLOR Color0, GUI_COLOR Color1);
void GUI_DrawGradientRoundedMVEx(const GUI_RECT * pRect, int rd, GUI_GRADIENT_INFO * pGradientInfo, int NumColors);
void GUI_DrawGraph             (I16 * pay, int NumPoints, int x0, int y0);
void GUI_DrawGraphEx           (I16 * pay, int NumPoints, int x0, int y0, int Numerator, int Denominator, int MirrorX);
void GUI_DrawHLine             (int y0, int x0, int x1);
void GUI_DrawLine              (int x0, int y0, int x1, int y1);
void GUI_DrawLineRel           (int dx, int dy);
void GUI_DrawLineTo            (int x, int y);
void GUI_DrawPie               (int x0, int y0, int r, int a0, int a1, int Type);
void GUI_DrawPieHR             (int x0, int y0, int r, I32 a0, I32 a1);
void GUI_DrawPixel             (int x, int y);
void GUI_DrawPoint             (int x, int y);
void GUI_DrawPolygon           (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GUI_DrawPolyLine          (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GUI_DrawFocusRect         (const GUI_RECT  * pRect, int Dist);
void GUI_DrawFrame             (int x0, int y0, int x1, int y1, int Size);
void GUI_DrawFrameEx           (const GUI_RECT * pRect, int Size);
void GUI_DrawRect              (int x0, int y0, int x1, int y1);
void GUI_DrawRectEx            (const GUI_RECT * pRect);
void GUI_DrawRoundedFrame      (int x0, int y0, int x1, int y1, int r, int w);
void GUI_DrawRoundedFrameEx    (const GUI_RECT * pRect, int r, int w);
void GUI_DrawRoundedRect       (int x0, int y0, int x1, int y1, int r);
void GUI_DrawRoundedRectEx     (const GUI_RECT * pRect, int r);
void GUI_DrawVLine             (int x0, int y0, int y1);
void GUI_FillCircle            (int x0, int y0, int r);
void GUI_FillEllipse           (int x0, int y0, int rx, int ry);
void GUI_FillEllipseXL         (int x0, int y0, int rx, int ry);
void GUI_FillPolygon           (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GUI_FillRect              (int x0, int y0, int x1, int y1);
void GUI_FillRectEx            (const GUI_RECT * pRect);
void GUI_FillRoundedRect       (int x0, int y0, int x1, int y1, int r);
void GUI_FillRoundedRectEx     (const GUI_RECT * pRect, int r);
void GUI_GetClientRect         (GUI_RECT * pRect);
void GUI_InvertRect            (int x0, int y0, int x1, int y1);
void GUI_MoveRel               (int dx, int dy);
void GUI_MoveTo                (int x, int y);
void GUI_SetAlphaMask8888      (U32 OrMask, U32 AndMask);

/*********************************************************************
*
*       GUI_GET_DATA_FUNC
* 
*  Function description
*    Abstract function type which may refer to either \uref{GUI_GET_DATA_FUNC_I}
*    or \uref{GUI_GET_DATA_FUNC_II}.
*
*    It should only be used for function pointers which accept both above
*    mentioned functions.
*/
typedef int GUI_GET_DATA_FUNC(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off);

/*********************************************************************
*
*       GUI_GET_DATA_FUNC_I
* 
*  Function description
*    A callback which is used for loading external data into RAM.
* 
*    This routine must manage its own buffer to read the external data
*    into and pass the address to this buffer to *ppData.
* 
*  Parameters
*    p:        [IN]     Application defined void pointer, could be e.g.
*                       a file handle.
*    ppData:   [IN/OUT] This function must set ppData to point to the
*                       location where the requested data resides in.
*    NumBytes:          Number of requested bytes.
*    Off:               Defines the offset to use for reading the source data.
* 
*  Return value
*    The number of bytes which were actually read. If the number of read bytes does not match,
*    the drawing function will return immediately.
*/
typedef int GUI_GET_DATA_FUNC_I(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off);

/*********************************************************************
*
*       GUI_GET_DATA_FUNC_II
* 
*  Function description
*    A callback which is used for loading external data into RAM.
* 
*    This routine does not manage its own buffer. Instead, it receives a
*    preallocated buffer to load the data into. The address to this buffer
*    is *ppData.
* 
*  Parameters
*    p:        [IN] Application defined void pointer, could be e.g.
*                   a file handle.
*    ppData:   [IN] Points to the location of the buffer address that needs
*                   to be filled with data read by this function.
*    NumBytes:      Number of requested bytes.
*    Off:           Defines the offset to use for reading the source data.
* 
*  Return value
*    The number of bytes which were actually read. If the number of read bytes does not match,
*    the drawing function will return immediately.
*/
typedef int GUI_GET_DATA_FUNC_II(void * p, const U8 ** ppData, unsigned NumBytes, U32 Off);


/*********************************************************************
*
*       IMAGE file support
*/
/*********************************************************************
*
*       GIF file support
*/
/*********************************************************************
*
*       GUI_GIF_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the GIF format, for more details see
*    \uref{GUI_GET_DATA_FUNC_I}.
*/
typedef GUI_GET_DATA_FUNC_I    GUI_GIF_GET_DATA_FUNC;

int GUI_GIF_Draw           (const void * pGIF, U32 NumBytes,             int x0, int y0);
int GUI_GIF_DrawEx         (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0);
int GUI_GIF_DrawSub        (const void * pGIF, U32 NumBytes,             int x0, int y0, int Index);
int GUI_GIF_DrawSubEx      (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0, int Index);
int GUI_GIF_DrawSubScaled  (const void * pGIF, U32 NumBytes,             int x0, int y0, int Index, int Num, int Denom);
int GUI_GIF_DrawSubScaledEx(GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0, int Index, int Num, int Denom);
int GUI_GIF_GetComment     (const void * pGIF, U32 NumBytes,             U8 * pBuffer, int MaxSize, int Index);
int GUI_GIF_GetCommentEx   (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, U8 * pBuffer, int MaxSize, int Index);
int GUI_GIF_GetImageInfo   (const void * pGIF, U32 NumBytes,             GUI_GIF_IMAGE_INFO * pInfo, int Index);
int GUI_GIF_GetImageInfoEx (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, GUI_GIF_IMAGE_INFO * pInfo, int Index);
int GUI_GIF_GetInfo        (const void * pGIF, U32 NumBytes,             GUI_GIF_INFO * pInfo);
int GUI_GIF_GetInfoEx      (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p, GUI_GIF_INFO * pInfo);
int GUI_GIF_GetXSize       (const void * pGIF);
int GUI_GIF_GetXSizeEx     (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p);
int GUI_GIF_GetYSize       (const void * pGIF);
int GUI_GIF_GetYSizeEx     (GUI_GIF_GET_DATA_FUNC * pfGetData, void * p);
int GUI_GIF_SetFillTrans   (int OnOff);

/*********************************************************************
*
*       BMP file support
*/
/*********************************************************************
*
*       GUI_BMP_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the BMP format, for more details see
*    \uref{GUI_GET_DATA_FUNC_I}.
*/
typedef GUI_GET_DATA_FUNC_I     GUI_BMP_GET_DATA_FUNC;

int  GUI_BMP_Draw        (const void * pFileData,                      int x0, int y0);
int  GUI_BMP_DrawEx      (GUI_BMP_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0);
int  GUI_BMP_DrawScaled  (const void * pFileData,                      int x0, int y0, int Num, int Denom);
int  GUI_BMP_DrawScaledEx(GUI_BMP_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0, int Num, int Denom);
int  GUI_BMP_GetXSize    (const void * pFileData);
int  GUI_BMP_GetXSizeEx  (GUI_BMP_GET_DATA_FUNC * pfGetData, void * p);
int  GUI_BMP_GetYSize    (const void * pFileData);
int  GUI_BMP_GetYSizeEx  (GUI_BMP_GET_DATA_FUNC * pfGetData, void * p);
void GUI_BMP_EnableAlpha (void);
void GUI_BMP_EnableBE    (int OnOff);
void GUI_BMP_DisableAlpha(void);

/*********************************************************************
*
*       PNG file support
*/
/*********************************************************************
*
*       GUI_PNG_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the PNG format, for more details see
*    \uref{GUI_GET_DATA_FUNC_II}.
*/
typedef GUI_GET_DATA_FUNC_II    GUI_PNG_GET_DATA_FUNC;

int GUI_PNG_Draw      (const void * pFileData, int DataSize,        int x0, int y0);
int GUI_PNG_DrawEx    (GUI_PNG_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0);
int GUI_PNG_GetXSize  (const void * pFileData, int FileSize);
int GUI_PNG_GetXSizeEx(GUI_PNG_GET_DATA_FUNC * pfGetData, void * p);
int GUI_PNG_GetYSize  (const void * pFileData, int FileSize);
int GUI_PNG_GetYSizeEx(GUI_PNG_GET_DATA_FUNC * pfGetData, void * p);

/*********************************************************************
*
*       JPEG file support
*/
/*********************************************************************
*
*       GUI_JPEG_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the JPEG format, for more details see
*    \uref{GUI_GET_DATA_FUNC_I}.
*/
typedef GUI_GET_DATA_FUNC_I     GUI_JPEG_GET_DATA_FUNC;

/*********************************************************************
*
*       GUI_JPEG_INFO
*
*   Description
*     Information about a JPEG image.
*/
typedef struct {
  int XSize;       // X-size of the image.
  int YSize;       // Y-size of the image.
  int Progessive;  // Indicates if JPEG is progressive or not
} GUI_JPEG_INFO;

typedef struct GUI_JPEG_DCONTEXT GUI_JPEG_DCONTEXT;
/*********************************************************************
*
*       GUI_JPEG_WRITECLIPPEDPIXELS_FUNC
*
*   Description
*     Function that retrieves the decoded result of a JPEG and writes
*     the pixels to the display.
* 
*   Parameters
*     x0:            X drawing position of the JPEG.
*     y0:            Y drawing position of the JPEG.
*     xSize:         Width of one line.
*     pColor:        [IN] Pointer to decoded JPEG data.
*     pContext:      [IN] Pointer to JPEG context.
*     pColorConvAPI: [IN] Pointer to selected color conversion API.
* 
*/
typedef void (GUI_JPEG_WRITECLIPPEDPIXELS_FUNC)(int x0, int y0, int xSize, LCD_COLOR * pColor, GUI_JPEG_DCONTEXT * pContext, const LCD_API_COLOR_CONV * pColorConvAPI);

int                                GUI_JPEG_Draw            (const void * pFileData, int DataSize,         int x0, int y0);
int                                GUI_JPEG_DrawEx          (GUI_JPEG_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0);
int                                GUI_JPEG_DrawScaled      (const void * pFileData, int DataSize,         int x0, int y0, int Num, int Denom);
int                                GUI_JPEG_DrawScaledEx    (GUI_JPEG_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0, int Num, int Denom);
int                                GUI_JPEG_GetInfo         (const void * pFileData, int DataSize,         GUI_JPEG_INFO * pInfo);
int                                GUI_JPEG_GetInfoEx       (GUI_JPEG_GET_DATA_FUNC * pfGetData, void * p, GUI_JPEG_INFO * pInfo);
void                               GUI_JPEG_SetpfDrawEx     (int (* pfDrawEx)(GUI_JPEG_GET_DATA_FUNC * pfGetData, void * p, int x0, int y0));
GUI_JPEG_WRITECLIPPEDPIXELS_FUNC * GUI_JPEG_SetpfWritePixels(GUI_JPEG_WRITECLIPPEDPIXELS_FUNC * pFunc);


/*********************************************************************
*
*       GPU drivers
*/
//
// Opaque driver type for public usage.
//
typedef void  GUI_GPU_DRIVER_STRUCT;

/*********************************************************************
*
*       GUI_GPU_DRIVER
* 
*  Description
*    All available GPU drivers for run-time usage with the GUI_GPU_... routines.
*/
typedef const GUI_GPU_DRIVER_STRUCT * (GUI_GPU_DRIVER)(void);

#if (GUI_SUPPORT_GPU)
  // 
  // The pointer passed to identify a driver is a function
  // so that only the directly referenced drivers are linked.
  //
  #define GUI_GPU_CHROMART    (&GUI_GPU_ChromART)
  #define GUI_GPU_PXP         (&GUI_GPU_PixelPipeline)
  #define GUI_GPU_DAVE2D      (&GUI_GPU_Dave2D)
  #define GUI_GPU_OPENVG      (&GUI_GPU_OpenVG)
  #define GUI_GPU_VGLITE      (&GUI_GPU_VGLite)
  #define GUI_GPU_NEMAVG      (&GUI_GPU_NemaVG)
  #define GUI_GPU_NANOVG      (&GUI_GPU_NanoVG)
  #define GUI_GPU_THORVG      (&GUI_GPU_ThorVG)
  #define GUI_GPU_IPA         (&GUI_GPU_ImgProcAccel)
  #define GUI_GPU_ARM2D       (&GUI_GPU_Arm2D)
  #define GUI_GPU_NULL        (&GUI_GPU_Null)
#else
  #define GUI_GPU_CHROMART    NULL
  #define GUI_GPU_PXP         NULL
  #define GUI_GPU_DAVE2D      NULL
  #define GUI_GPU_OPENVG      NULL
  #define GUI_GPU_VGLITE      NULL
  #define GUI_GPU_NEMAVG      NULL
  #define GUI_GPU_NANOVG      NULL
  #define GUI_GPU_THORVG      NULL
  #define GUI_GPU_IPA         NULL
  #define GUI_GPU_ARM2D       NULL
  #define GUI_GPU_NULL        NULL
#endif

/*********************************************************************
*
*       SVG file support
*/
typedef GUI_HMEM    GUI_SVG_Handle;

/*********************************************************************
*
*       GUI_SVG_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the SVG format, for more details see
*    \uref{GUI_GET_DATA_FUNC_II}.
*/
typedef GUI_GET_DATA_FUNC_II    GUI_SVG_GET_DATA_FUNC;

/*********************************************************************
*
*       GUI_POINTF
*
*  Description
*    Defines a point in floating point coordinates.
*/
typedef struct {
  float x; // X coordinate in floating points.
  float y; // Y coordinate in floating points.
} GUI_POINTF;

/*********************************************************************
*
*       GUI_MATRIX
*
*  Description
*    A column-major 3x3 matrix used for all kinds of affine transformations.
*
*  Additional information
*    emWin uses column-major matrices which means that the matrix elements
*    are stored in columns from top to bottom and left to right.
* 
*    The order is the same as in OpenVG, therefore GUI_MATRIX is implicitly
*    convertible to the 3x3 matrix type used in OpenVG.
*/
typedef struct {
  float sx;   // The scaling factor in the x-direction.
  float shy;  // The shearing factor in the y-direction.
  float w0;   // Should be left as \c{0.0F} to ensure affinity.
  float shx;  // The shearing factor in the x-direction.
  float sy;   // Scaling factor in y.
  float w1;   // Should be left as \c{0.0F} to ensure affinity.
  float tx;   // The translation factor in the x-direction.
  float ty;   // The translation factor in the y-direction. 
  float w2;   // Should be left as \c{1.0F} to ensure affinity.
} GUI_MATRIX;

void    GUI_MATRIX_Initialize   (GUI_MATRIX * pMatrix, float sx, float shx, float tx, float shy, float sy, float ty, float w0, float w1, float w2);
void    GUI_MATRIX_Identity     (GUI_MATRIX * pMatrix);
float * GUI_MATRIX_GetCellPtr   (GUI_MATRIX * pMatrix, unsigned Row, unsigned Col);
int     GUI_MATRIX_Equals       (const GUI_MATRIX * p0, const GUI_MATRIX * p1);
void    GUI_MATRIX_Multiply     (GUI_MATRIX * pMatrix, const GUI_MATRIX * pMult);
void    GUI_MATRIX_MultiplyPoint(const GUI_MATRIX * pMatrix, GUI_POINTF * pPoint);
void    GUI_MATRIX_Translate    (GUI_MATRIX * pMatrix, float tx, float ty);
void    GUI_MATRIX_Scale        (GUI_MATRIX * pMatrix, float sx, float sy);
void    GUI_MATRIX_Rotate       (GUI_MATRIX * pMatrix, float a);
void    GUI_MATRIX_Shear        (GUI_MATRIX * pMatrix, float shx, float shy);

/*********************************************************************
*
*       GUI_SVG_VIEWBOX
*
*  Description
*    The SVG's view box, as it is defined by the \c{viewBox} attribute
*    in the \c{<svg>} tag.
*/
typedef struct {
  float x;      // X position of the view box.
  float y;      // Y position of the view box.
  float xSize;  // Width of the view box.
  float ySize;  // Height of the view box.
} GUI_SVG_VIEWBOX;

/*********************************************************************
*
*       GUI_SVG_BBOX
*
*  Description
*    Boundary box of an SVG based on the currently set parameters like position,
*    scaling, rotation, etc.
*/
typedef struct {
  float xMin;  // Minimum X position on the display in floating point coordinates.
  float yMin;  // Minimum Y position on the display in floating point coordinates.
  float xMax;  // Maximum X position on the display in floating point coordinates.
  float yMax;  // Maximum Y position on the display in floating point coordinates.
} GUI_SVG_BBOX;

/*********************************************************************
*
*       GUI_SVG_INFO
*
*  Description
*    Information about an SVG document, generated by the \c{GUI_SVG_GetInfo...()}
*    functions.
* 
*  Additional information
*    In case the \c{"width"} and \c{"height"} attributes of the SVG are in any other
*    unit than pixels, the dimensions will be converted into pixels internally.
*    Therefore, the struct members Width and Height are always in pixels.
*    The conversion from other units into pixels is done using the currently set DPI
*    (see GUI_SVG_SetDPI()).
*/
typedef struct {
  GUI_SVG_VIEWBOX ViewBox;  // Viewbox of the SVG, defines the coordinate range.
  GUI_SVG_BBOX    BBox;     // Boundary box of the SVG, based on the currently set affine transformation matrix.
  float           xSize;    // Viewport width of the SVG, defined by the \c{"width"} attribute in the \c{<svg>} tag.
  float           ySize;    // Viewport height of the SVG, defined by the \c{"height"} attribute in the \c{<svg>} tag.
} GUI_SVG_INFO;

/*********************************************************************
*
*       GUI_SVG_HOOKS
*
*  Description
*    Allows to hook in into various spots during the SVG drawing process.
* 
*    Only the desired hooks need to be set. The hooks can be set with GUI_SVG_SetHooks().
*/
typedef struct {
  U8   (* pfPreInitDriverHook) (void);  // Called before the SVG driver is initialized. Returns 1 on error
                                        // to abort the driver initialization.
  U8   (* pfPostInitDriverHook)(void);  // Called when the SVG driver initialization has finished. Returns 1 on error
                                        // to abort the driver initialization.
  void (* pfDeinitDriverHook)  (void);  // Called after the SVG driver has been de-initialized.
  void (* pfSwitchBufferHook)  (void);  // Called during the drawing process, after the drawing commands have been flushed.
  void (* pfBeginDrawPreHook)  (void);  // Called before the initialization of the drawing process of an SVG.
  void (* pfBeginDrawPostHook) (void);  // Called at the end of the initialization of the drawing process of an SVG.
  void (* pfEndDrawHook)       (void);  // Called once the SVG drawing process has finished.
} GUI_SVG_HOOKS;

//
// SVG module related
//
int      GUI_SVG_Enable           (int Enable);
void     GUI_SVG_EnablePNG        (void);
void     GUI_SVG_EnableJPEG       (void);
void     GUI_SVG_EnableGIF        (void);
void     GUI_SVG_EnableBMP        (void);
unsigned GUI_SVG_SetDPI           (unsigned NumDotsPerInch);
unsigned GUI_SVG_SetFileBufferSize(unsigned NumBytes);
//
// SVG drawing API
//
int GUI_SVG_Draw               (const void * pFile, U32 FileSize, float x, float y);
int GUI_SVG_DrawEx             (GUI_SVG_GET_DATA_FUNC * pfGetData, void * p, float x, float y);
int GUI_SVG_DrawScaled         (const void * pFile, U32 FileSize, float x, float y, float Scale);
int GUI_SVG_DrawScaledEx       (GUI_SVG_GET_DATA_FUNC * pfGetData, void * p, float x, float y, float Scale);
int GUI_SVG_DrawScaledRotated  (const void * pFile, U32 FileSize, float x, float y, float Scale, float Angle);
int GUI_SVG_DrawScaledRotatedEx(GUI_SVG_GET_DATA_FUNC * pfGetData, void * p, float x, float y, float Scale, float Angle);
//
// SVG handle API
//
GUI_SVG_Handle GUI_SVG_Create         (const void * pFile, U32 FileSize);
GUI_SVG_Handle GUI_SVG_CreateEx       (GUI_SVG_GET_DATA_FUNC * pfGetData, void * p);
void           GUI_SVG_Delete         (GUI_SVG_Handle hSVG);
int            GUI_SVG_DrawH          (GUI_SVG_Handle hSVG, float x, float y);
void           GUI_SVG_EnableCacheMode(GUI_SVG_Handle hSVG, int Enable);
void           GUI_SVG_Identity       (GUI_SVG_Handle hSVG);
int            GUI_SVG_Render         (GUI_SVG_Handle hSVG);
void           GUI_SVG_Rotate         (GUI_SVG_Handle hSVG, float Angle);
void           GUI_SVG_RotateEx       (GUI_SVG_Handle hSVG, float Angle, float x, float y);
void           GUI_SVG_Scale          (GUI_SVG_Handle hSVG, float xScale, float yScale);
void           GUI_SVG_ScaleEx        (GUI_SVG_Handle hSVG, float xScale, float yScale, float x, float y);
void           GUI_SVG_ScaleToSize    (GUI_SVG_Handle hSVG, float xSize, float ySize);
void           GUI_SVG_SetBkColor     (GUI_SVG_Handle hSVG, GUI_COLOR BkColor);
void           GUI_SVG_Transform      (GUI_SVG_Handle hSVG, const GUI_MATRIX * pMatrix);
void           GUI_SVG_Translate      (GUI_SVG_Handle hSVG, float x, float y);
//
// Info API
//
int GUI_SVG_GetInfo  (const void * pFile, U32 FileSize, GUI_SVG_INFO * pInfo);
int GUI_SVG_GetInfoEx(GUI_SVG_GET_DATA_FUNC * pfGetData, void * p, GUI_SVG_INFO * pInfo);
int GUI_SVG_GetInfoH (GUI_SVG_Handle hSVG, GUI_SVG_INFO * pInfo);

/*********************************************************************
*
*       Compatability support for old GUI_SVG_DRIVER_... API
*
**********************************************************************
*/
// 
// The pointer passed to identify a driver is a function
// so that only the directly referenced drivers are linked.
//
#define GUI_SVG_DRIVER_OPENVG    GUI_GPU_OPENVG
#define GUI_SVG_DRIVER_VGLITE    GUI_GPU_VGLITE
#define GUI_SVG_DRIVER_NEMAVG    GUI_GPU_NEMAVG
#define GUI_SVG_DRIVER_NANOVG    GUI_GPU_NANOVG
#define GUI_SVG_DRIVER           GUI_GPU_DRIVER

#if (GUI_SUPPORT_GPU)
  //
  // GUI_SVG_DRIVER API
  //
  GUI_GPU_DRIVER * GUI_SVG_GetSelectedGPU (void);
  int              GUI_SVG_SelectGPU      (GUI_GPU_DRIVER * pDriver);
  void             GUI_SVG_SetGPUHooks    (GUI_GPU_DRIVER * pDriver, const GUI_SVG_HOOKS * pHooks);

  //
  // Compatability macros
  //
  #define GUI_SVG_DRIVER_BindDynamicAPI(pDrv, pAPI, cb)   GUI_GPU_BindDynamicAPI(pDrv, cb)
  #define GUI_SVG_DRIVER_BindAPI                          GUI_GPU_BindAPI
  #define GUI_SVG_DRIVER_HasBoundAPI                      GUI_GPU_HasBoundAPI
  #define GUI_SVG_DRIVER_SetVersion                       GUI_GPU_SetVersion
  #define GUI_SVG_DRIVER_GetSelected                      GUI_SVG_GetSelectedGPU
  #define GUI_SVG_DRIVER_Select                           GUI_SVG_SelectGPU
  #define GUI_SVG_DRIVER_SetHooks                         GUI_SVG_SetGPUHooks
#endif

/*********************************************************************
*
*       GUI_TIMER module
*/
#define GUI_TIMER_CF_WINDOW (1 << 0)
#define GUI_TIMER_CF_CURSOR (1 << 1)

typedef GUI_HMEM GUI_TIMER_HANDLE;

typedef struct {
  GUI_TIMER_TIME   Time;
  PTR_ADDR         Context;
  GUI_TIMER_HANDLE hTimer;
} GUI_TIMER_MESSAGE;

typedef void GUI_TIMER_CALLBACK(/*const*/ GUI_TIMER_MESSAGE* pTM);

GUI_TIMER_HANDLE GUI_TIMER_Create   (GUI_TIMER_CALLBACK * cb, GUI_TIMER_TIME Time, PTR_ADDR Context, U16 Flags);
void             GUI_TIMER_Delete   (GUI_TIMER_HANDLE hObj);

/* Methods changing properties */
GUI_TIMER_TIME GUI_TIMER_GetPeriod(GUI_TIMER_HANDLE hObj);
void           GUI_TIMER_SetPeriod(GUI_TIMER_HANDLE hObj, GUI_TIMER_TIME Period);
void           GUI_TIMER_SetDelay (GUI_TIMER_HANDLE hObj, GUI_TIMER_TIME Delay);
void           GUI_TIMER_Restart  (GUI_TIMER_HANDLE hObj);
int            GUI_TIMER_GetFlag  (GUI_TIMER_HANDLE hObj, int Flag); /* Not to be documented */
int            GUI_TIMER_Exec     (void);

/*********************************************************************
*
*       MOVIE file support
*/
/*********************************************************************
*
*       Movie notifications
*
*   Description
*     Notifications sent to the movie's callback function. The
*     callback function can be set with GUI_MOVIE_SetpfNotify().
*/
#define GUI_MOVIE_NOTIFICATION_PREDRAW  0    // Sent immediately before a frame is drawn.
#define GUI_MOVIE_NOTIFICATION_POSTDRAW 1    // Sent immediately after a frame is drawn.
#define GUI_MOVIE_NOTIFICATION_START    2    // Sent when starting to play a movie.
#define GUI_MOVIE_NOTIFICATION_STOP     3    // Sent when the movie has stopped.
#define GUI_MOVIE_NOTIFICATION_DELETE   4    // Sent when the movie has been deleted.

typedef GUI_HMEM GUI_MOVIE_HANDLE;

typedef void GUI_MOVIE_FUNC(GUI_MOVIE_HANDLE hMovie, int Notification, U32 CurrentFrame);

/*********************************************************************
*
*       GUI_MOVIE_INFO
*
*   Description
*     Information about a movie.
*/
typedef struct {
  int xSize;         // Horizontal resolution of the movie in pixels.
  int ySize;         // Vertical resolution of the movie in pixels.
  int msPerFrame;    // Period of one frame in ms.
  U32 NumFrames;     // Number of frames of the movie file.
} GUI_MOVIE_INFO;

/*********************************************************************
*
*       GUI_MOVIE_GET_DATA_FUNC
* 
*  Description
*    GetData function used for emWin movie files (EMF), for more details see
*    \uref{GUI_GET_DATA_FUNC_II}.
*/
typedef  GUI_GET_DATA_FUNC_II    GUI_MOVIE_GET_DATA_FUNC;

GUI_MOVIE_HANDLE GUI_MOVIE_Create         (const void * pFileData, U32 FileSize, GUI_MOVIE_FUNC * pfNotify);
GUI_MOVIE_HANDLE GUI_MOVIE_CreateEx       (GUI_MOVIE_GET_DATA_FUNC * pfGetData, void * pParam, GUI_MOVIE_FUNC * pfNotify);
int              GUI_MOVIE_Delete         (GUI_MOVIE_HANDLE hMovie);
void             GUI_MOVIE_DrawFrame      (GUI_MOVIE_HANDLE hMovie, int Index, int x, int y);
U32              GUI_MOVIE_GetFrameIndex  (GUI_MOVIE_HANDLE hMovie);
int              GUI_MOVIE_GetInfo        (const void * pFileData, U32 FileSize, GUI_MOVIE_INFO * pInfo);
int              GUI_MOVIE_GetInfoEx      (GUI_MOVIE_GET_DATA_FUNC * pfGetData, void * pParam, GUI_MOVIE_INFO * pInfo);
int              GUI_MOVIE_GetInfoH       (GUI_MOVIE_HANDLE hMovie, GUI_MOVIE_INFO * pInfo);
int              GUI_MOVIE_GetNumFrames   (GUI_MOVIE_HANDLE hMovie);
int              GUI_MOVIE_GetPos         (GUI_MOVIE_HANDLE hMovie, int * pxPos, int * pyPos, int * pxSize, int * pySize);
int              GUI_MOVIE_GotoFrame      (GUI_MOVIE_HANDLE hMovie, U32 Frame);
int              GUI_MOVIE_IsPlaying      (GUI_MOVIE_HANDLE hMovie);
int              GUI_MOVIE_Pause          (GUI_MOVIE_HANDLE hMovie);
int              GUI_MOVIE_Play           (GUI_MOVIE_HANDLE hMovie);
int              GUI_MOVIE_SetPeriod      (GUI_MOVIE_HANDLE hMovie, unsigned Period);
void             GUI_MOVIE_SetpfNotify    (GUI_MOVIE_FUNC * pfNotify);
int              GUI_MOVIE_SetPos         (GUI_MOVIE_HANDLE hMovie, int xPos, int yPos);
void             GUI_MOVIE_SetSecondHandle(GUI_MOVIE_HANDLE hMovie, void * pParamTable);
int              GUI_MOVIE_Show           (GUI_MOVIE_HANDLE hMovie, int xPos, int yPos, int DoLoop);

void             GUI_MOVIE__OnTimeSlice (GUI_MOVIE_HANDLE hMovie, int Run, int DeactivateWM);
int              GUI_MOVIE__Play        (GUI_MOVIE_HANDLE hMovie, GUI_TIMER_CALLBACK * cbTimer, int DoLoop, GUI_HMEM hExtra);
int              GUI_MOVIE__Pause       (GUI_MOVIE_HANDLE hMovie, int ShowFrame, int DeactivateWM);
void             GUI_MOVIE__SetTimerFlag(GUI_MOVIE_HANDLE hMovie);
GUI_MOVIE_HANDLE GUI_MOVIE__CreateUser  (GUI_MOVIE_GET_DATA_FUNC * pfGetData, void * pParam, GUI_MOVIE_FUNC * pfNotify, void * pVoid);
void           * GUI_MOVIE__GetUserData (GUI_MOVIE_HANDLE hMovie);
void             GUI_MOVIE__SetUserData (GUI_MOVIE_HANDLE hMovie, void * pVoid);

/*********************************************************************
*
*       Splines
*/
GUI_HMEM GUI_SPLINE_Create  (const int * px, const int * py, unsigned NumPoints);
void     GUI_SPLINE_Draw    (GUI_HMEM hSpline, int x, int y);
void     GUI_SPLINE_Delete  (GUI_HMEM hSpline);
I16      GUI_SPLINE_GetY    (GUI_HMEM hSpline, unsigned Index, float * py);
unsigned GUI_SPLINE_GetXSize(GUI_HMEM hSpline);
void     GUI_SPLINE_DrawAA  (GUI_HMEM hSpline, int x, int y, unsigned Width);

/*********************************************************************
*
*       Cursor routines
*/
#define GUI_CURSOR_SHOW 0
#define GUI_CURSOR_HIDE 1

typedef struct {
  const GUI_BITMAP  * pBitmap;
  int                 xHot;
  int                 yHot;
} GUI_CURSOR;

/*********************************************************************
*
*       GUI_CURSOR_ANIM
*
*  Description
*    Structure that stores information about a cursor animation used by GUI_CURSOR_SelectAnim().
*
*  Additional information
*    The bitmaps addressed by ppBM need to fulfill with the following requirements:
*    
*    \item They need to have exactly the same X- and Y-size.
*    \item They should not be compressed.
*    \item They need to be transparent.
*    \item They need to be palette based bitmaps with 1, 2, 4 or 8bpp.
*
*    Other bitmaps or insufficient memory cause the function to fail.
*
*    The pPeriod is only required if the periods for the images are different. If the same
*    period should be used for all images Period should be used instead of pPeriod. In
*    this case pPeriod should be \c{NULL}.
*
*    xHot and yHot determine the hot spot position of the cursor. This means the relative
*    position in X and Y from the upper left corner of the image to the position of the
*    pointer input device.
*
*    Customized cursors can be realized by passing a pointer to a custom defined
*    GUI_CURSOR_ANIM structure.
*/
typedef struct {
  const GUI_BITMAP ** ppBm;       // Pointer to an array of pointers to bitmaps to be used for the animated cursor.
  int                 xHot;       // X-position of hot spot. Details can be found below.
  int                 yHot;       // Y-position of hot spot. Details can be found below.
  unsigned            Period;     // Period to be used to switch between the images.
  const unsigned    * pPeriod;    // Pointer to an array containing the periods to be used to switch between the images.
  int                 NumItems;   // Number of images used for the animation.
} GUI_CURSOR_ANIM;

#if GUI_SUPPORT_CURSOR
  int                GUI_CURSOR_GetState     (void);
  int                GUI_CURSOR_GetStateEx   (int Layer);
  void               GUI_CURSOR_Hide         (void);
  void               GUI_CURSOR_HideEx       (int Layer);
  const GUI_CURSOR * GUI_CURSOR_Select       (const GUI_CURSOR * pCursor);
  const GUI_CURSOR * GUI_CURSOR_SelectEx     (const GUI_CURSOR * pCursor, int Layer);
  int                GUI_CURSOR_SelectAnim   (const GUI_CURSOR_ANIM * pCursorAnim);
  int                GUI_CURSOR_SelectAnimEx (const GUI_CURSOR_ANIM * pCursorAnim, int LayerIndex);
  int                GUI_CURSOR_SetBitmap    (const GUI_BITMAP * pBM);
  int                GUI_CURSOR_SetBitmapEx  (const GUI_BITMAP * pBM, int Layer);
  void               GUI_CURSOR_SetPosition  (int x, int y);
  void               GUI_CURSOR_SetPositionEx(int xNewPos, int yNewPos, int Layer);
  void               GUI_CURSOR_Show         (void);
  void               GUI_CURSOR_ShowEx       (int Layer);
  GUI_HSPRITE        GUI_CURSOR__GetSpriteEx (int LayerIndex, int * pxPos, int * pyPos);
  void               GUI_CURSOR__SetSpriteEx (GUI_HSPRITE hSprite, const GUI_CURSOR * pCursor, int LayerIndex);
#else
  #define GUI_CURSOR_Show();
  #define GUI_CURSOR_Clear();
#endif

/*********************************************************************
*
*       Sprite support
*/
#define GUI_SPRITE_CF_STAYONTOP (1 << 0)
#define GUI_SPRITE_CF_SHOW      (1 << 1)

#define GUI_SPRITE_SHOW 0
#define GUI_SPRITE_HIDE 1

GUI_HSPRITE GUI_SPRITE__CreateEx           (const GUI_BITMAP * pBM, int x, int y, int Layer, U16 Flags); /* Not to be documented, only used by cursor modul */
void        GUI_SPRITE__SetCallback        (GUI_HSPRITE hSprite, GUI_HMEM hContext, void (* pCB)(GUI_HSPRITE, int));
GUI_HSPRITE GUI_SPRITE_Create              (const GUI_BITMAP * pBM, int x, int y);
GUI_HSPRITE GUI_SPRITE_CreateAnim          (const GUI_BITMAP ** ppBm, int x, int y, unsigned Period, const unsigned * pPeriod, int NumItems);
GUI_HSPRITE GUI_SPRITE_CreateEx            (const GUI_BITMAP * pBM, int x, int y, int Layer);
GUI_HSPRITE GUI_SPRITE_CreateExAnim        (const GUI_BITMAP ** ppBm, int x, int y, unsigned Period, const unsigned * pPeriod, int NumItems, int LayerIndex);
GUI_HSPRITE GUI_SPRITE_CreateHidden        (const GUI_BITMAP * pBM, int x, int y);
GUI_HSPRITE GUI_SPRITE_CreateHiddenEx      (const GUI_BITMAP * pBM, int x, int y, int Layer);
void        GUI_SPRITE_Delete              (GUI_HSPRITE hSprite);
int         GUI_SPRITE_GetState            (GUI_HSPRITE hSprite);
void        GUI_SPRITE_Hide                (GUI_HSPRITE hSprite);
int         GUI_SPRITE_SetBitmap           (GUI_HSPRITE hSprite, const GUI_BITMAP * pBM);
int         GUI_SPRITE_SetBitmapAndPosition(GUI_HSPRITE hSprite, const GUI_BITMAP * pBM, int x, int y);
int         GUI_SPRITE_SetLoop             (GUI_HSPRITE hSprite, int OnOff);
void        GUI_SPRITE_SetPosition         (GUI_HSPRITE hSprite, int x, int y);
int         GUI_SPRITE_StartAnim           (GUI_HSPRITE hSprite);
int         GUI_SPRITE_StopAnim            (GUI_HSPRITE hSprite);
void        GUI_SPRITE_Show                (GUI_HSPRITE hSprite);

/*********************************************************************
*
*       Cursors and their bitmaps
*/
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorArrowS,  GUI_CursorArrowSI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorArrowM,  GUI_CursorArrowMI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorArrowL,  GUI_CursorArrowLI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorCrossS,  GUI_CursorCrossSI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorCrossM,  GUI_CursorCrossMI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorCrossL,  GUI_CursorCrossLI;
extern GUI_CONST_STORAGE GUI_CURSOR GUI_CursorHeaderM, GUI_CursorHeaderMI;
extern GUI_CONST_STORAGE GUI_CURSOR_ANIM GUI_CursorAnimHourglassM;

/*********************************************************************
*
*       GUI_WRAPMODE
*
*  Description
*    Configuration how text will be wrapped.
*
*  Additional information
*    If word wrapping should be performed and the given rectangle is too small for a word, char
*    wrapping is executed at this word.
*/
typedef enum {
  GUI_WRAPMODE_NONE,   // No wrapping will be performed.
  GUI_WRAPMODE_WORD,   // Text is wrapped word wise.
  GUI_WRAPMODE_CHAR    // Text is wrapped char wise.
} GUI_WRAPMODE;

/*********************************************************************
*
*       Text related routines
*/
void             GUI_DispCEOL                  (void);
void             GUI_DispChar                  (U16 c);
void             GUI_DispCharAt                (U16 c, I16P x, I16P y);
void             GUI_DispChars                 (U16 c, int Cnt);
void             GUI_DispNextLine              (void);
void             GUI_DispString                (const char * s);
void             GUI_DispStringAt              (const char * s, int x, int y);
void             GUI_DispStringAtCEOL          (const char * s, int x, int y);
void             GUI_DispStringAtFmt           (const char * s, int x, int y, ...);
void             GUI_DispStringAtFmtV          (const char * s, int x, int y, va_list Args);
void             GUI_DispStringHCenterAt       (const char * s, int x, int y);
void             GUI__DispStringInRect         (const char * s, const GUI_RECT * pRect, int TextAlign, int MaxNumChars);
void             GUI_DispStringInRect          (const char * s, const GUI_RECT * pRect, int TextAlign);
void             GUI_DispStringInRectFmt       (const char * s, const GUI_RECT * pRect, int TextAlign, ...);
void             GUI_DispStringInRectFmtV      (const char * s, const GUI_RECT * pRect, int TextAlign, va_list Args);
void             GUI_DispStringInRectMax       (const char * s, const GUI_RECT * pRect, int TextAlign, int MaxLen); /* Not to be doc. */
void             GUI_DispStringInRectWrap      (const char * s, const GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode);
#if GUI_SUPPORT_ROTATION
void             GUI_DispStringInRectEx        (const char * s, const GUI_RECT * pRect, int TextAlign, int MaxLen, const GUI_ROTATION * pLCD_Api);
void             GUI_DispStringInRectWrapEx    (const char * s, const GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode, const GUI_ROTATION * pLCD_Api);
void             GUI_DispStringInRectWrapExFmt (const char * s, const GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode, const GUI_ROTATION * pLCD_Api, ...);
void             GUI_DispStringInRectWrapExFmtV(const char * s, const GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode, const GUI_ROTATION * pLCD_Api, va_list Args);
#endif
void             GUI_DispStringLen             (const char * s, int Len);
U16              GUI_GetCharFromPos            (const char * pText, int x, int * pIndex);
const GUI_FONT * GUI_GetFallbackFont           (U16 c);
void             GUI_GetTextExtend             (GUI_RECT* pRect, const char * s, int Len);
int              GUI_GetYAdjust                (void);
int              GUI_GetDispPosX               (void);
int              GUI_GetDispPosY               (void);
const GUI_FONT * GUI_GetFont                   (void);
int              GUI_GetCharDistX              (U16 c);
int              GUI_GetCharDistXEx            (U16 c, int * pSizeX);
int              GUI_GetStringDistX            (const char * s);
int              GUI_GetStringDistXEx          (const char * s, int n);
GUI_DRAWMODE     GUI_GetDrawMode               (void);
int              GUI_GetFontDistY              (void);
int              GUI_GetFontSizeY              (void);
void             GUI_GetFontInfo               (const GUI_FONT * pFont, GUI_FONTINFO * pfi);
void             GUI_GetOrg                    (int * px, int * py);
int              GUI_GetYSizeOfFont            (const GUI_FONT * pFont);
int              GUI_GetYDistOfFont            (const GUI_FONT * pFont);
int              GUI_GetTextAlign              (void);
int              GUI_GetTextMode               (void);
char             GUI_IsInFont                  (const GUI_FONT * pFont, U16 c);
U8               GUI_SetClearTextRectMode      (unsigned OnOff);
const GUI_FONT * GUI_SetFallbackFont           (const GUI_FONT * pNewFont);
void             GUI_SetFuncGetFallbackFont    (const GUI_FONT * (* pfcbGetFallbackFont)(U16 c));
int              GUI_SetTextAlign              (int Align);
int              GUI_SetTextMode               (int Mode);
char             GUI_SetTextStyle              (char Style);
int              GUI_SetLBorder                (int x);
U8               GUI_SetStrikeWidth            (U8 StrikeWidth);
const GUI_FONT * GUI_SetFont                   (const GUI_FONT * pNewFont);
char             GUI_GotoXY                    (int x, int y);
char             GUI_GotoX                     (int x);
char             GUI_GotoY                     (int y);
void             GUI_ShowMissingCharacters     (int OnOff);
int              GUI_GetShowMissingCharacters  (void);
int              GUI_WrapGetNumLines           (const char * pText, int xSize, GUI_WRAPMODE WrapMode);
int              GUI_WrapGetPositions          (const char * pText, int xSize, GUI_WRAPMODE WrapMode, int * aPos, int NumItems);
void             GUI_WrapSetSeparators         (const U16 * pSep, int NumSeps);
int              GUI_GetLeadingBlankCols       (U16 c);
int              GUI_GetTrailingBlankCols      (U16 c);
int              GUI_GetLeadingBlankRows       (U16 c);
int              GUI_GetTrailingBlankRows      (U16 c);

/*********************************************************************
*
*       System independent fonts (SIF)
*/
void GUI_SIF_CreateFont(const void * pFontData, GUI_FONT * pFont, const GUI_SIF_TYPE * pFontType);
void GUI_SIF_DeleteFont(GUI_FONT * pFont);

/*********************************************************************
*
*       External binary fonts (XBF)
*/
int  GUI_XBF_CreateFont(GUI_FONT * pFont, GUI_XBF_DATA * pXBF, const GUI_XBF_TYPE * pFontType, GUI_XBF_GET_DATA_FUNC * pfGetData, void * pVoid);
void GUI_XBF_DeleteFont(GUI_FONT * pFont);

#define GUI_XBF_HEADER_SIZE    18
#define GUI_XBF_TABLEITEM_SIZE (sizeof(U32) + sizeof(U16))

/*********************************************************************
*
*       FreeType engine
*/
void GUI_FT_DestroyCache       (void);
void GUI_FT_Done               (void);
void GUI_FT_GetMeasurementChars(U16 * pLowerHeight, U16 * pUpperHeight);
void GUI_FT_SetCacheSize       (unsigned MaxFaces, unsigned MaxSizes, U32 MaxBytes);
void GUI_FT_SetMeasurementChars(U16 LowerHeight, U16 UpperHeight);

/*********************************************************************
*
*       TrueType support (TTF)
*/
void GUI_TTF_AddRowBottom        (GUI_FONT * pFont, int NumRows);
void GUI_TTF_AddRowTop           (GUI_FONT * pFont, int NumRows);
int  GUI_TTF_CreateFont          (GUI_FONT * pFont, GUI_TTF_CS * pCS);
int  GUI_TTF_CreateFontAA        (GUI_FONT * pFont, GUI_TTF_CS * pCS);
void GUI_TTF_DeleteFont          (GUI_FONT * pFont);
int  GUI_TTF_EmboldenFont        (GUI_FONT * pFont, U8 Embolden);
int  GUI_TTF_EmboldenFontEx      (GUI_FONT * pFont, I32 Strength);
int  GUI_TTF_EnableKerning       (GUI_FONT * pFont, int OnOff);
int  GUI_TTF_GetFamilyName       (GUI_FONT * pFont, char * pBuffer, int NumBytes);
int  GUI_TTF_GetStyleName        (GUI_FONT * pFont, char * pBuffer, int NumBytes);
int  GUI_TTF_ObliqueFont         (GUI_FONT * pFont, U8 Oblique);
int  GUI_TTF_ObliqueFontEx       (GUI_FONT * pFont, I32 Angle);
void GUI_TTF_EnableHarfBuzz      (GUI_FONT * pFont, int Enable);
void GUI_TTF_SetHarfBuzzCacheSize(unsigned NumBytes);

/*********************************************************************
*
*       Glyph Bitmap Distribution Format support (BDF)
*/
int GUI_BDF_CreateFont      (GUI_FONT * pFont, const U8 * pData, U32 Size);
int GUI_BDF_DeleteFont      (GUI_FONT * pFont);
int GUI_BDF_EmboldenFont    (GUI_FONT * pFont, U8 Embolden);
int GUI_BDF_EmboldenFontEx  (GUI_FONT * pFont, I32 Strength);
int GUI_BDF_GetFamilyName   (GUI_FONT * pFont, char * pBuffer, int NumBytes);
int GUI_BDF_GetStyleName    (GUI_FONT * pFont, char * pBuffer, int NumBytes);
int GUI_BDF_SetLetterSpacing(GUI_FONT * pFont, int Spacing);

/*********************************************************************
*
*       Kerning
*/
void               GUI_AttachKerning (const GUI_FONT * pFont, GUI_KERNING_INFO * pInfo, const U16 * pData, U32 NumItems);
int                GUI_DetachKerning (const GUI_FONT * pFont);
GUI_KERNING_INFO * GUI_GetKerning    (const GUI_FONT * pFont);                    // Not to be documented
I16                GUI_GetKerningDist(GUI_KERNING_INFO * pInfo, U16 cl, U16 cr);  // Not to be documented

/*********************************************************************
*
*       Resource file support
*/
/*********************************************************************
*
*       GUI_LANG_GET_DATA_FUNC
* 
*  Description
*    GetData function used for the language module, for more details see
*    \uref{GUI_GET_DATA_FUNC_II}.
*/
typedef GUI_GET_DATA_FUNC_II     GUI_LANG_GET_DATA_FUNC;

void         GUI_LANG_Clear            (void);
int          GUI_LANG_GetLang          (void);
int          GUI_LANG_GetNumItems      (int IndexLang);
const char * GUI_LANG_GetText          (int IndexText);
int          GUI_LANG_GetTextBuffered  (int IndexText, char * pBuffer, int SizeOfBuffer);
int          GUI_LANG_GetTextBufferedEx(int IndexText, int IndexLang, char * pBuffer, int SizeOfBuffer);
const char * GUI_LANG_GetTextEx        (int IndexText, int IndexLang);
int          GUI_LANG_GetTextLen       (int IndexText);
int          GUI_LANG_GetTextLenEx     (int IndexText, int IndexLang);
int          GUI_LANG_LoadCSV          (U8 * pFileData, U32 FileSize);
int          GUI_LANG_LoadCSVEx        (GUI_LANG_GET_DATA_FUNC * pfGetData, void * p);
int          GUI_LANG_LoadText         (U8 * pFileData, U32 FileSize, int IndexLang);
int          GUI_LANG_LoadTextEx       (GUI_LANG_GET_DATA_FUNC * pfGetData, void * p, int IndexLang);
int          GUI_LANG_SetLang          (int IndexLang);
unsigned     GUI_LANG_SetMaxNumLang    (unsigned MaxNumLang);
U16          GUI_LANG_SetSep           (U16 Sep);

/*********************************************************************
*
*       Unicode support
*/
int  GUI_UC_ConvertUC2UTF8(const U16 * s, int Len, char * pBuffer, int BufferSize);
int  GUI_UC_ConvertUTF82UC(const char * s, int Len, U16 * pBuffer, int BufferSize);
int  GUI_UC_EnableBIDI    (int OnOff);
int  GUI_UC_EnableThai    (int OnOff);
int  GUI_UC_Encode        (char * s, U16 Char);
int  GUI_UC_GetCharSize   (const char * s);
U16  GUI_UC_GetCharCode   (const char * s);
void GUI_UC_SetEncodeNone (void);
void GUI_UC_SetEncodeSJIS (void);
void GUI_UC_SetEncodeUTF8 (void);
void GUI_UC_SetBaseDir    (int Dir);
int  GUI_UC_GetBaseDir    (void);

void GUI_UC_DispString(const U16 * s);
void GUI_UC2DB (U16 Code, U8 * pOut);
U16  GUI_DB2UC (U8 Byte0, U8 Byte1);

/*********************************************************************
*
*       Bidi support
*/
#define GUI_BIDI_BASEDIR_LTR  0
#define GUI_BIDI_BASEDIR_RTL  1
#define GUI_BIDI_BASEDIR_AUTO 2

#define GUI_BIDI_LOG2VIS_CALC     0
#define GUI_BIDI_LOG2VIS_GETCACHE 1

/*********************************************************************
*
*       Drawing of binary, decimal and hexadecimal values
*/
void GUI_DispBin      (U32  v, U8 Len);
void GUI_DispBinAt    (U32  v, I16P x, I16P y, U8 Len);
void GUI_DispDec      (I32 v, U8 Len);
void GUI_DispDecAt    (I32 v, I16P x, I16P y, U8 Len);
void GUI_DispDecMin   (I32 v);
void GUI_DispDecShift (I32 v, U8 Len, U8 Shift);
void GUI_DispDecSpace (I32 v, U8 MaxDigits);
void GUI_DispHex      (U32 v, U8 Len);
void GUI_DispHexAt    (U32 v, I16P x, I16P y, U8 Len);
void GUI_DispSDec     (I32 v, U8 Len);
void GUI_DispSDecShift(I32 v, U8 Len, U8 Shift);

/*********************************************************************
*
*       Drawing of floating point values
*/
void GUI_DispFloat    (float v, char Len);
void GUI_DispFloatFix (float v, char Len, char Fract);
void GUI_DispFloatMin (float v, char Fract);
void GUI_DispSFloatFix(float v, char Len, char Fract);
void GUI_DispSFloatMin(float v, char Fract);

/*********************************************************************
*
*       Dynamic memory management
*/
typedef struct {
  U32 TotalBytes;
  U32 FreeBytes;
  U32 UsedBytes;
  U32 AllocSize;
  U32 NumFixedBytes;
  U32 MaxUsedBytes;
} GUI_ALLOC_INFO;

GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumFreeBlocks(void);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumFreeBytes (void);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumUsedBlocks(void);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetNumUsedBytes (void);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetMaxUsedBytes (void);

void * GUI_ALLOC_calloc       (size_t NumItems, size_t Size);
void   GUI_ALLOC_free         (void * p);
void * GUI_ALLOC_malloc       (size_t Size);
void * GUI_ALLOC_realloc      (void * p, size_t NewSize);
void * GUI_ALLOC_aligned_alloc(size_t Alignment, size_t Size);
void   GUI_ALLOC_aligned_free (void * p);

void GUI_ALLOC_GetMemInfo  (GUI_ALLOC_INFO * pInfo);
void GUI_ALLOC_SuppressPeak(int OnOff);

GUI_HMEM           GUI_ALLOC_AllocInit       (const void * pInitData, GUI_ALLOC_DATATYPE Size);
GUI_HMEM           GUI_ALLOC_AllocNoInit     (GUI_ALLOC_DATATYPE size);
GUI_HMEM           GUI_ALLOC_AllocZero       (GUI_ALLOC_DATATYPE size);
void               GUI_ALLOC_AssignMemory    (void * p, U32 NumBytes);
void               GUI_ALLOC_Free            (GUI_HMEM  hMem);
void               GUI_ALLOC_FreeFixedBlock  (void * p);
void               GUI_ALLOC_FreePtrArray    (GUI_HMEM * pArray, int NumElems);
void               GUI_ALLOC_FreePtr         (GUI_HMEM * phMem);
void *             GUI_ALLOC_GetFixedBlock   (GUI_ALLOC_DATATYPE Size);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetMaxSize      (void);
GUI_ALLOC_DATATYPE GUI_ALLOC_GetSize         (GUI_HMEM  hMem);
void *             GUI_ALLOC_h2p             (GUI_HMEM  hMem);
GUI_HMEM           GUI_ALLOC_p2h             (void * p);
void               GUI_ALLOC_Lock            (void);
void *             GUI_ALLOC_LockH           (GUI_HMEM  hMem);
GUI_HMEM           GUI_ALLOC_Realloc         (GUI_HMEM hOld, int NewSize);
GUI_ALLOC_DATATYPE GUI_ALLOC_RequestSize     (void);
void               GUI_ALLOC_SetShortOfRAM   (void (* pfOnShortOfRAM)(void));
void               GUI_ALLOC_Unlock          (void);
void *             GUI_ALLOC_UnlockH         (void ** pp);
//const void *     GUI_ALLOC_UnlockC         (const void ** pp);
void               GUI_ALLOC_UnlockC         (const void ** pp);
int                GUI_ALLOC_SetMaxPercentage(int MaxPercentage);
void               GUI_ALLOC_Exit            (void);

/*********************************************************************
*
*       Memory device flags
*
*  Description
*    Flags to be used for the creation of a memory device.
*/
#define GUI_MEMDEV_HASTRANS       0    // Default: The Memory Device is created with a transparency flag which ensures that the background will be drawn correctly.
#define GUI_MEMDEV_NOTRANS  (1 << 0)   // Creates a Memory Device without transparency. The user must make sure that the background is drawn correctly.
                                       // This way the Memory Device can be used for non-rectangular areas. An other advantage is the higher speed: Using this
                                       // flag accelerates the Memory Device approx. by 30 - 50%.
/* (ignore - emDoc should stop here) */

typedef GUI_HMEM GUI_MEMDEV_Handle;
typedef void     GUI_CALLBACK_VOID_P        (void * p);
typedef int      GUI_ANIMATION_CALLBACK_FUNC(int TimeRem, void * pVoid);

extern GUI_ANIMATION_CALLBACK_FUNC * GUI_MEMDEV__pCbAnimation;
extern void                        * GUI_MEMDEV__pVoid;

extern void (* GUI_MEMDEV__pfMEMDEV_Write)    (GUI_MEMDEV_Handle hMem);
extern void (* GUI_MEMDEV__pfMEMDEV_CopyToLCD)(GUI_MEMDEV_Handle hMem);

typedef struct {
  GUI_RECT rView, rPrev;
  char FirstCall;
} GUI_AUTODEV;

typedef struct {
  char DrawFixed;
  char IsMeasurement;
} GUI_AUTODEV_INFO;

int  GUI_MEMDEV_CreateAuto(GUI_AUTODEV * pAutoDev);
void GUI_MEMDEV_DeleteAuto(GUI_AUTODEV * pAutoDev);
int  GUI_MEMDEV_DrawAuto  (GUI_AUTODEV * pAutoDev, GUI_AUTODEV_INFO * pAutoDevInfo, GUI_CALLBACK_VOID_P * pfDraw, void * pData);

/* Create a memory device which is compatible to the selected LCD */
GUI_MEMDEV_Handle GUI_MEMDEV_Create       (int x0, int y0, int xSize, int ySize);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateCopy   (GUI_MEMDEV_Handle hMemSrc);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateEx     (int x0, int y0, int xSize, int ySize, int Flags);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateFixed  (int x0, int y0, int xSize, int ySize, int Flags,
                                           const GUI_DEVICE_API     * pDeviceAPI,
                                           const LCD_API_COLOR_CONV * pColorConvAPI);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateFixed32(int x0, int y0, int xSize, int ySize);

void                       GUI_MEMDEV_Clear               (GUI_MEMDEV_Handle hMem);
int                        GUI_MEMDEV_ClearAlpha          (GUI_MEMDEV_Handle hMemData, GUI_MEMDEV_Handle hMemMask);
void                       GUI_MEMDEV_CopyFromLCD         (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_CopyFromLCDAA       (GUI_MEMDEV_Handle hMem);
int                        GUI_MEMDEV_CopyRect            (GUI_MEMDEV_Handle hMemSrc, GUI_MEMDEV_Handle hMemDst, const GUI_RECT * pRectSrc, int xDst, int yDst);
void                       GUI_MEMDEV_CopyToLCD           (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_CopyToLCDAA         (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_CopyToLCDAt         (GUI_MEMDEV_Handle hMem, int x, int y);
int                        GUI_MEMDEV_CompareWithLCD      (GUI_MEMDEV_Handle hMem, int * px, int * py, int * pExp, int * pAct);
void                       GUI_MEMDEV_Delete              (GUI_MEMDEV_Handle MemDev);
void                       GUI_MEMDEV_DrawBitmap32HQHR    (const GUI_BITMAP * pBm, int x0HR, int y0HR);
void                       GUI_MEMDEV_DrawPerspectiveX    (GUI_MEMDEV_Handle hMem, int x, int y, int h0, int h1, int dx, int dy);
void                       GUI_MEMDEV_DrawDevice32HQHR    (GUI_MEMDEV_Handle hMemSrc, I32 x0HR, int y0HR);
void                       GUI_MEMDEV_Fill                (GUI_MEMDEV_Handle hMem, U32 Value);
int                        GUI_MEMDEV_GetXPos             (GUI_MEMDEV_Handle hMem);
int                        GUI_MEMDEV_GetXSize            (GUI_MEMDEV_Handle hMem);
int                        GUI_MEMDEV_GetYPos             (GUI_MEMDEV_Handle hMem);
int                        GUI_MEMDEV_GetYSize            (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_MarkDirty           (GUI_MEMDEV_Handle hMem, int x0, int y0, int x1, int y1);
void                       GUI_MEMDEV_ReduceYSize         (GUI_MEMDEV_Handle hMem, int YSize);
void                       GUI_MEMDEV_Rotate              (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
void                       GUI_MEMDEV_RotateAlpha         (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag, U8 Alpha);
void                       GUI_MEMDEV_RotateHR            (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, I32 dx, I32 dy, int a, int Mag);
void                       GUI_MEMDEV_RotateHQ            (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
void                       GUI_MEMDEV_RotateHQAlpha       (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag, U8 Alpha);
void                       GUI_MEMDEV_RotateHQHR          (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, I32 dx, I32 dy, int a, int Mag);
void                       GUI_MEMDEV_RotateHQT           (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
void                       GUI_MEMDEV_RotateHQTI          (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag);
GUI_MEMDEV_Handle          GUI_MEMDEV_Select              (GUI_MEMDEV_Handle hMem);  /* Select (activate) a particular memory device. */
void                       GUI_MEMDEV_SetOrg              (GUI_MEMDEV_Handle hMem, int x0, int y0);
void                       GUI_MEMDEV_WriteAt             (GUI_MEMDEV_Handle hMem, int x, int y);
void                       GUI_MEMDEV_Write               (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_WriteAlphaAt        (GUI_MEMDEV_Handle hMem, int Alpha, int x, int y);
void                       GUI_MEMDEV_WriteAlpha          (GUI_MEMDEV_Handle hMem, int Alpha);
void                       GUI_MEMDEV_WriteExAt           (GUI_MEMDEV_Handle hMem, int x, int y, int xMag, int yMag, int Alpha);
void                       GUI_MEMDEV_WriteEx             (GUI_MEMDEV_Handle hMem, int xMag, int yMag, int Alpha);
void                       GUI_MEMDEV_WriteOpaque         (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_WriteOpaqueAt       (GUI_MEMDEV_Handle hMem, int x, int y);
int                        GUI_MEMDEV_Draw                (GUI_RECT * pRect, GUI_CALLBACK_VOID_P * pfDraw, void * pData, int NumLines, int Flags);
void *                     GUI_MEMDEV_GetDataPtr          (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV_SetColorConv        (GUI_MEMDEV_Handle hMem, const LCD_API_COLOR_CONV * pColorConvAPI);
const LCD_API_COLOR_CONV * GUI_MEMDEV_GetColorConv        (GUI_MEMDEV_Handle hMemDev);
int                        GUI_MEMDEV_GetBitsPerPixel     (GUI_MEMDEV_Handle hMemDev);
GUI_MEMDEV_Handle          GUI_MEMDEV_GetSelMemdev        (void);
int                        GUI_MEMDEV_FadeInDevices       (GUI_MEMDEV_Handle hMem0, GUI_MEMDEV_Handle hMem1, int Period);
int                        GUI_MEMDEV_FadeOutDevices      (GUI_MEMDEV_Handle hMem0, GUI_MEMDEV_Handle hMem1, int Period);
void                       GUI_MEMDEV_SerializeBMP        (GUI_MEMDEV_Handle hDev, GUI_CALLBACK_VOID_U8_P * pfSerialize, void * p);
void                       GUI_MEMDEV_SerializeExBMP      (GUI_MEMDEV_Handle hDev, GUI_CALLBACK_VOID_U8_P * pfSerialize, void * p, int xPos, int yPos, int xSize, int ySize);
void                       GUI_MEMDEV_SetAnimationCallback(GUI_ANIMATION_CALLBACK_FUNC * pCbAnimation, void * pVoid);
int                        GUI_MEMDEV_PunchOutDevice      (GUI_MEMDEV_Handle hMemData, GUI_MEMDEV_Handle hMemMask);
void                       GUI_MEMDEV_SetTimePerFrame     (unsigned TimePerFrame);
int                        GUI_MEMDEV_MULTIBUF_Enable     (int OnOff);

/* Private functions */
#if GUI_WINSUPPORT
U8                         GUI_MEMDEV__SetCanvas          (GUI_MEMDEV_Handle hMem);
void                       GUI_MEMDEV__ClearCanvas        (void);
void                       GUI_MEMDEV__ClearCanvasEx      (U8 SetActive);
#else
  #define GUI_MEMDEV__SetCanvas(x)
  #define GUI_MEMDEV__ClearCanvas()
  #define GUI_MEMDEV__ClearCanvasEx(x)
#endif

typedef void (GUI_MIXCOLORSBULK_FUNC)(U32 * pFG, U32 * pBG, U32 * pDst, unsigned OffFG, unsigned OffBG, unsigned OffDest, unsigned xSize, unsigned ySize, U8 Intens);

void                       GUI_MEMDEV__FadeDevice         (GUI_MEMDEV_Handle hMemWin, GUI_MEMDEV_Handle hMemBk, GUI_MEMDEV_Handle hMemDst, U8 Intens);
void                       GUI_MEMDEV__FadeDeviceEx       (GUI_MEMDEV_Handle hMemWin, GUI_MEMDEV_Handle hMemBk, GUI_MEMDEV_Handle hMemDst, U8 Intens, int xPosWin, int yPosWin);
void                       GUI_MEMDEV__FadeDeviceExCustom (GUI_MEMDEV_Handle hMemWin, GUI_MEMDEV_Handle hMemBk, GUI_MEMDEV_Handle hMemDst, U8 Intens, int xPosWin, int yPosWin, GUI_MIXCOLORSBULK_FUNC * pFunc);
void                       GUI_MEMDEV__Rotate             (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, int dx, int dy, int a, int Mag, U32 Mask);
void                       GUI_MEMDEV__RotateHR           (GUI_MEMDEV_Handle hSrc, GUI_MEMDEV_Handle hDst, I32 dx, I32 dy, int a, int Mag, U32 Mask);

void  GUI_SelectLCD(void);

/* Blurring, dithering and blending */
GUI_MEMDEV_Handle GUI_MEMDEV_CreateBlurredDevice32  (GUI_MEMDEV_Handle hMem, U8 Depth);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateBlurredDevice32HQ(GUI_MEMDEV_Handle hMem, U8 Depth);
GUI_MEMDEV_Handle GUI_MEMDEV_CreateBlurredDevice32LQ(GUI_MEMDEV_Handle hMem, U8 Depth);
void              GUI_MEMDEV_SetBlurHQ              (void);
void              GUI_MEMDEV_SetBlurLQ              (void);
int               GUI_MEMDEV_BlendColor32           (GUI_MEMDEV_Handle hMem, U32 BlendColor, U8 BlendIntens);
int               GUI_MEMDEV_Dither32               (GUI_MEMDEV_Handle hMem, const LCD_API_COLOR_CONV * pColorConvAPI);

/*********************************************************************
*
*       GUI_DTA_GET_DATA_FUNC
* 
*  Description
*    GetData function used for streamed bitmaps (DTA), for more details see
*    \uref{GUI_GET_DATA_FUNC_II}.
*/
typedef GUI_GET_DATA_FUNC_II     GUI_DTA_GET_DATA_FUNC;

/*********************************************************************
*
*       Memory bitmaps
*/
typedef struct {
  U8     Cmd;
  int    xSize;
  int    ySize;
  void * pExtra;
} GUI_MBITMAP_PARAM;

typedef struct {
  GUI_BITMAP Bitmap;
  void *     pExtra;
  void *     pData;
} GUI_MBITMAP;

typedef int               GUI_MBITMAP_DRAW_IMAGE   (const void * p, int Size, GUI_MBITMAP_PARAM * pPara);
typedef int               GUI_MBITMAP_DRAW_IMAGE_EX(GUI_GET_DATA_FUNC * pfGetData, void * p, GUI_MBITMAP_PARAM * pPara);
typedef GUI_MEMDEV_Handle GUI_MBITMAP_CREATE_MEMDEV(int xSize, int ySize, void * pExtra);
typedef void              GUI_MBITMAP_DELETE_MEMDEV(GUI_MEMDEV_Handle hMem, void * pExtra);

typedef struct {
  GUI_MBITMAP_CREATE_MEMDEV * pfCreateMemdev;
  GUI_MBITMAP_DELETE_MEMDEV * pfDeleteMemdev;
  void *                      pExtra;
} GUI_MBITMAP_CONFIG;

//
// GUI_MBITMAP prototypes
//
GUI_MBITMAP * GUI_MBITMAP_Create        (GUI_MBITMAP_DRAW_IMAGE * pFuncDraw, const void * pData, int SizeOfData);
GUI_MBITMAP * GUI_MBITMAP_CreateEx      (GUI_MBITMAP_DRAW_IMAGE_EX * pFuncDrawEx, GUI_GET_DATA_FUNC * pfGetData, void * p);
GUI_MBITMAP * GUI_MBITMAP_CreateUser    (GUI_MBITMAP_DRAW_IMAGE * pFuncDraw, const void * pData, int SizeOfData, int xSize, int ySize, GUI_MBITMAP_CONFIG * pConfig);
GUI_MBITMAP * GUI_MBITMAP_CreateUserEx  (GUI_MBITMAP_DRAW_IMAGE_EX * pFuncDrawEx, GUI_GET_DATA_FUNC * pfGetData, void * p, int xSize, int ySize, GUI_MBITMAP_CONFIG * pConfig);
GUI_MBITMAP * GUI_MBITMAP_CreateString  (const char * s, const GUI_RECT * pRect, int TextAlign, GUI_WRAPMODE WrapMode);
void          GUI_MBITMAP_Delete        (GUI_MBITMAP * pMBitmap);
void          GUI_MBITMAP_SetColorFormat(const GUI_DEVICE_API * pDeviceAPI, const LCD_API_COLOR_CONV * pColorConvAPI);

//
// Drawing functions
//
#define GUI_MBITMAP_DRAW_FUNC(TYPE)     int GUI_MBITMAP_Draw##TYPE(const void * p, int Size, GUI_MBITMAP_PARAM * pPara);
#define GUI_MBITMAP_DRAW_FUNC_EX(TYPE)  int GUI_MBITMAP_Draw##TYPE##Ex(GUI_##TYPE##_GET_DATA_FUNC * pfGetData, void * p, GUI_MBITMAP_PARAM * pPara);

//
// Create prototypes
//
GUI_MBITMAP_DRAW_FUNC(Bitmap)
GUI_MBITMAP_DRAW_FUNC(BMP)
GUI_MBITMAP_DRAW_FUNC(DTA)
GUI_MBITMAP_DRAW_FUNC(GIF)
GUI_MBITMAP_DRAW_FUNC(JPEG)
GUI_MBITMAP_DRAW_FUNC(PNG)
GUI_MBITMAP_DRAW_FUNC(SVG)
GUI_MBITMAP_DRAW_FUNC_EX(BMP)
GUI_MBITMAP_DRAW_FUNC_EX(DTA)
GUI_MBITMAP_DRAW_FUNC_EX(GIF)
GUI_MBITMAP_DRAW_FUNC_EX(JPEG)
GUI_MBITMAP_DRAW_FUNC_EX(PNG)
GUI_MBITMAP_DRAW_FUNC_EX(SVG)

//
//  Allow drawing types are:
//   "Bitmap"
//   "BMP"  - "BMPEx"
//   "DTA"  - "DTAEx"
//   "GIF"  - "GIFEx"
//   "JPEG" - "JPEGEx"
//   "PNG"  - "PNGEx"
//   "SVG"  - "SVGEx"
//
#define GUI_MBITMAP_DRAW(TYPE)   GUI_MBITMAP_Draw##TYPE

#define GUI_MBITMAP_BITMAP  GUI_MBITMAP_DRAW(Bitmap)
#define GUI_MBITMAP_BMP     GUI_MBITMAP_DRAW(BMP)
#define GUI_MBITMAP_DTA     GUI_MBITMAP_DRAW(DTA)
#define GUI_MBITMAP_GIF     GUI_MBITMAP_DRAW(GIF)
#define GUI_MBITMAP_JPEG    GUI_MBITMAP_DRAW(JPEG)
#define GUI_MBITMAP_PNG     GUI_MBITMAP_DRAW(PNG)
#define GUI_MBITMAP_SVG     GUI_MBITMAP_DRAW(SVG)
#define GUI_MBITMAP_BMP_EX  GUI_MBITMAP_DRAW(BMPEx)
#define GUI_MBITMAP_DTA_EX  GUI_MBITMAP_DRAW(DTAEx)
#define GUI_MBITMAP_GIF_EX  GUI_MBITMAP_DRAW(GIFEx)
#define GUI_MBITMAP_JPEG_EX GUI_MBITMAP_DRAW(JPEGEx)
#define GUI_MBITMAP_PNG_EX  GUI_MBITMAP_DRAW(PNGEx)
#define GUI_MBITMAP_SVG_EX  GUI_MBITMAP_DRAW(SVGEx)

//
// Conversion macro from GUI_MBITMAP to GUI_BITMAP
//
#define GUI_MBITMAP_2BITMAP(x)      ((GUI_BITMAP *)x)

//
// Commands used by drawing functions
//
#define GUI_MBITMAP_CMD_XSIZE  (0)
#define GUI_MBITMAP_CMD_YSIZE  (1)
#define GUI_MBITMAP_CMD_DRAW   (2)

/*********************************************************************
*
*       Emscripten related 
*/

/*********************************************************************
*
*       GUI_EMSCRIPTEN_DATE
* 
*  Description
*    Structure to hold information about a date.
*/
typedef struct {
  int Year;    // Year (e.g. 2024)
  int Month;   // Month (January == 1, February == 2, ...)
  int Day;     // Day of the month (1st day of month == 1)
  int Weekday; // Day of the week (Monday == 1, Tuesday == 2, ...)
  int Hour;    // Hour (valid range: [0-23]).
  int Minute;  // Minute (valid range: [0-59]).
  int Second;  // Second (valid range: [0-59]).
} GUI_EMSCRIPTEN_DATE;

void GUI_EMSCRIPTEN_GetNow        (GUI_EMSCRIPTEN_DATE * pDate);
void GUI_EMSCRIPTEN_GetNowTimezone(GUI_EMSCRIPTEN_DATE * pDate, const char * sTimezone);

/*********************************************************************
*
*       Alpha blending
*/
/*********************************************************************
*
*       GUI_ALPHA_STATE
*
*   Description
*     Used for storing the alpha value with GUI_SetUserAlpha().
*/
typedef struct {
  U32 UserAlpha;   // Alpha value to be used.
} GUI_ALPHA_STATE;

#define GUI_MAKE_ALPHA(Alpha, Color)  ((U32)(((U32)Alpha << 24) | (Color & 0xFFFFFF)))

unsigned     GUI_EnableAlpha          (unsigned OnOff);
U32          GUI_RestoreUserAlpha     (GUI_ALPHA_STATE * pAlphaState);
U8           GUI_GetAlphaEx           (int LayerIndex);
U8           GUI_GetAlpha             (void);
unsigned     GUI_SetAlpha             (U8 Alpha);
U32          GUI_SetUserAlpha         (GUI_ALPHA_STATE * pAlphaState, U32 UserAlpha);
LCD_COLOR (* GUI_SetFuncMixColors     (LCD_COLOR (* pFunc)(LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens)))
                                                          (LCD_COLOR Color, LCD_COLOR BkColor, U8 Intens);
unsigned     GUI_PreserveTrans        (unsigned OnOff);
void         GUI_AlphaEnableFillRectHW(int OnOff);

/*********************************************************************
*
*       Multi layer support
*/
unsigned GUI_SelectLayer(unsigned Index);
unsigned GUI_GetSelLayer(void);

int  GUI_SetLayerPosEx  (unsigned Index, int xPos, int yPos);
int  GUI_SetLayerSizeEx (unsigned Index, int xSize, int ySize);
int  GUI_SetLayerVisEx  (unsigned Index, int OnOff);
int  GUI_SetLayerAlphaEx(unsigned Index, int Alpha);
int  GUI_GetLayerPosEx  (unsigned Index, int * pxPos, int * pyPos);

void     GUI_AssignCursorLayer(unsigned Index, unsigned CursorLayer);
unsigned GUI_GetCursorLayer   (unsigned Index);

/*********************************************************************
*
*       Multiple buffers and display origin
*/
void GUI_SetOrg(int x, int y);

void GUI_MULTIBUF_Begin          (void);
void GUI_MULTIBUF_BeginEx        (int LayerIndex);
void GUI_MULTIBUF_End            (void);
void GUI_MULTIBUF_EndEx          (int LayerIndex);
void GUI_MULTIBUF_Config         (int NumBuffers);
void GUI_MULTIBUF_ConfigEx       (int LayerIndex, int NumBuffers);
void GUI_MULTIBUF_Confirm        (int Index);
void GUI_MULTIBUF_ConfirmEx      (int LayerIndex, int BufferIndex);
int  GUI_MULTIBUF_GetNumBuffers  (void);
int  GUI_MULTIBUF_GetNumBuffersEx(int LayerIndex);
void GUI_MULTIBUF_Reset          (void);
void GUI_MULTIBUF_ResetEx        (int LayerIndex);
void GUI_MULTIBUF_SetLayerMask   (U32 LayerMask);
void GUI_MULTIBUF_UseSingleBuffer(void);

/*********************************************************************
*
*       emWinSPY
*/
int  GUI_SPY_Process       (GUI_tSend pfSend, GUI_tRecv pfRecv, void * pConnectInfo);
void GUI_SPY_SetProcessFunc(int (* pProcess)(U8, void *));
void GUI_SPY_SetMemHandler (GUI_tMalloc pMalloc, GUI_tFree pFree);
int  GUI_SPY_StartServer   (void);
int  GUI_SPY_StartServerEx (int (* pGUI_SPY_X_StartServer)(void));
int  GUI_SPY_X_StartServer (void);

/*********************************************************************
*
*       Barcode types
*
*  Description
*    Type of barcode to be drawn.
*/
#define GUI_BARCODE_ITF 0   // \b{Interleaved 2 of 5 (ITF)} which can display an even amount of digits.
#define GUI_BARCODE_128 1   // \b{Code128} which can display all 128 ASCII characters.

int GUI_BARCODE_Draw    (int xPos, int yPos, int ModuleSize, int ySize, int Type, const char * sBarcode);
int GUI_BARCODE_GetXSize(int Type, int ModuleSize, const char * sBarcode);

/*********************************************************************
*
*       ECC levels for QR codes
*
*  Description
*    Error correction level to be used by the Reed-Solomon error correction
*    for a QR code.
*/
#define GUI_QR_ECLEVEL_L 0   // About 7% or less errors can be corrected.
#define GUI_QR_ECLEVEL_M 1   // About 15% or less errors can be corrected.
#define GUI_QR_ECLEVEL_Q 2   // About 25% or less errors can be corrected.
#define GUI_QR_ECLEVEL_H 3   // About 30% or less errors can be corrected.

/*********************************************************************
*
*       GUI_QR_INFO
*
*   Description
*     Information about a QR code.
*/
typedef struct {
  int Version;   // Version according to QR code documentation.
  int Width;     // Number of 'Modules'.
  int Size;      // Size of bitmap in pixels.
} GUI_QR_INFO;

GUI_HMEM GUI_QR_Create      (const char * pText, int PixelSize, int EccLevel, int Version);
GUI_HMEM GUI_QR_CreateFramed(const char * pText, int PixelSize, int EccLevel, int Version);
void     GUI_QR_Draw        (GUI_HMEM hQR, int xPos, int yPos);
void     GUI_QR_GetInfo     (GUI_HMEM hQR, GUI_QR_INFO * pInfo);
void     GUI_QR_Delete      (GUI_HMEM hQR);

/*********************************************************************
*
*       Basics for animations
*/
#define ANIM_LINEAR     GUI_ANIM__Linear
#define ANIM_ACCEL      GUI_ANIM__Accel
#define ANIM_DECEL      GUI_ANIM__Decel
#define ANIM_ACCELDECEL GUI_ANIM__AccelDecel

/*********************************************************************
*
*       Animation states
*
*   Description
*     Describes the current state of an animation. Sent with the
*     \a{State} member of the GUI_ANIM_INFO structure to an animation callback.
*/
#define GUI_ANIM_START   0   // First execution.
#define GUI_ANIM_RUNNING 1   // Passed to all items which are not the first and not the last.
#define GUI_ANIM_END     2   // Last execution.

/*********************************************************************
*
*       GUI_ANIM_RANGE
*/
#ifndef   GUI_ANIM_RANGE
  #define GUI_ANIM_RANGE 32767
#endif

typedef GUI_HMEM GUI_ANIM_HANDLE;

typedef I32 (* GUI_ANIM_GETPOS_FUNC)(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME tNow);

/*********************************************************************
*
*       GUI_ANIM_INFO
*
*   Description
*     Contains information about the current state of an animation.
*/
typedef struct {
  int Pos;                // Position value calculated by the selected position calculation routine.
  int State;              // State of the animation. See \ref{Animation states} for valid values.
  GUI_ANIM_HANDLE hAnim;  // Handle of the animation object.
  GUI_TIMER_TIME Period;  // Period of the animation object.
  unsigned Index;         // Item index
} GUI_ANIM_INFO;

typedef void GUI_ANIMATION_FUNC(GUI_ANIM_INFO * pInfo, void * pVoid);

I32 GUI_ANIM__Linear    (GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME tNow);
I32 GUI_ANIM__Decel     (GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME tNow);
I32 GUI_ANIM__Accel     (GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME tNow);
I32 GUI_ANIM__AccelDecel(GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_TIMER_TIME tNow);

int             GUI_ANIM_AddItem          (GUI_ANIM_HANDLE hAnim, GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_ANIM_GETPOS_FUNC pfGetPos, void * pVoid, GUI_ANIMATION_FUNC * pfAnim);
int             GUI_ANIM_AddItemById      (I16 Id,                GUI_TIMER_TIME ts, GUI_TIMER_TIME te, GUI_ANIM_GETPOS_FUNC pfGetPos, void * pVoid, GUI_ANIMATION_FUNC * pfAnim);
GUI_ANIM_HANDLE GUI_ANIM_Create           (GUI_TIMER_TIME Period, unsigned MinTimePerFrame, void * pVoid, void (* pfSlice)(int, void *));
GUI_ANIM_HANDLE GUI_ANIM_CreateWithId     (GUI_TIMER_TIME Period, unsigned MinTimePerSlice, void * pVoid, void (* pfSlice)(int, void *), I16 Id);
void            GUI_ANIM_Delete           (GUI_ANIM_HANDLE hAnim);
int             GUI_ANIM_DeleteById       (I16 Id);
void            GUI_ANIM_DeleteAll        (void);
GUI_ANIM_HANDLE GUI_ANIM_Get              (I16 Id);
void          * GUI_ANIM_GetData          (GUI_ANIM_HANDLE hAnim);
void          * GUI_ANIM_GetItemDataLocked(GUI_ANIM_HANDLE hAnim, unsigned Index);
GUI_ANIM_HANDLE GUI_ANIM_GetFirst         (void);
void          * GUI_ANIM_GetItemData      (GUI_ANIM_HANDLE hAnim, unsigned Index);
GUI_ANIM_HANDLE GUI_ANIM_GetNext          (GUI_ANIM_HANDLE hAnim);
int             GUI_ANIM_GetNumItems      (GUI_ANIM_HANDLE hAnim);
GUI_TIMER_TIME  GUI_ANIM_GetPeriod        (GUI_ANIM_HANDLE hAnim);
int             GUI_ANIM_Exec             (GUI_ANIM_HANDLE hAnim);
void            GUI_ANIM_ExecAll          (void);
int             GUI_ANIM_IsRunning        (GUI_ANIM_HANDLE hAnim);
void            GUI_ANIM_SetData          (GUI_ANIM_HANDLE hAnim, void * pVoid);
int             GUI_ANIM_SetItemData      (GUI_ANIM_HANDLE hAnim, unsigned Index, const void * pVoid, U32 NumBytes);
void            GUI_ANIM_Start            (GUI_ANIM_HANDLE hAnim);
int             GUI_ANIM_StartExId        (I16 Id,                int NumLoops, void (* pfOnDelete)(void * pVoid));
void            GUI_ANIM_StartEx          (GUI_ANIM_HANDLE hAnim, int NumLoops, void (* pfOnDelete)(void * pVoid));
void            GUI_ANIM_Stop             (GUI_ANIM_HANDLE hAnim);
int             GUI_ANIM_StopById         (I16 Id);

/*********************************************************************
*
*       YUV device
*/
int   GUI_YUV_Create        (void);
int   GUI_YUV_CreateEx      (int LayerIndex, unsigned Period);
int   GUI_YUV_Delete        (void);
int   GUI_YUV_DeleteEx      (int LayerIndex);
U32 * GUI_YUV_GetpData      (U32 * pSize);
U32 * GUI_YUV_GetpDataEx    (int LayerIndex, U32 * pSize);
void  GUI_YUV_InvalidateArea(int x, int y, int xSize, int ySize);
int   GUI_YUV_SetPeriodEx   (int LayerIndex, unsigned Period);
int   GUI_YUV_SetPeriod     (unsigned Period);

/*********************************************************************
*
*       Display orientation
*/
/*********************************************************************
*
*       GUI_ORIENTATION_API
*/
typedef struct {
  void           (* pfDrawBitmap   )(GUI_DEVICE * pDevice, int x0, int y0, int xsize, int ysize, int BitsPerPixel, int BytesPerLine, const U8 * pData, int Diff, const LCD_PIXELINDEX * pTrans);
  void           (* pfDrawHLine    )(GUI_DEVICE * pDevice, int x0, int y0,  int x1);
  void           (* pfDrawVLine    )(GUI_DEVICE * pDevice, int x , int y0,  int y1);
  void           (* pfFillRect     )(GUI_DEVICE * pDevice, int x0, int y0, int x1, int y1);
  LCD_PIXELINDEX (* pfGetPixelIndex)(GUI_DEVICE * pDevice, int x, int y);
  void           (* pfSetPixelIndex)(GUI_DEVICE * pDevice, int x, int y, LCD_PIXELINDEX ColorIndex);
  void           (* pfXorPixel     )(GUI_DEVICE * pDevice, int x, int y);
  int            BytesPerPixel;
} GUI_ORIENTATION_API;

extern const GUI_ORIENTATION_API GUI_OrientationAPI_C0;
extern const GUI_ORIENTATION_API GUI_OrientationAPI_C8;
extern const GUI_ORIENTATION_API GUI_OrientationAPI_C16;
extern const GUI_ORIENTATION_API GUI_OrientationAPI_C32;

#define GUI_ORIENTATION_C0  &GUI_OrientationAPI_C0
#define GUI_ORIENTATION_C8  &GUI_OrientationAPI_C8
#define GUI_ORIENTATION_C16 &GUI_OrientationAPI_C16
#define GUI_ORIENTATION_C32 &GUI_OrientationAPI_C32

int  GUI_SetOrientation        (int Orientation);
int  GUI_SetOrientationEx      (int Orientation, int LayerIndex);
int  GUI_SetOrientationExCached(int Orientation, int LayerIndex, const GUI_ORIENTATION_API * pAPI);
void GUI_OrientationEnableTouch(void);

/*********************************************************************
*
*       Measure device: GUI_MEASDEV
*/
typedef GUI_HMEM GUI_MEASDEV_Handle;

GUI_MEASDEV_Handle GUI_MEASDEV_Create (void);
void               GUI_MEASDEV_Delete (GUI_MEASDEV_Handle hMemDev);
void               GUI_MEASDEV_Select (GUI_MEASDEV_Handle hMem);
void               GUI_MEASDEV_GetRect(GUI_MEASDEV_Handle hMem, GUI_RECT * pRect);
void               GUI_MEASDEV_ClearRect(GUI_MEASDEV_Handle hMem);

/*********************************************************************
*
*       Polygon helpers
*/
void GUI_RotatePolygon (GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, float Angle);
void GUI_MagnifyPolygon(GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, int Mag);
void GUI_EnlargePolygon(GUI_POINT * pDest, const GUI_POINT * pSrc, int NumPoints, int Len);

/*********************************************************************
*
*       Streamed bitmaps
*/
#define GUI_BITMAPSTREAM_GET_BUFFER     1
#define GUI_BITMAPSTREAM_RELEASE_BUFFER 2
#define GUI_BITMAPSTREAM_MODIFY_PALETTE 3

#define DECLARE_CREATE_FROM_STREAM(ID) int GUI_CreateBitmapFromStream##ID(GUI_BITMAP * pBMP, GUI_LOGPALETTE * pPAL, const void * p);

DECLARE_CREATE_FROM_STREAM(IDX)
DECLARE_CREATE_FROM_STREAM(RLE1)
DECLARE_CREATE_FROM_STREAM(RLE4)
DECLARE_CREATE_FROM_STREAM(RLE8)
DECLARE_CREATE_FROM_STREAM(565)
DECLARE_CREATE_FROM_STREAM(M565)
DECLARE_CREATE_FROM_STREAM(555)
DECLARE_CREATE_FROM_STREAM(M555)
DECLARE_CREATE_FROM_STREAM(A565)
DECLARE_CREATE_FROM_STREAM(AM565)
DECLARE_CREATE_FROM_STREAM(A555)
DECLARE_CREATE_FROM_STREAM(AM555)
DECLARE_CREATE_FROM_STREAM(RLE16)
DECLARE_CREATE_FROM_STREAM(RLEM16)
DECLARE_CREATE_FROM_STREAM(24)
DECLARE_CREATE_FROM_STREAM(A8)
DECLARE_CREATE_FROM_STREAM(Alpha)
DECLARE_CREATE_FROM_STREAM(M8888I)
DECLARE_CREATE_FROM_STREAM(RLEAlpha)
DECLARE_CREATE_FROM_STREAM(RLE32)
DECLARE_CREATE_FROM_STREAM(444_12)
DECLARE_CREATE_FROM_STREAM(M444_12)
DECLARE_CREATE_FROM_STREAM(444_12_1)
DECLARE_CREATE_FROM_STREAM(M444_12_1)
DECLARE_CREATE_FROM_STREAM(444_16)
DECLARE_CREATE_FROM_STREAM(M444_16)

int  GUI_CreateBitmapFromStream       (GUI_BITMAP * pBMP, GUI_LOGPALETTE * pPAL, const void * p);
void GUI_DrawStreamedBitmap           (const void * p, int x, int y);
void GUI_DrawStreamedBitmapAuto       (const void * p, int x, int y);
int  GUI_DrawStreamedBitmapEx         (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapExAuto     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapIDXEx      (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap444_12Ex   (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM444_12Ex  (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap444_12_1Ex (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM444_12_1Ex(GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap444_16Ex   (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM444_16Ex  (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap555Ex      (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM555Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap565Ex      (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM565Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapA555Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapAM555Ex    (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapA565Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapAM565Ex    (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap24Ex       (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmap8888Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapM8888IEx   (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLE1Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLE4Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLE8Ex     (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLE16Ex    (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLEM16Ex   (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
int  GUI_DrawStreamedBitmapRLE32Ex    (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, int x, int y);
void GUI_GetStreamedBitmapInfo        (const void * p, GUI_BITMAPSTREAM_INFO * pInfo);
int  GUI_GetStreamedBitmapInfoEx      (GUI_DTA_GET_DATA_FUNC * pfGetData, const void * p, GUI_BITMAPSTREAM_INFO * pInfo);
void GUI_SetStreamedBitmapHook        (GUI_BITMAPSTREAM_CALLBACK pfStreamedBitmapHook);

void LCD__RLE1_SetFunc (GUI_DTA_GET_DATA_FUNC * pfGetData, void * pVoid, U32 Off, const LCD_LOGPALETTE * pLogPal);
void LCD__RLE4_SetFunc (GUI_DTA_GET_DATA_FUNC * pfGetData, void * pVoid, U32 Off, const LCD_LOGPALETTE * pLogPal);
void LCD__RLE8_SetFunc (GUI_DTA_GET_DATA_FUNC * pfGetData, void * pVoid, U32 Off, const LCD_LOGPALETTE * pLogPal);
void LCD__RLE16_SetFunc(GUI_DTA_GET_DATA_FUNC * pfGetData, void * pVoid, U32 Off);
void LCD__RLE32_SetFunc(GUI_DTA_GET_DATA_FUNC * pfGetData, void * pVoid, U32 Off);

/*********************************************************************
*
*       BMP-export
*/
void GUI_BMP_Serialize     (GUI_CALLBACK_VOID_U8_P * pfSerialize, void * p);
void GUI_BMP_SerializeEx   (GUI_CALLBACK_VOID_U8_P * pfSerialize, int x0, int y0, int xSize, int ySize, void * p);
void GUI_BMP_SerializeExBpp(GUI_CALLBACK_VOID_U8_P * pfSerialize, int x0, int y0, int xSize, int ySize, void * p, int BitsPerPixel);

void GUI_BMP__WriteBitmapHeader(GUI_CALLBACK_VOID_U8_P * pfSerialize, int xSize, int ySize, void * p, int BitsPerPixel, int BytesPerLine, int BytesPerPixel, int NumColors, int Padding);

/*********************************************************************
*
*       Time / execution related routines
*/
void           GUI_Delay       (int Period);
GUI_TIMER_TIME GUI_GetTime     (void);
int            GUI_GetTimeSlice(void);
int            GUI_Exec        (void);        /* Execute all jobs ... Return 0 if nothing was done. */
int            GUI_Exec1       (void);        /* Execute one job  ... Return 0 if nothing was done. */
void           GUI_SetTimeSlice(int TimeSlice);

/*********************************************************************
*
*       Private use only...
*/
void GUI__TimeStop    (void);
void GUI__TimeContinue(void);

/*********************************************************************
*
*       MessageBox
*/
int     GUI_MessageBox   (const char * sMessage, const char * sCaption, int Flags);
#define GUI_MESSAGEBOX_CF_MOVEABLE (1 << 0)
#define GUI_MESSAGEBOX_CF_MODAL    (1 << 1)

#define GUI_MB_OK                20
#define GUI_MB_WARNING           21

/*********************************************************************
*
*       Anti Aliasing
*/
#define GUI_AA_TRANS   0  // Foreground color mixed up with current content of framebuffer
#define GUI_AA_NOTRANS 1  // Foreground color mixed up with current background color

void GUI_AA_DisableHiRes      (void);
void GUI_AA_EnableHiRes       (void);
int  GUI_AA_IsHiResEnabled    (void);
void GUI_AA_DrawPie           (int x0, int y0, int r, I32 a0, I32 a1);
int  GUI_AA_GetFactor         (void);
void GUI_AA_SetFactor         (int Factor);
void GUI_AA_DrawArc           (int x0, int y0, int rx, int ry, int a0, int a1);
void GUI_AA_DrawArcHR         (int x0, int y0, int rx, int ry, I32 a0, I32 a1);
void GUI_AA_DrawArcEx         (int mx, int my, int r, I32 a0, I32 a1, int c0, int c1);
void GUI_AA_DrawCircle        (int x0, int y0, int r);
void GUI_AA_DrawLine          (int x0, int y0, int x1, int y1);
void GUI_AA_DrawRoundedFrame  (int x0, int y0, int x1, int y1, int r);
void GUI_AA_DrawRoundedFrameEx(const GUI_RECT * pRect, int r);
void GUI_AA_DrawRoundedRect   (int x0, int y0, int x1, int y1, int r);
void GUI_AA_DrawRoundedRectEx (const GUI_RECT * pRect, int r);
void GUI_AA_FillCircle        (int x0, int y0, int r);
void GUI_AA_FillEllipse       (int x0, int y0, int rx, int ry);
void GUI_AA_FillEllipseXL     (int x0, int y0, int rx, int ry);
void GUI_AA_DrawEllipse       (int x0, int y0, int rx, int ry);
void GUI_AA_FillPolygon       (const GUI_POINT * pPoints, int NumPoints, int x0, int y0);
void GUI_AA_DrawPolygon       (const GUI_POINT * pSrc, int NumPoints, int x, int y);
void GUI_AA_DrawPolygonEx     (const GUI_POINT * pSrc, int NumPoints, int Thickness, int x, int y);
int  GUI_AA_FillRoundedRect   (int x0, int y0, int x1, int y1, int r);
int  GUI_AA_FillRoundedRectEx (const GUI_RECT * pRect, int r);
void GUI_AA_SetBufferSize     (int BufferSize);
int  GUI_AA_SetDrawMode       (int Mode);
void GUI_AA_SetpfDrawCharAA4  (int (* pfDrawChar)(int LayerIndex, int x, int y, U8 const * p, int xSize, int ySize, int BytesPerLine));
void GUI_AA_SetGammaAA4       (U8 * pGamma);
void GUI_AA_GetGammaAA4       (U8 * pGamma);
void GUI_AA_EnableGammaAA4    (int OnOff);

/*********************************************************************
*
*       Keyboard
*/
/* Message layer */
void GUI_StoreKeyMsg(int Key, int Pressed);
void GUI_SendKeyMsg (int Key, int Pressed);
int  GUI_PollKeyMsg (void);
void GUI_GetKeyState(GUI_KEY_STATE * pState);

void GUI_KEY__SetHook(void (* pfHook)(const GUI_KEY_STATE *));

/* Application layer */
int  GUI_GetKey(void);
int  GUI_WaitKey(void);
void GUI_StoreKey(int c);
void GUI_ClearKeyBuffer(void);
int  GUI_GetKeyPressed(void);

/*********************************************************************
*
*       Task synchronization
*/
void GUI_WaitEvent            (void);
void GUI_SignalEvent          (void);
void GUI_SetSignalEventFunc   (GUI_SIGNAL_EVENT_FUNC     pfSignalEvent);
void GUI_SetWaitEventFunc     (GUI_WAIT_EVENT_FUNC       pfWaitEvent);
void GUI_SetWaitEventTimedFunc(GUI_WAIT_EVENT_TIMED_FUNC pfWaitEventTimed);

/*********************************************************************
*
*       PID  (Pointer input device ... mouse/touch)
*/
#define GUI_PID_RegisterHook GUI_PID_RegisterPreHook
void GUI_PID_StoreState      (const GUI_PID_STATE * pState);
int  GUI_PID_GetState        (      GUI_PID_STATE * pState);
void GUI_PID_GetCurrentState (      GUI_PID_STATE * pState);
int  GUI_PID_IsEmpty         (void);
int  GUI_PID_IsPressed       (void);
void GUI_PID_RegisterPreHook (GUI_REGISTER_HOOK * pRegisterHook);
void GUI_PID_SetHook         (void (* pfHook)(      GUI_PID_STATE *));  // Public
void GUI_PID__SetHook        (void (* pfHook)(const GUI_PID_STATE *));  // Private
void GUI_PID__OrientationHook(GUI_PID_STATE * pState);                  // Private
void GUI_PID_RegisterReadHook(GUI_REGISTER_HOOK * pRegisterReadHook);
U8   GUI_PID__BlockInput     (int OnOff);                               // Private
U8   GUI_PID__IsInputBlocked (void);                                    // Private

/*********************************************************************
*
*       Mouse, generic
*/
int  GUI_MOUSE_GetState  (      GUI_PID_STATE * pState);
void GUI_MOUSE_StoreState(const GUI_PID_STATE * pState);

/*********************************************************************
*
*       TOUCH screen, generic
*/
int  GUI_TOUCH_GetLayer     (void);
int  GUI_TOUCH_GetState     (GUI_PID_STATE * pState);
void GUI_TOUCH_GetUnstable  (int * px, int * py);  /* for diagnostics only */
void GUI_TOUCH_SetLayer     (int Layer);
void GUI_TOUCH_StoreState   (int x, int y);
void GUI_TOUCH_StoreStateEx (const GUI_PID_STATE * pState);
void GUI_TOUCH_StoreUnstable(int x, int y);

/*********************************************************************
*
*       Mouse, PS2 driver
*/
void GUI_MOUSE_DRIVER_PS2_Init(void);               /* optional */
void GUI_MOUSE_DRIVER_PS2_OnRx(unsigned char Data);

/*********************************************************************
*
*       TOUCH screen, analog driver
*/
int  GUI_TOUCH_CalcCoefficients (int NumPoints, int * pxRef, int * pyRef, int * pxSample, int * pySample, int xSize, int ySize);
int  GUI_TOUCH_Calibrate        (int Coord, int Log0, int Log1, int Phys0, int Phys1);
int  GUI_TOUCH_CalibratePoint   (int * px, int * py);
void GUI_TOUCH_EnableCalibration(int OnOff);
void GUI_TOUCH_Exec             (void);
int  GUI_TOUCH_GetxPhys         (void);    /* for diagnostics only */
int  GUI_TOUCH_GetyPhys         (void);    /* for diagnostics only */
void GUI_TOUCH_SetCalibration   (int (* pFunc)(int *, int *)); /* Not to be documented */
void GUI_TOUCH_SetOrientation   (unsigned Orientation);
int  GUI_TOUCH_TransformPoint   (int * px, int * py);          /* Not to be documented */

/*********************************************************************
*
*       TOUCH: imports
*
* Please note: The following functions are required by the module.
* They need to be part of your application software (or rather, part
* of the hardware-layer of your software).
*/
void GUI_TOUCH_X_ActivateX(void);
void GUI_TOUCH_X_ActivateY(void);
void GUI_TOUCH_X_Disable  (void);
int  GUI_TOUCH_X_MeasureX (void);
int  GUI_TOUCH_X_MeasureY (void);

/*********************************************************************
*
*       LCD: Palette conversion table
*
* Please note: These functions were originally in GUI_Private.h but
* were also used in several LCDConf.c. Since GUI_Private.h is not
* shipped any more with object code we moved these functions here.
*/
LCD_PIXELINDEX * LCD_GetpPalConvTable        (const LCD_LOGPALETTE * pLogPal);
LCD_PIXELINDEX * LCD_GetpPalConvTableUncached(const LCD_LOGPALETTE * pLogPal);
LCD_PIXELINDEX * LCD_GetpPalConvTableBM      (const LCD_LOGPALETTE * pLogPal, const GUI_BITMAP * pBitmap, int LayerIndex);

/*********************************************************************
*
*       GUI_X_
*
* Externals, to be defined by application
*
* The externals defined below should be defined by the
* application. They are per default contained in the module
* GUI_X.c.
* Note that a lot if not all of these are not required in most target
* systems.
* For this module, samples are available for configurations
* with or without operating system.
*/
//
// Configuration
//
void GUI_X_Config(void);
void GUI_X_Init  (void);

//
// Timing routines
//
GUI_TIMER_TIME GUI_X_GetTime(void);
void           GUI_X_Delay  (int Period);

//
// Multitask routines - required only if multitasking is used (#define GUI_OS 1)
//
void GUI_X_Unlock   (void);
void GUI_X_Lock     (void);
U32  GUI_X_GetTaskId(void);
void GUI_X_InitOS   (void);

//
// Event driving (optional with multitasking)
//
void GUI_X_ExecIdle      (void);
void GUI_X_WaitEvent     (void);
void GUI_X_WaitEventTimed(int Period);
void GUI_X_SignalEvent   (void);

//
// Recording (logs/warnings and errors) - required only for higher levels
//
void GUI_X_Log     (const char * s);
void GUI_X_Warn    (const char * s);
void GUI_X_ErrorOut(const char * s);

/*********************************************************************
*
*       Constants for fonts and bitmaps
*/
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE1;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE1Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE4;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE4Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE8;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE8_Stream;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE8Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE16;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE16_Stream;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE16Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLEM16;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLEM16_Stream;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLEM16Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE32;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLE32Ex;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsRLEAlpha;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsA8;

extern const GUI_BITMAP_METHODS GUI_BitmapMethods444_12;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM444_12;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods444_12_1;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM444_12_1;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods444_16;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM444_16;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods555;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM555;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods565;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM565;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods24;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods888;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM888;
extern const GUI_BITMAP_METHODS GUI_BitmapMethods8888;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsM8888I;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsA565;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsAM565;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsA555;
extern const GUI_BITMAP_METHODS GUI_BitmapMethodsAM555;

#define GUI_COMPRESS_RLE1 0  // Used to initialize GUI_BITMAP struct if saved with the Bitmap Converter as RLE bitmap
#define GUI_COMPRESS_RLE4 0  // Used to initialize GUI_BITMAP struct if saved with the Bitmap Converter as RLE bitmap
#define GUI_COMPRESS_RLE8 0  // Used to initialize GUI_BITMAP struct if saved with the Bitmap Converter as RLE bitmap

#define GUI_DRAW_RLE1          &GUI_BitmapMethodsRLE1           /* Method table ! */
#define GUI_DRAW_RLE4          &GUI_BitmapMethodsRLE4           /* Method table ! */
#define GUI_DRAW_RLE8          &GUI_BitmapMethodsRLE8           /* Method table ! */
#define GUI_DRAW_RLE8_Stream   &GUI_BitmapMethodsRLE8_Stream    /* Method table ! */
#define GUI_DRAW_RLE16         &GUI_BitmapMethodsRLE16          /* Method table ! */
#define GUI_DRAW_RLE16_Stream  &GUI_BitmapMethodsRLE16_Stream   /* Method table ! */
#define GUI_DRAW_RLEM16        &GUI_BitmapMethodsRLEM16         /* Method table ! */
#define GUI_DRAW_RLEM16_Stream &GUI_BitmapMethodsRLEM16_Stream  /* Method table ! */
#define GUI_DRAW_RLE32         &GUI_BitmapMethodsRLE32          /* Method table ! */
#define GUI_DRAW_RLEALPHA      &GUI_BitmapMethodsRLEAlpha       /* Method table ! */
#define GUI_DRAW_A8            &GUI_BitmapMethodsA8             /* Method table ! */
 
#define GUI_DRAW_BMP444_12     &GUI_BitmapMethods444_12         /* Method table ! */
#define GUI_DRAW_BMPM444_12    &GUI_BitmapMethodsM444_12        /* Method table ! */
#define GUI_DRAW_BMP444_12_1   &GUI_BitmapMethods444_12_1       /* Method table ! */
#define GUI_DRAW_BMPM444_12_1  &GUI_BitmapMethodsM444_12_1      /* Method table ! */
#define GUI_DRAW_BMP444_16     &GUI_BitmapMethods444_16         /* Method table ! */
#define GUI_DRAW_BMPM444_16    &GUI_BitmapMethodsM444_16        /* Method table ! */
#define GUI_DRAW_BMP555        &GUI_BitmapMethods555            /* Method table ! */
#define GUI_DRAW_BMPM555       &GUI_BitmapMethodsM555           /* Method table ! */
#define GUI_DRAW_BMP565        &GUI_BitmapMethods565            /* Method table ! */
#define GUI_DRAW_BMPM565       &GUI_BitmapMethodsM565           /* Method table ! */
#define GUI_DRAW_BMP24         &GUI_BitmapMethods24             /* Method table ! */
#define GUI_DRAW_BMP888        &GUI_BitmapMethods888            /* Method table ! */
#define GUI_DRAW_BMPM888       &GUI_BitmapMethodsM888           /* Method table ! */
#define GUI_DRAW_BMP8888       &GUI_BitmapMethods8888           /* Method table ! */
#define GUI_DRAW_BMPM8888I     &GUI_BitmapMethodsM8888I         /* Method table ! */
#define GUI_DRAW_BMPA555       &GUI_BitmapMethodsA555           /* Method table ! */
#define GUI_DRAW_BMPAM555      &GUI_BitmapMethodsAM555          /* Method table ! */
#define GUI_DRAW_BMPA565       &GUI_BitmapMethodsA565           /* Method table ! */
#define GUI_DRAW_BMPAM565      &GUI_BitmapMethodsAM565          /* Method table ! */

extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_Ext;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_Frm;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA2;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA4;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA2_EXT;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA4_EXT;
extern const tGUI_SIF_APIList GUI_SIF_APIList_Prop_AA8_EXT;

extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop;
extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop_Ext;
extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop_Frm;
extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop_AA2_Ext;
extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop_AA4_Ext;
extern const tGUI_XBF_APIList GUI_XBF_APIList_Prop_AA8_Ext;

/*********************************************************************
*
*       GUI_KEY_...
*
* These ID values are basically meant to be used with widgets
* Note that we have chosen the values to be close to existing
* "standards", so do not change them unless forced to.
*
*/
#define GUI_KEY_BACKSPACE         8         /* ASCII: BACKSPACE Crtl-H */
#define GUI_KEY_TAB               9         /* ASCII: TAB       Crtl-I */
#define GUI_KEY_BACKTAB           10
#define GUI_KEY_ENTER             13        /* ASCII: ENTER     Crtl-M */
#define GUI_KEY_ALT               15
#define GUI_KEY_LEFT              16
#define GUI_KEY_UP                17
#define GUI_KEY_RIGHT             18
#define GUI_KEY_DOWN              19
#define GUI_KEY_PGUP              20
#define GUI_KEY_PGDOWN            21
#define GUI_KEY_HOME              23
#define GUI_KEY_END               24
#define GUI_KEY_SHIFT             25
#define GUI_KEY_CONTROL           26
#define GUI_KEY_ESCAPE            27        /* ASCII: ESCAPE    0x1b   */
#define GUI_KEY_INSERT            29
#define GUI_KEY_DELETE            30
#define GUI_KEY_SPACE             32

#define GUI_KEY_F1                40
#define GUI_KEY_F2                41

/*********************************************************************
*
*       Mouse buttons
*/
#define GUI_LBUTTON (1 << 0) /* Left button */
#define GUI_RBUTTON (1 << 1) /* Right button */
#define GUI_MBUTTON (1 << 2) /* Middle button */
#define GUI_DBUTTON (1 << 7) /* Double-click button */

/*********************************************************************
*
*       Text style flags
*
*  Description
*    Text style how a text will be displayed.
*/
#define GUI_TS_NORMAL           (0)        // Renders text normal (default).
#define GUI_TS_UNDERLINE        (1 << 0)   // Renders text underlined.
#define GUI_TS_STRIKETHRU       (1 << 1)   // Renders text in strike through type.
#define GUI_TS_OVERLINE         (1 << 2)   // Renders text in overline type.

/*********************************************************************
*
*       Line styles
*
*  Description
*    Style how a line is drawn.
*/
#define GUI_LS_SOLID        (0)    // Lines are drawn solid (default).
#define GUI_LS_DASH         (1)    // Lines are drawn dashed.
#define GUI_LS_DOT          (2)    // Lines are drawn dotted.
#define GUI_LS_DASHDOT      (3)    // Lines are drawn alternating with dashes and dots.
#define GUI_LS_DASHDOTDOT   (4)    // Lines are drawn alternating with dashes and double dots.

/*********************************************************************
*
*       Pen shapes
*/
#define GUI_PS_ROUND        (0)
#define GUI_PS_FLAT         (1)
#define GUI_PS_SQUARE       (2)

/*********************************************************************
*
*       Standard colors
*/
#define GUI_INVALID_COLOR ((((U32)GUI_TRANS_BYTE) << 24) | 0x00ABCDEFul)  /* Invalid color (transparency + determined color) */

#if (GUI_USE_ARGB)
  #define GUI_MAKE_COLOR(ABGR)  (((((U32)ABGR) & 0xFF000000ul) ^ 0xFF000000ul) | ((((U32)ABGR) & 0x00FF0000ul) >> 16) | (((U32)ABGR) & 0x0000FF00ul) | ((((U32)ABGR) & 0x000000FFul) << 16))
  #define GUI_MAKE_TRANS(Alpha) (255 - (Alpha))
#else
  #define GUI_MAKE_COLOR(ABGR)  (ABGR)
  #define GUI_MAKE_TRANS(Alpha) (Alpha)
#endif

#if (GUI_USE_ARGB)
  #define GUI_TRANS_BYTE 0x00
#else
  #define GUI_TRANS_BYTE 0xFF
#endif

#define GUI_BLUE          GUI_MAKE_COLOR(0x00FF0000)
#define GUI_GREEN         GUI_MAKE_COLOR(0x0000FF00)
#define GUI_RED           GUI_MAKE_COLOR(0x000000FF)
#define GUI_CYAN          GUI_MAKE_COLOR(0x00FFFF00)
#define GUI_MAGENTA       GUI_MAKE_COLOR(0x00FF00FF)
#define GUI_YELLOW        GUI_MAKE_COLOR(0x0000FFFF)
#define GUI_LIGHTBLUE     GUI_MAKE_COLOR(0x00FF8080)
#define GUI_LIGHTGREEN    GUI_MAKE_COLOR(0x0080FF80)
#define GUI_LIGHTRED      GUI_MAKE_COLOR(0x008080FF)
#define GUI_LIGHTCYAN     GUI_MAKE_COLOR(0x00FFFF80)
#define GUI_LIGHTMAGENTA  GUI_MAKE_COLOR(0x00FF80FF)
#define GUI_LIGHTYELLOW   GUI_MAKE_COLOR(0x0080FFFF)
#define GUI_DARKBLUE      GUI_MAKE_COLOR(0x00800000)
#define GUI_DARKGREEN     GUI_MAKE_COLOR(0x00008000)
#define GUI_DARKRED       GUI_MAKE_COLOR(0x00000080)
#define GUI_DARKCYAN      GUI_MAKE_COLOR(0x00808000)
#define GUI_DARKMAGENTA   GUI_MAKE_COLOR(0x00800080)
#define GUI_DARKYELLOW    GUI_MAKE_COLOR(0x00008080)
#define GUI_DARKPINK      GUI_MAKE_COLOR(0x0060087E)
#define GUI_WHITE         GUI_MAKE_COLOR(0x00FFFFFF)
#define GUI_LIGHTGRAY     GUI_MAKE_COLOR(0x00D3D3D3)
#define GUI_GRAY          GUI_MAKE_COLOR(0x00808080)
#define GUI_DARKGRAY      GUI_MAKE_COLOR(0x00404040)
#define GUI_BLACK         GUI_MAKE_COLOR(0x00000000)
#define GUI_BROWN         GUI_MAKE_COLOR(0x002A2AA5)
#define GUI_ORANGE        GUI_MAKE_COLOR(0x0000A5FF)
#define GUI_PINK          GUI_MAKE_COLOR(0x00C00FFC)

#define GUI_TRANSPARENT   GUI_MAKE_COLOR(0xFF000000)

#define GUI_GRAY_3F       GUI_MAKE_COLOR(0x003F3F3F)
#define GUI_GRAY_50       GUI_MAKE_COLOR(0x00505050)
#define GUI_GRAY_55       GUI_MAKE_COLOR(0x00555555)
#define GUI_GRAY_60       GUI_MAKE_COLOR(0x00606060)
#define GUI_GRAY_7C       GUI_MAKE_COLOR(0x007C7C7C)
#define GUI_GRAY_9A       GUI_MAKE_COLOR(0x009A9A9A)
#define GUI_GRAY_AA       GUI_MAKE_COLOR(0x00AAAAAA)
#define GUI_GRAY_C0       GUI_MAKE_COLOR(0x00C0C0C0)
#define GUI_GRAY_C8       GUI_MAKE_COLOR(0x00C8C8C8)
#define GUI_GRAY_D0       GUI_MAKE_COLOR(0x00D0D0D0)
#define GUI_GRAY_E7       GUI_MAKE_COLOR(0x00E7E7E7)

#define GUI_BLUE_98       GUI_MAKE_COLOR(0x00980000)

/*********************************************************************
*
*       MultiTouch-Support
*/
/*********************************************************************
*
*       MultiTouch flags
*
*  Description
*    Data structure used by GUI_MTOUCH_GetEvent() to store a multi touch
*    event in.
*/
#define GUI_MTOUCH_FLAG_DOWN (1 << 0)    // New touch point has touched the surface.
#define GUI_MTOUCH_FLAG_MOVE (1 << 1)    // Touch point has been moved.
#define GUI_MTOUCH_FLAG_UP   (1 << 2)    // Touch point has released the surface.

/*********************************************************************
*
*       Variables
*/
extern T_GUI_MTOUCH_STOREEVENT GUI_MTOUCH__pStoreEvent;

/*********************************************************************
*
*       Interface
*/
void GUI_MTOUCH_Enable          (int OnOff);
int  GUI_MTOUCH_GetEvent        (GUI_MTOUCH_EVENT * pEvent);
int  GUI_MTOUCH_GetTouchInput   (GUI_MTOUCH_EVENT * pEvent, GUI_MTOUCH_INPUT * pBuffer, unsigned Index);
int  GUI_MTOUCH_IsEmpty         (void);
void GUI_MTOUCH_SetOrientation  (int Orientation);
void GUI_MTOUCH_SetOrientationEx(int Orientation, int LayerIndex);
void GUI_MTOUCH_StoreEvent      (GUI_MTOUCH_EVENT * pEvent, GUI_MTOUCH_INPUT * pInput);

/*********************************************************************
*
*       GUI_GPU_FUNCTION_INDEX
*
*  Description
*    Index values for association of GPU functions of each driver.
*/
typedef enum {
  GUI_GPU_FUNCTION_COPYRECT,             // Function to copy a rectangular area on the screen.
  GUI_GPU_FUNCTION_COPYBUFFERRECT,       // Function to copy a rectangular area between buffers.
  GUI_GPU_FUNCTION_FILLCIRCLE,           // Function for filling a non-antialiased circle.
  GUI_GPU_FUNCTION_DRAWCIRCLE,           // Function for drawing a non-antialiased circle.
  GUI_GPU_FUNCTION_DRAWLINE,             // Function for drawing a non-antialiased line.
  GUI_GPU_FUNCTION_FILLRECT,             // Function for opaque filling a rectangle.
  GUI_GPU_FUNCTION_FILLALPHARECT,        // Function for filling a rectangle with alpha blending.
  GUI_GPU_FUNCTION_DRAWRECT,             // Function for drawing a non-antialiased rectangle.
  GUI_GPU_FUNCTION_FILLROUNDEDRECT,      // Function for filling a non-antialiased rounded rectangle.
  GUI_GPU_FUNCTION_DRAWROUNDEDRECT,      // Function for drawing a non-antialiased rounded rectangle.
  GUI_GPU_FUNCTION_DRAWBITMAPEX_16,      // Function for drawing a 16bpp bitmap scaled and/or flipped.
  GUI_GPU_FUNCTION_DRAWBITMAPEX_32,      // Function for drawing a 32bpp bitmap scaled and/or flipped.
  GUI_GPU_FUNCTION_DRAWBITMAP_8,         // Function for drawing a 8bpp palette-based bitmap.
  GUI_GPU_FUNCTION_DRAWBITMAP_16,        // Function for drawing a 16bpp bitmap.
  GUI_GPU_FUNCTION_DRAWBITMAP_24,        // Function for drawing a 24bpp bitmap.
  GUI_GPU_FUNCTION_DRAWALPHABITMAP_4,    // Function for drawing a 4bpp alpha bitmap.
  GUI_GPU_FUNCTION_DRAWALPHABITMAP_8,    // Function for drawing a 8bpp alpha bitmap.
  GUI_GPU_FUNCTION_DRAWALPHABITMAP_32,   // Function for drawing a 32bpp alpha bitmap or opaque bitmap.
  GUI_GPU_FUNCTION_ALPHABLENDING,        // Function for alpha blending of buffers.
  GUI_GPU_FUNCTION_MIXCOLORSBULK,        // Function for bulking mixing of colors.
  GUI_GPU_FUNCTION_DRAWARCAA,            // Function for drawing an anti-aliased arc.
  GUI_GPU_FUNCTION_DRAWPIEAA,            // Function for filling an anti-aliased circle sector.
  GUI_GPU_FUNCTION_FILLCIRCLEAA,         // Function for filling an anti-aliased circle.
  GUI_GPU_FUNCTION_DRAWCIRCLEAA,         // Function for drawing an anti-aliased circle.
  GUI_GPU_FUNCTION_DRAWELLIPSEAA,        // Function for drawing an anti-aliased ellipse.
  GUI_GPU_FUNCTION_FILLELLIPSEAA,        // Function for filling an anti-aliased ellipse.
  GUI_GPU_FUNCTION_DRAWLINEAA,           // Function for drawing an anti-aliased line.
  GUI_GPU_FUNCTION_DRAWPOLYGONAA,        // Function for drawing an anti-aliased polygon outline.
  GUI_GPU_FUNCTION_FILLPOLYGONAA,        // Function for filling an anti-aliased polygon.
  GUI_GPU_FUNCTION_DRAWROUNDEDRECTAA,    // Function for filling an anti-aliased rounded rectangle.
  GUI_GPU_FUNCTION_FILLROUNDEDRECTAA,    // Function for filling an anti-aliased rounded rectangle.
  GUI_GPU_FUNCTION_MEMDEV_BLEND,         // Function for blending a color into a 32bpp memory device.
  GUI_GPU_FUNCTION_MEMDEV_ROTATE,        // Function for rotating and scaling a memory device.
  GUI_GPU_FUNCTION_MEMDEV_BLUR,          // Function for blurring a memory device.
  GUI_GPU_FUNCTION_COLORTOINDEX_M1555I,  // Function to convert colors to indices of format GUICC_M1555I.
  GUI_GPU_FUNCTION_COLORTOINDEX_M565,    // Function to convert colors to indices of format GUICC_M565.
  GUI_GPU_FUNCTION_COLORTOINDEX_M4444I,  // Function to convert colors to indices of format GUICC_M4444I.
  GUI_GPU_FUNCTION_COLORTOINDEX_M888,    // Function to convert colors to indices of format GUICC_M888.
  GUI_GPU_FUNCTION_COLORTOINDEX_M8888I,  // Function to convert colors to indices of format GUICC_M8888I.
  GUI_GPU_FUNCTION_INDEXTOCOLOR_M1555I,  // Function to convert GUICC_M1555I indices to colors.
  GUI_GPU_FUNCTION_INDEXTOCOLOR_M565,    // Function to convert GUICC_M565 indices to colors.
  GUI_GPU_FUNCTION_INDEXTOCOLOR_M4444I,  // Function to convert GUICC_M4444I indices to colors.
  GUI_GPU_FUNCTION_INDEXTOCOLOR_M888,    // Function to convert GUICC_M888 indices to colors.
  GUI_GPU_FUNCTION_INDEXTOCOLOR_M8888I,  // Function to convert GUICC_M8888I indices to colors.
  GUI_GPU_FUNCTION_GETPALCONVTABLE,      // Function to calculate the index values to be used for the given palette when drawing a bitmap.
  GUI_GPU_FUNCTION_DRAWSVG,              // Drawing SVG files.
  GUI_GPU_FUNCTION_DRAWGRADIENT,         // Drawing linear gradients (horizontal and vertical, with or without radius)
  GUI_GPU_FUNCTION_DRAWMULTIGRADIENT,    // Drawing linear multi-color gradients (horizontal and vertical, with or without radius)
  GUI_GPU_FUNCTION_COUNT,                // Number of functions.
  GUI_GPU_FUNCTION_INVALID = 0x7FFFFFFF, // Invalid function ID.
} GUI_GPU_FUNCTION_INDEX;

/*********************************************************************
*
*       Axis values
* 
*  Description
*    Defines to distinguish between the X and Y axis. Used in various
*    emWin functions.
*/
#define GUI_COORD_X     0     // X axis.
#define GUI_COORD_Y     1     // Y axis.

/*********************************************************************
*
*       Horizontal / Vertical
*/
#define GUI_HORIZONTAL (1 << GUI_COORD_X)
#define GUI_VERTICAL   (1 << GUI_COORD_Y)

/*********************************************************************
*
*       Text and drawing modes
*
* These defines come in two flavors: the long version (.._DRAWMODE_..)
* and the short ones (.._DM_..). They are identical, feel free to use
* which ever one you like best.
*/
#define GUI_DRAWMODE_NORMAL  LCD_DRAWMODE_NORMAL
#define GUI_DRAWMODE_XOR     LCD_DRAWMODE_XOR
#define GUI_DRAWMODE_TRANS   LCD_DRAWMODE_TRANS
#define GUI_DRAWMODE_REV     LCD_DRAWMODE_REV
#define GUI_DM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_DM_XOR           LCD_DRAWMODE_XOR
#define GUI_DM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_DM_REV           LCD_DRAWMODE_REV

#define GUI_TEXTMODE_NORMAL  LCD_DRAWMODE_NORMAL
#define GUI_TEXTMODE_XOR     LCD_DRAWMODE_XOR
#define GUI_TEXTMODE_TRANS   LCD_DRAWMODE_TRANS
#define GUI_TEXTMODE_REV     LCD_DRAWMODE_REV
#define GUI_TM_NORMAL        LCD_DRAWMODE_NORMAL
#define GUI_TM_XOR           LCD_DRAWMODE_XOR
#define GUI_TM_TRANS         LCD_DRAWMODE_TRANS
#define GUI_TM_REV           LCD_DRAWMODE_REV

/* Text alignment flags, horizontal */
#define GUI_TA_LEFT       (0)
#define GUI_TA_HORIZONTAL (3u)
#define GUI_TA_RIGHT      (1u)
#define GUI_TA_CENTER     (2u)
#define GUI_TA_HCENTER    GUI_TA_CENTER  /* easier to remember :-)  */

/* Text alignment flags, vertical */
#define GUI_TA_TOP        (0)
#define GUI_TA_VERTICAL   (3u << 2)
#define GUI_TA_BOTTOM     (1u << 2)
#define GUI_TA_BASELINE   (2u << 2)
#define GUI_TA_VCENTER    (3u << 2)

/* General alignment flags */
#define GUI_ALIGN_LEFT       GUI_TA_LEFT
#define GUI_ALIGN_HCENTER    GUI_TA_HCENTER
#define GUI_ALIGN_RIGHT      GUI_TA_RIGHT
#define GUI_ALIGN_TOP        GUI_TA_TOP
#define GUI_ALIGN_VCENTER    GUI_TA_VCENTER
#define GUI_ALIGN_BOTTOM     GUI_TA_BOTTOM
#define GUI_ALIGN_HORIZONTAL GUI_TA_HORIZONTAL
#define GUI_ALIGN_VERTICAL   GUI_TA_VERTICAL
#define GUI_ALIGN_CENTER     (GUI_TA_HCENTER | GUI_TA_VCENTER)

/* General orientation flags */
#define GUI_MIRROR_X (1u)
#define GUI_MIRROR_Y (1u << 1)
#define GUI_SWAP_XY  (1u << 2)

#define GUI_ROTATION_0     (0)
#define GUI_ROTATION_CW    (GUI_MIRROR_X | GUI_SWAP_XY)
#define GUI_ROTATION_180   (GUI_MIRROR_X | GUI_MIRROR_Y)
#define GUI_ROTATION_CCW   (GUI_MIRROR_Y | GUI_SWAP_XY)
#define GUI_ROTATION_LEFT  (1u << 3)
#define GUI_ROTATION_RIGHT (1u << 4)

/*********************************************************************
*
*       Min/Max coordinates
*
* Define minimum and maximum coordinates in x and y
*/
#define GUI_XMIN -16383
#define GUI_XMAX  16383
#define GUI_YMIN -16383
#define GUI_YMAX  16383

//
// Include at the end because functions are referenced that are defined in this header
//
#include "GUI_Legacy.h"

#if defined(__cplusplus)
}
#endif

#endif   /* ifdef GUI_H */

/*************************** End of file ****************************/
