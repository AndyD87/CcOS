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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     To keep code working on different compiler and platforms,
 *            sometimes it is needed to define some datatypes like uchar.
 */
#pragma once

//! Define extern C macros.
//! This will allow to filter sources for external c code, and keeps it more simple to use.
//! @{
#ifdef __cplusplus
  #define CCEXTERNC extern "C"          //!< Mark next statement as c code
  #define CCEXTERNC_BEGIN extern "C" {  //!< Begin extern c block for c++ code
  #define CCEXTERNC_END }               //!< Close extern c block for c++ code
#else
  #define CCEXTERNC       //!< Ignore definition for C-Code
  #define CCEXTERNC_BEGIN //!< Ignore definition for C-Code
  #define CCEXTERNC_END   //!< Ignore definition for C-Code
#endif
//! @}

#if defined(_MSC_VER) || (defined(__INTEL_COMPILER) && defined(_WIN32))
  #if defined(_M_X64)
    #define CCOS_X64 //!< Current build is for a 64 bit Environment
  #else
    #define CCOS_X32 //!< Current build is for a 32 bit Environment
  #endif
#elif defined(__clang__) || defined(__INTEL_COMPILER) || defined(__GNUC__)
  #if defined(__x86_64)
    #define CCOS_X64 //!< Current build is for a 64 bit Environment
  #else
    #define CCOS_X32 //!< Current build is for a 32 bit Environment
  #endif
#else
  #if __LONG_MAX__ == 2147483647L
    #define CCOS_X32 //!< Current build is for a 32 bit Environment
  #else
    #define CCOS_X64 //!< Current build is for a 64 bit Environment
  #endif
#endif
#ifdef LINUXKERNEL
  #define CCKERNEL_MODE
  #define GENERIC             TRUE
  typedef signed long         time_t;
  #ifdef __cplusplus
    #include <cstdint>
    typedef uintptr_t           uintptr;//!< define unsigned integer for pointer addresses
    typedef intptr_t            intptr; //!< define integer for pointer addresses
  #else
    typedef unsigned int        uintptr;//!< define unsigned integer for pointer addresses
    typedef int                 intptr; //!< define integer for pointer addresses
  #endif
  #ifdef CCOS_X64
    typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
    typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
    typedef unsigned short      uint16; //!< define global uint16 for bit-save-types
    typedef unsigned int        uint32; //!< define global uint32 for bit-save-types
    typedef unsigned long       uint64; //!< define global uint64 for bit-save-types
    typedef signed   char       int8;   //!< define global uint8 for bit-save-types
    typedef signed short        int16;  //!< define global int16 for bit-save-types
    typedef signed int          int32;  //!< define global int32 for bit-save-types
    typedef signed long         int64;  //!< define global int64 for bit-save-types
    typedef unsigned char       byte;   //!< define global byte for bit-save-types
    typedef unsigned int        uint;   //!< define uint for better readability.
  #else
    typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
    typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
    typedef unsigned short      uint16; //!< define global uint16 for bit-save-types
    typedef unsigned long       uint32; //!< define global uint32 for bit-save-types
    typedef unsigned long long  uint64; //!< define global uint64 for bit-save-types
    typedef signed   char       int8;   //!< define global uint8 for bit-save-types
    typedef signed short        int16;  //!< define global int16 for bit-save-types
    typedef signed long         int32;  //!< define global int32 for bit-save-types
    typedef signed long long    int64;  //!< define global int64 for bit-save-types
    typedef unsigned char       byte;   //!< define global byte for bit-save-types
    typedef unsigned int        uint;   //!< define uint for better readability.
  #endif
#elif defined(__linux__)
  #ifndef LINUX
    #define LINUX
  #endif
  #include <stdint.h>                 //!< Get all basic integers
  #include <time.h>                   //!< Import of types time_t and tm
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
  typedef uintptr_t           uintptr;//!< define unsigned integer for pointer addresses
  typedef intptr_t            intptr; //!< define integer for pointer addresses
