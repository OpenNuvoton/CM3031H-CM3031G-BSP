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
File        : GUI_GPU_Dave2D.h
Purpose     : Header of Dave2D GPU driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_DAVE2D_H
#define GUI_GPU_DAVE2D_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Dave2D header files
*
**********************************************************************
*/
#ifndef   GUI_GPU_DAVE2D_DRIVER_HEADER
  #define GUI_GPU_DAVE2D_DRIVER_HEADER    <dave_driver.h>
#endif

#ifdef GUI_GPU_HAS_DAVE2D
  #include GUI_GPU_DAVE2D_DRIVER_HEADER
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Type definitions
*
**********************************************************************
*/
#ifndef GUI_GPU_HAS_DAVE2D
  typedef void          d2_device;
  typedef void          d2_renderbuffer;
  typedef U8            d2_u8;
  typedef I16           d2_s16;
  typedef U16           d2_u16;
  typedef I32           d2_s32;
  typedef U32           d2_u32;
  typedef unsigned long d2_color;
  typedef short         d2_point;
  typedef d2_u16        d2_blitpos;
  typedef d2_s16        d2_border;
  typedef d2_s16        d2_width;
  typedef d2_u32        d2_modeflags;
  typedef d2_u32        d2_texturemodes;
  typedef d2_u32        d2_blitflags;
  typedef d2_u32        d2_blendflags;
  typedef d2_u32        d2_blendmodes;
  typedef d2_u8         d2_alpha;
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_DAVE2D_API_CONTEXT
* 
*  Description
*    Structure that holds the contexts created by the Dave2D API.
*    This structure type is used as the return type when calling
*    GUI_GPU_AcquireContext(GUI_GPU_DAVE2D).
*/
typedef struct {
  d2_device       * d2_handle;      // Main Dave2D device instance.
  d2_renderbuffer * renderbuffer;   // Instance of type d2_renderbuffer.
} GUI_GPU_DAVE2D_API_CONTEXT;

