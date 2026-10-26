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
File        : GUI_GPU_NemaVG.h
Purpose     : NemaVG GPU driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_NEMAVG_H
#define GUI_GPU_NEMAVG_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"

/*********************************************************************
*
*       Use legacy definitions from SVG driver
*
**********************************************************************
*/
#ifdef    GUI_SVG_NEMA_VG_HEADER
  #define GUI_GPU_NEMA_VG_HEADER           GUI_SVG_NEMA_VG_HEADER
#endif

#ifdef    GUI_SVG_NEMA_VG_VERSION_HEADER
  #define GUI_GPU_NEMA_VG_VERSION_HEADER   GUI_SVG_NEMA_VG_VERSION_HEADER
#endif

#ifdef    GUI_SVG_HAS_NEMAVG
  #define GUI_GPU_HAS_NEMAVG
#endif

/*********************************************************************
*
*       NemaVG header files.
*
**********************************************************************
*/
#ifndef   GUI_GPU_NEMA_VG_HEADER
  #define GUI_GPU_NEMA_VG_HEADER           <nema_vg.h>
#endif

#ifndef   GUI_GPU_NEMA_VG_VERSION_HEADER
  #define GUI_GPU_NEMA_VG_VERSION_HEADER   <nema_vg_version.h>
#endif

#ifdef GUI_GPU_HAS_NEMAVG
  #include GUI_GPU_NEMA_VG_HEADER
  #include GUI_GPU_NEMA_VG_VERSION_HEADER
#endif

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       API version
*
**********************************************************************
*/
//
// API versions
//
#define NEMA_VG_VERSION_1_1_5   (0x010105)
#define NEMA_VG_VERSION_1_1_7   (0x010107)

#ifndef GUI_GPU_HAS_NEMAVG
  //
  // NemaVG API version
  //
  #define NEMA_VG_API_VERSION            NEMA_VG_VERSION_1_1_5    // Minimum v1.1.5 required
#endif

/*********************************************************************
*
*       Type definitions
*
**********************************************************************
*/
#ifndef GUI_GPU_HAS_NEMAVG
  //
  // Handle types
  //
  #define NEMA_VG_HANDLE         void *
  #define NEMA_VG_PATH_HANDLE    NEMA_VG_HANDLE
  #define NEMA_VG_PAINT_HANDLE   NEMA_VG_HANDLE
  #define NEMA_VG_GRAD_HANDLE    NEMA_VG_HANDLE
  //
  // General types
  //
  typedef uint32_t nema_tex_format_t;
  typedef float    nema_vg_float_t;
  typedef uint8_t  nema_tex_mode_t;
  typedef float    nema_matrix3x3_t[3][3];
  //
  // Define public struct types.
  //
  GUI_DEFINE_STRUCT_TYPE(color_var_t,
    float r;
    float g;
    float b;
    float a;
  );
  GUI_DEFINE_STRUCT_TYPE(nema_buffer_t,
    int        size;      
    int        fd;        
    void     * base_virt; 
    uintptr_t  base_phys; 
  );
  GUI_DEFINE_STRUCT_TYPE(nema_cmdlist_t,
    nema_buffer_t bo;
    int           size;                     
    int           offset;                   
    uint32_t      flags;               
    int32_t       submission_id;
    void        * next; 
    void        * root; 
  );
  GUI_DEFINE_STRUCT_TYPE(nema_ringbuffer_t,
    nema_buffer_t bo;
    int           offset;
    int	          last_submission_id;
  );
  GUI_DEFINE_STRUCT_TYPE(nema_img_obj_t,
    nema_buffer_t bo;
    uint16_t      w;
    uint16_t      h;
    int           stride;
    uint32_t      color;
    uint8_t       format;
    uint8_t       sampling_mode;
  );
#endif

