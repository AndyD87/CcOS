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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcArray
 */
#ifndef CcArray_H_
#define CcArray_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include <cstring>

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
    operator=(std::move(oToMove));
  }

  TYPE* address() 
  {
    return m_pBuffer;
  }

  /**
  * @brief Destructor
  */
  ~CcArray(void) 
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
    if(m_pBuffer != nullptr)
      delete[] m_pBuffer;
    m_uiBufferSize = oBufferSize;
    m_pBuffer = new TYPE[oBufferSize]; 
    CCMONITORNEW(m_pBuffer);
  }

  void deleteBuffer()
  {
    CCMONITORDELETE(m_pBuffer);
    delete[] m_pBuffer;
  }

  size_t size() const
  {
    return m_uiBufferSize;
  }

  TYPE& operator[](size_t uiPos) const
  {
    return m_pBuffer[uiPos];
  }
  
  CcArray& operator=(const CcArray<TYPE>& oToCopy) const
  {
    m_uiBufferSize = oToCopy.m_uiBufferSize.
    memcpy(m_pBuffer, oToCopy.m_pBuffer, oToCopy.m_uiBufferSize);
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

private:
  TYPE* m_pBuffer = nullptr;
  size_t m_uiBufferSize = 0;
};

#endif /* CcArray_H_ */
