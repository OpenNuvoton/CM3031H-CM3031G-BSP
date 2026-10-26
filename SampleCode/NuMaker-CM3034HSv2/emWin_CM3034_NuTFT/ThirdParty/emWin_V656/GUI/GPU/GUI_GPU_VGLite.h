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
File        : GUI_GPU_VGLite.h
Purpose     : VGLite GPU driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_VGLITE_H
#define GUI_GPU_VGLITE_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Use legacy definitions from SVG driver
*
**********************************************************************
*/
#ifdef    GUI_SVG_VGLITE_HEADER
  #define GUI_GPU_VGLITE_HEADER     GUI_SVG_VGLITE_HEADER
#endif

#ifdef    GUI_SVG_VGLITE_OS_HEADER
  #define GUI_GPU_VGLITE_OS_HEADER  GUI_SVG_VGLITE_OS_HEADER
#endif

#ifdef    GUI_SVG_HAS_VGLITE
  #define GUI_GPU_HAS_VGLITE
#endif

/*********************************************************************
*
*       VGLite header files.
*
**********************************************************************
*/
#ifndef   GUI_GPU_VGLITE_HEADER
  #define GUI_GPU_VGLITE_HEADER      <vg_lite.h>
#endif

#ifndef   GUI_GPU_VGLITE_OS_HEADER
  #define GUI_GPU_VGLITE_OS_HEADER   <vg_lite_os.h>
#endif

#ifndef   GUI_GPU_VGLITE_HAL_HEADER
  #define GUI_GPU_VGLITE_HAL_HEADER  <vg_lite_hal.h>
#endif