/*********************************************************************
*
*       Enums
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_NEMAVG_IRQ_INDEX
*
*  Description
*    Index of NemaVG interrupt handler, passed as parameter
*    to GUI_GPU_IRQHandler().
*/
typedef enum {
  GUI_GPU_NEMAVG_IRQ,        // Interrupt handler for main NemaVG interrupt.
  GUI_GPU_NEMAVG_ERROR_IRQ,  // Interrupt handler for NemaVG error interrupt.
} GUI_GPU_NEMAVG_IRQ_INDEX;

/*********************************************************************
*
*       API function table
*
**********************************************************************
*/
//
// Lists of function arguments
//
#define _NEMA_SETCLIP_ARGS(ARG)              ARG(int32_t, x), ARG(int32_t, y), ARG(uint32_t, w), ARG(uint32_t, h)
#define _NEMA_BINDDSTTEX_ARGS(ARG)           ARG(uintptr_t, baseaddr_phys), ARG(uint32_t, width), ARG(uint32_t, height), ARG(nema_tex_format_t, format), ARG(int32_t, stride)
#define _NEMA_SETBLENDFILL_ARGS(ARG)         ARG(uint32_t, blending_mode)
#define _NEMA_CLCREATESIZED_ARGS(ARG)        ARG(int, size_bytes)
#define _NEMA_CLBINDCIRCULAR_ARGS(ARG)       ARG(nema_cmdlist_t *, cl)
#define _NEMA_CLSUBMIT_ARGS(ARG)             ARG(nema_cmdlist_t *, cl)
#define _NEMA_CLWAIT_ARGS(ARG)               ARG(nema_cmdlist_t *, cl)
#define _NEMA_CLREWIND_ARGS(ARG)             ARG(nema_cmdlist_t *, cl)
#define _NEMA_CLDESTROY_ARGS(ARG)            ARG(nema_cmdlist_t *, cl)
#define _NEMA_VGINIT_ARGS(ARG)               ARG(int, width), ARG(int, height)
#define _NEMA_HANDLELARGECOORDS_ARGS(ARG)    ARG(uint8_t, enable), ARG(uint8_t, allow_internal_alloc)
#define _NEMA_SETBLEND_ARGS(ARG)             ARG(uint32_t, blend)
#define _NEMA_SETFILLRULE_ARGS(ARG)          ARG(uint8_t, fill_rule)
#define _NEMA_SETQUALITY_ARGS(ARG)           ARG(uint8_t, quality)
#define _NEMA_DRAWLINE_ARGS(ARG)             ARG(float, x1), ARG(float, y1), ARG(float, x2), ARG(float, y2), ARG(nema_matrix3x3_t, m), ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_DRAWPATH_ARGS(ARG)             ARG(NEMA_VG_PATH_HANDLE, path), ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_DRAWRECT_ARGS(ARG)             ARG(float, x), ARG(float, y), ARG(float, width), ARG(float, height), ARG(nema_matrix3x3_t, m), ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_DRAWROUNDEDRECT_ARGS(ARG)      ARG(float, x), ARG(float, y), ARG(float, width), ARG(float, height), ARG(float, rx), ARG(float, ry), ARG(nema_matrix3x3_t, m), ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_DRAWCIRCLE_ARGS(ARG)           ARG(float, cx), ARG(float, cy), ARG(float, r), ARG(nema_matrix3x3_t, m), ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_PATHSETMATRIX_ARGS(ARG)        ARG(NEMA_VG_PATH_HANDLE, path), ARG(nema_matrix3x3_t, m)
#define _NEMA_PATHSETSHAPE_ARGS(ARG)         ARG(NEMA_VG_PATH_HANDLE, path), ARG(const size_t, seg_size), ARG(const uint8_t *, seg), ARG(const size_t, data_size), ARG(const nema_vg_float_t *, data)
#define _NEMA_PATHDESTROY_ARGS(ARG)          ARG(NEMA_VG_PATH_HANDLE, path)
#define _NEMA_GRADSET_ARGS(ARG)              ARG(NEMA_VG_GRAD_HANDLE, grad), ARG(int, stops_count), ARG(float *, stops), ARG(color_var_t *, colors)
#define _NEMA_GRADDESTROY_ARGS(ARG)          ARG(NEMA_VG_GRAD_HANDLE, grad)
#define _NEMA_PAINTSETTYPE_ARGS(ARG)         ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(uint8_t, type)
#define _NEMA_PAINTSETGRADLINEAR_ARGS(ARG)   ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(NEMA_VG_GRAD_HANDLE, grad), ARG(float, x0), ARG(float, y0), ARG(float, x1), ARG(float, y1), ARG(nema_tex_mode_t, sampling_mode)
#define _NEMA_PAINTSETGRADRADIAL2_ARGS(ARG)  ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(NEMA_VG_GRAD_HANDLE, grad), ARG(float, x0), ARG(float, y0), ARG(float, rx), ARG(float, ry), ARG(nema_tex_mode_t, sampling_mode)
#define _NEMA_PAINTSETOPACITY_ARGS(ARG)      ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(float, opacity)
#define _NEMA_PAINTSETPAINTCOLOR_ARGS(ARG)   ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(uint32_t, rgba)
#define _NEMA_PAINTSETSTROKEWIDTH_ARGS(ARG)  ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(float, stroke_width)
#define _NEMA_PAINTSETTEX_ARGS(ARG)          ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(nema_img_obj_t *, tex)
#define _NEMA_PAINTSETTEXMATRIX_ARGS(ARG)    ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(nema_matrix3x3_t, m)
#define _NEMA_PAINTDESTROY_ARGS(ARG)         ARG(NEMA_VG_PAINT_HANDLE, paint)
#define _NEMA_STROKESETCAPSTYLE_ARGS(ARG)    ARG(uint8_t, start_cap_style), ARG(uint8_t, end_cap_style)
#define _NEMA_STROKESETJOINSTYLE_ARGS(ARG)   ARG(uint8_t, join_style)
#define _NEMA_STROKESETMITERLIMIT_ARGS(ARG)  ARG(float, MiterLimit)
#define _NEMA_DRAWRINGGENERIC_ARGS(ARG)      ARG(float, cx), ARG(float, cy), ARG(float, ring_radius), ARG(float, angle_start), ARG(float, angle_end), ARG(NEMA_VG_PAINT_HANDLE, paint), ARG(uint8_t, has_caps)
#define _NEMA_RBINIT_ARGS(ARG)               ARG(nema_ringbuffer_t *, rb), ARG(int, reset)
//
// Return types
//
#define _NEMA_CMDLIST_RET(ARG)               GUI_RET_TYPE(nema_cmdlist_t,       ARG)
#define _NEMA_PATH_RET(ARG)                  GUI_RET_TYPE(NEMA_VG_PATH_HANDLE,  ARG)
#define _NEMA_GRAD_RET(ARG)                  GUI_RET_TYPE(NEMA_VG_GRAD_HANDLE,  ARG)
#define _NEMA_PAINT_RET(ARG)                 GUI_RET_TYPE(NEMA_VG_PAINT_HANDLE, ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN, RETURN_TYPE, ARG_LIST)
//
#define NEMAVG_API_TABLE(FX)                                                                 \
  FX(nema_init,                      GUI_RETURN(int),      GUI_NO_ARGS                     ) \
  FX(nema_set_clip,                  GUI_RETURN(void),     _NEMA_SETCLIP_ARGS              ) \
  FX(nema_bind_dst_tex,              GUI_RETURN(void),     _NEMA_BINDDSTTEX_ARGS           ) \
  FX(nema_set_blend_fill,            GUI_RETURN(void),     _NEMA_SETBLENDFILL_ARGS         ) \
  FX(nema_cl_create_sized,           _NEMA_CMDLIST_RET,    _NEMA_CLCREATESIZED_ARGS        ) \
  FX(nema_cl_bind_circular,          GUI_RETURN(void),     _NEMA_CLBINDCIRCULAR_ARGS       ) \
  FX(nema_cl_submit,                 GUI_RETURN(void),     _NEMA_CLSUBMIT_ARGS             ) \
  FX(nema_cl_wait,                   GUI_RETURN(int),      _NEMA_CLWAIT_ARGS               ) \
  FX(nema_cl_rewind,                 GUI_RETURN(void),     _NEMA_CLREWIND_ARGS             ) \
  FX(nema_cl_destroy,                GUI_RETURN(void),     _NEMA_CLDESTROY_ARGS            ) \
  FX(nema_vg_init,                   GUI_RETURN(void),     _NEMA_VGINIT_ARGS               ) \
  FX(nema_vg_deinit,                 GUI_RETURN(void),     GUI_NO_ARGS                     ) \
  FX(nema_vg_handle_large_coords,    GUI_RETURN(void),     _NEMA_HANDLELARGECOORDS_ARGS    ) \
  FX(nema_vg_set_blend,              GUI_RETURN(void),     _NEMA_SETBLEND_ARGS             ) \
  FX(nema_vg_set_fill_rule,          GUI_RETURN(void),     _NEMA_SETFILLRULE_ARGS          ) \
  FX(nema_vg_set_quality,            GUI_RETURN(void),     _NEMA_SETQUALITY_ARGS           ) \
  FX(nema_vg_draw_line,              GUI_RETURN(uint32_t), _NEMA_DRAWLINE_ARGS             ) \
  FX(nema_vg_draw_path,              GUI_RETURN(uint32_t), _NEMA_DRAWPATH_ARGS             ) \
  FX(nema_vg_draw_rect,              GUI_RETURN(uint32_t), _NEMA_DRAWRECT_ARGS             ) \
  FX(nema_vg_draw_rounded_rect,      GUI_RETURN(uint32_t), _NEMA_DRAWROUNDEDRECT_ARGS      ) \
  FX(nema_vg_draw_circle,            GUI_RETURN(uint32_t), _NEMA_DRAWCIRCLE_ARGS           ) \
  FX(nema_vg_path_create,            _NEMA_PATH_RET,       GUI_NO_ARGS                     ) \
  FX(nema_vg_path_set_matrix,        GUI_RETURN(void),     _NEMA_PATHSETMATRIX_ARGS        ) \
  FX(nema_vg_path_set_shape,         GUI_RETURN(void),     _NEMA_PATHSETSHAPE_ARGS         ) \
  FX(nema_vg_path_destroy,           GUI_RETURN(void),     _NEMA_PATHDESTROY_ARGS          ) \
  FX(nema_vg_grad_create,            _NEMA_GRAD_RET,       GUI_NO_ARGS                     ) \
  FX(nema_vg_grad_set,               GUI_RETURN(void),     _NEMA_GRADSET_ARGS              ) \
  FX(nema_vg_grad_destroy,           GUI_RETURN(void),     _NEMA_GRADDESTROY_ARGS          ) \
  FX(nema_vg_paint_create,           _NEMA_PAINT_RET,      GUI_NO_ARGS                     ) \
  FX(nema_vg_paint_set_type,         GUI_RETURN(void),     _NEMA_PAINTSETTYPE_ARGS         ) \
  FX(nema_vg_paint_set_grad_linear,  GUI_RETURN(void),     _NEMA_PAINTSETGRADLINEAR_ARGS   ) \
  FX(nema_vg_paint_set_grad_radial2, GUI_RETURN(void),     _NEMA_PAINTSETGRADRADIAL2_ARGS  ) \
  FX(nema_vg_paint_set_opacity,      GUI_RETURN(void),     _NEMA_PAINTSETOPACITY_ARGS      ) \
  FX(nema_vg_paint_set_paint_color,  GUI_RETURN(void),     _NEMA_PAINTSETPAINTCOLOR_ARGS   ) \
  FX(nema_vg_paint_set_stroke_width, GUI_RETURN(void),     _NEMA_PAINTSETSTROKEWIDTH_ARGS  ) \
  FX(nema_vg_paint_set_tex,          GUI_RETURN(void),     _NEMA_PAINTSETTEX_ARGS          ) \
  FX(nema_vg_paint_set_tex_matrix,   GUI_RETURN(void),     _NEMA_PAINTSETTEXMATRIX_ARGS    ) \
  FX(nema_vg_paint_destroy,          GUI_RETURN(void),     _NEMA_PAINTDESTROY_ARGS         ) \
  FX(nema_rb_init,                   GUI_RETURN(int),      _NEMA_RBINIT_ARGS               )