#elif defined(_WIN32)
  // Support for MinGW
  #ifdef __GNUC__
    #ifndef MINGW
      #define MINGW         //!< Define mingw detecten for later use
    #endif
    #ifndef _WIN32_WINNT
      #define _WIN32_WINNT _WIN32_WINNT_VISTA //!< Set minimum version to Windows Vista
    #endif
    #ifndef WINVER
      #define WINVER       _WIN32_WINNT_VISTA //!< Set minimum version to Windows Vista
    #endif
    #include <basetyps.h>
    #ifndef __declspec
      #define __declspec(VAL) __attribute__((VAL)) //!< Define declspec for gcc if not already done
    #endif
  #endif

  #ifdef _KERNEL_MODE
    #define CCKERNEL_MODE
    #define GENERIC             TRUE
    //! Define windows, if not already done, for a more readably define
    #ifndef WINDOWSKERNEL
      #define WINDOWSKERNEL
    #endif
    #define CCNDEBUG
    #define NOEXCEPT
    #include <crtdefs.h>
  #else
    //! Define windows, if not already done, for a more readably define
    #if !defined(WINDOWS) && !defined(GENERIC)
      #define WINDOWS
    #endif
    #include <time.h>                   //!< Import of types time_t and tm
    #include <stdint.h>                 //!< Get all basic integers
  #endif // _KERNEL_MODE
  typedef uintptr_t           uintptr;//!< define unsigned integer for pointer addresses
  typedef intptr_t            intptr; //!< define integer for pointer addresses
  typedef unsigned char       uchar;  //!< define global uchar for bit-save-types
  typedef unsigned char       uint8;  //!< define global uint8 for bit-save-types
  typedef unsigned short      uint16; //!< define global uint16 for bit-save-types
  typedef unsigned long       uint32; //!< define global uint32 for bit-save-types
  typedef unsigned long long  uint64; //!< define global uint64 for bit-save-types
  typedef signed   char       int8;   //!< define global uint8 for bit-save-types
  typedef signed short        int16;  //!< define global int16 for bit-save-types
  typedef long                int32;  //!< define global int32 for bit-save-types
  typedef signed long long    int64;  //!< define global int64 for bit-save-types
  typedef unsigned char       byte;   //!< define global byte for bit-save-types
  typedef unsigned int        uint;   //!< define uint for better readability.
#elif defined(__XC8)
  typedef unsigned char   uint8;  //!< define global uint8 for bit-save-types
  typedef char             int8;  //!< define global int8 for bit-save-types
  typedef unsigned short  uint16; //!< define global uint16 for bit-save-types
  typedef short            int16; //!< define global int16 for bit-save-types
  typedef unsigned long   uint32; //!< define global uint16 for bit-save-types
  typedef long             int32; //!< define global int16 for bit-save-types
  typedef unsigned char    bool;  //!< define global bool value for C
  // It is an 8 bit processor, avoid using of higher bit values

  #define FALSE 0     //!< False value for bool
  #define TRUE  1     //!< True value for bool
  #define false FALSE //!< For supporting reason, define false value similar to FALSE
  #define true  TRUE  //!< For supporting reason, define true value similar to TRUE

  #define UINT8_MAX     0xff      //!< Max Value a uint8 value can have
  #define UINT16_MAX  0xffff      //!< Max Value a uint16 value can have
  #define UINT32_MAX  0xffffffff  //!< Max Value a uint16 value can have
#else
  //! Define for marking CcOS as generic operating system.
  #ifndef GENERIC
    #define GENERIC
  #endif
  #if defined(__GNUC__) || defined(__GNUG__)
    #include <stdint.h>                 //!< Get all basic integers
    #include <time.h>                   //!< Import of types time_t and tm
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
    typedef uintptr_t           uintptr;//!< define unsigned integer for pointer addresses
    typedef intptr_t            intptr; //!< define integer for pointer addresses
  #else
    #include <stdint.h>                 //!< Get all basic integers
    #include <time.h>                   //!< Import of types time_t and tm
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
    typedef uintptr_t           uintptr;//!< define unsigned integer for pointer addresses
    typedef intptr_t            intptr; //!< define integer for pointer addresses
  #endif
#endif

#if (defined(LINUX) || defined(WINDOWS)) && !defined(LINUXKERNEL) && !defined(WINDOWSKERNEL)
  #define FULL_OS_AVAILABLE
  #ifdef __cplusplus
    #include <utility> // for CCMOVE
  #endif // __cplusplus
#elif defined(GENERIC)
  #ifdef __cplusplus
    #include <utility> // for CCMOVE
  #endif // __cplusplus
#endif

