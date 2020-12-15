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
#ifndef H_CCBASE_H_
#define H_CCBASE_H_

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

#ifdef LINUXKERNEL
  #define GENERIC TRUE
  #define _KERNEL_MODE
  typedef signed long         time_t;
  typedef void*               uintptr;//!< define unsigned integer for pointer addresses
  typedef void*               intptr; //!< define integer for pointer addresses
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
    #define GENERIC TRUE
    //! Define windows, if not already done, for a more readably define
    #ifndef WINDOWSKERNEL
      #define WINDOWSKERNEL
    #endif
    #define CCNDEBUG
    #define CCMOVE(VAR) VAR
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

#if defined(_MSC_VER) && !defined(_KERNEL_MODE)
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

#if defined(DEBUG) && defined __cplusplus && !defined(NO_CCOS) && defined(FULL_OS_AVAILABLE)
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
#elif defined(_KERNEL_MODE)
  #include "CcMalloc.h"
  #if defined(DEBUG)
    #define CCDEBUG(...)    CcMalloc_print(__VA_ARGS__)    //!< if DEBUG is defined, Write Debug message with debug tag to debug output
    #define CCDEBUGONFALSE(CONDITION,...) if(CONDITION==false) CcMalloc_print(__VA_ARGS__)   //!< Write to CCDEBUG if condition is false
  #else
    #define CCDEBUG(...)    (void)0                   //!< DEBUG not defined, so ignore debug message
    #define CCDEBUGONFALSE(CONDITION,...) (void)0;    //!< Write to CCDEBUG if condition is false
  #endif
  #define CCINFO(...)     CcMalloc_print(__VA_ARGS__)  //!< if DEBUG is defined, Write Info message with info tag to debug output
  #define CCWARNING(...)  CcMalloc_print(__VA_ARGS__)  //!< if DEBUG is defined, Write Warning message with warning tag to debug output
  #define CCERROR(...)    CcMalloc_print(__VA_ARGS__)  //!< if DEBUG is defined, Write Error message with error tag to debug output
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

//! MemoryMonitor functions to track used memories.
//! @{
#if defined(MEMORYMONITOR_ENABLED) && !defined(NO_CCOS) && !defined(CCMONITOR_IGNORE) && defined(FULL_OS_AVAILABLE)
  extern void CcKernelSHARED CcMemoryMonitor__remove(const void* pBuffer);
  extern void CcKernelSHARED CcMemoryMonitor__insert(const void* pBuffer, const char* pFile, int iLine);
  #define CCMONITORNEW(VAR) CcMemoryMonitor__insert(static_cast<void*>(VAR), __FILE__, __LINE__)
  #define CCMONITORDELETE(VAR) CcMemoryMonitor__remove(static_cast<void*>(VAR))
#else
  #define CCMONITORNEW(VAR)    CCUNUSED(VAR)
  #define CCMONITORDELETE(VAR) CCUNUSED(VAR)
#endif
//! @}

//! MemoryManaging functions for allocating memory and track them
//! @{
#define CCNEW(VAR,TYPE,...)   \
  VAR = new TYPE(__VA_ARGS__);\
  CCMONITORNEW(VAR)
#define CCNEWTYPE(VAR,TYPE,...)     \
  TYPE* VAR = new TYPE(__VA_ARGS__);\
  CCMONITORNEW(VAR)
#define CCNEWARRAY(VAR,TYPE,SIZE)   \
  VAR = new TYPE[SIZE]();             \
  CCMONITORNEW(VAR)
#define CCNEWARRAYTYPE(VAR,TYPE,SIZE)   \
  TYPE* VAR = new TYPE[SIZE]();           \
  CCMONITORNEW(VAR)
#define CCNEWARRAYINIT(VAR,TYPE,SIZE,...)  \
  VAR = new TYPE[SIZE]{__VA_ARGS__};       \
  CCMONITORNEW(VAR)
