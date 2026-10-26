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
File        : GUI_GPU_Arm2D.h
Purpose     : Header of Arm2D GPU driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_GPU_ARM2D_H
#define GUI_GPU_ARM2D_H

#include "GUI.h"

#if (GUI_SUPPORT_GPU)

#include "GUI_GPU.h"
#include <stdbool.h>

/*********************************************************************
*
*       Arm2D header files
*
**********************************************************************
*/
#ifndef   GUI_GPU_ARM2D_HEADER
  #define GUI_GPU_ARM2D_HEADER    <arm_2d.h>
#endif

#ifdef GUI_GPU_HAS_ARM2D
  #include GUI_GPU_ARM2D_HEADER
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
#ifndef GUI_GPU_HAS_ARM2D
  GUI_DEFINE_UNION_TYPE(arm_2d_color_info_t,
    struct {
      uint8_t bHasAlpha       : 1; 
      uint8_t u3ColourSZ      : 3; 
      uint8_t bBigEndian      : 1; 
      uint8_t u2Variant       : 2;
      uint8_t                 : 1;
    };
    struct {
      uint8_t u7ColourFormat  : 7;
      uint8_t                 : 1;
    };
    uint8_t chScheme;
  );
  GUI_DEFINE_STRUCT_TYPE(arm_2d_location_t,
    int16_t iX;
    int16_t iY;
  );
  GUI_DEFINE_STRUCT_TYPE(arm_2d_size_t,
    int16_t iWidth; 
    int16_t iHeight;
  );
  GUI_DEFINE_STRUCT_TYPE(arm_2d_region_t,
    arm_2d_location_t tLocation;
    arm_2d_size_t     tSize;    
  );
  GUI_DEFINE_STRUCT_TYPE_EX(arm_2d_tile_t,
    struct {
      uint8_t             bIsRoot              : 1; 
      uint8_t             bHasEnforcedColour   : 1; 
      uint8_t             bDerivedResource     : 1; 
      uint8_t             bVirtualResource     : 1; 
      uint8_t             bVirtualScreen       : 1; 
      uint8_t             u3ExtensionID        : 3; 
      arm_2d_color_info_t tColourInfo;  
      union {
        uint16_t                    : 16;
        struct {
          uint8_t bIsNewFrame     : 1;
          uint8_t bIsDryRun       : 1;
        } PFB;
        struct {
          int16_t iTargetStride;
        } VRES;
      } Extension;
    } tInfo;
    arm_2d_region_t tRegion;
    union {
      arm_2d_tile_t * ptParent;       
      uint8_t       * pchBuffer;      
      uint16_t      * phwBuffer;      
      uint32_t      * pwBuffer;       
      intptr_t        nAddress;        
    };
  );
  //
  // macros for colour attributes
  //
  #define ARM_2D_M_COLOUR_SZ_1BIT             0   
  #define ARM_2D_M_COLOUR_SZ_2BIT             1   
  #define ARM_2D_M_COLOUR_SZ_4BIT             2   
  #define ARM_2D_M_COLOUR_SZ_8BIT             3   
  #define ARM_2D_M_COLOUR_SZ_16BIT            4   
  #define ARM_2D_M_COLOUR_SZ_32BIT            5   
  #define ARM_2D_M_COLOUR_SZ_24BIT            6   
  #define ARM_2D_M_COLOUR_SZ_1BIT_msk         (ARM_2D_M_COLOUR_SZ_1BIT << 1) 
  #define ARM_2D_M_COLOUR_SZ_2BIT_msk         (ARM_2D_M_COLOUR_SZ_2BIT << 1) 
  #define ARM_2D_M_COLOUR_SZ_4BIT_msk         (ARM_2D_M_COLOUR_SZ_4BIT << 1) 
  #define ARM_2D_M_COLOUR_SZ_8BIT_msk         (ARM_2D_M_COLOUR_SZ_8BIT << 1) 
  #define ARM_2D_M_COLOUR_SZ_16BIT_msk        (ARM_2D_M_COLOUR_SZ_16BIT<< 1) 
  #define ARM_2D_M_COLOUR_SZ_32BIT_msk        (ARM_2D_M_COLOUR_SZ_32BIT<< 1) 
  #define ARM_2D_M_COLOUR_SZ_24BIT_msk        (ARM_2D_M_COLOUR_SZ_24BIT<< 1) 
  #define ARM_2D_M_COLOUR_SZ_msk              (0x07 << 1),                   
  enum {
    ARM_2D_COLOUR_SZ_1BIT = 0,  
    ARM_2D_COLOUR_SZ_2BIT = 1,  
    ARM_2D_COLOUR_SZ_4BIT = 2,  
    ARM_2D_COLOUR_SZ_8BIT = 3,  
    ARM_2D_COLOUR_SZ_16BIT = 4, 
    ARM_2D_COLOUR_SZ_32BIT = 5, 
    ARM_2D_COLOUR_SZ_24BIT = 6, 
    ARM_2D_COLOUR_SZ_1BIT_msk   =   ARM_2D_COLOUR_SZ_1BIT << 1,
    ARM_2D_COLOUR_SZ_2BIT_msk   =   ARM_2D_COLOUR_SZ_2BIT << 1,
    ARM_2D_COLOUR_SZ_4BIT_msk   =   ARM_2D_COLOUR_SZ_4BIT << 1,
    ARM_2D_COLOUR_SZ_8BIT_msk   =   ARM_2D_COLOUR_SZ_8BIT << 1,
    ARM_2D_COLOUR_SZ_16BIT_msk  =   ARM_2D_COLOUR_SZ_16BIT<< 1,
    ARM_2D_COLOUR_SZ_32BIT_msk  =   ARM_2D_COLOUR_SZ_32BIT<< 1,
    ARM_2D_COLOUR_SZ_24BIT_msk  =   ARM_2D_COLOUR_SZ_24BIT<< 1,
    ARM_2D_COLOUR_SZ_msk        =   (0x07 << 1),
    ARM_2D_COLOUR_LITTLE_ENDIAN       = 0,
    ARM_2D_COLOUR_BIG_ENDIAN          = 1,
    ARM_2D_COLOUR_LITTLE_ENDIAN_msk   = ARM_2D_COLOUR_LITTLE_ENDIAN << 4,
    ARM_2D_COLOUR_BIG_ENDIAN_msk      = ARM_2D_COLOUR_BIG_ENDIAN    << 4,
    ARM_2D_COLOUR_NO_ALPHA = 0,
    ARM_2D_COLOUR_HAS_ALPHA = 1,
    ARM_2D_COLOUR_NO_ALPHA_msk        = ARM_2D_COLOUR_NO_ALPHA      << 0,
    ARM_2D_COLOUR_HAS_ALPHA_msk       = ARM_2D_COLOUR_HAS_ALPHA     << 0,
    ARM_2D_COLOUR_VARIANT_pos = 5,
    ARM_2D_COLOUR_VARIANT_msk         = 0x03 << ARM_2D_COLOUR_VARIANT_pos,
  };
  enum {
    ARM_2D_COLOUR_MONOCHROME  =   ARM_2D_COLOUR_SZ_1BIT_msk | ARM_2D_COLOUR_VARIANT_msk,
    ARM_2D_COLOUR_BIN         =   ARM_2D_COLOUR_MONOCHROME,
    ARM_2D_COLOUR_1BIT        =   ARM_2D_COLOUR_MONOCHROME,
    ARM_2D_COLOUR_MASK_A1     =   ARM_2D_COLOUR_MONOCHROME,
    ARM_2D_COLOUR_MASK_A2     =   ARM_2D_M_COLOUR_SZ_2BIT_msk,
    ARM_2D_COLOUR_MASK_A4     =   ARM_2D_M_COLOUR_SZ_4BIT_msk,
    ARM_2D_COLOUR_2BIT        =   ARM_2D_M_COLOUR_SZ_2BIT_msk,
    ARM_2D_COLOUR_4BIT        =   ARM_2D_M_COLOUR_SZ_4BIT_msk,
    ARM_2D_COLOUR_8BIT        =   ARM_2D_COLOUR_SZ_8BIT_msk,
    ARM_2D_COLOUR_GRAY8       =   ARM_2D_COLOUR_SZ_8BIT_msk,
    ARM_2D_COLOUR_MASK_A8     =   ARM_2D_COLOUR_SZ_8BIT_msk,
    ARM_2D_COLOUR_16BIT       =   ARM_2D_COLOUR_SZ_16BIT_msk,
    ARM_2D_COLOUR_RGB16       =   ARM_2D_COLOUR_SZ_16BIT_msk,
    ARM_2D_COLOUR_RGB565      =   ARM_2D_COLOUR_RGB16,
    ARM_2D_COLOUR_24BIT       =   ARM_2D_COLOUR_SZ_24BIT_msk        ,
    ARM_2D_COLOUR_RGB24       =   ARM_2D_COLOUR_SZ_24BIT_msk        ,
    ARM_2D_COLOUR_32BIT       =   ARM_2D_COLOUR_SZ_32BIT_msk        ,
    ARM_2D_COLOUR_RGB32       =   ARM_2D_COLOUR_SZ_32BIT_msk        ,
    ARM_2D_COLOUR_CCCN888     =   ARM_2D_COLOUR_RGB32               ,
    ARM_2D_COLOUR_CCCA8888    =   ARM_2D_COLOUR_SZ_32BIT_msk        |
    ARM_2D_COLOUR_HAS_ALPHA_msk       ,
    ARM_2D_COLOUR_RGB888      =   ARM_2D_COLOUR_CCCN888             ,
    ARM_2D_COLOUR_BGRA8888    =   ARM_2D_COLOUR_CCCA8888            ,
    ARM_2D_CHANNEL_8in32      =   ARM_2D_COLOUR_SZ_32BIT_msk        |
    ARM_2D_COLOUR_HAS_ALPHA_msk       |
    ARM_2D_COLOUR_VARIANT_msk   ,
  };
  typedef enum {
    arm_fsm_rt_err          = -1,
    arm_fsm_rt_cpl          = 0, 
    arm_fsm_rt_on_going     = 1, 
    arm_fsm_rt_wait_for_obj = 2, 
    arm_fsm_rt_async        = 3, 
    arm_fsm_rt_wait_for_res = 4, 
    __arm_fsm_rt_last,
  } GUI_DEFINE_ENUM_TYPE(arm_fsm_rt_t);
  GUI_DEFINE_UNION_TYPE(arm_2d_color_cccn888_t,
    uint32_t tValue;
    struct {
      uint8_t u8C[3];
      uint8_t : 8;
    };
  );
  //
  // Only used as pointer, can be void.
  //
  typedef void arm_2d_op_fill_cl_msk_opc_t;
  typedef void arm_2d_op_alpha_t;
  typedef void arm_2d_op_fill_cl_opc_t;