#if defined(_MSC_VER) && !defined(CCKERNEL_MODE)
# ifndef CcKernelSHARED
#   ifdef CcKernel_EXPORTS
//    Cmake definition for shared build is set
#     define CcKernelSHARED __declspec(dllexport)
#   elif defined(CC_STATIC)
//    CCOS will be build as static library no im-/export
#     define CcKernelSHARED
#   else
//    if no definition found, we are on importing as dll
#     define CcKernelSHARED __declspec(dllimport)
#   endif
# endif
#else
# define CcKernelSHARED
#endif

#include <stddef.h>     //!< Import of default types like size_t

//! Basic define to get 0xfff.... for an unsigned integer type
//! @param TYPE: unsigned integertype like uint8
//! @return 0xfff.. based on type
#ifndef TYPE_MAX
# ifdef __cplusplus
#   define TYPE_MAX(TYPE) (static_cast<TYPE>(~static_cast<TYPE>(0)))
# else
#   define TYPE_MAX(TYPE) ((TYPE)(~(TYPE)(0)))
# endif
#endif

//! Important value definitions
//! @{
//! 0xfff for size_t
#ifndef SIZE_MAX
# define SIZE_MAX TYPE_MAX(size_t)          //!< define -1 for unsigned size_t, used for masks and error states
#endif

//! 0xffffffffffffffff for uint64
#ifndef UINT64_MAX
# define UINT64_MAX TYPE_MAX(uint64)        //!< define -1 for unsigned int 32, used for masks and error states
#endif

//! 0xffffffff for uint32
#ifndef UINT32_MAX
# define UINT32_MAX TYPE_MAX(uint32)        //!< define -1 for unsigned int 32, used for masks and error states
#endif

//! 0xffff for uint16
#ifndef UINT16_MAX
# define UINT16_MAX TYPE_MAX(uint16)        //!< define -1 for unsigned int 32, used for masks and error states
#endif

//! 0xff for uint8
#ifndef UINT8_MAX
# define UINT8_MAX TYPE_MAX(uint8)          //!< define -1 for unsigned int 32, used for masks and error states
#endif

//! define NULL if it is used anywhere.
//! For C++, use nullptr instead
#ifndef NULL
# define NULL (void*)0
#endif
//! @}

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
#ifndef CCUNUSED_PARAM
  //! Mark a paramter as unused and remove variable access to avoid using of unused paramter
  #define CCUNUSED_PARAM(unused)
#endif // CCUNUSED_PARAM

#ifdef DEBUG
  #define CCUNUSED_DEBUG(unused) CCUNUSED(unused)
  #define CCUNUSED_RELEASE(unused) unused
#else
  //! CCUNUSED_DEBUG is working similar to CCUNUSED but it will work only in debug builds.
  //! Release builds have to implement the variable.
  #define CCUNUSED_DEBUG(unused) unused
  //! CCUNUSED_RELEASE is working similar to CCUNUSED but it will work only in release builds.
  //! Debug builds have to implement the variable.
  #define CCUNUSED_RELEASE(unused) CCUNUSED(unused)
#endif

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

//! Add a number of bytes to given voidpointer
#define CCVOIDPTRADD(PTR,OFFSET) static_cast<void*>(static_cast<char*>(PTR)+OFFSET)

//! Add a number of bytes to given voidpointer
#define CCVOIDPTRCONSTADD(PTR,OFFSET) static_cast<const void*>(static_cast<const char*>(PTR)+OFFSET)

//! @brief Check if an object is like a type of CcStatic::getNullRef<TYPE>()
#define CCISNULLREF(object) (static_cast<const void*>(&object) == nullptr)

/**
 * @brief Get class as type from poitner.
 *        For example:
 *          @code{.cpp}
 *          CcString *pString;
 *          CLASSOFP(pString)::fromNumber(1);
 *          @endcode
 * @param POINTER: Pointer to receive class from
 */
#define CLASSOFP(POINTER) std::remove_pointer<decltype(POINTER)>::type

/**
 * @brief Get Distance of two pointer in bytes
 * @param PH: Higher-Pointer
 * @param PL: Lower-Pointer
 * @return Number of distance in Bytes.
 */
#define CCPOINTER_DISTANCE(PH,PL)   (CCVOIDPTRCAST(char*,PH)-CCVOIDPTRCAST(char*,PL))