#define CCNEWARRAYTYPEINIT(VAR,TYPE,SIZE,...) \
  TYPE* VAR = new TYPE[SIZE]{__VA_ARGS__};    \
  CCMONITORNEW(VAR)
  //! For masking new`s wich are known and okay, use this!
#define CCKNOWNNEW new
//! @}

/**
 * @brief Check if null, then delete a variable, remove it from monitoring if running and set variable to null.
 * @param VAR: Variable to delete
 */
#define CCDELETE(VAR) if(VAR!=nullptr){CCMONITORDELETE(VAR);delete VAR;VAR = nullptr;}void(0)

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
#define CCDELETEARRAY(VAR) CCDELETEARR(VAR)

//! @group Cc return states
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
  #define CCMOVE(VAR) std::move(VAR)
#endif

#if !defined(NOEXCEPT)
  #if defined(__clang__)
    #if __has_feature(cxx_noexcept)
      #define HAS_NOEXCEPT
    #endif
  #else
    #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
      #define HAS_NOEXCEPT
    #elif !defined(_GLIBCXX_USE_NOEXCEPT)
      #define _GLIBCXX_USE_NOEXCEPT
    #endif
  #endif

  #ifdef HAS_NOEXCEPT
    #define NOEXCEPT noexcept
  #else
    #define NOEXCEPT
  #endif
#endif

#if !defined(NOEXCEPT_IMPLICIT)
  #if defined(__GXX_EXPERIMENTAL_CXX0X__) && __GNUC__ * 10 + __GNUC_MINOR__ >= 46 || defined(_MSC_FULL_VER) && _MSC_FULL_VER >= 190023026
    #define HAS_NOEXCEPT_IMPLICIT
  #endif
  #ifdef HAS_NOEXCEPT_IMPLICIT
    #define NOEXCEPT_IMPLICIT noexcept
  #else
    #define NOEXCEPT_IMPLICIT
  #endif
#endif

#define cat(a,...) cat_impl(a, __VA_ARGS__)
#define cat_impl(a,...) a ## __VA_ARGS__

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

#define CCDEFINE_EQUAL_OPERATORS(CLASS) \
      inline bool operator==(const CLASS&) const { return false; }\
      inline bool operator!=(const CLASS&) const { return true;  }
#define CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CLASS) \
      CLASS(const CLASS& oToCopy) NOEXCEPT { operator=(oToCopy); }
#define CCDEFINE_MOVE_CONSTRUCTOR_TO_OPERATOR(CLASS) \
      CLASS(CLASS&& oToMove) NOEXCEPT { operator=(CCMOVE(oToMove)); }
#define CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CLASS) \
      CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CLASS)\
      CCDEFINE_MOVE_CONSTRUCTOR_TO_OPERATOR(CLASS)

#define CCDEFINE_EQUAL_OPERATORS(CLASS) \
      inline bool operator==(const CLASS&) const { return false; }\
      inline bool operator!=(const CLASS&) const { return true;  }

#define CCDEFINE_COPY_DENIED(CLASS)         \
      CLASS(const CLASS& oToCopy) = delete; \
      CLASS(CLASS&& oToMove) = delete;

#define CCMACRO_TO_STRING(x)         #x
#define CCMACRO_TO_VALUE(x)         CCMACRO_TO_STRING(x)
#define CCMACRO_TO_STRING_EQ_VALUE(VAR)   #VAR " = "  CCMACRO_TO_VALUE(VAR)

#define CCSIZEOFARRAY(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

#ifndef _GLIBCXX_THROW
  #define _GLIBCXX_THROW(BLAH)
#endif
#ifndef _GLIBCXX_USE_NOEXCEPT
  #define _GLIBCXX_USE_NOEXCEPT
#endif
#ifndef WINDOWSKERNEL
  #define WINCEXPORT
#else
  #define WINCEXPORT __cdecl
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
  int WINCEXPORT main(int uiArgc, char** pcArgv);
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

#endif // H_CcBASE_H_
