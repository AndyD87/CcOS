/*
 * This file is part of CcOS.
 *
 * CcOS is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcOS.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcKernel
 * @subpage   CcBase
 *
 * @page      CcBase
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     To keep code working on different compiler and platforms,
 *            sometimes it is needed to define some datatypes like uchar.
 */
#ifndef CCBASE_H_
#define CCBASE_H_

#include "stddef.h" //!< Import of default types like size_t
#include <utility>
#ifdef __linux__
  #include "stdint.h"
  #include "time.h"   //!< Import of types time_t and tm
typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
typedef signed   char       int8;   //!< define global uint8 for bit-save-types
typedef __uint16_t          uint16; //!< define global uint16 for bit-save-types
typedef __uint32_t          uint32; //!< define global uint32 for bit-save-types
typedef __uint64_t          uint64; //!< define global uint64 for bit-save-types
typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
typedef __int16_t           int16;  //!< define global int16 for bit-save-types
typedef __int32_t           int32;  //!< define global int32 for bit-save-types
typedef __int64_t           int64;  //!< define global int64 for bit-save-types
typedef unsigned char       byte;   //!< define global byte for bit-save-types
typedef unsigned int        uint;   //!< define uint for better readability.
#elif _WIN32
  #include "stdint.h"
  #include "time.h"   //!< Import of types time_t and tm
typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
typedef unsigned short      uint16; //!< define global uint16 for bit-save-types
typedef unsigned int        uint32; //!< define global uint32 for bit-save-types
typedef unsigned long long  uint64; //!< define global uint64 for bit-save-types
typedef signed   char       int8;   //!< define global uint8 for bit-save-types
typedef signed short        int16;  //!< define global int16 for bit-save-types
typedef int                 int32;  //!< define global int32 for bit-save-types
typedef signed long long    int64;  //!< define global int64 for bit-save-types
typedef unsigned char       byte;   //!< define global byte for bit-save-types
typedef unsigned int        uint;   //!< define uint for better readability.
#elif defined(__GNUC__) || defined(__GNUG__)
#include "stdint.h"
#include "time.h"   //!< Import of types time_t and tm
typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
typedef signed   char       int8;   //!< define global uint8 for bit-save-types
typedef __uint16_t          uint16; //!< define global uint16 for bit-save-types
typedef unsigned int        uint32; //!< define global uint32 for bit-save-types
typedef __uint64_t          uint64; //!< define global uint64 for bit-save-types
typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
typedef __int16_t           int16;  //!< define global int16 for bit-save-types
typedef int                 int32;  //!< define global int32 for bit-save-types
typedef __int64_t           int64;  //!< define global int64 for bit-save-types
typedef unsigned char       byte;   //!< define global byte for bit-save-types
typedef unsigned int        uint;   //!< define uint for better readability.
#else
  #include "stdint.h"
  #include "time.h"   //!< Import of types time_t and tm
typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
typedef signed   char       int8;   //!< define global uint8 for bit-save-types
typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
typedef unsigned short      uint16; //!< define global uint16 for bit-save-types
typedef unsigned long       uint32; //!< define global uint32 for bit-save-types
typedef unsigned long long  uint64; //!< define global uint64 for bit-save-types
typedef signed short        int16;  //!< define global int16 for bit-save-types
typedef signed long         int32;  //!< define global int32 for bit-save-types
typedef signed long long    int64;  //!< define global int64 for bit-save-types
typedef unsigned char       byte;   //!< define global byte for bit-save-types
typedef unsigned int        uint;   //!< define uint for better readability.
#endif

/**
 * Important value definitions
 */
#ifndef SIZE_MAX
# define SIZE_MAX (size_t)~0        //!< define -1 for unsigned size_t, used for masks and error states
#endif
#ifndef NULL
# define NULL (void*)0                     //!< define NULL as 0 to have a default value for unused pointer
#endif

/// Check if a specific bit is set in a variable
#define IS_FLAG_SET(Var,Flag) ((Var & Flag) == Flag)
/// Check if a specific bit is not set in a variable
#define IS_FLAG_NOT_SET(Var,Flag) ((Var & Flag) != Flag)

/// Check if a specific bit is set in a variable
#define SET_FLAG(Var,Flag) (Var |= Flag)
/// Check if a specific bit is not set in a variable
#define REMOVE_FLAG(Var,Flag) (Var &= ~Flag)

/// Sometimes it's important to define a variable without using it. To
/// To avoid warings, variables should be declared as unused by using this macro
#define CCUNUSED(unused) ((void)unused)
/// Similar to CCUNUSED but marked as TODO because of an implementation wich is not done or required yet.
#define CCUNUSED_TODO(unused) (CCUNUSED(unused))

#define CCNULLREF(object) *(object*)(nullptr)

/**
 * Setup global Debug definitions,
 */
#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG /// If a System is just defining _DEBUG not DEBUG, define it too.
#endif
#endif

#ifdef __GNUC__
  #ifdef NDEBUG
    #ifdef DEBUG
      #undef DEBUG
    #endif
  #else
    #define DEBUG
  #endif
#endif

#ifdef DEBUG
#include "CcDebug.h"
#define CCDEBUG(MSG)    CcDebug::writeDebug(MSG)    //!< if DEBUG is defined, Write Debug message with debug tag to debug output
#ifdef VERBOSE
#define CCVERBOSE(MSG)  CcDebug::writeVerbose(MSG)  //!< if DEBUG is defined, Write Verbose message with verbose tag to debug output
#else
#define CCVERBOSE(MSG)    (void)0 //!< VERBOSE not defined, so ignore debug message
#endif
#define CCINFO(MSG)     CcDebug::writeInfo(MSG)     //!< if DEBUG is defined, Write Info message with info tag to debug output
#define CCWARNING(MSG)  CcDebug::writeWarning(MSG)  //!< if DEBUG is defined, Write Warning message with warning tag to debug output
#define CCERROR(MSG)    CcDebug::writeError(MSG)    //!< if DEBUG is defined, Write Error message with error tag to debug output
#else
#define CCVERBOSE(MSG)    (void)0 //!< VERBOSE not defined, so ignore debug message
#define CCDEBUG(MSG)    (void)0 //!< DEBUG not defined, so ignore debug message
#define CCINFO(MSG)     (void)0 //!< if DEBUG is defined, Write Info message with info tag to debug output
#define CCWARNING(MSG)  (void)0 //!< if DEBUG is defined, Write Warning message with warning tag to debug output
#define CCERROR(MSG)    (void)0 //!< if DEBUG is defined, Write Error message with error tag to debug output
#endif


#ifdef WIN32
#define SOCKETFD uint
#else
#define SOCKETFD int
#endif

// Include the most important files:
#include "Types/CcHandle.h"

#ifdef MEMORYMONITOR_ENABLED
#include "Types/CcMemoryMonitor.h"
#define CCMONITORNEW(VAR) CcMemoryMonitor::insert(VAR, __FILE__, __LINE__)
#define CCMONITORDELETE(VAR) CcMemoryMonitor::remove(VAR)
//#define CCMONITORNEW(VAR)    (void)0
//#define CCMONITORDELETE(VAR) (void)0
#else
#define CCMONITORNEW(VAR)    (void)0
#define CCMONITORDELETE(VAR) (void)0
#endif

/// @group Cc return states
/// @{
#define CCSUCCESS 0 //!< All Operations succeeded
/// @}


#endif /* CCBASE_H_ */