//! @brief Notify if fall through in switch case is wanted!
#ifdef __clang__
  // clang does not warn here at the moment
  #define CCFALLTHROUGH CCUNUSED(0)
#elif defined(__GNUC__)
  #if defined(__cplusplus)
    #if __GNUG__ > 6
      #define CCFALLTHROUGH __attribute__((fallthrough))
    #elif defined(__clang__)
      [[clang::fallthrough]]
    #else
      // Older gcc versions requires an text to warn for fall through
      #define CCFALLTHROUGH  /* fall through */ CCUNUSED(0)
    #endif
  #else
    // Older gcc versions requires an text to warn for fall through
    #define CCFALLTHROUGH  /* fall through */ CCUNUSED(0)
  #endif
#else
  // No other compiler is know who warnes here
  #define CCFALLTHROUGH CCUNUSED(0)
#endif

/**
 * Setup global Debug definitions,
 */
#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG //!< If a System is just defining _DEBUG not DEBUG, define it too.
#endif
#endif

#ifdef __GNUC__
  #ifdef NDEBUG
    #ifdef DEBUG
      #undef DEBUG
    #endif
  #else
    #define DEBUG //!< If a System is just defining NDEBUG not DEBUG, define it too.
  #endif
#endif

#if !defined(__GNUC__) && !defined(__has_attribute)
  #define __has_attribute(VAL) 0
#endif
#if defined(__GNUC__) || __has_attribute(always_inline)
  #define CPPCODEC_ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER) && !__INTEL_COMPILER && _MSC_VER >= 1310 // since Visual Studio .NET 2003
  #define CC_FORCE_INLINE inline __forceinline
#else
  #define CC_FORCE_INLINE inline
#endif

#if (defined(DEBUG) || defined(DEBUG_OUTPUT)) && defined __cplusplus && !defined(NO_CCOS) && defined(FULL_OS_AVAILABLE)
  #include "CcDebug.h"
  #define CCDEBUG(MSG)    CcDebug::writeDebug(MSG)    //!< if DEBUG is defined, Write Debug message with debug tag to debug output
  #define CCDEBUGONFALSE(CONDITION,MSG) if(CONDITION==false)CCDEBUG(MSG)   //!< Write to CCDEBUG if condition is false
  #define CCINFO(MSG)     CcDebug::writeInfo(MSG)     //!< if DEBUG is defined, Write Info message with info tag to debug output
  #define CCWARNING(MSG)  CcDebug::writeWarning(MSG)  //!< if DEBUG is defined, Write Warning message with warning tag to debug output
  #define CCERROR(MSG)    CcDebug::writeError(MSG)    //!< if DEBUG is defined, Write Error message with error tag to debug output
  #ifdef VERBOSE
    #define CCVERBOSE(MSG)  CcDebug::writeVerbose(MSG)  //!< if VERBOSE is defined, Write Verbose message with verbose tag to debug output
  #else
    #define CCVERBOSE(MSG)    (void)0 //!< VERBOSE not defined, so ignore debug message
  #endif
#elif defined(CCKERNEL_MODE) && !defined(NO_CCOS)
  #include "CcMalloc.h"
  #if defined(DEBUG)
    #define CCDEBUG(...)  CcMalloc_print("[dbg ] "); CcMalloc_print(__VA_ARGS__); CcMalloc_print("\n")    //!< if DEBUG is defined, Write Debug message with debug tag to debug output
    #define CCDEBUGONFALSE(CONDITION,...) if(CONDITION==false) CCDEBUG(__VA_ARGS__)   //!< Write to CCDEBUG if condition is false
  #else
    #define CCDEBUG(...)    (void)0                   //!< DEBUG not defined, so ignore debug message
    #define CCDEBUGONFALSE(CONDITION,...) (void)0;    //!< Write to CCDEBUG if condition is false
  #endif
  #define CCINFO(...)     CcMalloc_print("[info] "); CcMalloc_print(__VA_ARGS__); CcMalloc_print("\n")  //!< if DEBUG is defined, Write Info message with info tag to debug output
  #define CCWARNING(...)  CcMalloc_print("[warn] "); CcMalloc_print(__VA_ARGS__); CcMalloc_print("\n")  //!< if DEBUG is defined, Write Warning message with warning tag to debug output
  #define CCERROR(...)    CcMalloc_print("[err ] "); CcMalloc_print(__VA_ARGS__); CcMalloc_print("\n")  //!< if DEBUG is defined, Write Error message with error tag to debug output
  #ifdef VERBOSE
  #define CCVERBOSE(...)  CcMalloc_print(__VA_ARGS__)  //!< if VERBOSE is defined, Write Verbose message with verbose tag to debug output
  #else
  #define CCVERBOSE(...)  (void)0 //!< VERBOSE not defined, so ignore debug message
  #endif