#endif

/*********************************************************************
*
*       API function table
*
**********************************************************************
*/
//
// Function arguments
//
#define _ARM2D_ALPHA_BITMAP_ARGS(ARG)   ARG(arm_2d_op_fill_cl_msk_opc_t *, ptOP), ARG(const arm_2d_tile_t *, ptTarget), ARG(const arm_2d_region_t *, ptRegion), ARG(const arm_2d_tile_t *, ptAlpha), ARG(arm_2d_color_cccn888_t, tColour), ARG(uint8_t, chOpacity)
#define _ARM2D_BITMAP_ARGS(ARG)         ARG(arm_2d_op_alpha_t *, ptOP), ARG(const arm_2d_tile_t *, ptSource), ARG(const arm_2d_tile_t *, ptTarget), ARG(const arm_2d_region_t *, ptRegion), ARG(uint_fast8_t, chRatio)
#define _ARM2D_FILL_ARGS(ARG)           ARG(arm_2d_op_fill_cl_opc_t *, ptOP), ARG(const arm_2d_tile_t *, ptTarget), ARG(const arm_2d_region_t *, ptRegion), ARG(arm_2d_color_cccn888_t, tColour), ARG(uint_fast8_t, chRatio)
#define _ARM2D_GEN_CHILD_ARGS(ARG)      ARG(const arm_2d_tile_t *, ptParentTile), ARG(const arm_2d_region_t *, ptRegion), ARG(arm_2d_tile_t *, ptOutput), ARG(bool, bClipRegion)
//
// Function return types
//
#define _ARM2D_GENERIC_RET(ARG)         GUI_RET_TYPE(arm_fsm_rt_t,   ARG)
#define _ARM2D_TILE_RET(ARG)            GUI_RET_TYPE(arm_2d_tile_t*, ARG)