//
// API extensions with V1.1.7
//
#define NEMAVG_API_TABLE_1_1_7(FX)                                                           \
  FX(nema_vg_stroke_set_cap_style,   GUI_RETURN(void),     _NEMA_STROKESETCAPSTYLE_ARGS    ) \
  FX(nema_vg_stroke_set_join_style,  GUI_RETURN(void),     _NEMA_STROKESETJOINSTYLE_ARGS   ) \
  FX(nema_vg_stroke_set_miter_limit, GUI_RETURN(void),     _NEMA_STROKESETMITERLIMIT_ARGS  ) \
  FX(nema_vg_draw_ring_generic,      GUI_RETURN(uint32_t), _NEMA_DRAWRINGGENERIC_ARGS      )

//
// Make macro templates for API table
//
#define NEMAVG_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(NEMAVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NEMAVG_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(NEMAVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NEMAVG_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(NEMAVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NEMAVG_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(NEMAVG, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       OSA API function table
*
**********************************************************************
*/
#define _OSA_WAIT_IRQ_CL_ARGS(ARG)         ARG(int, cl_id)                   
#define _OSA_WAIT_IRQ_BRK_ARGS(ARG)        ARG(int, brk_id)                  
#define _OSA_REG_READ_ARGS(ARG)            ARG(uint32_t, reg)                
#define _OSA_REG_WRITE_ARGS(ARG)           ARG(uint32_t, reg), ARG(uint32_t, value)
#define _OSA_BUFFER_CREATE_ARGS(ARG)       ARG(int, size)                    
#define _OSA_BUFFER_CREATE_POOL_ARGS(ARG)  ARG(int, pool), ARG(int, size)          
#define _OSA_BUFFER_MAP_ARGS(ARG)          ARG(nema_buffer_t *, bo)          
#define _OSA_BUFFER_UNMAP_ARGS(ARG)        ARG(nema_buffer_t *, bo)          
#define _OSA_BUFFER_DESTROY_ARGS(ARG)      ARG(nema_buffer_t *, bo)          
#define _OSA_BUFFER_PHYS_ARGS(ARG)         ARG(nema_buffer_t *, bo)          
#define _OSA_BUFFER_FLUSH_ARGS(ARG)        ARG(nema_buffer_t *, bo)          
#define _OSA_HOST_MALLOC_ARGS(ARG)         ARG(unsigned, size)               
#define _OSA_HOST_FREE_ARGS(ARG)           ARG(void *, ptr)                  
#define _OSA_MUTEX_LOCK_ARGS(ARG)          ARG(int, mutex_id)                
#define _OSA_MUTEX_UNLOCK_ARGS(ARG)        ARG(int, mutex_id)               
//
// Return types
//
#define _OSA_BUFFER_RET(ARG)     GUI_RET_TYPE(nema_buffer_t, ARG)
#define _OSA_VOIDP_RET(ARG)      GUI_RET_TYPE(void*,         ARG)

