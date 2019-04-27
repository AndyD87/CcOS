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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     To keep code working on different compiler and platforms,
 *            sometimes it is needed to define some datatypes like uchar.
 */
#ifndef _CCBASE_H_
#define _CCBASE_H_

#include "stddef.h" //!< Import of default types like size_t
#ifdef __linux__
  #ifndef LINUX
    #define LINUX
  #endif
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
  #ifndef WINDOWS
    #define WINDOWS
  #endif
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
#else
  // Generic implementations
  #ifndef GENERIC
    #define GENERIC
  #endif
  #if defined(__GNUC__) || defined(__GNUG__)
    #include "stdint.h"
    #include "time.h"   //!< Import of types time_t and tm
    typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
    typedef int8_t              int8;   //!< define global uint8 for bit-save-types
    typedef __uint16_t          uint16; //!< define global uint16 for bit-save-types
    typedef uint32_t            uint32; //!< define global uint32 for bit-save-types
    typedef __uint64_t          uint64; //!< define global uint64 for bit-save-types
    typedef uint8_t             uint8;  //!< define global uint8 for bit-save-types
    typedef __int16_t           int16;  //!< define global int16 for bit-save-types
    typedef int32_t             int32;  //!< define global int32 for bit-save-types
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
#endif

#ifndef TYPE_MAX
#define TYPE_MAX(TYPE) (~static_cast<TYPE>(0))
#endif

/**
 * Important value definitions
 */
#ifndef SIZE_MAX
# define SIZE_MAX TYPE_MAX(size_t)         //!< define -1 for unsigned size_t, used for masks and error states
#endif
#ifndef UINT64_MAX
# define UINT64_MAX TYPE_MAX(uint64)         //!< define -1 for unsigned int 32, used for masks and error states
#endif
#ifndef UINT32_MAX
# define UINT32_MAX TYPE_MAX(uint32)         //!< define -1 for unsigned int 32, used for masks and error states
#endif
#ifndef UINT16_MAX
# define UINT16_MAX TYPE_MAX(uint16)         //!< define -1 for unsigned int 32, used for masks and error states
#endif
#ifndef UINT8_MAX
# define UINT8_MAX TYPE_MAX(uint8)          //!< define -1 for unsigned int 32, used for masks and error states
#endif

#ifndef NULL
# define NULL (void*)0            //!< define NULL as 0 to have a default value for unused pointer
#endif

//! Check if all Flags are set in Var
#define IS_FLAG_SET(Var,Flag) ((Var & Flag) == Flag)
//! Check if at least one Flag is not set
#define IS_FLAG_NOT_SET(Var,Flag) ((Var & Flag) != Flag)
//! Check if at least on Flag is set in Var
#define IS_ONE_FLAG_SET(Var,Flag) ((Var & Flag) != 0)

//! Check if a specific bit is set in a variable
#define SET_FLAG(Var,Flag) (Var |= Flag)
//! Check if a specific bit is not set in a variable
#define REMOVE_FLAG(Var,Flag) (Var &= ~Flag)

//! Sometimes it's important to define a variable without using it. To
//! To avoid warings, variables should be declared as unused by using this macro
#define CCUNUSED(unused) ((void)unused)
//! Similar to CCUNUSED but marked as TODO because of an implementation wich is not done or required yet.
#define CCUNUSED_TODO(unused) (CCUNUSED(unused))

//! Get the smaller of two values
#define CCMIN(A,B) (A<B?A:B)
//! Get the greater of two values
#define CCMAX(A,B) (A>B?A:B)

//! Direct casting from, for example, unsigned* to signed* pointers causes warnings. This
//! can be avoided by casting through void*
#define CCVOIDPTRCAST(TYPE,VAR) static_cast<TYPE>(static_cast<void*>(VAR))

//! Like CCVOIDPTRCAST, CCVOIDPTRCONSTCAST can cast to another type, but it will also adding const to
//! target pointer.
#define CCVOIDPTRCONSTCAST(TYPE,VAR) static_cast<const TYPE>(static_cast<const void*>(VAR))