#else
  #define CCVERBOSE(MSG)    (void)0 //!< VERBOSE not defined, so ignore debug message
  #define CCDEBUG(MSG)    (void)0 //!< DEBUG not defined, so ignore debug message
  #define CCDEBUGONFALSE(CONDITION,MSG) (void)0;   //!< Write to CCDEBUG if condition is false
  #define CCINFO(MSG)     (void)0 //!< if DEBUG is defined, Write Info message with info tag to debug output
  #define CCWARNING(MSG)  (void)0 //!< if DEBUG is defined, Write Warning message with warning tag to debug output
  #define CCERROR(MSG)    (void)0 //!< if DEBUG is defined, Write Error message with error tag to debug output
#endif

//! Define platform defending file descriptor for network sockets.
#ifdef WINDOWS
#define SOCKETFD uint
#else
#define SOCKETFD int
#endif

//! For macros wich Params wich requires a comma like MAKRO(Template<var1,var2>)
//! a seperated define for "," is reuired.
#define CCCOMMA ,

//! @def CCMONITORNEW
//! @brief Create entry for buffer in memory monitor.
//! @def CCMONITORDELETE
//! @brief Remove entry for buffer in memory monitor.

//! MemoryMonitor functions to track used memories.
//! @{
#if defined(MEMORYMONITOR_ENABLED) && !defined(NO_CCOS) && !defined(CCMONITOR_IGNORE) && defined(FULL_OS_AVAILABLE)
  extern void CcKernelSHARED CcMemoryMonitor__remove(const void* pBuffer);
  extern void CcKernelSHARED CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine);
  #ifdef __cplusplus
    template<typename T>
    inline T* CcMemoryMonitor___insert_inline(T* pNewObject, const char* pFile, int iLine)
    { CcMemoryMonitor__insert(pNewObject, pFile, iLine); return pNewObject; }
    #define CCMONITORNEW_INLINE(VAR,...) CcMemoryMonitor___insert_inline(new VAR(__VA_ARGS__), __FILE__, __LINE__)
    #define CCMONITORNEW(VAR) CcMemoryMonitor__insert(static_cast<void*>(VAR), __FILE__, __LINE__)
    #define CCMONITORDELETE(VAR) CcMemoryMonitor__remove(static_cast<void*>(VAR))
  #else
    extern CcKernelSHARED void* CcMemoryMonitor__insert_inline(void* pBuffer, const char* pFile, int iLine);
    #define CCMONITORNEW_INLINE(VAR,...) (VAR*)CcMemoryMonitor__insert_inline(new VAR(__VA_ARGS__), __FILE__, __LINE__)
    #define CCMONITORNEW(VAR) CcMemoryMonitor__insert((void*)(VAR), __FILE__, __LINE__)
    #define CCMONITORDELETE(VAR) CcMemoryMonitor__remove((void*)(VAR))
  #endif
#else
  #define CCMONITORNEW(VAR)    CCUNUSED(VAR)
  #define CCMONITORDELETE(VAR) CCUNUSED(VAR)
  #define CCMONITORNEW_INLINE(VAR,...) new VAR(__VA_ARGS__)
#endif
//! @}

//! @brief Create new class for existing pointer
//! @param VAR:   Variable to store new memory
//! @param TYPE:  Classname to create
//! @param ...:   Arguments for constructor
#define CCNEW(VAR,TYPE,...)   \
  VAR = new TYPE(__VA_ARGS__);\
  CCMONITORNEW(VAR)

//! @brief Create new class and return
//! @param TYPE:  Classname to create
//! @param ...:   Arguments for constructor
#define CCNEW_INLINE(TYPE,...)   \
  CCMONITORNEW_INLINE(TYPE,__VA_ARGS__)

