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
File        : GUI_GPU_ThorVG.h
Purpose     : ThorVG interface
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_THORVG_H
#define GUI_GPU_THORVG_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"
#include <stdbool.h>

/*********************************************************************
*
*       ThorVG header files.
*
**********************************************************************
*/
#ifndef   GUI_GPU_THORVG_CAPI_HEADER
  #define GUI_GPU_THORVG_CAPI_HEADER   <thorvg_capi.h>
#endif

#ifdef GUI_GPU_HAS_THORVG
  #include GUI_GPU_THORVG_CAPI_HEADER
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
#ifndef GUI_GPU_HAS_THORVG
  //
  // Define enums
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID(TVG_ENGINE_OPTION_NONE,               0)
    GUI_DEFINE_ENUM_ID(TVG_ENGINE_OPTION_DEFAULT,      (1 << 0))
    GUI_DEFINE_ENUM_ID(TVG_ENGINE_OPTION_SMART_RENDER, (1 << 1))
  } GUI_DEFINE_ENUM_TYPE(Tvg_Engine_Option);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_CAP_BUTT)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_CAP_ROUND)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_CAP_SQUARE)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Stroke_Cap);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_JOIN_MITER)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_JOIN_ROUND)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_JOIN_BEVEL)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Stroke_Join);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_FILL_PAD)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_FILL_REFLECT)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_STROKE_FILL_REPEAT)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Stroke_Fill);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_FILL_RULE_NON_ZERO)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_FILL_RULE_EVEN_ODD)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Fill_Rule);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_SUCCESS)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_INVALID_ARGUMENT)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_INSUFFICIENT_CONDITION)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_FAILED_ALLOCATION)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_MEMORY_CORRUPTION)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_RESULT_NOT_SUPPORTED)
    GUI_DEFINE_ENUM_ID(TVG_RESULT_UNKNOWN, 255)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Result);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(TVG_COLORSPACE_ABGR8888)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_COLORSPACE_ARGB8888)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_COLORSPACE_ABGR8888S)
    GUI_DEFINE_ENUM_ID_AUTO(TVG_COLORSPACE_ARGB8888S)
    GUI_DEFINE_ENUM_ID(TVG_COLORSPACE_UNKNOWN, 255)
  } GUI_DEFINE_ENUM_TYPE(Tvg_Colorspace);
  //
  // Define structs
  //
  GUI_DEFINE_STRUCT_TYPE(Tvg_Matrix,
    float e11;
    float e12;
    float e13;
    float e21;
    float e22;
    float e23;
    float e31;
    float e32;
    float e33;
  );
  GUI_DEFINE_STRUCT_TYPE(Tvg_Color_Stop,
    float   offset;
    uint8_t r;   
    uint8_t g;   
    uint8_t b;   
    uint8_t a;   
  );
  //
  // These types are only accessed through pointers here...
  //
  typedef void* Tvg_Canvas;
  typedef void* Tvg_Gradient;
  typedef void* Tvg_Paint;
#endif

/*********************************************************************
*
*       Config struct for ThorVG
*
**********************************************************************
*/
/*********************************************************************
* 
*       GUI_GPU_THORVG_RENDER_BACKEND
* 
*  Description
*    Render backend to be selected for ThorVG.
*/
typedef enum {
  GUI_GPU_THORVG_RENDER_BACKEND_SOFTWARE,  // CPU-based renderer.
  GUI_GPU_THORVG_RENDER_BACKEND_OPENGL,    // OpenGL-based renderer.
  GUI_GPU_THORVG_RENDER_BACKEND_WEBGPU,    // WebGPU-based renderer.
} GUI_GPU_THORVG_RENDER_BACKEND;

/*********************************************************************
* 
*       GUI_GPU_THORVG_CONFIG
* 
*  Description
*    Configures the ThorVG GPU driver when it is created.
*/
typedef struct {
  GUI_GPU_THORVG_RENDER_BACKEND RenderBackend;  // Render backend to be used for ThorVG.
} GUI_GPU_THORVG_CONFIG;

