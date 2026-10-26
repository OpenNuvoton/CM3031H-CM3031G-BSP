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
File        : GUI_GPU_NanoVG.h
Purpose     : Header of NanoVG GPU driver + vector support
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_NANOVG_H
#define GUI_GPU_NANOVG_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Use legacy definitions from SVG driver
*
**********************************************************************
*/
#ifdef    GUI_SVG_OPENGL2_HEADER
  #define GUI_GPU_OPENGL2_HEADER          GUI_SVG_OPENGL2_HEADER
#endif

#ifdef    GUI_SVG_OPENGL3_HEADER
  #define GUI_GPU_OPENGL3_HEADER          GUI_SVG_OPENGL3_HEADER
#endif

#ifdef    GUI_SVG_OPENGLES2_HEADER
  #define GUI_GPU_OPENGLES2_HEADER        GUI_SVG_OPENGLES2_HEADER
#endif

#ifdef    GUI_SVG_OPENGLES3_HEADER
  #define GUI_GPU_OPENGLES3_HEADER        GUI_SVG_OPENGLES3_HEADER
#endif

#ifdef    GUI_SVG_NANOVG_HEADER
  #define GUI_GPU_NANOVG_HEADER           GUI_SVG_NANOVG_HEADER
#endif

#ifdef    GUI_SVG_NANOVG_GL_HEADER
  #define GUI_GPU_NANOVG_GL_HEADER        GUI_SVG_NANOVG_GL_HEADER
#endif

#ifdef    GUI_SVG_NANOVG_RENDER_BACKEND
  #define GUI_GPU_NANOVG_RENDER_BACKEND   GUI_SVG_NANOVG_RENDER_BACKEND
#endif

#ifdef    GUI_SVG_HAS_NANOVG
  #define GUI_GPU_HAS_NANOVG
#endif

/*********************************************************************
*
*       OpenGL header files
*
**********************************************************************
*/
#ifndef   GUI_GPU_OPENGL2_HEADER
  #define GUI_GPU_OPENGL2_HEADER      <GL/gl.h>
#endif

#ifndef   GUI_GPU_OPENGL3_HEADER
  #define GUI_GPU_OPENGL3_HEADER      <GL/gl3.h>
#endif

#ifndef   GUI_GPU_OPENGLES2_HEADER
  #define GUI_GPU_OPENGLES2_HEADER    <GLES2/gl2.h>
#endif

#ifndef   GUI_GPU_OPENGLES3_HEADER
  #define GUI_GPU_OPENGLES3_HEADER    <GLES3/gl3.h>
#endif

/*********************************************************************
*
*       NanoVG header files.
*
**********************************************************************
*/
#ifndef   GUI_GPU_NANOVG_HEADER
  #define GUI_GPU_NANOVG_HEADER      <nanovg.h>
#endif

#ifndef   GUI_GPU_NANOVG_GL_HEADER
  #define GUI_GPU_NANOVG_GL_HEADER   <nanovg_gl.h>
#endif

/*********************************************************************
*
*       NanoVG rendering backend
*
**********************************************************************
*/
#define GUI_GPU_NANOVG_BACKEND_GL2     0
#define GUI_GPU_NANOVG_BACKEND_GL3     1
#define GUI_GPU_NANOVG_BACKEND_GLES2   2
#define GUI_GPU_NANOVG_BACKEND_GLES3   3
//
// Default: OpenGL ES 2.0
//
#ifndef   GUI_GPU_NANOVG_RENDER_BACKEND
  #define GUI_GPU_NANOVG_RENDER_BACKEND   GUI_GPU_NANOVG_BACKEND_GLES2
#endif
//
// Make compile time selection of rendering backend.
//
#if   (GUI_GPU_NANOVG_RENDER_BACKEND == GUI_GPU_NANOVG_BACKEND_GL2)
  #ifdef GUI_GPU_NANOVG_IMPLEMENTATION
    #define NANOVG_GL2_IMPLEMENTATION
  #endif
  #define GUI_GPU_NANOVG_RENDER_BACKEND_HEADER     GUI_GPU_OPENGL2_HEADER
  #define FUNC_nvgCreate                           nvgCreateGL2
  #define FUNC_nvgDelete                           nvgDeleteGL2