//! @brief Create new class and create a variable
//! @param VAR:   Variable to create and store new memory
//! @param TYPE:  Classname to create
//! @param ...:   Arguments for constructor
#define CCNEWTYPE(VAR,TYPE,...)     \
  TYPE* VAR = new TYPE(__VA_ARGS__);\
  CCMONITORNEW(VAR)
//! @brief Create new array of classes for existing pointer
//! @param VAR:   Variable to store new memory
//! @param TYPE:  Classname to create
//! @param SIZE:  Number of elements to allocate for array
#define CCNEWARRAY(VAR,TYPE,SIZE)   \
  VAR = new TYPE[SIZE]();             \
  CCMONITORNEW(VAR)
//! @brief Create new array of classes and create a variable
//! @param VAR:   Variable to create and store new memory
//! @param TYPE:  Classname to create
//! @param SIZE:  Number of elements to allocate for array
#define CCNEWARRAYTYPE(VAR,TYPE,SIZE)   \
  TYPE* VAR = new TYPE[SIZE]();           \
  CCMONITORNEW(VAR)
//! @brief Create new array of classes for existing pointer
//! @param VAR:   Variable to store new memory
//! @param TYPE:  Classname to create
//! @param SIZE:  Number of elements to allocate for array
//! @param ...:   Arguments for all constructors
#define CCNEWARRAYINIT(VAR,TYPE,SIZE,...)  \
  VAR = new TYPE[SIZE]{__VA_ARGS__};       \
  CCMONITORNEW(VAR)
//! @brief Create new array of classes and create a variable
//! @param VAR:   Variable to create and store new memory
//! @param TYPE:  Classname to create
//! @param SIZE:  Number of elements to allocate for array
//! @param ...:   Arguments for all constructors
#define CCNEWARRAYTYPEINIT(VAR,TYPE,SIZE,...) \
  TYPE* VAR = new TYPE[SIZE]{__VA_ARGS__};    \
  CCMONITORNEW(VAR)

//! For masking new`s wich are known and okay, use this!
#define CCKNOWNNEW      new
//! For masking deletes`s wich are known and okay, use this!
#define CCKNOWNDELETE   delete

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running and set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETE(VAR) if(VAR!=nullptr){CCMONITORDELETE(VAR);delete VAR;VAR = nullptr;}void(0)

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running, but do not set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETE_RO(VAR) if(VAR!=nullptr){CCMONITORDELETE(VAR);delete VAR;}void(0)

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running and set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETEREF(VAR) if(VAR!=nullptr&&VAR->referenceCountDecrement()){CCDELETE(VAR);}VAR = nullptr;void(0)

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running and set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETEARR(VAR) if(VAR!=nullptr){CCMONITORDELETE(VAR);delete[] VAR;VAR = nullptr;}void(0)

/**
 * @brief Check if null, then delete a array, remove it from monitoring if running and set variable to null.
 * @param VAR: Array to delete
 */
#define CCDELETEARRAY(VAR) CCDELETEARR(VAR)

//! @brief Cc return states
//! @{
#define CCSUCCESS 0 //!< All Operations succeeded
//! @}

#ifdef __cplusplus
  // Include global status class
  #ifndef NO_CCOS
    #include "CcStatus.h"
  #endif // N_CCOS
#endif

#ifndef CCMOVE
  /**
   * @brief Mark object to move
   * @param VAR: Object to move
   */
  #define CCMOVE(VAR) std::move(VAR)
#endif

#if !defined(NOEXCEPT)
  #if defined(__clang__)
    #if __has_feature(cxx_noexcept)
      #define HAS_NOEXCEPT
    #endif
  #else
    #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
      #define HAS_NOEXCEPT                //!< No definition available for NOEXCEPT
    #elif !defined(_GLIBCXX_USE_NOEXCEPT)
      #define _GLIBCXX_USE_NOEXCEPT       //!< No definition for NOEXCEPT
    #endif
  #endif

  #ifdef HAS_NOEXCEPT
    #define NOEXCEPT noexcept   //!< Use common noexcept for NOEXCEPT
  #else
    #define NOEXCEPT            //!< No definition for NOEXCEPT
  #endif
#endif

#if !defined(NOEXCEPT_IMPLICIT)
  #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
    #define HAS_NOEXCEPT_IMPLICIT //!< No definition for NOEXCEPT_IMPLICIT
  #endif
  #ifdef HAS_NOEXCEPT_IMPLICIT
    #define NOEXCEPT_IMPLICIT noexcept
  #else
    #define NOEXCEPT_IMPLICIT     //!< No definition for NOEXCEPT_IMPLICIT
  #endif
