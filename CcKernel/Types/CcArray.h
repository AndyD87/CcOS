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
 * @brief     Class CcArray
 */
#pragma once

#include "CcBase.h"
#include "CcStatic.h"

/**
 * @brief Array with fixed size
 */
template <typename TYPE>
class CcArray
{
public:
  /**
   * @brief Create array with fixed size
   * @param uiBufferSize: Number of bytes to create
   */
  CcArray(const size_t uiBufferSize)
  {
    createBuffer(uiBufferSize);
  }

  /**
   * @brief Create array with fixed size and copy from existing pBuffer
   * @param pBuffer: Buffer to copy from
   * @param uiSize:  Number of bytes to create array and copy from pBuffer
   */
  CcArray(const TYPE* pBuffer, size_t uiSize)
  {
    copyBuffer(pBuffer, uiSize);
  }

  /**
   * @brief Copy constructor
   * @param oToCopy: Object to copy from
   */
  CcArray(const CcArray<TYPE>& oToCopy)
  {
    copyBuffer(oToCopy.getArray(), oToCopy.size());
  }

  /**
   * @brief Move constructor
   * @param oToMove: Object to move from
   */
  CcArray(CcArray<TYPE>&& oToMove)
  {
    operator=(CCMOVE(oToMove));
  }

  //! @return Pointer to array
  TYPE* getArray()
  { return m_pBuffer; }

  //! @return Get const pointer to array
  const TYPE* getArray() const
  { return m_pBuffer; }

  /**
   * @brief Destructor with buffer cleanup
   */
  ~CcArray()
  {
    deleteBuffer();
  }

  /**
   * @brief Remove curren buffer and create new by copy from pBuffer
   * @param pBuffer: Buffer to copy from
   * @param uiSize:  Number of bytes to create array and copy from pBuffer
   */
  void copyBuffer(const TYPE* pBuffer, size_t uiSize)
  {
    createBuffer(uiSize);
    memcpy(m_pBuffer, pBuffer, uiSize);
  }

  /**
   * @brief Create array with fixed size.
   * @param uiSize: Number of bytes to create array
   */
  void createBuffer(const size_t uiSize)
  {
    deleteBuffer();
    m_uiBufferSize = uiSize;
    CCNEWARRAY(m_pBuffer,TYPE,uiSize);
  }

  /**
   * @brief Delete current buffer.
   */
  void deleteBuffer()
  {
    CCDELETEARR(m_pBuffer);
  }

  //! @return Get size of stored array
  size_t size() const
  { return m_uiBufferSize; }

  /**
   * @brief Get item on index.
   * @param uiPos: Index to get item from
   * @return Handle to indexed object.
   */
  TYPE& operator[](size_t uiPos) const
  {
    return m_pBuffer[uiPos];
  }

  /**
   * @brief Move content form another array to this
   * @param oToMove: Object to move from.
   * @return Handle to this
   */
  CcArray& operator=(CcArray<TYPE>&& oToMove)
  {
    if (&oToMove != this)
    {
      deleteBuffer();
      m_uiBufferSize = oToMove.m_uiBufferSize;
      m_pBuffer = oToMove.m_pBuffer;
      oToMove.m_pBuffer = nullptr;
      oToMove.m_uiBufferSize = 0;
      oToMove.m_pBuffer = 0;
    }
    return *this;
  }

  /**
   * @brief Copy content form another array to this
   * @param oToCopy: Object to copy from.
   * @return Handle to this
   */
  CcArray& operator=(const CcArray<TYPE>& oToCopy)
  {
    m_uiBufferSize = oToCopy.m_uiBufferSize;
    CcStatic::memcpy(m_pBuffer, oToCopy.m_pBuffer, oToCopy.m_uiBufferSize);
    return *this;
  }

private:
  TYPE* m_pBuffer = nullptr;  //!< Pointer to stored array.
  size_t m_uiBufferSize = 0;  //!< Size of m_pBuffer.
};
