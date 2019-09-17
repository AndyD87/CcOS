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
 * @subpage   CcStatic
 *
 * @page      CcStatic
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStatic
 */
#ifndef H_CcStatic_H_
#define H_CcStatic_H_

#include "CcBase.h"
#include "CcKernelBase.h"

#define CcStatic_memsetZeroObject(OBJECT) CcStatic::memset(&OBJECT, 0, sizeof(OBJECT));
#define CcStatic_memsetZeroPointer(POINTER) CcStatic::memset(POINTER, 0, sizeof(*POINTER));

/**
 * @brief Static class common methods like memset.
 */
class CcKernelSHARED CcStatic 
{
public:
  /**
   * @brief Overwrite each byte of a buffer with a new iValue.
   * @param pBuffer: Target buffer to overwrite
   * @param iValue: new value to set on each byte.
   * @param uiSize: Size of pBuffer.
   * @return Pointer to pBuffer;
   * @todo add to TestFramework
   */
  static void* memset(void* pBuffer, int iValue, size_t uiSize);

  static int memcmp(const void* pBuffer1, const void* pBuffer2, size_t uiLen);

  /**
   * @brief Fill a memory with 0 values.
   * @param pBuffer: Target buffer to fill with 0.
   * @param uiSize: Size of pBuffer.
   * @return Pointer to pBuffer;
   * @todo add to TestFramework
   */
  static void* zerofill(void* pBuffer, size_t uiSize);

  /**
   * @brief Copy content of buffers.
   * @param pDestination: Target buffer copy content to
   * @param pSource: Source buffer copy content from
   * @param uiSize: Number of Bytes to copy from pSource to pDestination
   * @return Pointer to pDestination;
   * @todo add to TestFramework
   */
  static void* memcpy(void* pDestination, const void* pSource, size_t uiSize);

  /**
   * @brief Copy content of buffers in reverse order.
   * @param pDestination: Target buffer copy content to
   * @param pSource: Source buffer copy content from
   * @param uiSize: Number of Bytes to copy from pSource to pDestination
   * @return Pointer to pDestination;
   * @todo add to TestFramework
   */
  static void* memcpySwapped(void* pDestination, const void* pSource, size_t uiSize);

  //! @brief  get a reference to an null object
  //!         Never access this object. It should be just used as an invalid return value of methods.
  //!         To check if an object of this type is set, use ISNULLREF makro.
  template <typename X>
  static X& getNullRef()
  { return  (*(reinterpret_cast<X*>(g_pNull))); }

  //! @brief  get a reference to an null object
  //!         Never access this object. It should be just used as an invalid return value of methods.
  //!         To check if an object of this type is set, use ISNULLREF makro.
  template <typename X>
  static const X& getConstNullRef()
  { return  (*(reinterpret_cast<X*>(g_pNull))); }

  /**
   * @brief Lock memory so it will be forced to keep in RAM and not swaped out.
   * @param pMemory: Target Buffer to lock
   * @param uiSize:  Number of bytes to be locked within pMemory
   * @return CcStatus
   */
  static CcStatus mlock(void *pMemory, size_t uiSize);

  /**
   * @brief Unlock memory so it will become back the ability to be swaped out.
   * @param pMemory: Target Buffer to lock
   * @param uiSize:  Number of bytes to be locked within pMemory
   * @return CcStatus
   */
  static CcStatus munlock(void *pMemory, size_t uiSize);

  static void swap(void* pBuffer, size_t uiSize);
  static uint64 swapUint64(uint64 uiToSwap);
  static uint32 swapUint32(uint32 uiToSwap);
  static uint16 swapUint16(uint16 uiToSwap);
  static int64 swapInt64(int64 iToSwap)
    { return static_cast<int64>(swapUint64(static_cast<uint64>(iToSwap))); }
  static int32 swapInt32(int32 iToSwap)
    { return static_cast<int32>(swapUint32(static_cast<uint32>(iToSwap))); }
  static int16 swapInt16(int16 iToSwap)
    { return static_cast<int16>(swapUint16(static_cast<uint16>(iToSwap))); }

private:
  static void* g_pNull;
};

#endif // H_CcStatic_H_
