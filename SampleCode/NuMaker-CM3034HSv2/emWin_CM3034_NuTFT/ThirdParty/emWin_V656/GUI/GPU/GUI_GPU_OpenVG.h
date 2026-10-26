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
File        : GUI_GPU_OpenVG.h
Purpose     : OpenVG interface for SVG
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_OPENVG_H
#define GUI_GPU_OPENVG_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Use legacy definitions from SVG driver
*
**********************************************************************
*/
#ifdef    GUI_SVG_OPENVG_HEADER
  #define GUI_GPU_OPENVG_HEADER    GUI_SVG_OPENVG_HEADER
#endif

#ifdef    GUI_SVG_VGU_HEADER
  #define GUI_GPU_VGU_HEADER       GUI_SVG_VGU_HEADER
#endif

#ifdef    GUI_SVG_EGL_HEADER
  #define GUI_GPU_EGL_HEADER       GUI_SVG_EGL_HEADER
#endif

#ifdef    GUI_SVG_HAS_OPENVG
  #define GUI_GPU_HAS_OPENVG
#endif

#ifdef    GUI_SVG_HAS_EGL
  #define GUI_GPU_HAS_EGL
#endif

/*********************************************************************
*
*       OpenVG and EGL header files.
*
**********************************************************************
*/
#ifndef   GUI_GPU_OPENVG_HEADER
  #define GUI_GPU_OPENVG_HEADER   <VG/openvg.h>
#endif

#ifndef   GUI_GPU_VGU_HEADER
  #define GUI_GPU_VGU_HEADER      <VG/vgu.h>
#endif

#ifndef   GUI_GPU_EGL_HEADER
  #define GUI_GPU_EGL_HEADER      <EGL/egl.h>
#endif

#ifdef GUI_GPU_HAS_OPENVG
  #include GUI_GPU_OPENVG_HEADER
  #include GUI_GPU_VGU_HEADER
#endif