//! @brief Check if an object is like a type of CcStatic::getNullRef<TYPE>()
#define ISNULLREF(object) (static_cast<const void*>(&object) == nullptr)

//! @brief Notify if fall through in switch case is wanted!
#ifdef __clang__
  // clang does not warn here at the moment
  #define CCFALLTHROUGH
#elif __GNUC__
  #if __cplusplus
    #if __GNUG__ > 6
      #define CCFALLTHROUGH [[gnu::fallthrough]];
    #else
      #define CCFALLTHROUGH
    #endif
  #else
    // Older gcc versions requires an text to warn for fall through
    #define CCFALLTHROUGH  /* fall through */
  #endif
#else
  // No other compiler is know who warnes here
  #define CCFALLTHROUGH
#endif

/**
 * Setup global Debug definitions,
 */
#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG //! If a System is just defining _DEBUG not DEBUG, define it too.
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

#if defined(DEBUG) && !defined(GENERIC)
#include "CcDebug.h"
#define CCDEBUG(MSG)    CcDebug::writeDebug(MSG)    //!< if DEBUG is defined, Write Debug message with debug tag to debug output
#define CCDEBUGONFALSE(CONDITION,MSG) if(CONDITION==false)CCDEBUG(MSG)   //!< Write to CCDEBUG if condition is false
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
#define CCDEBUGONFALSE(CONDITION,MSG) (void)0;   //!< Write to CCDEBUG if condition is false
#define CCINFO(MSG)     (void)0 //!< if DEBUG is defined, Write Info message with info tag to debug output
#define CCWARNING(MSG)  (void)0 //!< if DEBUG is defined, Write Warning message with warning tag to debug output
#define CCERROR(MSG)    (void)0 //!< if DEBUG is defined, Write Error message with error tag to debug output
#endif


#ifdef WINDOWS
#define SOCKETFD uint
#else
#define SOCKETFD int
#endif

#ifdef MEMORYMONITOR_ENABLED
#include "Types/CcMemoryMonitor.h"
#define CCMONITORNEW(VAR) CcMemoryMonitor::insert(VAR, __FILE__, __LINE__)
#define CCMONITORDELETE(VAR) CcMemoryMonitor::remove(VAR)
#else
#ifdef DEBUG
  extern void CCMONITORNEW(const void* pData);
#else
  #define CCMONITORNEW(VAR)    (void)0
#endif
#define CCMONITORDELETE(VAR) (void)0
#endif

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running and set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETE(VAR) if(VAR!=nullptr){CCMONITORDELETE(VAR);delete VAR;VAR = nullptr;}

#ifdef __cplusplus
  /**
   * @brief It does the same like CCDELETE but it casts VAR to void before delte.
   * @param VAR: Variable to delete
   */
  #define CCDELETEVOID(VAR) if(VAR!=nullptr){void*__pVoid = static_cast<void*>(VAR);CCMONITORDELETE(__pVoid);delete __pVoid;__pVoid = nullptr;}
#else
  /**
   * @brief It does the same like CCDELETE but it casts VAR to void before delte.
   * @param VAR: Variable to delete
   */
  #define CCDELETEVOID(VAR) if(VAR!=nullptr){void*__pVoid = (void*)(VAR);CCMONITORDELETE(__pVoid);delete __pVoid;__pVoid = nullptr;}
#endif

//! @group Cc return states
//! @{
#define CCSUCCESS 0 //!< All Operations succeeded
//! @}

#ifdef __cplusplus
  #include <utility>
  // Include global status class
  #include "CcStatus.h"
#endif

#ifdef __cplusplus
  #define CCEXTERNC extern "C"
  #define CCEXTERNC_BEGIN extern "C" {
  #define CCEXTERNC_END }
#else
  #define CCEXTERNC
  #define CCEXTERNC_BEGIN
  #define CCEXTERNC_END
#endif

#endif /* _CCBASE_H_ */
