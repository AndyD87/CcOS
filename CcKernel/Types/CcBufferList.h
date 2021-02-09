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
 * @brief     Class CcBufferList
 */
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "CcByteArray.h"

#ifdef _MSC_VER
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
   * @param pBuffer: Buffer to load data to.
   * @param uSize: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t read(void* pBuffer, size_t uSize);

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param pBuffer: Buffer to load data from.
   * @param uSize: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t write(const void* pBuffer, size_t uSize);

  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param pBuffer: Buffer to load data to.
   * @param uSize: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t readAll(void* pBuffer, size_t uSize) const;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param pBuffer: Buffer to load data from.
   * @param uSize: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t writeAll(const void* pBuffer, size_t uSize);

  /**
   * @brief Append an byte array to buffer by copying data.
   * @param oByteArray: Data to copy
   * @return Handle to this
   */
  CcBufferList& append(const CcByteArray& oByteArray);

  /**
   * @brief Append an byte array to buffer by moving data.
   * @param oByteArray: Data to move
   * @return Handle to this
   */
  CcBufferList& append(CcByteArray&& oByteArray);

  /**
   * @brief Append an string to buffer by copying data.
   * @param sString: String to copy
   * @return Handle to this
   */
  CcBufferList& append(const CcString& sString);

  /**
   * @brief Append an string to buffer by moving data.
   * @param sString: String to move
   * @return Handle to this
   */
  CcBufferList& append(CcString&& sString);

  /**
   * @brief Append data to buffer, without changing file pointer
   * @param pBuffer:  Buffer to copy data from.
   * @param uSize:    Maximum size of buffer to read.
   * @return Handly to this
   */
  CcBufferList& append(const void* pBuffer, size_t uSize);

  /**
   * @brief Prepend an byte array to buffer by copying data.
   * @param oByteArray: Data to copy
   * @return Handle to this
   */
  CcBufferList& prepend(const CcByteArray& oByteArray);

  /**
   * @brief Prepend an byte array to buffer by moving data.
   * @param oByteArray: Data to move
   * @return Handle to this
   */
  CcBufferList& prepend(CcByteArray&& oByteArray);

  /**
   * @brief Prepend an string to buffer by copying data.
   * @param sString: String to copy
   * @return Handle to this
   */
  CcBufferList& prepend(const CcString& sString);

  /**
   * @brief Prepend an string to buffer by moving data.
   * @param sString: String to move
   * @return Handle to this
   */
  CcBufferList& prepend(CcString&& sString);

  /**
   * @brief Prepend data to buffer, without changing file pointer
   * @param pBuffer:  Buffer to copy data from.
   * @param uSize:    Maximum size of buffer to read.
   * @return Handly to this
   */
  CcBufferList& prepend(const void* pBuffer, size_t uSize);

  /**
   * @brief Transfer an amount of Data to the beginning of this buffer.
   *        The buffer will take the ownership and delete/overwrite it.
   * @param pBuffer:  Buffer to load data from.
   * @param uSize:    Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  void transferBegin(void* pBuffer, size_t uSize);

  /**
   * @brief Transfer an amount of Data to the end of this buffer.
   *        The buffer will take the ownership and delete/overwrite it.
   * @param pBuffer:  Buffer to load data from.
   * @param uSize:    Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  void transfer(void* pBuffer, size_t uSize);

  //! @param uiToAdd: Add offset to file pointer
  void addPosition(size_t uiToAdd)
  { m_uiPosition += uiToAdd; }
  //! @param uiPosition: Set offset to file pointer
  void setPosition(size_t uiPosition)
  { m_uiPosition = uiPosition; }
  //! @return Get current size of buffer
  size_t size() const
  { return m_uiSize; }
  //! @return Get number of buffers in list
  size_t getChunkCount() const
  { return CcList<CcByteArray>::size(); }

  /**
   * @brief Clear all buffers
   */
  void clear();
  /**
   * @brief Merge all buffers to one big
   */
  void collapse();

  /**
   * @brief Get pointer to full buffer it will collapse before.
   * @return Pointer full buffer
   */
  void* getBuffer();

  /**
   * @brief Get buffer on current position
   * @return Pointer to current postion
   */
  void* getCurrentBuffer();

  /**
   * @brief Get pointer to last appended chunk of buffer
   * @return Buffer on last postition if available or nullptr
   */
  void* getLastBuffer() const
  { return (size() > 0) ? at(CcList<CcByteArray>::size()-1).getArray():nullptr; }

  /**
   * @brief Get pointer to first buffer.
   * @return Buffer on first postition if available or nullptr
   */
  void* getFirstBuffer() const
  { return (size() > 0) ? at(0).getArray() : nullptr; }

  //! @return Get size left for read from current position
  size_t getCurrentSize() const
  { return m_uiSize - m_uiPosition; }
  //! @return Get current position of file pointer
  size_t getPosition() const
  {return m_uiPosition;}
  //! @return Get Crc32 over while buffer
  uint32 getCrc32();

  /**
   * @brief Clear buffer and copy ByteArray as new buffer.
   * @param oByteArray: New content to copy from
   * @return Handle to this
   */
  CcBufferList& operator=(const CcByteArray& oByteArray)
  { clear(); return append(oByteArray); }

  /**
   * @brief Clear buffer and move data from ByteArray to this.
   * @param oByteArray: Byte array with content to move from
   * @return Handle to this
   */
  CcBufferList& operator=(CcByteArray&& oByteArray)
  { clear(); return append(CCMOVE(oByteArray)); }

  /**
   * @brief Append an byte array to buffer by copying data.
   * @param oByteArray: Data to copy
   * @return Handle to this
   */
  CcBufferList& operator+=(const CcByteArray& oByteArray)
  { return append(oByteArray); }

  /**
   * @brief Append an byte array to buffer by moving data.
   * @param oByteArray: Data to move
   * @return Handle to this
   */
  CcBufferList& operator+=(CcByteArray&& oByteArray)
  { return append(CCMOVE(oByteArray)); }

private:
  size_t              m_uiPosition = 0;
  size_t              m_uiSize = 0;
};