#ifdef GUI_GPU_HAS_EGL
  #include GUI_GPU_EGL_HEADER
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
#ifndef GUI_GPU_HAS_OPENVG
  //
  // Basic types
  //
  typedef enum {
    VG_FALSE = 0,
    VG_TRUE  = 1
  } VGboolean;
  #define VGfloat      float
  #define VGbyte       I8
  #define VGubyte      U8
  #define VGshort      U16
  #define VGint        I32
  #define VGuint       unsigned
  #define VGbitfield   U32
  //
  // Handle types
  //
  #define VGHandle     VGuint
  #define VGPath       VGuint
  #define VGPaint      VGuint
  #define VGImage      VGuint
  //
  // Enum types
  //
  #define VGParamType               int
  #define VGPathDatatype            int
  #define VGImageFormat             int
  #define VGPaintType               int
  #define VGPathParamType           int
  #define VGPaintParamType          int
  #define VGBlendMode               int
  #define VGPathCapabilities        int
  #define VGRenderingQuality        int
  #define VGImageQuality            int
  #define VGMatrixMode              int
  #define VGPaintMode               int
  #define VGFillRule                int
  #define VGColorRampSpreadMode     int
  //
  // Enum constants
  //
  #define VG_MATRIX_MODE                   ((VGParamType)0x1100)
  #define VG_STROKE_LINE_WIDTH             ((VGParamType)0x1110)
  #define VG_STROKE_CAP_STYLE              ((VGParamType)0x1111)
  #define VG_STROKE_JOIN_STYLE             ((VGParamType)0x1112)
  #define VG_STROKE_MITER_LIMIT            ((VGParamType)0x1113)
  #define VG_STROKE_DASH_PATTERN           ((VGParamType)0x1114)
  #define VG_STROKE_DASH_PHASE             ((VGParamType)0x1115)
  #define VG_STROKE_DASH_PHASE_RESET       ((VGParamType)0x1116)
  #define VG_STROKE_PATH                   ((VGPaintMode)(1 << 0))
  #define VG_FILL_PATH                     ((VGPaintMode)(1 << 1))
  #define VG_PATH_SCALE                    ((VGPathParamType)0x1602)
  #define VG_PATH_BIAS                     ((VGPathParamType)0x1603)
  #define VG_PATH_FORMAT_STANDARD          0
  #define VG_PATH_DATATYPE_F               ((VGPathDatatype)3)
  #define VG_PATH_CAPABILITY_ALL           ((VGPathCapabilities)((1 << 12) - 1))
  #define VG_PAINT_TYPE                    ((VGPaintParamType)0x1A00)
  #define VG_PAINT_COLOR                   ((VGPaintParamType)0x1A01)
  #define VG_PAINT_COLOR_RAMP_SPREAD_MODE  ((VGPaintParamType)0x1A02)
  #define VG_PAINT_COLOR_RAMP_STOPS        ((VGPaintParamType)0x1A03)
  #define VG_PAINT_LINEAR_GRADIENT         ((VGPaintParamType)0x1A04)
  #define VG_PAINT_RADIAL_GRADIENT         ((VGPaintParamType)0x1A05)
  #define VG_PAINT_TYPE_COLOR              ((VGPaintType)0x1B00)
  #define VG_PAINT_TYPE_LINEAR_GRADIENT    ((VGPaintType)0x1B01)
  #define VG_PAINT_TYPE_RADIAL_GRADIENT    ((VGPaintType)0x1B02)
  #define VG_CLEAR_COLOR                   ((VGParamType)0x1121)
  #define VG_FILL_RULE                     ((VGParamType)0x1101)
  #define VG_RENDERING_QUALITY             ((VGParamType)0x1103)
  #define VG_BLEND_MODE                    ((VGParamType)0x1104)
  #define VG_SCISSOR_RECTS                 ((VGParamType)0x1106)
  #define VG_SCISSORING                    ((VGParamType)0x1131)
  #define VG_BLEND_SRC_OVER                ((VGBlendMode)0x2001)
  #define VG_RENDERING_QUALITY_FASTER      ((VGRenderingQuality)0x1201)
  #define VG_RENDERING_QUALITY_BETTER      ((VGRenderingQuality)0x1202)
  #define VG_IMAGE_QUALITY_BETTER          ((VGImageQuality)(1 << 2))
  #define VG_MATRIX_PATH_USER_TO_SURFACE   ((VGMatrixMode)0x1400)
  #define VG_MATRIX_IMAGE_USER_TO_SURFACE  ((VGMatrixMode)0x1401)
  #define VG_MATRIX_FILL_PAINT_TO_USER     ((VGMatrixMode)0x1402)
  #define VG_MATRIX_STROKE_PAINT_TO_USER   ((VGMatrixMode)0x1403)
  #define VG_NON_ZERO                      ((VGFillRule)0x1901)
  #define VG_BW_1                          ((VGImageFormat)12)
  #define VG_sBGR_565                      ((VGImageFormat)(3 | (1 << 7)))
  #define VG_sRGB_565                      ((VGImageFormat) 3)
  #define VG_sABGR_1555                    ((VGImageFormat)(4 | (1 << 6) | (1 << 7)))
  #define VG_sARGB_1555                    ((VGImageFormat)(4 | (1 << 6)))
  #define VG_sXBGR_8888                    ((VGImageFormat)(0 | (1 << 6) | (1 << 7)))
  #define VG_sABGR_8888                    ((VGImageFormat)(1 | (1 << 6) | (1 << 7)))
  #define VG_sRGBA_8888                    ((VGImageFormat) 1)
  #define VG_sXRGB_8888                    ((VGImageFormat)(0 | (1 << 6)))
  #define VG_sARGB_8888                    ((VGImageFormat)(1 | (1 << 6)))
#endif

#ifndef GUI_GPU_HAS_EGL
  //
  // EGL constants
  //
  #define EGL_DONT_CARE               ((EGLint)-1)
  #define EGL_ALPHA_SIZE              0x3021
  #define EGL_BLUE_SIZE               0x3022
  #define EGL_GREEN_SIZE              0x3023
  #define EGL_RED_SIZE                0x3024
  #define EGL_SAMPLES                 0x3031
  #define EGL_SURFACE_TYPE            0x3033
  #define EGL_NONE                    0x3038
  #define EGL_LUMINANCE_SIZE          0x303D
  #define EGL_WINDOW_BIT              0x0004
  #define EGL_DEFAULT_DISPLAY         ((void *)0)
  #define EGL_SUCCESS                 0x3000
  #define EGL_OPENVG_API              0x30A1
  #define EGL_RENDERABLE_TYPE         0x3040
  #define EGL_OPENVG_BIT              0x0002
  #define EGL_PIXMAP_BIT              0x0002
  //
  // Generic types
  //
  #define NativeWindowType            void*
  //
  // EGL structs
  //
  typedef struct {
    int	   format;
    int	   width;
    int	   height;
    int	   stride;
    void * data;
  } NativePixmap;
  typedef NativePixmap*	NativePixmapType;
  //
  // EGL types
  //
  #define EGLDisplay           void*
  #define EGLSurface           void*
  #define EGLContext           void*
  #define EGLConfig            void*
  #define EGLNativeDisplayType void*
  #define EGLBoolean           unsigned int
  #define EGLint               int
  #define EGLenum              unsigned int
  #define EGLNativePixmapType  NativePixmapType
