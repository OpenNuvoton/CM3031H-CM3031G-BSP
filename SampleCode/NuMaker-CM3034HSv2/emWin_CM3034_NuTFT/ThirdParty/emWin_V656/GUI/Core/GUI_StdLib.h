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
File        : GUI_StdLib.h
Purpose     : Overloading C standard library functions
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_STDLIB_H
#define GUI_STDLIB_H

#include "GUI_Type.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#ifndef   GUI_MEMSET
  #define GUI_MEMSET     memset
#endif
#ifndef   GUI_MEMCPY
  #define GUI_MEMCPY     memcpy
#endif
#ifndef   GUI_STRCMP
  #define GUI_STRCMP     strcmp
#endif
#ifndef   GUI_STRLEN
  #define GUI_STRLEN     strlen
#endif
#ifndef   GUI_STRCPY
  #define GUI_STRCPY     strcpy
#endif
#ifndef   GUI_VSNPRINTF
  #define GUI_VSNPRINTF  vsnprintf
#endif
#ifndef   GUI_STRCHR
  #define GUI_STRCHR     strchr
#endif
#ifndef   GUI_STRRCHR
  #define GUI_STRRCHR    strrchr
#endif
#ifndef   GUI_STRTOUL
  #define GUI_STRTOUL    strtoul
#endif
#ifndef   GUI_STRTOL
  #define GUI_STRTOL     strtol
#endif
#ifndef   GUI_STRTOD
  #define GUI_STRTOD     strtod
#endif
#ifndef   GUI_RAND
  #define GUI_RAND       rand
#endif
#ifndef   GUI_SRAND
  #define GUI_SRAND      srand
#endif
#ifndef   GUI_MEMMOVE
  #define GUI_MEMMOVE    memmove
#endif
#ifndef   GUI_MEMCMP
  #define GUI_MEMCMP     memcmp
#endif
#ifndef   GUI_STRNCPY
  #define GUI_STRNCPY    strncpy
#endif
#ifndef   GUI_STRNCMP
  #define GUI_STRNCMP    strncmp
#endif
#ifndef   GUI_SPRINTF
  #define GUI_SPRINTF    sprintf
#endif
#ifndef   GUI_SNPRINTF
  #define GUI_SNPRINTF   snprintf
#endif
#ifndef   GUI_STRCAT
  #define GUI_STRCAT     strcat
#endif

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef void            * (GUI_MEMSET_FUNC)   (void * pDest, int c, size_t Cnt);
typedef void            * (GUI_MEMCPY_FUNC)   (void * pDest, const void * pSrc, size_t Cnt);
typedef int               (GUI_STRCMP_FUNC)   (const char * pStr1, const char * pStr2);
typedef size_t            (GUI_STRLEN_FUNC)   (const char * pStr);
typedef char            * (GUI_STRCPY_FUNC)   (char * pDst, const char * pSrc);
typedef int               (GUI_VSNPRINTF_FUNC)(char * s, size_t n, const char * format, va_list arg);
typedef const char      * (GUI_STRCHR_FUNC)   (const char * s, int c);
typedef const char      * (GUI_STRRCHR_FUNC)  (const char * s, int c);
typedef unsigned long int (GUI_STRTOUL_FUNC)  (const char* str, char** endptr, int base);
typedef long int          (GUI_STRTOL_FUNC)   (const char* str, char** endptr, int base);
typedef double            (GUI_STRTOD_FUNC)   (const char* str, char** endptr);
typedef int               (GUI_RAND_FUNC)     (void);
typedef void              (GUI_SRAND_FUNC)    (unsigned int seed);
typedef void            * (GUI_MEMMOVE_FUNC)  (void * destination, const void * source, size_t num);
typedef int               (GUI_MEMCMP_FUNC)   (const void * ptr1, const void * ptr2, size_t num);
typedef char            * (GUI_STRNCPY_FUNC)  (char * destination, const char * source, size_t num);
typedef int               (GUI_STRNCMP_FUNC)  (const char * str1, const char * str2, size_t num);
typedef int               (GUI_SPRINTF_FUNC)  (char * str, const char * format, ...);
typedef int               (GUI_SNPRINTF_FUNC) (char * s, size_t n, const char * format, ...);
typedef char            * (GUI_STRCAT_FUNC)   (char * destination, const char * source);