#define ARM2D_API_TABLE(FX)                                                                                 \
  FX(arm_2d_init,                                          GUI_RETURN(void),      GUI_NO_ARGS)              \
  FX(arm_2dp_cccn888_fill_colour_with_a4_mask_and_opacity, _ARM2D_GENERIC_RET,    _ARM2D_ALPHA_BITMAP_ARGS) \
  FX(arm_2dp_cccn888_fill_colour_with_mask_and_opacity,    _ARM2D_GENERIC_RET,    _ARM2D_ALPHA_BITMAP_ARGS) \
  FX(arm_2dp_cccn888_tile_copy_with_opacity_only,          _ARM2D_GENERIC_RET,    _ARM2D_BITMAP_ARGS)       \
  FX(arm_2dp_rgb565_tile_copy_with_opacity_only,           _ARM2D_GENERIC_RET,    _ARM2D_BITMAP_ARGS)       \
  FX(arm_2dp_cccn888_fill_colour_with_opacity,             _ARM2D_GENERIC_RET,    _ARM2D_FILL_ARGS)         \
  FX(arm_2d_tile_generate_child,                           _ARM2D_TILE_RET,       _ARM2D_GEN_CHILD_ARGS)


//
// Make macro templates for API table
//
#define ARM2D_GENERATE_API_LOAD_CALL(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_LOAD_CALL(ARM2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define ARM2D_GENERATE_WRAPPER_FUNC(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_WRAPPER_FUNC(ARM2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define ARM2D_GENERATE_API_STRUCT_MEMBER(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_STRUCT_MEMBER(ARM2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