//
// Prototype is
// FX(FUNC_NAME, RETURN_STATEMENT, ARG_LIST)
//
#define NEMAVG_OSA_API_TABLE(FX)                                                       \
  FX(nema_sys_init,             GUI_RETURN(int32_t),    GUI_NO_ARGS                  ) \
  FX(nema_wait_irq,             GUI_RETURN(int),        GUI_NO_ARGS                  ) \
  FX(nema_wait_irq_cl,          GUI_RETURN(int),        _OSA_WAIT_IRQ_CL_ARGS        ) \
  FX(nema_wait_irq_brk,         GUI_RETURN(int),        _OSA_WAIT_IRQ_BRK_ARGS       ) \
  FX(nema_reg_read,             GUI_RETURN(uint32_t),   _OSA_REG_READ_ARGS           ) \
  FX(nema_reg_write,            GUI_RETURN(void),       _OSA_REG_WRITE_ARGS          ) \
  FX(nema_buffer_create,        _OSA_BUFFER_RET,        _OSA_BUFFER_CREATE_ARGS      ) \
  FX(nema_buffer_create_pool,   _OSA_BUFFER_RET,        _OSA_BUFFER_CREATE_POOL_ARGS ) \
  FX(nema_buffer_map,           _OSA_VOIDP_RET,         _OSA_BUFFER_MAP_ARGS         ) \
  FX(nema_buffer_unmap,         GUI_RETURN(void),       _OSA_BUFFER_UNMAP_ARGS       ) \
  FX(nema_buffer_destroy,       GUI_RETURN(void),       _OSA_BUFFER_DESTROY_ARGS     ) \
  FX(nema_buffer_phys,          GUI_RETURN(uintptr_t),  _OSA_BUFFER_PHYS_ARGS        ) \
  FX(nema_buffer_flush,         GUI_RETURN(void),       _OSA_BUFFER_FLUSH_ARGS       ) \
  FX(nema_host_malloc,          _OSA_VOIDP_RET,         _OSA_HOST_MALLOC_ARGS        ) \
  FX(nema_host_free,            GUI_RETURN(void),       _OSA_HOST_FREE_ARGS          ) \
  FX(nema_mutex_lock,           GUI_RETURN(int),        _OSA_MUTEX_LOCK_ARGS         ) \
  FX(nema_mutex_unlock,         GUI_RETURN(int),        _OSA_MUTEX_UNLOCK_ARGS       )