typedef struct {
  GUI_MEMCMP_FUNC     * pfMemcmp;
  GUI_MEMCPY_FUNC     * pfMemcpy;
  GUI_MEMMOVE_FUNC    * pfMemmove;
  GUI_MEMSET_FUNC     * pfMemset;
  GUI_RAND_FUNC       * pfRand;
  GUI_SNPRINTF_FUNC   * pfSnprintf;
  GUI_SPRINTF_FUNC    * pfSprintf;
  GUI_SRAND_FUNC      * pfSrand;
  GUI_STRCAT_FUNC     * pfStrcat;
  GUI_STRCHR_FUNC     * pfStrchr;
  GUI_STRCMP_FUNC     * pfStrcmp;
  GUI_STRCPY_FUNC     * pfStrcpy;
  GUI_STRLEN_FUNC     * pfStrlen;
  GUI_STRNCMP_FUNC    * pfStrncmp;
  GUI_STRNCPY_FUNC    * pfStrncpy;
  GUI_STRRCHR_FUNC    * pfStrrchr;
  GUI_STRTOD_FUNC     * pfStrtod;
  GUI_STRTOL_FUNC     * pfStrtol;
  GUI_STRTOUL_FUNC    * pfStrtoul;
  GUI_VSNPRINTF_FUNC  * pfVsnprintf;
} GUI_STDLIB_INTERFACE;

/*********************************************************************
*
*       GUI_STDLIB_FUNCTION_INDEX
* 
*  Description
*    Index for a function to be used in GUI_SetStdLibFunc().
*/
typedef enum {
  GUI_STDLIB_FUNCTION_MEMCMP,     // C standard library function \c{memcmp()}.
  GUI_STDLIB_FUNCTION_MEMCPY,     // C standard library function \c{memcpy()}.
  GUI_STDLIB_FUNCTION_MEMMOVE,    // C standard library function \c{memmove()}.
  GUI_STDLIB_FUNCTION_MEMSET,     // C standard library function \c{memset()}.
  GUI_STDLIB_FUNCTION_RAND,       // C standard library function \c{rand()}.
  GUI_STDLIB_FUNCTION_SNPRINTF,   // C standard library function \c{srand()}.
  GUI_STDLIB_FUNCTION_SPRINTF,    // C standard library function \c{sprintf()}.
  GUI_STDLIB_FUNCTION_SRAND,      // C standard library function \c{srand()}.
  GUI_STDLIB_FUNCTION_STRCAT,     // C standard library function \c{strcat()}.
  GUI_STDLIB_FUNCTION_STRCHR,     // C standard library function \c{strchr()}.
  GUI_STDLIB_FUNCTION_STRCMP,     // C standard library function \c{strcmp()}.
  GUI_STDLIB_FUNCTION_STRCPY,     // C standard library function \c{strcpy()}.
  GUI_STDLIB_FUNCTION_STRLEN,     // C standard library function \c{strlen()}.
  GUI_STDLIB_FUNCTION_STRNCMP,    // C standard library function \c{strncmp()}.
  GUI_STDLIB_FUNCTION_STRNCPY,    // C standard library function \c{strncpy()}.
  GUI_STDLIB_FUNCTION_STRRCHR,    // C standard library function \c{strrchr()}.
  GUI_STDLIB_FUNCTION_STRTOD,     // C standard library function \c{strtod()}.
  GUI_STDLIB_FUNCTION_STRTOL,     // C standard library function \c{strtol()}.
  GUI_STDLIB_FUNCTION_STRTOUL,    // C standard library function \c{strtoul()}.
  GUI_STDLIB_FUNCTION_VSNPRINTF,  // C standard library function \c{vsnprintf()}.
} GUI_STDLIB_FUNCTION_INDEX;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_INDEX Func, GUI_FUNCTION * pFunc);
//
// Legacy functions
//
#define GUI_SetpfMemcpy(pFunc)    GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_MEMCPY, (GUI_FUNCTION *)pFunc);
#define GUI_SetpfMemset(pFunc)    GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_MEMSET, (GUI_FUNCTION *)pFunc);
#define GUI_SetpfStrcmp(pFunc)    GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_STRCMP, (GUI_FUNCTION *)pFunc);
#define GUI_SetpfStrcpy(pFunc)    GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_STRCPY, (GUI_FUNCTION *)pFunc);
#define GUI_SetpfStrlen(pFunc)    GUI_SetStdLibFunc(GUI_STDLIB_FUNCTION_STRLEN, (GUI_FUNCTION *)pFunc);

#endif // GUI_STDLIB_H

/*************************** End of file ****************************/