/*********************************************************************
*
*       API function table
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _D2_OPENDEVICE_ARGS(ARG)           ARG(d2_u32, flags)
#define _D2_CLOSEDEVICE_ARGS(ARG)          ARG(d2_device *, handle)
#define _D2_INITHW_ARGS(ARG)               ARG(d2_device *, handle) , ARG(d2_u32, flags)
#define _D2_DEINITHW_ARGS(ARG)             ARG(d2_device *, handle)
#define _D2_NEWRENDERBUFFER_ARGS(ARG)      ARG(d2_device *, handle) , ARG(d2_u32, initialsize) , ARG(d2_u32, stepsize)
#define _D2_FREERENDERBUFFER_ARGS(ARG)     ARG(d2_device *, handle) , ARG(d2_renderbuffer *, buffer)
#define _D2_FBBLITCOPY_ARGS(ARG)           ARG(d2_device *, handle) , ARG(d2_u16, width) , ARG(d2_u16, height), ARG(d2_blitpos, srcx) , ARG(d2_blitpos, srcy) , ARG(d2_blitpos, dstx), ARG(d2_blitpos, dsty), ARG(d2_u32, flags)
#define _D2_FRAMEBUFFER_ARGS(ARG)          ARG(d2_device *, handle) , ARG(void *, ptr) , ARG(d2_s32, pitch) , ARG(d2_u32, width) , ARG(d2_u32, height) , ARG(d2_s32, format)
#define _D2_SELECTRENDERBUFFER_ARGS(ARG)   ARG(d2_device *, handle) , ARG(d2_renderbuffer *, buffer)
#define _D2_EXECUTERENDERBUFFER_ARGS(ARG)  ARG(d2_device *, handle) , ARG(d2_renderbuffer *, buffer) , ARG(d2_u32, flags)
#define _D2_FLUSHFRAME_ARGS(ARG)           ARG(d2_device *, handle)
#define _D2_SETCOLOR_ARGS(ARG)             ARG(d2_device *, handle) , ARG(d2_s32, index) , ARG(d2_color, color)
#define _D2_CLIPRECT_ARGS(ARG)             ARG(d2_device *, handle) , ARG(d2_border, xmin) , ARG(d2_border, ymin), ARG(d2_border, xmax), ARG(d2_border, ymax)
#define _D2_BLITCOPY_ARGS(ARG)             ARG(d2_device *, handle) , ARG(d2_s32, srcwidth), ARG(d2_s32, srcheight), ARG(d2_blitpos, srcx) , ARG(d2_blitpos, srcy) , ARG(d2_width, dstwidth) , ARG(d2_width, dstheight) , ARG(d2_point, dstx) ,ARG(d2_point, dsty), ARG(d2_u32, flags)
#define _D2_SETBLITSRC_ARGS(ARG)           ARG(d2_device *, handle) , ARG(void *, ptr), ARG(d2_s32, pitch), ARG(d2_s32, width), ARG(d2_s32, height), ARG(d2_u32, format)
#define _D2_RENDERBOX_ARGS(ARG)            ARG(d2_device *, handle), ARG(d2_point, x1), ARG(d2_point, y1), ARG(d2_width, w), ARG(d2_width, h)
#define _D2_RENDERCIRCLE_ARGS(ARG)         ARG(d2_device *, handle), ARG(d2_point, x), ARG(d2_point, y), ARG(d2_width, r), ARG(d2_width, w)
#define _D2_RENDERPOLYGON_ARGS(ARG)        ARG(d2_device *, handle), ARG(const d2_point *, data), ARG(d2_u32, count), ARG(d2_u32, flags)
#define _D2_RENDERPOLYLINE_ARGS(ARG)       ARG(d2_device *, handle), ARG(const d2_point *, data), ARG(d2_u32, count), ARG(d2_width, w), ARG(d2_u32, flags)
#define _D2_RENDERLINE_ARGS(ARG)           ARG(d2_device *, handle), ARG(d2_point, x1), ARG(d2_point, y1), ARG(d2_point, x2), ARG(d2_point, y2), ARG(d2_width, w), ARG(d2_u32, flags)
#define _D2_RENDERWEDGE_ARGS(ARG)          ARG(d2_device *, handle), ARG(d2_point, x), ARG(d2_point, y), ARG(d2_width, r), ARG(d2_width, w), ARG(d2_s32, nx1), ARG(d2_s32, ny1), ARG(d2_s32, nx2), ARG(d2_s32, ny2), ARG(d2_u32, flags)
#define _D2_SELECTRENDERMODE_ARGS(ARG)     ARG(d2_device *, handle), ARG(d2_u32, mode)
#define _D2_SETALPHABLENDMODEEX_ARGS(ARG)  ARG(d2_device *, handle), ARG(d2_u32, srcfactor), ARG(d2_u32, dstfactor), ARG(d2_u32, blendflags)
#define _D2_SETTEXOPPARAM_ARGS(ARG)        ARG(d2_device *, handle), ARG(d2_u32, index), ARG(d2_u32, p1), ARG(d2_u32, p2)
#define _D2_SETTEXTUREOPERATION_ARGS(ARG)  ARG(d2_device *, handle), ARG(d2_u8, amode), ARG(d2_u8, rmode), ARG(d2_u8, gmode), ARG(d2_u8, bmode)
#define _D2_SETFILLMODE_ARGS(ARG)          ARG(d2_device *, handle), ARG(d2_u32, mode)
#define _D2_GETFILLMODE_ARGS(ARG)          ARG(d2_device *, handle)
#define _D2_SETTEXCLUTFORMAT_ARGS(ARG)     ARG(d2_device *, handle), ARG(d2_u32, format)
#define _D2_SETTEXTURE_ARGS(ARG)           ARG(d2_device *, handle), ARG(void *, ptr), ARG(d2_s32, pitch), ARG(d2_s32, width), ARG(d2_s32, height), ARG(d2_u32, format)
#define _D2_SETTEXCLUT_ARGS(ARG)           ARG(d2_device *, handle), ARG(d2_color*, clut)
#define _D2_SETTEXTUREMAPPING_ARGS(ARG)    ARG(d2_device *, handle), ARG(d2_point, x), ARG(d2_point, y), ARG(d2_s32, u0), ARG(d2_s32, v0), ARG(d2_s32, dxu), ARG(d2_s32, dyu), ARG(d2_s32, dxv), ARG(d2_s32, dyv)
#define _D2_SETALPHA_ARGS(ARG)             ARG(d2_device *, handle), ARG(d2_alpha, alpha)
#define _D2_SETBLENDMODE_ARGS(ARG)         ARG(d2_device *, handle), ARG(d2_u32, srcfactor), ARG(d2_u32, dstfactor)
//
// Return types
//
#define _D2_GENERIC_RET(ARG)               GUI_RET_TYPE(d2_s32,           ARG)
#define _D2_DEVICE_RET(ARG)                GUI_RET_TYPE(d2_device*,       ARG)
#define _D2_RENDERBUFFER_RET(ARG)          GUI_RET_TYPE(d2_renderbuffer*, ARG)
#define _D2_BYTE_RET(ARG)                  GUI_RET_TYPE(d2_u8,            ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define DAVE2D_API_TABLE(FX)                                                      \
  FX(d2_opendevice,          _D2_DEVICE_RET,       _D2_OPENDEVICE_ARGS          ) \
  FX(d2_closedevice,         _D2_GENERIC_RET,      _D2_CLOSEDEVICE_ARGS         ) \
  FX(d2_inithw,              _D2_GENERIC_RET,      _D2_INITHW_ARGS              ) \
  FX(d2_deinithw,            _D2_GENERIC_RET,      _D2_DEINITHW_ARGS            ) \
  FX(d2_newrenderbuffer,     _D2_RENDERBUFFER_RET, _D2_NEWRENDERBUFFER_ARGS     ) \
  FX(d2_freerenderbuffer,    _D2_GENERIC_RET,      _D2_FREERENDERBUFFER_ARGS    ) \
  FX(d2_utility_fbblitcopy,  _D2_GENERIC_RET,      _D2_FBBLITCOPY_ARGS          ) \
  FX(d2_framebuffer,         _D2_GENERIC_RET,      _D2_FRAMEBUFFER_ARGS         ) \
  FX(d2_selectrenderbuffer,  _D2_GENERIC_RET,      _D2_SELECTRENDERBUFFER_ARGS  ) \
  FX(d2_executerenderbuffer, _D2_GENERIC_RET,      _D2_EXECUTERENDERBUFFER_ARGS ) \
  FX(d2_flushframe,          _D2_GENERIC_RET,      _D2_FLUSHFRAME_ARGS          ) \
  FX(d2_setcolor,            _D2_GENERIC_RET,      _D2_SETCOLOR_ARGS            ) \
  FX(d2_cliprect,            _D2_GENERIC_RET,      _D2_CLIPRECT_ARGS            ) \
  FX(d2_blitcopy,            _D2_GENERIC_RET,      _D2_BLITCOPY_ARGS            ) \
  FX(d2_setblitsrc,          _D2_GENERIC_RET,      _D2_SETBLITSRC_ARGS          ) \
  FX(d2_renderbox,           _D2_GENERIC_RET,      _D2_RENDERBOX_ARGS           ) \
  FX(d2_rendercircle,        _D2_GENERIC_RET,      _D2_RENDERCIRCLE_ARGS        ) \
  FX(d2_renderpolygon,       _D2_GENERIC_RET,      _D2_RENDERPOLYGON_ARGS       ) \
  FX(d2_renderpolyline,      _D2_GENERIC_RET,      _D2_RENDERPOLYLINE_ARGS      ) \
  FX(d2_renderline,          _D2_GENERIC_RET,      _D2_RENDERLINE_ARGS          ) \
  FX(d2_renderwedge,         _D2_GENERIC_RET,      _D2_RENDERWEDGE_ARGS         ) \
  FX(d2_selectrendermode,    _D2_GENERIC_RET,      _D2_SELECTRENDERMODE_ARGS    ) \
  FX(d2_setalphablendmodeex, _D2_GENERIC_RET,      _D2_SETALPHABLENDMODEEX_ARGS ) \
  FX(d2_settexopparam,       _D2_GENERIC_RET,      _D2_SETTEXOPPARAM_ARGS       ) \
  FX(d2_settextureoperation, _D2_GENERIC_RET,      _D2_SETTEXTUREOPERATION_ARGS ) \
  FX(d2_setfillmode,         _D2_GENERIC_RET,      _D2_SETFILLMODE_ARGS         ) \
  FX(d2_getfillmode,         _D2_BYTE_RET,         _D2_GETFILLMODE_ARGS         ) \
  FX(d2_settexclut_format,   _D2_GENERIC_RET,      _D2_SETTEXCLUTFORMAT_ARGS    ) \
  FX(d2_settexture,          _D2_GENERIC_RET,      _D2_SETTEXTURE_ARGS          ) \
  FX(d2_settexclut,          _D2_GENERIC_RET,      _D2_SETTEXCLUT_ARGS          ) \
  FX(d2_settexturemapping,   _D2_GENERIC_RET,      _D2_SETTEXTUREMAPPING_ARGS   ) \
  FX(d2_setalpha,            _D2_GENERIC_RET,      _D2_SETALPHA_ARGS            ) \
  FX(d2_setblendmode,        _D2_GENERIC_RET,      _D2_SETBLENDMODE_ARGS        )

//
// Make macro templates for API table
//
#define DAVE2D_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(DAVE2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define DAVE2D_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(DAVE2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define DAVE2D_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(DAVE2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define DAVE2D_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(DAVE2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
DAVE2D_API_TABLE(DAVE2D_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for Dave2D
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_DAVE2D_API_STRUCT
*
*  Description
*    Maps the required functions of the TES Dave2D API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the Dave2D code (meaning \c{GUI_GPU_HAS_DAVE2D} was not defined.
*
*    For more details about what each function does, please refer to the Dave2D
*    API documentation.
*/
typedef struct {
  DAVE2D_API_TABLE(DAVE2D_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_DAVE2D_API_STRUCT;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_DAVE2D_API
* 
*  Description
*   Macro to fill a structure of the type GUI_GPU_DAVE2D_API_STRUCT
*    filled with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_DAVE2D
  #define GUI_GPU_DAVE2D_API  DAVE2D_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_DAVE2D_API  DAVE2D_API_TABLE(GUI_GENERATE_NULL)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_DAVE2D_H */

/*************************** End of file ****************************/