#define NEMAVG_PLATFORM_API_TABLE(FX)                                                  \
  FX(platform_disable_cache,    GUI_RETURN(void),       GUI_NO_ARGS                  ) \
  FX(platform_invalidate_cache, GUI_RETURN(void),       GUI_NO_ARGS                  )


//
// Make macro templates for API table
//
#define _NEMAVG_OSA_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN, RETURN_TYPE, CALL_ARGS, TYPE_ARGS) \
  RETURN_TYPE FUNC_NAME TYPE_ARGS { RETURN NEMAVG_OSA_API->FUNC_NAME CALL_ARGS ; }

#define _GENERATE_PROTOTYPE(FUNC_NAME, RETURN, RETURN_TYPE, CALL_ARGS, TYPE_ARGS) \
  RETURN_TYPE FUNC_NAME TYPE_ARGS;

#define NEMAVG_OSA_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  _NEMAVG_OSA_GENERATE_WRAPPER_FUNC(FUNC_NAME, _GENERATE_RET_KEYWORD(RETURN_STATEMENT), _GENERATE_RET_TYPE(RETURN_STATEMENT),                \
                                   _GENERATE_CALL_ARGS(ARG_LIST), _GENERATE_TYPE_ARGS(ARG_LIST))

#define NEMAVG_OSA_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(NEMAVG_OSA, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NEMAVG_OSA_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(NEMAVG_OSA, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define NEMAVG_OSA_GENERATE_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  _GENERATE_PROTOTYPE(FUNC_NAME, _GENERATE_RET_KEYWORD(RETURN_STATEMENT), _GENERATE_RET_TYPE(RETURN_STATEMENT),                           \
                      _GENERATE_CALL_ARGS(ARG_LIST), _GENERATE_TYPE_ARGS(ARG_LIST))

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
//
// OSA layer, always in GUI_GPU_NemaVG.c
//
#ifndef GUI_GPU_HAS_NEMAVG
  NEMAVG_OSA_API_TABLE(NEMAVG_OSA_GENERATE_PROTOTYPE)