/*********************************************************************
*
*       API function table
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _TVG_SWCANVASCREATE_ARGS(ARG)           ARG(Tvg_Engine_Option, op)
#define _TVG_SWCANVASSETTARGET_ARGS(ARG)        ARG(Tvg_Canvas*, canvas), ARG(uint32_t*, buffer), ARG(uint32_t, stride), ARG(uint32_t, w), ARG(uint32_t, h), ARG(Tvg_Colorspace, cs)
#define _TVG_CANVASSETVIEWPORT_ARGS(ARG)        ARG(Tvg_Canvas*, canvas), ARG(int32_t, x), ARG(int32_t, y), ARG(int32_t, w), ARG(int32_t, h)
#define _TVG_CANVASUPDATE_ARGS(ARG)             ARG(Tvg_Canvas*, canvas)
#define _TVG_CANVASDRAW_ARGS(ARG)               ARG(Tvg_Canvas*, canvas), ARG(bool, clear)
#define _TVG_CANVASSYNC_ARGS(ARG)               ARG(Tvg_Canvas*, canvas)
#define _TVG_CANVASDESTROY_ARGS(ARG)            ARG(Tvg_Canvas*, canvas)
#define _TVG_CANVASPUSH_ARGS(ARG)               ARG(Tvg_Canvas*, canvas), ARG(Tvg_Paint*, paint)
#define _TVG_PAINTSETTRANSFORM_ARGS(ARG)        ARG(Tvg_Paint*, paint), ARG(const Tvg_Matrix*, m)
#define _TVG_SCENEPUSH_ARGS(ARG)                ARG(Tvg_Paint*, scene), ARG(Tvg_Paint*, paint)
#define _TVG_SCENEGAUSSBLUR_ARGS(ARG)           ARG(Tvg_Paint*, scene), ARG(double, sigma), ARG(int, direction), ARG(int, border), ARG(int, quality)
#define _TVG_SHAPEMOVETO_ARGS(ARG)              ARG(Tvg_Paint*, paint), ARG(float, x), ARG(float, y)
#define _TVG_SHAPELINETO_ARGS(ARG)              ARG(Tvg_Paint*, paint), ARG(float, x), ARG(float, y)
#define _TVG_SHAPECUBICTO_ARGS(ARG)             ARG(Tvg_Paint*, paint), ARG(float, cx1), ARG(float, cy1), ARG(float, cx2), ARG(float, cy2), ARG(float, x), ARG(float, y)
#define _TVG_SHAPECLOSE_ARGS(ARG)               ARG(Tvg_Paint*, paint)
#define _TVG_SHAPESETSTROKECOLOR_ARGS(ARG)      ARG(Tvg_Paint*, paint), ARG(uint8_t, r), ARG(uint8_t, g), ARG(uint8_t, b), ARG(uint8_t, a)
#define _TVG_SHAPESETSTROKEWIDTH_ARGS(ARG)      ARG(Tvg_Paint*, paint), ARG(float, width)
#define _TVG_SHAPESETSTROKEMITERLIMIT_ARGS(ARG) ARG(Tvg_Paint*, paint), ARG(float, miterlimit)
#define _TVG_SHAPESETSTROKEDASH_ARGS(ARG)       ARG(Tvg_Paint*, paint), ARG(const float*, dashPattern), ARG(uint32_t, cnt), ARG(float, offset)
#define _TVG_SHAPESETSTROKEJOIN_ARGS(ARG)       ARG(Tvg_Paint*, paint), ARG(Tvg_Stroke_Join, join)
#define _TVG_SHAPESETSTROKECAP_ARGS(ARG)        ARG(Tvg_Paint*, paint), ARG(Tvg_Stroke_Cap, cap)
#define _TVG_SHAPESETFILLCOLOR_ARGS(ARG)        ARG(Tvg_Paint*, paint), ARG(uint8_t, r), ARG(uint8_t, g), ARG(uint8_t, b), ARG(uint8_t, a)
#define _TVG_SHAPESETFILLRULE_ARGS(ARG)         ARG(Tvg_Paint*, paint), ARG(Tvg_Fill_Rule, rule)
#define _TVG_SHAPEAPPENDRECT_ARGS(ARG)          ARG(Tvg_Paint*, paint), ARG(float, x), ARG(float, y), ARG(float, w), ARG(float, h), ARG(float, rx), ARG(float, ry), ARG(bool, cw)
#define _TVG_SHAPEAPPENDCIRCLE_ARGS(ARG)        ARG(Tvg_Paint*, paint), ARG(float, cx), ARG(float, cy), ARG(float, rx), ARG(float, ry), ARG(bool, cw)
#define _TVG_LINEARGRADIENTSET_ARGS(ARG)        ARG(Tvg_Gradient*, grad), ARG(float, x1), ARG(float, y1), ARG(float, x2), ARG(float, y2)
#define _TVG_RADIALGRADIENTSET_ARGS(ARG)        ARG(Tvg_Gradient*, grad), ARG(float, cx), ARG(float, cy), ARG(float, r), ARG(float, fx), ARG(float, fy), ARG(float, fr)
#define _TVG_GRADIENTSETTRANSFORM_ARGS(ARG)     ARG(Tvg_Gradient*, grad), ARG(const Tvg_Matrix*, m)
#define _TVG_GRADIENTSETSPREAD_ARGS(ARG)        ARG(Tvg_Gradient*, grad), ARG(const Tvg_Stroke_Fill, spread)
#define _TVG_GRADIENTSETCOLORSTOPS_ARGS(ARG)    ARG(Tvg_Gradient*, grad), ARG(const Tvg_Color_Stop*, color_stop), ARG(uint32_t, cnt)
#define _TVG_GRADIENTDEL_ARGS(ARG)              ARG(Tvg_Gradient*, grad)
#define _TVG_SHAPESETGRADIENT_ARGS(ARG)         ARG(Tvg_Paint*, paint), ARG(Tvg_Gradient*, gradient)
#define _TVG_SHAPESETSTROKEGRADIENT_ARGS(ARG)   ARG(Tvg_Paint*, paint), ARG(Tvg_Gradient*, gradient)
#define _TVG_PICTURELOADRAW_ARGS(ARG)           ARG(Tvg_Paint*, paint), ARG(uint32_t*, data), ARG(uint32_t, w), ARG(uint32_t, h), ARG(Tvg_Colorspace, cs), ARG(bool, copy)
#define _TVG_ENGINEINIT_ARGS(ARG)               ARG(unsigned, threads)
//
// Return types
//
#define _TVG_GENERIC_RET(ARG)   GUI_RET_TYPE(Tvg_Result,     ARG)
#define _TVG_CANVAS_RET(ARG)    GUI_RET_TYPE(Tvg_Canvas*,    ARG)
#define _TVG_PAINT_RET(ARG)     GUI_RET_TYPE(Tvg_Paint*,     ARG)
#define _TVG_GRADIENT_RET(ARG)  GUI_RET_TYPE(Tvg_Gradient*,  ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN, RETURN_TYPE, ARG_LIST)
//
#define THORVG_API_TABLE(FX)                                                                      \
  FX(tvg_swcanvas_create,                 _TVG_CANVAS_RET,   _TVG_SWCANVASCREATE_ARGS           ) \
  FX(tvg_swcanvas_set_target,             _TVG_GENERIC_RET,  _TVG_SWCANVASSETTARGET_ARGS        ) \
  FX(tvg_canvas_set_viewport,             _TVG_GENERIC_RET,  _TVG_CANVASSETVIEWPORT_ARGS        ) \
  FX(tvg_canvas_update,                   _TVG_GENERIC_RET,  _TVG_CANVASUPDATE_ARGS             ) \
  FX(tvg_canvas_draw,                     _TVG_GENERIC_RET,  _TVG_CANVASDRAW_ARGS               ) \
  FX(tvg_canvas_sync,                     _TVG_GENERIC_RET,  _TVG_CANVASSYNC_ARGS               ) \
  FX(tvg_canvas_destroy,                  _TVG_GENERIC_RET,  _TVG_CANVASDESTROY_ARGS            ) \
  FX(tvg_canvas_push,                     _TVG_GENERIC_RET,  _TVG_CANVASPUSH_ARGS               ) \
  FX(tvg_paint_set_transform,             _TVG_GENERIC_RET,  _TVG_PAINTSETTRANSFORM_ARGS        ) \
  FX(tvg_scene_new,                       _TVG_PAINT_RET,    GUI_NO_ARGS                        ) \
  FX(tvg_scene_push,                      _TVG_GENERIC_RET,  _TVG_SCENEPUSH_ARGS                ) \
  FX(tvg_scene_push_effect_gaussian_blur, _TVG_GENERIC_RET,  _TVG_SCENEGAUSSBLUR_ARGS           ) \
  FX(tvg_shape_new,                       _TVG_PAINT_RET,    GUI_NO_ARGS                        ) \
  FX(tvg_shape_move_to,                   _TVG_GENERIC_RET,  _TVG_SHAPEMOVETO_ARGS              ) \
  FX(tvg_shape_line_to,                   _TVG_GENERIC_RET,  _TVG_SHAPELINETO_ARGS              ) \
  FX(tvg_shape_cubic_to,                  _TVG_GENERIC_RET,  _TVG_SHAPECUBICTO_ARGS             ) \
  FX(tvg_shape_close,                     _TVG_GENERIC_RET,  _TVG_SHAPECLOSE_ARGS               ) \
  FX(tvg_shape_set_stroke_color,          _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKECOLOR_ARGS      ) \
  FX(tvg_shape_set_stroke_width,          _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKEWIDTH_ARGS      ) \
  FX(tvg_shape_set_stroke_miterlimit,     _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKEMITERLIMIT_ARGS ) \
  FX(tvg_shape_set_stroke_dash,           _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKEDASH_ARGS       ) \
  FX(tvg_shape_set_stroke_join,           _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKEJOIN_ARGS       ) \
  FX(tvg_shape_set_stroke_cap,            _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKECAP_ARGS        ) \
  FX(tvg_shape_set_fill_color,            _TVG_GENERIC_RET,  _TVG_SHAPESETFILLCOLOR_ARGS        ) \
  FX(tvg_shape_set_fill_rule,             _TVG_GENERIC_RET,  _TVG_SHAPESETFILLRULE_ARGS         ) \
  FX(tvg_shape_append_rect,               _TVG_GENERIC_RET,  _TVG_SHAPEAPPENDRECT_ARGS          ) \
  FX(tvg_shape_append_circle,             _TVG_GENERIC_RET,  _TVG_SHAPEAPPENDCIRCLE_ARGS        ) \
  FX(tvg_linear_gradient_new,             _TVG_GRADIENT_RET, GUI_NO_ARGS                        ) \
  FX(tvg_radial_gradient_new,             _TVG_GRADIENT_RET, GUI_NO_ARGS                        ) \
  FX(tvg_linear_gradient_set,             _TVG_GENERIC_RET,  _TVG_LINEARGRADIENTSET_ARGS        ) \
  FX(tvg_radial_gradient_set,             _TVG_GENERIC_RET,  _TVG_RADIALGRADIENTSET_ARGS        ) \
  FX(tvg_gradient_set_transform,          _TVG_GENERIC_RET,  _TVG_GRADIENTSETTRANSFORM_ARGS     ) \
  FX(tvg_gradient_set_spread,             _TVG_GENERIC_RET,  _TVG_GRADIENTSETSPREAD_ARGS        ) \
  FX(tvg_gradient_set_color_stops,        _TVG_GENERIC_RET,  _TVG_GRADIENTSETCOLORSTOPS_ARGS    ) \
  FX(tvg_gradient_del,                    _TVG_GENERIC_RET,  _TVG_GRADIENTDEL_ARGS              ) \
  FX(tvg_shape_set_gradient,              _TVG_GENERIC_RET,  _TVG_SHAPESETGRADIENT_ARGS         ) \
  FX(tvg_shape_set_stroke_gradient,       _TVG_GENERIC_RET,  _TVG_SHAPESETSTROKEGRADIENT_ARGS   ) \
  FX(tvg_picture_new,                     _TVG_PAINT_RET,    GUI_NO_ARGS                        ) \
  FX(tvg_picture_load_raw,                _TVG_PAINT_RET,    _TVG_PICTURELOADRAW_ARGS           ) \
  FX(tvg_engine_init,                     _TVG_GENERIC_RET,  _TVG_ENGINEINIT_ARGS               ) \
  FX(tvg_engine_term,                     _TVG_GENERIC_RET,  GUI_NO_ARGS                        )

//
// Make macro templates for API table
//
#define THORVG_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(THORVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define THORVG_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(THORVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define THORVG_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(THORVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define THORVG_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(THORVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
THORVG_API_TABLE(THORVG_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for ThorVG
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_THORVG_API_STRUCT
*
*  Description
*    Maps the required functions of the ThorVG API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the ThorVG code (meaning \c{GUI_GPU_HAS_THORVG} was not defined.
*
*    For more details about what each function does, please refer to the ThorVG
*    API documentation.
*/
typedef struct {
  THORVG_API_TABLE(THORVG_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_THORVG_API_STRUCT;

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_THORVG_API
* 
*  Description
*    Macro to fill a structure of the type GUI_GPU_THORVG_API_STRUCT
*    filled with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_THORVG
  #define GUI_GPU_THORVG_API   THORVG_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_THORVG_API   THORVG_API_TABLE(GUI_GENERATE_NULL)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_THORVG_H */

/*************************** End of file ****************************/