#endif

//! Build arch is defined by toolchain
#ifdef CC_BUILD_ARCH
  //! This definitions are same as defined in CcMacros.cmake
  //! @{
  #define CC_BUILD_ARCH_x86    0
  #define CC_BUILD_ARCH_x64    1
  #define CC_BUILD_ARCH_arm    2
  #define CC_BUILD_ARCH_arm64  3
  #define CC_BUILD_ARCH_xtensa 4
  //! @}
  #if CC_BUILD_ARCH == CC_BUILD_ARCH_xtensa
    // Xetensa requires an alignment of 4
    // Microcontroller will crash if a pointer is out of this alignment
    #define CC_ALIGNMENT_MIN  4
  #endif
#endif

#ifndef CC_ALIGNMENT_MIN
  #define CC_ALIGNMENT_MIN  1
#endif // CC_ALIGNMENT_MIN

//! @brief Define false returning default operators for all classes which have
//!        to implement them, but do not use them.
//! @param CLASS: Class to define this operators.
#define CCDEFINE_EQUAL_OPERATORS(CLASS) \
      inline bool operator==(const CLASS&) const { return false; }\
      inline bool operator!=(const CLASS&) const { return false; }

//! @brief Define a copy construction which is calling operator=
//! @param CLASS: Class to define this operators.
#define CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CLASS) \
      CLASS(const CLASS& oToCopy) NOEXCEPT { operator=(oToCopy); } \
      //CLASS(const CLASS& oToCopy) NOEXCEPT

//! @brief Define a move construction which is calling operator=&&
//! @param CLASS: Class to define this operators.
#define CCDEFINE_MOVE_CONSTRUCTOR_TO_OPERATOR(CLASS) \
      CLASS(CLASS&& oToMove) NOEXCEPT { operator=(CCMOVE(oToMove)); }

//! @brief Define a copy and move construction which is calling operator=&&
//! @param CLASS: Class to define this operators.
#define CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CLASS)   \
      CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CLASS) \
      CCDEFINE_MOVE_CONSTRUCTOR_TO_OPERATOR(CLASS)

//! @brief Define a copy and move consturctor as denied
#define CCDEFINE_COPY_DENIED(CLASS)         \
      CLASS(const CLASS& oToCopy) = delete; \
      CLASS(CLASS&& oToMove) = delete;

//! @brief Get macro name as string
//! @param x: Macro to convert to string
#define CCMACRO_TO_STRING(x)         #x

//! @brief Get value of macrostring
//! @param x: Macro to convert to string
#define CCMACRO_TO_VALUE(x)         CCMACRO_TO_STRING(x)

//! @brief Get macro name and value in one string like "DEBUG = 1"
//! @param VAR: Macro to convert to string
#define CCMACRO_TO_STRING_EQ_VALUE(VAR)   #VAR " = "  CCMACRO_TO_VALUE(VAR)

//! @brief Get number of items in array by its own and first elements size
//! @param ARRAY: Array to get number of items from
#define CCSIZEOFARRAY(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

//! @brief Get length of a string, not it's size
//! @param STRING: String to get the length from
#define CCLENGTHOFSTRING(STRING) (sizeof(STRING)-1)

#ifndef CCTHROW_BAD_ALLOC
  #if defined(_MSC_VER) || defined(__clang__)
    #define CCTHROW_BAD_ALLOC        //!< Define an empty CCTHROW_BAD_ALLOC if not existing
  #elif defined(WIN32)
    #define CCTHROW_BAD_ALLOC        //!< Define an empty CCTHROW_BAD_ALLOC if not existing
  #elif defined(GENERIC)
    #define CCTHROW_BAD_ALLOC NOEXCEPT //!< Define noexceptional CCTHROW_BAD_ALLOC if we are on generic
  #elif defined(__GNUC__) || defined(__GNUG__)
    #define CCTHROW_BAD_ALLOC _GLIBCXX_THROW(std::bad_alloc) //!< Use _GLIBCXX_THROW macro for gcc
  #else
    #define CCTHROW_BAD_ALLOC        //!< Define an empty CCTHROW_BAD_ALLOC if not existing
  #endif
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
  #define _GLIBCXX_USE_NOEXCEPT //!< Define an empty _GLIBCXX_USE_NOEXCEPT if not existing