#define ARM2D_GENERATE_API_FUNC_PROTOTYPE(FUNC_NAME, RETURN_STATEMENT, ARG_LIST) \
  GUI_GENERATE_API_FUNC_PROTOTYPE(ARM2D, FUNC_NAME, RETURN_STATEMENT, ARG_LIST)

/*********************************************************************
*
*       Function prototypes
*
**********************************************************************
*/
ARM2D_API_TABLE(ARM2D_GENERATE_API_FUNC_PROTOTYPE)

/*********************************************************************
*
*       API struct for Arm2D
*
**********************************************************************
*/
typedef struct {
  ARM2D_API_TABLE(ARM2D_GENERATE_API_STRUCT_MEMBER)
} GUI_GPU_ARM2D_API_STRUCT;

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       GUI_GPU_ARM2D_API
* 
*  Description
*   Macro to fill a structure of the type GUI_GPU_ARM2D_API_STRUCT
*   filled with the correct function pointers.
*/
#ifdef GUI_GPU_HAS_ARM2D
  #define GUI_GPU_ARM2D_API  ARM2D_API_TABLE(GUI_GENERATE_API_STRUCT_INITIALIZER)
#else
  #define GUI_GPU_ARM2D_API  ARM2D_API_TABLE(GUI_GENERATE_NULL)
#endif

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SUPPORT_GPU */

#endif /* GUI_GPU_CHROMART_H */

/*************************** End of file ****************************/