#endif
//
// Platform layer, in GUI_GPU_NemaVG.c when needed by NemaVG.
//
#ifndef NEMA_VG_INVALIDATE_CACHE
  NEMAVG_PLATFORM_API_TABLE(NEMAVG_OSA_GENERATE_PROTOTYPE)
#endif

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
NEMAVG_API_TABLE(NEMAVG_GENERATE_API_FUNC_PROTOTYPE)
NEMAVG_API_TABLE_1_1_7(NEMAVG_GENERATE_API_FUNC_PROTOTYPE)
//
// OSA layer
//
NEMAVG_OSA_API_TABLE(NEMAVG_OSA_GENERATE_API_FUNC_PROTOTYPE)
NEMAVG_PLATFORM_API_TABLE(NEMAVG_OSA_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for NemaVG
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_NEMAVG_API_STRUCT
*
*  Description
*    Maps the required functions of the ThinkSilicon NemaVG API.
*
*    A structure of this type can be set with GUI_GPU_BindAPI()
*    when a precompiled emWin library is used, that was compiled without
*    the NemaVG code (meaning \c{GUI_GPU_HAS_NEMAVG} was not defined.
*
*    For more details about these functions, please refer to the NemaVG
*    API documentation.
*/
typedef struct {
  NEMAVG_API_TABLE(NEMAVG_GENERATE_API_STRUCT_MEMBER)
  NEMAVG_API_TABLE_1_1_7(NEMAVG_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_NEMAVG_API_STRUCT;

/*********************************************************************
*
*       GUI_GPU_NEMAVG_OSA_API_STRUCT
*
*  Description
*    Maps the required functions of the OSA layer of the ThinkSilicon
*    NemaVG API.
* 
*    The layer can be switched out via the configuration of the
*    driver (see GUI_GPU_NEMAVG_CONFIG).
* 
*    For more details about these functions and the OSA layer,
*    please refer to the NemaVG API documentation.
*/
typedef struct {
  NEMAVG_OSA_API_TABLE(NEMAVG_OSA_GENERATE_API_STRUCT_MEMBER)
  NEMAVG_PLATFORM_API_TABLE(NEMAVG_OSA_GENERATE_API_STRUCT_MEMBER)
  void (* cbCommandListCompleted)(uint32_t last_cl_id);  // Pointer to callback that is triggered when a command list has finished processing.
} GUI_GPU_NEMAVG_OSA_API_STRUCT;

/*********************************************************************
*
*       Config struct
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_NEMAVG_CONFIG
*
*  Description
*    Configures the NemaVG GPU driver when it is created.
*/
typedef struct {
  void                                * pGPU2D; // Pointer to the GPU2D/NemaVG register.
  const GUI_GPU_NEMAVG_OSA_API_STRUCT * pOSA;   // Optional pointer to custom NemaVG OSA layer implementation,
                                                // can be left as NULL to use the default implementation.
} GUI_GPU_NEMAVG_CONFIG;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
//
// These functions are only available since v1.1.7, make sure they are
// not referenced in GUI_GPU_DECLARE_NEMAVG_API if unavailable.
//
#if (NEMA_VG_API_VERSION < NEMA_VG_VERSION_1_1_7)
  #define NEMA_VG_FUNCTIONS_1_1_7         NEMAVG_API_TABLE_1_1_7(GUI_GENERATE_NULL)
#else
  #define NEMA_VG_FUNCTIONS_1_1_7         NEMAVG_API_TABLE_1_1_7(GUI_GENERATE_API_STRUCT_INITIALIZER)
#endif

/*********************************************************************
*
*       GUI_GPU_NEMAVG_API
*
*  Description
*    Macro to fill a structure of the type GUI_GPU_NEMAVG_API_STRUCT
*    with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_NEMAVG
  #define GUI_GPU_NEMAVG_API                                 \
    NEMAVG_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)    \
    NEMA_VG_FUNCTIONS_1_1_7
#else
  #define GUI_GPU_NEMAVG_API                                 \
      NEMAVG_API_TABLE(GUI_GENERATE_NULL)                    \
      NEMA_VG_FUNCTIONS_1_1_7
#endif

/* emDoc stop */
//
// Legacy macro
//
#define GUI_SVG_DECLARE_NEMAVG_API(VAR_NAME)   \
  static const GUI_GPU_NEMAVG_API_STRUCT VAR_NAME = { GUI_GPU_NEMAVG_API }


#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_NEMAVG_H */

/*************************** End of file ****************************/