#elif (GUI_GPU_NANOVG_RENDER_BACKEND == GUI_GPU_NANOVG_BACKEND_GL3)
  #ifdef GUI_GPU_NANOVG_IMPLEMENTATION
    #define NANOVG_GL3_IMPLEMENTATION
  #endif
  #define GUI_GPU_NANOVG_RENDER_BACKEND_HEADER     GUI_GPU_OPENGL3_HEADER
  #define FUNC_nvgCreate                           nvgCreateGL3
  #define FUNC_nvgDelete                           nvgDeleteGL3
#elif (GUI_GPU_NANOVG_RENDER_BACKEND == GUI_GPU_NANOVG_BACKEND_GLES2)
  #ifdef GUI_GPU_NANOVG_IMPLEMENTATION
    #define NANOVG_GLES2_IMPLEMENTATION
  #endif
  #define GUI_GPU_NANOVG_RENDER_BACKEND_HEADER     GUI_GPU_OPENGLES2_HEADER
  #define FUNC_nvgCreate                           nvgCreateGLES2
  #define FUNC_nvgDelete                           nvgDeleteGLES2
#elif (GUI_GPU_NANOVG_RENDER_BACKEND == GUI_GPU_NANOVG_BACKEND_GLES3)
  #ifdef GUI_GPU_NANOVG_IMPLEMENTATION
    #define NANOVG_GLES3_IMPLEMENTATION
  #endif
  #define GUI_GPU_NANOVG_RENDER_BACKEND_HEADER     GUI_GPU_OPENGLES3_HEADER
  #define FUNC_nvgCreate                           nvgCreateGLES3
  #define FUNC_nvgDelete                           nvgDeleteGLES3
#endif

#ifdef GUI_GPU_HAS_NANOVG
  //
  // Configure NanoVG
  //
  #ifndef   NVG_MAX_STATES
    #define NVG_MAX_STATES  1    // To save memory, we don't save/restore states.
  #endif
  //
  // Now we can include NanoVG
  //
  #include GUI_GPU_NANOVG_RENDER_BACKEND_HEADER
  #include GUI_GPU_NANOVG_HEADER
  #include GUI_GPU_NANOVG_GL_HEADER
  //
  // Cleanup, prevent multiple definitions
  //
  #undef NANOVG_GL2_IMPLEMENTATION
  #undef NANOVG_GL3_IMPLEMENTATION
  #undef NANOVG_GLES2_IMPLEMENTATION
  #undef NANOVG_GLES3_IMPLEMENTATION
  #undef NANOVG_GL_IMPLEMENTATION
  #undef GUI_GPU_NANOVG_IMPLEMENTATION
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Type definitions: OpenGL
*
**********************************************************************
*/
#ifndef GUI_GPU_HAS_NANOVG
  typedef int             GLint;
  typedef int             GLsizei;
  typedef unsigned int    GLenum;
  typedef void            GLvoid;
  typedef unsigned int    GLbitfield;
#endif

/*********************************************************************
*
*       Type definitions: NanoVG
*
**********************************************************************
*/
#ifndef GUI_GPU_HAS_NANOVG
  GUI_DEFINE_STRUCT_TYPE(NVGcolor,
    union {
      float rgba[4];
      struct {
        float r;
        float g;
        float b;
        float a;
      };
    };
  );
  GUI_DEFINE_STRUCT_TYPE(NVGpaint,
    float xform[6];
    float extent[2];
    float radius;
    float feather;
    NVGcolor innerColor;
    NVGcolor outerColor;
    int image;
  );
  typedef void NVGcontext;
  typedef int  NVGcreateFlags;
  typedef int  NVGimageFlags;
  typedef int  NVGlineCap;
  typedef int  NVGsolidity;
#endif