#endif

/*********************************************************************
*
*       API function table: EGL
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _EGL_GETDISPLAY_ARGS(ARG)            ARG(EGLNativeDisplayType, display_id)
#define _EGL_CREATEPIXMAPSURFACE_ARGS(ARG)   ARG(EGLDisplay, dpy), ARG(EGLConfig, config), ARG(EGLNativePixmapType, pixmap), ARG(const EGLint *, attrib_list)
#define _EGL_DESTROYSURFACE_ARGS(ARG)        ARG(EGLDisplay, dpy), ARG(EGLSurface, surface)
#define _EGL_CREATECONTEXT_ARGS(ARG)         ARG(EGLDisplay, dpy), ARG(EGLConfig, config), ARG(EGLContext, share_context), ARG(const EGLint *, attrib_list)
#define _EGL_INITIALIZE_ARGS(ARG)            ARG(EGLDisplay, dpy), ARG(EGLint *, major), ARG(EGLint *, minor)
#define _EGL_BINDAPI_ARGS(ARG)               ARG(EGLenum, api)
#define _EGL_CHOOSECONFIG_ARGS(ARG)          ARG(EGLDisplay, dpy), ARG(const EGLint *, attrib_list), ARG(EGLConfig *, configs), ARG(EGLint, config_size), ARG(EGLint *, num_config)
#define _EGL_MAKECURRENT_ARGS(ARG)           ARG(EGLDisplay, dpy), ARG(EGLSurface, draw), ARG(EGLSurface, read), ARG(EGLContext, ctx)
//
// Return types
//
#define _EGL_DISPLAY_RET(ARG)     GUI_RET_TYPE(EGLDisplay, ARG)
#define _EGL_SURFACE_RET(ARG)     GUI_RET_TYPE(EGLSurface, ARG)
#define _EGL_BOOLEAN_RET(ARG)     GUI_RET_TYPE(EGLBoolean, ARG)
#define _EGL_CONTEXT_RET(ARG)     GUI_RET_TYPE(EGLContext, ARG)
#define _EGL_INT_RET(ARG)         GUI_RET_TYPE(EGLint,     ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define EGL_API_TABLE(FX)                                                      \
  FX(eglGetDisplay,          _EGL_DISPLAY_RET, _EGL_GETDISPLAY_ARGS          ) \
  FX(eglCreatePixmapSurface, _EGL_SURFACE_RET, _EGL_CREATEPIXMAPSURFACE_ARGS ) \
  FX(eglDestroySurface,      _EGL_BOOLEAN_RET, _EGL_DESTROYSURFACE_ARGS      ) \
  FX(eglCreateContext,       _EGL_CONTEXT_RET, _EGL_CREATECONTEXT_ARGS       ) \
  FX(eglInitialize,          _EGL_BOOLEAN_RET, _EGL_INITIALIZE_ARGS          ) \
  FX(eglBindAPI,             _EGL_BOOLEAN_RET, _EGL_BINDAPI_ARGS             ) \
  FX(eglChooseConfig,        _EGL_BOOLEAN_RET, _EGL_CHOOSECONFIG_ARGS        ) \
  FX(eglMakeCurrent,         _EGL_BOOLEAN_RET, _EGL_MAKECURRENT_ARGS         ) \
  FX(eglGetError,            _EGL_INT_RET,     GUI_NO_ARGS                   )

//
// Make macro templates for API table
//
#define _EGL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, CALL_ARGS, TYPE_ARGS) \
  pEGL->FUNC_NAME = (GUI_GPU_EGL_##FUNC_NAME##_FUNC *)cbLoadFunction(#FUNC_NAME);

#define EGL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  _EGL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, _GENERATE_CALL_ARGS(ARG_LIST), _GENERATE_TYPE_ARGS(ARG_LIST))

#define EGL_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(EGL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define EGL_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(EGL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define EGL_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(EGL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       API function table: OpenVG
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _OVG_LOADMATRIX_ARGS(ARG)      ARG(const VGfloat *, pMatrix)
#define _OVG_CREATEPATH_ARGS(ARG)      ARG(VGint, PathFormat), ARG(VGPathDatatype, PathType), ARG(VGfloat, Scale), ARG(VGfloat, Bias), ARG(VGint, SegmentCapacity), ARG(VGint, CoordCapacityHint), ARG(VGbitfield, PathCapabilities)
#define _OVG_DESTROYPATH_ARGS(ARG)     ARG(VGPath, hPath)
#define _OVG_CLEARPATH_ARGS(ARG)       ARG(VGPath, hPath), ARG(VGbitfield, pathCapabilities)
#define _OVG_CREATEIMAGE_ARGS(ARG)     ARG(VGImageFormat, format), ARG(VGint, width), ARG(VGint, height), ARG(VGbitfield, allowedQuality)
#define _OVG_DESTROYIMAGE_ARGS(ARG)    ARG(VGImage, hImg)
#define _OVG_DRAWIMAGE_ARGS(ARG)       ARG(VGImage, hImg)
#define _OVG_IMAGESUBDATA_ARGS(ARG)    ARG(VGImage, image), ARG(const void *, data), ARG(VGint, dataStride), ARG(VGImageFormat, dataFormat), ARG(VGint, x), ARG(VGint, y), ARG(VGint, width), ARG(VGint, height)
#define _OVG_APPENDPATHDATA_ARGS(ARG)  ARG(VGPath, hPath), ARG(VGint, NumSegments), ARG(const VGubyte *, pSegments), ARG(const void *, pData)
#define _OVG_DRAWPATH_ARGS(ARG)        ARG(VGPath, hPath), ARG(VGbitfield, PaintModes)
#define _OVG_SETPAINT_ARGS(ARG)        ARG(VGPaint, hPaint), ARG(VGbitfield, PaintModes)
#define _OVG_DESTROYPAINT_ARGS(ARG)    ARG(VGPaint, hPaint)
#define _OVG_WRITEPIXELS_ARGS(ARG)     ARG(const void *, data), ARG(VGint, dataStride), ARG(VGImageFormat, dataFormat), ARG(VGint, dx), ARG(VGint, dy), ARG(VGint, width), ARG(VGint, height)
#define _OVG_READPIXELS_ARGS(ARG)      ARG(void *, data), ARG(VGint, dataStride), ARG(VGImageFormat, dataFormat), ARG(VGint, sx), ARG(VGint, sy), ARG(VGint, width), ARG(VGint, height)
#define _OVG_SETF_ARGS(ARG)            ARG(VGParamType, ParamType), ARG(VGfloat, Val)
#define _OVG_SETI_ARGS(ARG)            ARG(VGParamType, ParamType), ARG(VGint, Val)
#define _OVG_SETFV_ARGS(ARG)           ARG(VGParamType, ParamType), ARG(VGint, Count), ARG(const VGfloat *, pData)
#define _OVG_SETIV_ARGS(ARG)           ARG(VGParamType, ParamType), ARG(VGint, Count), ARG(const VGint *, pData)
#define _OVG_SETPARAMETERI_ARGS(ARG)   ARG(VGHandle, hObj), ARG(VGint, ParamType), ARG(VGint, Val)
#define _OVG_SETPARAMETERFV_ARGS(ARG)  ARG(VGHandle, hObj), ARG(VGint, ParamType), ARG(VGint, Count), ARG(const VGfloat *, pData)
//
// Return types
//
#define _OVG_PATH_RET(ARG)    GUI_RET_TYPE(VGPath, ARG)
#define _OVG_IMAGE_RET(ARG)   GUI_RET_TYPE(VGImage, ARG)
#define _OVG_PAINT_RET(ARG)   GUI_RET_TYPE(VGPaint, ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define OPENVG_API_TABLE(FX)                                           \
  FX(vgLoadMatrix,     GUI_RETURN(void),    _OVG_LOADMATRIX_ARGS     ) \
  FX(vgCreatePath,     _OVG_PATH_RET,       _OVG_CREATEPATH_ARGS     ) \
  FX(vgDestroyPath,    GUI_RETURN(void),    _OVG_DESTROYPATH_ARGS    ) \
  FX(vgClearPath,      GUI_RETURN(void),    _OVG_CLEARPATH_ARGS      ) \
  FX(vgCreateImage,    _OVG_IMAGE_RET,      _OVG_CREATEIMAGE_ARGS    ) \
  FX(vgDestroyImage,   GUI_RETURN(void),    _OVG_DESTROYIMAGE_ARGS   ) \
  FX(vgDrawImage,      GUI_RETURN(void),    _OVG_DRAWIMAGE_ARGS      ) \
  FX(vgImageSubData,   GUI_RETURN(void),    _OVG_IMAGESUBDATA_ARGS   ) \
  FX(vgAppendPathData, GUI_RETURN(void),    _OVG_APPENDPATHDATA_ARGS ) \
  FX(vgDrawPath,       GUI_RETURN(void),    _OVG_DRAWPATH_ARGS       ) \
  FX(vgCreatePaint,    _OVG_PAINT_RET,      GUI_NO_ARGS              ) \
  FX(vgSetPaint,       GUI_RETURN(void),    _OVG_SETPAINT_ARGS       ) \
  FX(vgDestroyPaint,   GUI_RETURN(void),    _OVG_DESTROYPAINT_ARGS   ) \
  FX(vgFlush,          GUI_RETURN(void),    GUI_NO_ARGS              ) \
  FX(vgFinish,         GUI_RETURN(void),    GUI_NO_ARGS              ) \
  FX(vgWritePixels,    GUI_RETURN(void),    _OVG_WRITEPIXELS_ARGS    ) \
  FX(vgReadPixels,     GUI_RETURN(void),    _OVG_READPIXELS_ARGS     ) \
  FX(vgSetf,           GUI_RETURN(void),    _OVG_SETF_ARGS           ) \
  FX(vgSeti,           GUI_RETURN(void),    _OVG_SETI_ARGS           ) \
  FX(vgSetfv,          GUI_RETURN(void),    _OVG_SETFV_ARGS          ) \
  FX(vgSetiv,          GUI_RETURN(void),    _OVG_SETIV_ARGS          ) \
  FX(vgSetParameteri,  GUI_RETURN(void),    _OVG_SETPARAMETERI_ARGS  ) \
  FX(vgSetParameterfv, GUI_RETURN(void),    _OVG_SETPARAMETERFV_ARGS )

//
// Make macro templates for API table
//
#define OPENVG_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(OPENVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define OPENVG_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(OPENVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define OPENVG_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(OPENVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define OPENVG_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(OPENVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
EGL_API_TABLE(EGL_GENERATE_API_FUNC_PROTOTYPE)
OPENVG_API_TABLE(OPENVG_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for OpenVG
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_EGL_API_STRUCT
*
*  Description
*    Maps the required EGL functions of the Khronos OpenVG API.
*
*    Using EGL is optional since some OpenVG implementations do not implement
*    the EGL API.
*/
typedef struct {
  EGL_API_TABLE(EGL_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_EGL_API_STRUCT;

/*********************************************************************
*
*       GUI_GPU_OPENVG_API_STRUCT
*
*  Description
*    Maps the required functions of the Khronos OpenVG API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the OpenVG code (meaning \c{GUI_GPU_HAS_OPENVG} and \c{GUI_GPU_HAS_EGL}
*    were not defined.
*
*    For more details about these functions, please refer to the OpenVG
*    API documentation.
*/
typedef struct {
  OPENVG_API_TABLE(OPENVG_GENERATE_API_STRUCT_MEMBER)
  const GUI_GPU_EGL_API_STRUCT * pEGL; // Pointer to a GUI_GPU_EGL_API_STRUCT structure. May be NULL if EGL is not to be used.
} GUI_GPU_OPENVG_API_STRUCT;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_EGL_API
*
*  Description
*    Macro to fill a structure of the type GUI_GPU_EGL_API_STRUCT
*    with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_EGL
  #define GUI_GPU_EGL_API   EGL_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_EGL_API   EGL_API_TABLE(GUI_GENERATE_NULL)
#endif

/*********************************************************************
*
*       GUI_GPU_OPENVG_API
*
*  Description
*    Macro to fill a structure of the type GUI_GPU_OPENVG_API_STRUCT
*    with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_OPENVG
  #define GUI_GPU_OPENVG_API   OPENVG_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_OPENVG_API   OPENVG_API_TABLE(GUI_GENERATE_NULL)
#endif

/* emDoc stop */
//
// Legacy macros
//
#define GUI_SVG_DECLARE_OPENVG_API(VAR_NAME, EGL_PTR)  \
  static const GUI_GPU_OPENVG_API_STRUCT VAR_NAME = { GUI_GPU_OPENVG_API, EGL_PTR }

#define GUI_SVG_DECLARE_EGL_API(VAR_NAME)  \
  static const GUI_GPU_EGL_API VAR_NAME = { GUI_GPU_EGL_API }

#define GUI_SVG_DECLARE_OPENVG_AND_EGL_API(VAR_NAME) \
  GUI_SVG_DECLARE_EGL_API(_EGL);                     \
  GUI_SVG_DECLARE_OPENVG_API(VAR_NAME, &_EGL)

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_OPENVG_H */

/*************************** End of file ****************************/
