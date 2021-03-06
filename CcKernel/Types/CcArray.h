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
 * @page      Types
 * @subpage   CcArray
 *
 * @page      CcArray
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcArray
 */
#ifndef H_CcArray_H_
#define H_CcArray_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcStatic.h"

/**
 * @brief Communication Device for I2C
 */
template <typename TYPE>
class CcArray
{
public:
  /**
   * @brief Constructor
   */
  CcArray(const size_t oBufferSize)
  {
    createBuffer(oBufferSize);
  }

  CcArray(const TYPE* oBuffer, size_t uiSize)
  {
    copyBuffer(oBuffer, uiSize);
  }

  CcArray(const CcArray<TYPE>& oToCopy)
  {
    copyBuffer(oToCopy, oToCopy.size());
  }

  CcArray(CcArray<TYPE>&& oToMove)
  {
    operator=(CCMOVE(oToMove));
  }

  TYPE* getArray()
  {
    return m_pBuffer;
  }

  const TYPE* getArray() const
  {
    return m_pBuffer;
  }

  /**
   * @brief Destructor
   */
  ~CcArray() 
  { 
    deleteBuffer();
  }

  void copyBuffer(const TYPE* pBuffer, size_t uiSize)
  {
    createBuffer(uiSize);
    memcpy(m_pBuffer, pBuffer, uiSize);
  }

  void createBuffer(const size_t oBufferSize)
  {
    deleteBuffer();
    m_uiBufferSize = oBufferSize;
    CCNEWARRAY(m_pBuffer,TYPE,oBufferSize);
  }

  void deleteBuffer()
  {
    CCDELETEARR(m_pBuffer);
  }

  size_t size() const
  {
    return m_uiBufferSize;
  }

  TYPE& operator[](size_t uiPos) const
  {
    return m_pBuffer[uiPos];
  }
  
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
  }

  CcArray& operator=(const CcArray<TYPE>& oToCopy) const
  {
    m_uiBufferSize = oToCopy.m_uiBufferSize;
    CcStatic::memcpy(m_pBuffer, oToCopy.m_pBuffer, oToCopy.m_uiBufferSize);
  }

private:
  TYPE* m_pBuffer = nullptr;
  size_t m_uiBufferSize = 0;
};

#endif // H_CcArray_H_
