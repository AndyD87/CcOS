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
 * @brief     Implemtation of class CcKernelModule
 */

#include "CcKernelModule.h"
#include "CcMalloc.h"
#include "CcString.h"

/**
 * @brief Overloaded new method for CcOS to track allocations and
 *        to share buffer regions over modules
 * @param uiSize: Number of bytes to allocate
 * @return Pointer to allocated buffer or nullptr if error
 */
void* WINCEXPORT operator new(size_t uiSize) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(uiSize);
}

/**
 * @brief Overloaded new method for CcOS to track allocations and
 *        to share buffer regions over modules
 * @param uiSize: Number of bytes to allocate
 * @return Pointer to allocated buffer or nullptr if error
 */
void* WINCEXPORT operator new[](size_t uiSize) _GLIBCXX_THROW(std::bad_alloc)
{
    return CcMalloc_malloc(uiSize);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 */
void WINCEXPORT operator delete(void *pBuffer) NOEXCEPT_IMPLICIT
{
    CcMalloc_free(pBuffer);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 */
void WINCEXPORT operator delete[](void *pBuffer) NOEXCEPT_IMPLICIT
{
    CcMalloc_free(pBuffer);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 * @param uiSize:  Ignored param, pBuffer will be deleted completly
 */
void WINCEXPORT operator delete(void *pBuffer, size_t CCUNUSED_PARAM(uiSize)) NOEXCEPT_IMPLICIT
{
  CcMalloc_free(pBuffer);
}

/**
 * @brief Overloaded delete method for CcOS to track deallocations and
 *        to manage shared buffer regions over modules
 * @param pBuffer: Address of memory to delete
 * @param uiSize:  Ignored param, pBuffer will be deleted completly
 */
void WINCEXPORT operator delete[](void *pBuffer, size_t CCUNUSED_PARAM(uiSize)) NOEXCEPT_IMPLICIT
{
    CcMalloc_free(pBuffer);
}

/**
 * @brief Deinitialize object array by calling a destructor for all objects
 * @param pArray:   Pointer to array to destruct
 * @param uiSize:   Size of single element in array
 * @param uiCount:  Number of Elements in Array
 * @param pDtor:    Pointer to destructor method
 */
void WINCEXPORT __ehvec_dtor(
  void*       pArray,
  size_t      uiSize,
  size_t      uiCount,
  void(WINCEXPORT *pDtor)(void*)
)
{
  _Analysis_assume_(uiCount > 0);

  int success = 0;

  // Advance pointer past end of array
  pArray = (char*) pArray + uiSize * uiCount;

  __try
  {
    // Destruct elements
    while (uiCount-- > 0)
    {
      pArray = (char*) pArray - uiSize;
      (*pDtor)(pArray);
    }
    success = 1;
  }
  __finally
  {
  }
}

/**
 * @brief Initialize object array by calling a destructor for all objects
 * @param pArray:   Pointer to array to destruct
 * @param uiSize:   Size of single element in array
 * @param uiCount:  Number of Elements in Array
 * @param pDtor:    Pointer to destructor method
 */
void WINCEXPORT __ehvec_ctor(
  void*       pArray,                // Pointer to array to destruct
  size_t      uiSize,               // Size of each element (including padding)
  size_t      uiCount,              // Number of elements in the array
  void(WINCEXPORT *pCtor)(void*),   // Constructor to call
  void(WINCEXPORT *pDtor)(void*)    // Destructor to call should exception be thrown
)
{
  size_t uiPos = 0;      // Count of elements constructed
  int success = 0;

  __try
  {
    // Construct the elements of the array
    for (; uiPos < uiCount; uiPos++)
    {
      (*pCtor)(pArray);
      pArray = (char*) pArray + uiSize;
    }
    success = 1;
  }
  __finally
  {
    // Cleanup by calling destructors until position uiPos
    if (!success)
      __ehvec_dtor(pArray, uiSize, uiPos, pDtor);
  }
}

/**
 * @brief Terminate method for c runtime
 */
void terminate()
{
    //kern_log("terminate requested\n");
}

/**
 * @brief replacement for pure virtual function calls
 */
extern "C" void __cxa_pure_virtual()
{
    //kern_log("cxa_pure_virtual error handler\n");
}

/**
 * @brief Exit method for method for c runtime
 * @param destructor: Method to call on exit
 * @param arg: unused
 * @param dso: unused
 */
extern "C" int __cxa_atexit(void (*destructor) (void *), void *arg, void *dso)
{
  CCUNUSED(destructor);
  CCUNUSED(arg);
  CCUNUSED(dso);
  return 0;
}

/**
 * @brief Finalize methods for try calls without fetch
 * @param f: unused
 */
extern "C" void __cxa_finalize(void *f)
{
  CCUNUSED(f);
}

//! @brief global variable for bare metal builds
void* __dso_handle = nullptr;
