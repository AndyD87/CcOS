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
 * @subpage   CcBufferList
 *
 * @page      CcBufferList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcBufferList
 */

#ifndef _CcBufferList_H_
#define _CcBufferList_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcList.h"
#include "CcByteArray.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcByteArray>;
#endif

/**
 * @brief This Buffer is designed to transport Buffer in a chunked list of memory.
 */
class CcKernelSHARED CcBufferList : public CcList<CcByteArray>
{
public:
  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param buffer: Buffer to load data to.
   * @param size: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t read(void* pBuffer, size_t uSize);

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t write(const void* pBuffer, size_t uSize);

  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param buffer: Buffer to load data to.
   * @param size: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t readAll(void* pBuffer, size_t uSize) const;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t writeAll(const void* pBuffer, size_t uSize);

  /**
   * @brief Write an amount of Data to this buffer
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  CcBufferList& append(const CcByteArray& oByteArray);
  /**
   * @brief Write an amount of Data to this buffer
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  CcBufferList& append(CcByteArray&& oByteArray);
  CcBufferList& append(const CcString& sString);
  CcBufferList& append(CcString&& sString);

  /**
   * @brief Write an amount of Data to this buffer
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  CcBufferList& append(const void* pBuffer, size_t uSize);

  /**
   * @brief Transfer an amount of Data to this buffer.
   *        The buffer will take the ownership and delete/overwrite it.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  void transferBegin(void* pBuffer, size_t uSize);
  void transfer(void* pBuffer, size_t uSize);

  void setPosition(size_t uiPosition)
    { m_uiPosition = uiPosition; }
  size_t size() const
    { return m_uiSize; }
  size_t getChunkCount() const
    { return CcList<CcByteArray>::size(); }

  void clear();
  void collapse();
  void* getBuffer()
    { collapse(); return at(0).getArray(); }
  void* getCurrentBuffer() const
    { return (at(0).getArray() + m_uiPosition); }
  size_t getCurrentSize() const
    { return m_uiSize - m_uiPosition; }

  CcBufferList& operator=(const CcByteArray& oByteArray)
    { clear(); return append(oByteArray); }
  CcBufferList& operator=(CcByteArray&& oByteArray)
    { clear(); return append(std::move(oByteArray)); }
  CcBufferList& operator+=(const CcByteArray& oByteArray)
    { return append(oByteArray); }
  CcBufferList& operator+=(CcByteArray&& oByteArray)
    { return append(std::move(oByteArray)); }

private:
  size_t              m_uiPosition = 0;
  size_t              m_uiSize = 0;
};

#endif /* _CcBufferList_H_ */