/*********************************************************************
*
*       API function table: OpenGL
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _GL_CLEAR_ARGS(ARG)        ARG(GLbitfield, mask)
#define _GL_READPIXELS_ARGS(ARG)   ARG(GLint, x), ARG(GLint, y), ARG(GLsizei, width), ARG(GLsizei, height), ARG(GLenum, format), ARG(GLenum, type), ARG(GLvoid *, pixels)
//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define GL_API_TABLE(FX)                                    \
  FX(glClear,      GUI_RETURN(void), _GL_CLEAR_ARGS      )  \
  FX(glReadPixels, GUI_RETURN(void), _GL_READPIXELS_ARGS )

//
// Make macro templates for API table
//
#define _GL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, CALL_ARGS, TYPE_ARGS) \
  ((GUI_GPU_NANOVG_API_STRUCT *)pAPI)->GL.FUNC_NAME = (GUI_GPU_GL_##FUNC_NAME##_FUNC *)cbLoadFunction(#FUNC_NAME);

#define GL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  _GL_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, _GENERATE_CALL_ARGS(ARG_LIST), _GENERATE_TYPE_ARGS(ARG_LIST))

#define GL_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(GL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define GL_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(GL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define GL_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(GL, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       API function table: NanoVG
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _NVG_BEGINFRAME_ARGS(ARG)           ARG(NVGcontext *, vg), ARG(float, width), ARG(float, height), ARG(float, devicePixelRatio)
#define _NVG_BEGINPATH_ARGS(ARG)            ARG(NVGcontext *, vg)
#define _NVG_BEZIERTO_ARGS(ARG)             ARG(NVGcontext *, vg), ARG(float, c1x), ARG(float, c1y), ARG(float, c2x), ARG(float, c2y), ARG(float, x), ARG(float, y)
#define _NVG_CLOSEPATH_ARGS(ARG)            ARG(NVGcontext *, vg)
#define _NVG_CREATE_ARGS(ARG)               ARG(int, flags)
#define _NVG_CREATEIMAGERGBA_ARGS(ARG)      ARG(NVGcontext *, vg), ARG(int, width), ARG(int, height), ARG(int, flags), ARG(const unsigned char *, data)
#define _NVG_DELETE_ARGS(ARG)               ARG(NVGcontext *, vg)
#define _NVG_DELETEIMAGE_ARGS(ARG)          ARG(NVGcontext *, vg), ARG(int, image)
#define _NVG_ENDFRAME_ARGS(ARG)             ARG(NVGcontext *, vg)
#define _NVG_FILL_ARGS(ARG)                 ARG(NVGcontext *, vg)
#define _NVG_FILLCOLOR_ARGS(ARG)            ARG(NVGcontext *, vg), ARG(NVGcolor, color)
#define _NVG_FILLPAINT_ARGS(ARG)            ARG(NVGcontext *, vg), ARG(NVGpaint, paint)
#define _NVG_GLOBALALPHA_ARGS(ARG)          ARG(NVGcontext *, vg), ARG(float, alpha)
#define _NVG_IMAGEPATTERN_ARGS(ARG)         ARG(NVGcontext *, vg), ARG(float, ox), ARG(float, oy), ARG(float, ex), ARG(float, ey), ARG(float, angle), ARG(int, image), ARG(float, alpha)
#define _NVG_LINECAP_ARGS(ARG)              ARG(NVGcontext *, vg), ARG(int, cap)
#define _NVG_LINEJOIN_ARGS(ARG)             ARG(NVGcontext *, vg), ARG(int, join)
#define _NVG_LINETO_ARGS(ARG)               ARG(NVGcontext *, vg), ARG(float, x), ARG(float, y)
#define _NVG_MITERLIMIT_ARGS(ARG)           ARG(NVGcontext *, vg), ARG(float, limit)
#define _NVG_MOVETO_ARGS(ARG)               ARG(NVGcontext *, vg), ARG(float, x), ARG(float, y)
#define _NVG_PATHWINDING_ARGS(ARG)          ARG(NVGcontext *, vg), ARG(int, dir)
#define _NVG_QUADTO_ARGS(ARG)               ARG(NVGcontext *, vg), ARG(float, cx), ARG(float, cy), ARG(float, x), ARG(float, y)
#define _NVG_RECT_ARGS(ARG)                 ARG(NVGcontext *, vg), ARG(float, x), ARG(float, y), ARG(float, w), ARG(float, h)
#define _NVG_RESETSCISSOR_ARGS(ARG)         ARG(NVGcontext *, vg)
#define _NVG_RESETTRANSFORM_ARGS(ARG)       ARG(NVGcontext *, vg)
#define _NVG_RGBA_ARGS(ARG)                 ARG(unsigned char, r), ARG(unsigned char, g), ARG(unsigned char, b), ARG(unsigned char, a)
#define _NVG_SCISSOR_ARGS(ARG)              ARG(NVGcontext *, vg), ARG(float, x), ARG(float, y), ARG(float, w), ARG(float, h)
#define _NVG_SHAPEANTIALIAS_ARGS(ARG)       ARG(NVGcontext *, vg), ARG(int, enabled)
#define _NVG_STROKE_ARGS(ARG)               ARG(NVGcontext *, vg)
#define _NVG_STROKECOLOR_ARGS(ARG)          ARG(NVGcontext *, vg), ARG(NVGcolor, color)
#define _NVG_STROKEWIDTH_ARGS(ARG)          ARG(NVGcontext *, vg), ARG(float, size)
#define _NVG_TRANSFORM_ARGS(ARG)            ARG(NVGcontext *, vg), ARG(float, a), ARG(float, b), ARG(float, c), ARG(float, d), ARG(float, e), ARG(float, f)
//
// Return types
//
#define _NVG_RGBA_RET(ARG)                  GUI_RET_TYPE(NVGcolor,    ARG)
#define _NVG_IMAGEPATTERN_RET(ARG)          GUI_RET_TYPE(NVGpaint,    ARG)
#define _NVG_CREATE_RET(ARG)                GUI_RET_TYPE(NVGcontext*, ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_MACRO, ARG_LIST)
//
#define NANOVG_API_TABLE(FX)                                                          \
  FX(nvgBeginFrame,                GUI_RETURN(void),      _NVG_BEGINFRAME_ARGS      ) \
  FX(nvgBeginPath,                 GUI_RETURN(void),      _NVG_BEGINPATH_ARGS       ) \
  FX(nvgBezierTo,                  GUI_RETURN(void),      _NVG_BEZIERTO_ARGS        ) \
  FX(nvgClosePath,                 GUI_RETURN(void),      _NVG_CLOSEPATH_ARGS       ) \
  FX(nvgCreateImageRGBA,           GUI_RETURN(int),       _NVG_CREATEIMAGERGBA_ARGS ) \
  FX(GUI_RESOLVE(FUNC_nvgCreate),  _NVG_CREATE_RET,       _NVG_CREATE_ARGS          ) \
  FX(GUI_RESOLVE(FUNC_nvgDelete),  GUI_RETURN(void),      _NVG_DELETE_ARGS          ) \
  FX(nvgDeleteImage,               GUI_RETURN(void),      _NVG_DELETEIMAGE_ARGS     ) \
  FX(nvgEndFrame,                  GUI_RETURN(void),      _NVG_ENDFRAME_ARGS        ) \
  FX(nvgFill,                      GUI_RETURN(void),      _NVG_FILL_ARGS            ) \
  FX(nvgFillColor,                 GUI_RETURN(void),      _NVG_FILLCOLOR_ARGS       ) \
  FX(nvgFillPaint,                 GUI_RETURN(void),      _NVG_FILLPAINT_ARGS       ) \
  FX(nvgGlobalAlpha,               GUI_RETURN(void),      _NVG_GLOBALALPHA_ARGS     ) \
  FX(nvgImagePattern,              _NVG_IMAGEPATTERN_RET, _NVG_IMAGEPATTERN_ARGS    ) \
  FX(nvgLineCap,                   GUI_RETURN(void),      _NVG_LINECAP_ARGS         ) \
  FX(nvgLineJoin,                  GUI_RETURN(void),      _NVG_LINEJOIN_ARGS        ) \
  FX(nvgLineTo,                    GUI_RETURN(void),      _NVG_LINETO_ARGS          ) \
  FX(nvgMiterLimit,                GUI_RETURN(void),      _NVG_MITERLIMIT_ARGS      ) \
  FX(nvgMoveTo,                    GUI_RETURN(void),      _NVG_MOVETO_ARGS          ) \
  FX(nvgPathWinding,               GUI_RETURN(void),      _NVG_PATHWINDING_ARGS     ) \
  FX(nvgQuadTo,                    GUI_RETURN(void),      _NVG_QUADTO_ARGS          ) \
  FX(nvgRect,                      GUI_RETURN(void),      _NVG_RECT_ARGS            ) \
  FX(nvgResetScissor,              GUI_RETURN(void),      _NVG_RESETSCISSOR_ARGS    ) \
  FX(nvgResetTransform,            GUI_RETURN(void),      _NVG_RESETTRANSFORM_ARGS  ) \
  FX(nvgRGBA,                      _NVG_RGBA_RET,         _NVG_RGBA_ARGS            ) \
  FX(nvgScissor,                   GUI_RETURN(void),      _NVG_SCISSOR_ARGS         ) \
  FX(nvgShapeAntiAlias,            GUI_RETURN(void),      _NVG_SHAPEANTIALIAS_ARGS  ) \
  FX(nvgStroke,                    GUI_RETURN(void),      _NVG_STROKE_ARGS          ) \
  FX(nvgStrokeColor,               GUI_RETURN(void),      _NVG_STROKECOLOR_ARGS     ) \
  FX(nvgStrokeWidth,               GUI_RETURN(void),      _NVG_STROKEWIDTH_ARGS     ) \
  FX(nvgTransform,                 GUI_RETURN(void),      _NVG_TRANSFORM_ARGS       )

//
// Make macro templates for API table
//
#define NANOVG_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(NANOVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NANOVG_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(NANOVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NANOVG_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(NANOVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NANOVG_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(NANOVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
GL_API_TABLE(GL_GENERATE_API_FUNC_PROTOTYPE)
NANOVG_API_TABLE(NANOVG_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for OpenGL
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_GL_API_STRUCT
*
*  Description
*    Maps the required OpenGL functions required for NanoVG.
*
*    For more details about these functions, please refer to the OpenGL
*    API documentation.
*/
typedef struct {
  GL_API_TABLE(GL_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_GL_API_STRUCT;

/*********************************************************************
*
*       API struct for NanoVG
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_NANOVG_API_STRUCT
*
*  Description
*    Maps the required functions of the NanoVG API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the NanoVG code (meaning \c{GUI_GPU_HAS_NANOVG} was not defined.
*
*    For more details about these functions, please refer to the NanoVG
*    API documentation.
*/
typedef struct {
  GUI_GPU_GL_API_STRUCT GL; // Functions for OpenGL rendering backend.
  NANOVG_API_TABLE(NANOVG_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_NANOVG_API_STRUCT;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_NANOVG_API
*
*  Description
*    Macro to fill a structure of the type GUI_GPU_NANOVG_API_STRUCT
*    filled with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_NANOVG
  #define GUI_GPU_NANOVG_API                                   \
      {                                                        \
        GL_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)      \
      },                                                       \
      NANOVG_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_NANOVG_API                                   \
      {                                                        \
        GL_API_TABLE(GUI_GENERATE_NULL)                        \
      },                                                       \
      NANOVG_API_TABLE(GUI_GENERATE_NULL)
#endif

/* emDoc stop */
//
// Legacy macro
//
#define GUI_SVG_DECLARE_NANOVG_API(VAR_NAME) \
  static const GUI_GPU_NANOVG_API_STRUCT VAR_NAME = { GUI_GPU_NANOVG_API }

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_NEMAVG_H */

/*************************** End of file ****************************/