#ifdef GUI_GPU_HAS_VGLITE
  #include GUI_GPU_VGLITE_HEADER
  #include GUI_GPU_VGLITE_OS_HEADER
  #include GUI_GPU_VGLITE_HAL_HEADER
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
#ifndef GUI_GPU_HAS_VGLITE
  //
  // Define primitive types.
  //
  typedef float     vg_lite_float_t;
  typedef uint32_t  vg_lite_color_t;
  //
  // Define enums
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_SUCCESS)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_INVALID_ARGUMENT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_OUT_OF_MEMORY)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NO_CONTEXT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_TIMEOUT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_OUT_OF_RESOURCES)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_GENERIC_IO)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NOT_SUPPORT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_MULTI_THREAD_FAIL)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ALREADY_EXISTS)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NOT_ALIGNED)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_error_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_S8)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_S16)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_S32)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_FP32)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_format_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_HIGH)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_UPPER)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_MEDIUM)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_LOW)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_quality_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_DRAW_FILL_PATH)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_DRAW_STROKE_PATH)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_DRAW_FILL_STROKE_PATH)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_draw_path_type_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_LINEAR)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_TILED)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_buffer_layout_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGBA8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGRA8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGBX8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGRX8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGB565)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGR565)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGBA4444)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGRA4444)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGRA5551)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_A4)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_A8)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_L8)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YUYV)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YUY2)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NV12)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ANV12)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_AYUY2)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YV12)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YV24)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YV16)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NV16)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YUY2_TILED)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NV12_TILED)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ANV12_TILED)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_AYUY2_TILED)
    GUI_DEFINE_ENUM_ID(VG_LITE_INDEX_1, 100)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_INDEX_2)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_INDEX_4)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_INDEX_8)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGBA2222)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BGRA2222)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ABGR2222)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ARGB2222)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ABGR4444)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ARGB4444)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ABGR8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ARGB8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ABGR1555)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RGBA5551)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_ARGB1555)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_XBGR8888)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_XRGB8888)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_buffer_format_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_IMAGE_OPAQUE)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_IMAGE_TRANSPARENT)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_buffer_transparency_mode_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_NONE)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_SRC_OVER)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_DST_OVER)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_SRC_IN)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_DST_IN)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_SCREEN)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_MULTIPLY)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_ADDITIVE)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_BLEND_SUBTRACT)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_blend_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NORMAL_IMAGE_MODE)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_NONE_IMAGE_MODE)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_MULTIPLY_IMAGE_MODE)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_buffer_image_mode_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_CAP_BUTT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_CAP_ROUND)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_CAP_SQUARE)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_cap_style_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_JOIN_MITER)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_JOIN_ROUND)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_JOIN_BEVEL)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_join_style_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_FILL_NON_ZERO)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_FILL_EVEN_ODD)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_fill_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RADIAL_GRADIENT_SPREAD_FILL)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RADIAL_GRADIENT_SPREAD_PAD)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RADIAL_GRADIENT_SPREAD_REPEAT)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_RADIAL_GRADIENT_SPREAD_REFLECT)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_radial_gradient_spreadmode_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_IM_INDEX_FORMAT)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_PE_PREMULTIPLY)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_BORDER_CULLING)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_RGBA2_FORMAT)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_QUALITY_8X)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_RADIAL_GRADIENT)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_LINEAR_GRADIENT_EXT)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_COLOR_KEY)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_BIT_VG_DITHER)
    GUI_DEFINE_ENUM_ID_AUTO(gcFEATURE_COUNT)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_feature_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID(VG_LITE_FILTER_POINT,     0)
    GUI_DEFINE_ENUM_ID(VG_LITE_FILTER_LINEAR,    0x10000)
    GUI_DEFINE_ENUM_ID(VG_LITE_FILTER_BI_LINEAR, 0x20000)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_filter_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_SWIZZLE_UV)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_SWIZZLE_VU)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_swizzle_t);
  //
  typedef enum {
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YUV601)
    GUI_DEFINE_ENUM_ID_AUTO(VG_LITE_YUV709)
  } GUI_DEFINE_ENUM_TYPE(vg_lite_yuv2rgb_t);
  //
  // Define structs with public members.
  //
  GUI_DEFINE_STRUCT_TYPE(vg_lite_matrix_t,
    float m[3][3];
  );
  #define MAX_COLOR_RAMP_STOPS            256
  GUI_DEFINE_STRUCT_TYPE(vg_lite_yuvinfo_t,
    vg_lite_swizzle_t   swizzle;      
    vg_lite_yuv2rgb_t   yuv2rgb;      
    uint32_t            uv_planar;    
    uint32_t            v_planar;     
    uint32_t            alpha_planar; 
    uint32_t            uv_stride;    
    uint32_t            v_stride;     
    uint32_t            alpha_stride; 
    uint32_t            uv_height;    
    uint32_t            v_height;     
    void *              uv_memory;    
    void *              v_memory;     
    void *              uv_handle;    
    void *              v_handle;     
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_buffer_t,
    int32_t                            width;
    int32_t                            height;
    int32_t                            stride;
    vg_lite_buffer_layout_t            tiled;
    vg_lite_buffer_format_t            format;
    void                             * handle;
    void                             * memory;
    uint32_t                           address;
    vg_lite_yuvinfo_t                  yuv;
    vg_lite_buffer_image_mode_t        image_mode;
    vg_lite_buffer_transparency_mode_t transparency_mode;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_color_ramp_t,
    vg_lite_float_t stop;
    vg_lite_float_t red;
    vg_lite_float_t green;
    vg_lite_float_t blue;
    vg_lite_float_t alpha;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_linear_gradient_parameter_t,
    vg_lite_float_t X0;
    vg_lite_float_t Y0;
    vg_lite_float_t X1;
    vg_lite_float_t Y1;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_radial_gradient_parameter_t,
    vg_lite_float_t cx;
    vg_lite_float_t cy;
    vg_lite_float_t r;
    vg_lite_float_t fx;
    vg_lite_float_t fy;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_linear_gradient_ext_t,
    uint32_t                             count;            
    vg_lite_matrix_t                     matrix;   
    vg_lite_buffer_t                     image;    
    vg_lite_linear_gradient_parameter_t  linear_gradient;     
    uint32_t                             vg_color_ramp_length;  
    vg_lite_color_ramp_t                 vg_color_ramp[MAX_COLOR_RAMP_STOPS];
    uint32_t                             int_color_ramp_length;   
    vg_lite_color_ramp_t                 int_color_ramp[MAX_COLOR_RAMP_STOPS + 2];
    uint8_t                              color_ramp_premultiplied;
    vg_lite_radial_gradient_spreadmode_t spread_mode; 
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_radial_gradient_t,
    uint32_t                             count;         
    vg_lite_matrix_t                     matrix;
    vg_lite_buffer_t                     image; 
    vg_lite_radial_gradient_parameter_t  radialGradient;
    vg_lite_radial_gradient_spreadmode_t SpreadMode;   
    uint32_t                             vgColorRampLength;       
    vg_lite_color_ramp_t                 vgColorRamp[MAX_COLOR_RAMP_STOPS]; 
    uint32_t                             intColorRampLength; 
    vg_lite_color_ramp_t                 intColorRamp[MAX_COLOR_RAMP_STOPS + 2];
    uint8_t                              colorRampPremultiplied;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_hw_memory_t,
    void    * handle;  
    void    * memory;  
    uint32_t  address; 
    uint32_t  bytes;   
    uint32_t  property;
  );
  GUI_DEFINE_STRUCT_TYPE(vg_lite_path_t,
    vg_lite_float_t          bounding_box[4];
    vg_lite_quality_t        quality;      
    vg_lite_format_t         format;        
    vg_lite_hw_memory_t      uploaded;   
    int32_t                  path_length;            
    void                   * path;                     
    int8_t                   path_changed;            
    int8_t                   pdata_internal;          
    void                   * stroke_conversion;
    vg_lite_draw_path_type_t path_type;            
    void                   * stroke_path_data;        
    int32_t                  stroke_path_size;      
    vg_lite_color_t          stroke_color; 
  );