#endif
#ifndef WINDOWSKERNEL
  #define WINCEXPORT         //!< Define an empty WINCEXPORT if in KernelMode
#else
  #define WINCEXPORT __cdecl //!< Define an WINCEXPORT as __cdecl if not in KernelMode
#endif

#ifdef __cplusplus
  /**
   * @brief Overloaded new method for CcOS to track allocations and
   *        to share buffer regions over modules
   * @param uiSize: Number of bytes to allocate
   * @return Pointer to allocated buffer or nullptr if error
   */
  void* WINCEXPORT operator new(std::size_t uiSize) CCTHROW_BAD_ALLOC;

  /**
   * @brief Overloaded new method for CcOS to track allocations and
   *        to share buffer regions over modules
   * @param uiSize: Number of bytes to allocate
   * @return Pointer to allocated buffer or nullptr if error
   */
  void* WINCEXPORT operator new[](std::size_t uiSize) CCTHROW_BAD_ALLOC;

  /**
   * @brief Overloaded delete method for CcOS to track deallocations and
   *        to manage shared buffer regions over modules
   * @param pBuffer: Address of memory to delete
   */
  void WINCEXPORT operator delete(void* pBuffer) NOEXCEPT;

  /**
   * @brief Overloaded delete method for CcOS to track deallocations and
   *        to manage shared buffer regions over modules
   * @param pBuffer: Address of memory to delete
   */
  void WINCEXPORT operator delete[](void* pBuffer) NOEXCEPT;

  // Do not on mingw
  #if defined(WINDOWS) && !defined(__GNUC__)
    /**
     * @brief Overloaded delete method for CcOS to track deallocations and
     *        to manage shared buffer regions over modules
     * @param pBuffer: Address of memory to delete
     * @param uiSize:  Ignored param, pBuffer will be deleted completly
     */
    void WINCEXPORT operator delete(void* pBuffer, size_t uiSize) NOEXCEPT;

    /**
     * @brief Overloaded delete method for CcOS to track deallocations and
     *        to manage shared buffer regions over modules
     * @param pBuffer: Address of memory to delete
     * @param uiSize:  Ignored param, pBuffer will be deleted completly
     */
    void WINCEXPORT operator delete[](void* pBuffer, size_t uiSize) NOEXCEPT;
  #endif
#endif

// Some frameworks like ESP having their own main implementation
// If CCOS does remove this by define, this can be revoked here
#ifdef CCOS_MAIN_REPLACED
  // undef main definition
  #undef main
  //! @brief Declaration of main entry point
  //! @param uiArgc: Number of arguments passed on pcArgv
  //! @param pcArgv: String array with arguments
  //! @return Return code of application. 0 Should be set if all is ok.
  int WINCEXPORT main(int iArgc, char** pcArgv);
#endif

/**
 * @brief Convert 64bit integer with 32bit higher and lower parts
 *        It simplifies using 32bit register to represent 64bit values.
 *        It is also possible to use this union to convert singed and unsigned.
 */
typedef union
{
  int64   i64;    //!< Basic signed 64bit part
  uint64 ui64;    //!< Basic unsigned 64bit part
  struct
  {
    int32   L;    //!< Low part of 64bit as signed 32bit
    int32   H;    //!< High part of 64bit as signed 32bit
  } i32;    //!< signed 32bit values within 64bit value
  struct
  {
    uint32  L;    //!< Low part of 64bit as unsigned 32bit
    uint32  H;    //!< High part of 64bit as unsigned 32bit
  } ui32;   //!< unsigned 32bit values within 64bit value
} SInt64Converter;

#ifdef _MSC_VER
  #define INCLUDE_WARNING_LEVEL_START __pragma( warning(push, 3) )
  #define INCLUDE_WARNING_LEVEL_STOP  __pragma( warning(pop) )
#else
  #define INCLUDE_WARNING_LEVEL_START
  #define INCLUDE_WARNING_LEVEL_STOP 
#endif

#ifdef DEBUG
  //! Cc Projects are using name extensions for every debug generated file
  #define CC_DEBUG_EXTENSION "d"
#else
  //! Cc Projects are using name extensions for every debug generated file
  #define CC_DEBUG_EXTENSION ""
#endif