#endif

/*********************************************************************
*
*       API function table
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _VGL_BLIT_ARGS(ARG)               ARG(vg_lite_buffer_t *, target), ARG(vg_lite_buffer_t *, source), ARG(vg_lite_matrix_t *, matrix), ARG(vg_lite_blend_t, blend), ARG(vg_lite_color_t, color), ARG(vg_lite_filter_t, filter)
#define _VGL_DRAWPATH_ARGS(ARG)           ARG(vg_lite_buffer_t *, target), ARG(vg_lite_path_t *, path), ARG(vg_lite_fill_t, fill_rule),  ARG(vg_lite_matrix_t *, matrix), ARG(vg_lite_blend_t, blend), ARG(vg_lite_color_t, color)
#define _VGL_INITPATH_ARGS(ARG)           ARG(vg_lite_path_t *, path), ARG(vg_lite_format_t, data_format), ARG(vg_lite_quality_t, quality), ARG(uint32_t, path_length), ARG(void *, path_data), ARG(vg_lite_float_t, min_x),  ARG(vg_lite_float_t, min_y), ARG(vg_lite_float_t, max_x),  ARG(vg_lite_float_t, max_y)
#define _VGL_CLEARPATH_ARGS(ARG)          ARG(vg_lite_path_t *, path)
#define _VGL_SETPATHDRAWTYPE_ARGS(ARG)    ARG(vg_lite_path_t *, path), ARG(vg_lite_draw_path_type_t, path_type)
#define _VGL_SETSTROKE_ARGS(ARG)          ARG(vg_lite_path_t *, path), ARG(vg_lite_cap_style_t, stroke_cap_style), ARG(vg_lite_join_style_t, stroke_join_style), ARG(vg_lite_float_t, stroke_line_width), ARG(vg_lite_float_t, stroke_miter_limit), ARG(vg_lite_float_t *, stroke_dash_pattern), ARG(uint32_t, stroke_dash_pattern_count), ARG(vg_lite_float_t, stroke_dash_phase), ARG(vg_lite_color_t, stroke_color)
#define _VGL_UPDATESTROKE_ARGS(ARG)       ARG(vg_lite_path_t *, path)
#define _VGL_SETLINGRAD_ARGS(ARG)         ARG(vg_lite_linear_gradient_ext_t *, grad), ARG(uint32_t, count), ARG(vg_lite_color_ramp_t *, vg_color_ramp), ARG(vg_lite_linear_gradient_parameter_t, linear_gradient),  ARG(vg_lite_radial_gradient_spreadmode_t, spread_mode),  ARG(uint8_t, color_ramp_premultiplied)
#define _VGL_UPDATELINGRAD_ARGS(ARG)      ARG(vg_lite_linear_gradient_ext_t *, grad)
#define _VGL_GETLINGRADMATRIX_ARGS(ARG)   ARG(vg_lite_linear_gradient_ext_t *, grad)
#define _VGL_DRAWLINGRAD_ARGS(ARG)        ARG(vg_lite_buffer_t *, target), ARG(vg_lite_path_t *, path),  ARG(vg_lite_fill_t, fill_rule), ARG(vg_lite_matrix_t *, path_matrix), ARG(vg_lite_linear_gradient_ext_t *, grad), ARG(vg_lite_color_t, paint_color), ARG(vg_lite_blend_t, blend), ARG(vg_lite_filter_t, filter)
#define _VGL_CLEARLINGRAD_ARGS(ARG)       ARG(vg_lite_linear_gradient_ext_t *, grad)
#define _VGL_SETRADGRAD_ARGS(ARG)         ARG(vg_lite_radial_gradient_t *, grad), ARG(uint32_t, count), ARG(vg_lite_color_ramp_t *, vgColorRamp), ARG(vg_lite_radial_gradient_parameter_t, radialGradient), ARG(vg_lite_radial_gradient_spreadmode_t, spreadMode), ARG(uint8_t, colorRampPremultiplied)
#define _VGL_UPDATERADGRAD_ARGS(ARG)      ARG(vg_lite_radial_gradient_t *, grad)
#define _VGL_GETRADGRADMATRIX_ARGS(ARG)   ARG(vg_lite_radial_gradient_t *, grad)
#define _VGL_DRAWRADGRAD_ARGS(ARG)        ARG(vg_lite_buffer_t *, target), ARG(vg_lite_path_t *, path), ARG(vg_lite_fill_t, fill_rule), ARG(vg_lite_matrix_t *, path_matrix), ARG(vg_lite_radial_gradient_t *, grad), ARG(vg_lite_color_t, paint_color), ARG(vg_lite_blend_t, blend), ARG(vg_lite_filter_t, filter)
#define _VGL_CLEARRADGRAD_ARGS(ARG)       ARG(vg_lite_radial_gradient_t *, grad)
#define _VGL_SETSCISSOR_ARGS(ARG)         ARG(int32_t, x), ARG(int32_t, y), ARG(int32_t, width), ARG(int32_t, height)
#define _VGL_QUERY_FEATURE_ARGS(ARG)      ARG(vg_lite_feature_t, feature)
#define _VGL_INIT_ARGS(ARG)               ARG(int32_t, tessellation_width), ARG(int32_t, tessellation_height)
#define _VGL_INITMEM_ARGS(ARG)            ARG(uint32_t, register_mem_base), ARG(uint32_t, gpu_mem_base), ARG(volatile void *, contiguous_mem_base), ARG(uint32_t, contiguous_mem_size)
#define _VGL_SETCMDBUFFERSIZE_ARGS(ARG)   ARG(uint32_t, size)
//
// Return types
//
#define _VGL_GENERIC_RET(ARG)     GUI_RET_TYPE(vg_lite_error_t,   ARG)
#define _VGL_MATRIX_RET(ARG)      GUI_RET_TYPE(vg_lite_matrix_t*, ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define VGLITE_API_TABLE(FX)                                                              \
  FX(vg_lite_blit,                     _VGL_GENERIC_RET,     _VGL_BLIT_ARGS             ) \
  FX(vg_lite_draw,                     _VGL_GENERIC_RET,     _VGL_DRAWPATH_ARGS         ) \
  FX(vg_lite_init_path,                _VGL_GENERIC_RET,     _VGL_INITPATH_ARGS         ) \
  FX(vg_lite_init_arc_path,            _VGL_GENERIC_RET,     _VGL_INITPATH_ARGS         ) \
  FX(vg_lite_clear_path,               _VGL_GENERIC_RET,     _VGL_CLEARPATH_ARGS        ) \
  FX(vg_lite_set_draw_path_type,       _VGL_GENERIC_RET,     _VGL_SETPATHDRAWTYPE_ARGS  ) \
  FX(vg_lite_set_stroke,               _VGL_GENERIC_RET,     _VGL_SETSTROKE_ARGS        ) \
  FX(vg_lite_update_stroke,            _VGL_GENERIC_RET,     _VGL_UPDATESTROKE_ARGS     ) \
  FX(vg_lite_set_linear_grad,          _VGL_GENERIC_RET,     _VGL_SETLINGRAD_ARGS       ) \
  FX(vg_lite_update_linear_grad,       _VGL_GENERIC_RET,     _VGL_UPDATELINGRAD_ARGS    ) \
  FX(vg_lite_get_linear_grad_matrix,   _VGL_MATRIX_RET,      _VGL_GETLINGRADMATRIX_ARGS ) \
  FX(vg_lite_draw_linear_gradient,     _VGL_GENERIC_RET,     _VGL_DRAWLINGRAD_ARGS      ) \
  FX(vg_lite_clear_linear_grad,        _VGL_GENERIC_RET,     _VGL_CLEARLINGRAD_ARGS     ) \
  FX(vg_lite_set_rad_grad,             _VGL_GENERIC_RET,     _VGL_SETRADGRAD_ARGS       ) \
  FX(vg_lite_update_rad_grad,          _VGL_GENERIC_RET,     _VGL_UPDATERADGRAD_ARGS    ) \
  FX(vg_lite_get_rad_grad_matrix,      _VGL_MATRIX_RET,      _VGL_GETRADGRADMATRIX_ARGS ) \
  FX(vg_lite_draw_radial_gradient,     _VGL_GENERIC_RET,     _VGL_DRAWRADGRAD_ARGS      ) \
  FX(vg_lite_clear_rad_grad,           _VGL_GENERIC_RET,     _VGL_CLEARRADGRAD_ARGS     ) \
  FX(vg_lite_enable_scissor,           _VGL_GENERIC_RET,     GUI_NO_ARGS                ) \
  FX(vg_lite_disable_scissor,          _VGL_GENERIC_RET,     GUI_NO_ARGS                ) \
  FX(vg_lite_set_scissor,              _VGL_GENERIC_RET,     _VGL_SETSCISSOR_ARGS       ) \
  FX(vg_lite_flush,                    _VGL_GENERIC_RET,     GUI_NO_ARGS                ) \
  FX(vg_lite_finish,                   _VGL_GENERIC_RET,     GUI_NO_ARGS                ) \
  FX(vg_lite_query_feature,            GUI_RETURN(uint32_t), _VGL_QUERY_FEATURE_ARGS    ) \
  FX(vg_lite_IRQHandler,               GUI_RETURN(void),     GUI_NO_ARGS                ) \
  FX(vg_lite_init,                     _VGL_GENERIC_RET,     _VGL_INIT_ARGS             ) \
  FX(vg_lite_init_mem,                 GUI_RETURN(void),     _VGL_INITMEM_ARGS          ) \
  FX(vg_lite_set_command_buffer_size,  _VGL_GENERIC_RET,     _VGL_SETCMDBUFFERSIZE_ARGS ) \
  FX(vg_lite_close,                    _VGL_GENERIC_RET,     GUI_NO_ARGS                )

//
// Make macro templates for API table
//
#define VGLITE_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(VGLITE, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define VGLITE_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(VGLITE, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define VGLITE_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(VGLITE, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define VGLITE_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(VGLITE, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
VGLITE_API_TABLE(VGLITE_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for VGLite
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_VGLITE_API_STRUCT
*
*  Description
*    Maps the required functions of the Vivante VGLite API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the VGLite code (meaning \c{GUI_GPU_HAS_VGLITE} was not defined.
*
*    For more details about what each function does, please refer to the VGLite
*    API documentation.
*/
typedef struct {
  VGLITE_API_TABLE(VGLITE_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_VGLITE_API_STRUCT;

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_VGLITE_API
* 
*  Description
*    Macro to fill a structure of the type GUI_GPU_VGLITE_API_STRUCT
*    filled with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_VGLITE
  #define GUI_GPU_VGLITE_API    VGLITE_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_VGLITE_API    VGLITE_API_TABLE(GUI_GENERATE_NULL)
#endif

/* emDoc stop */
//
// Legacy macros
//
#define GUI_SVG_DECLARE_VGLITE_API(VAR_NAME)  \
  static const GUI_GPU_VGLITE_API_STRUCT VAR_NAME = { GUI_GPU_VGLITE_API }

/*********************************************************************
*
*       Config struct
*
**********************************************************************
*/
/*********************************************************************
* 
*       GUI_GPU_VGLITE_CONFIG
* 
*  Description
*    Configures the VGLite GPU driver when it is created.
*/
typedef struct {
  void     * pGPU2D;              // Pointer to the GPU2D/VGLite register.
  unsigned   BufferSize;          // Buffer size in bytes for VGLite tessellation buffer. Pass 0 to use the default size of 16 MB.
  unsigned   CommandBufferSize;   // Command buffer size for VGLite, pass 0 to use the default size of 256 KB.
} GUI_GPU_VGLITE_CONFIG;

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_VGLITE_H */

/*************************** End of file ****************************/
