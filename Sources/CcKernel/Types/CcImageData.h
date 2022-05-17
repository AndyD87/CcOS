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
 * @brief     Class CcImageData
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcDateTime.h"

/**
 * @brief Type of Image in Buffer
 */
enum class EImageType : uint16
{
  Unknown = 0,
  Raw,
  Jpeg,
  Png,
  Bmp,
  Gif,
  Pbm,
  Pgm,
  Ppm
};

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcImageData
{
public:
  CcImageData() = default;
  ~CcImageData() = default;

  /**
   * @brief Create image data with import data.
   * @param oBuffer: Binary data to import
   * @param eType:   Type of data in buffer.
   */
  CcImageData(const CcByteArray &oBuffer, EImageType eType);

  /**
   * @brief Fill Buffer with raw Data
   * @param oToCopy:  Buffer with image data
   * @param eType:    Set type of Image the raw data are from
   */
  void setBuffer(const CcByteArray &oToCopy, EImageType eType = EImageType::Unknown);

  /**
   * @brief Get Image Data as Array
   * @return Binary Buffer from Image
   */
  const CcByteArray& getBuffer() const
  { return m_oBuffer; }

  /**
   * @brief Get Image Data as Array
   * @return Binary Buffer from Image
   */
  CcByteArray& getBuffer()
  { return m_oBuffer; }

  /**
   * @brief Get target file extension for this image
   * @return File extension as string without dot
   */
  const CcString& getFileExtension()
  { return getFileExtension(m_eType); }

  /**
   * @brief Get file extension for specific image type
   * @param eType: Type to query extension for
   * @return File extension as string without dot
   */
  static const CcString& getFileExtension(EImageType eType);

  /**
   * @brief Get Type of Image currently stored in Class
   * @return Returns the current format of picture in Buffer, if no picture is in buffer
   *         type is set to eNoImage;
   */
  EImageType getType()
  { return m_eType; }
  //! @return Get timestamp information of this image
  const CcDateTime& getTimestamp()
  { return m_oTimestamp; }

  //! @param eType: Set type of this image
  void setType(EImageType eType)
  { m_eType = eType; }
  //! @param oTimestamp: Set timestamp of this image
  void setTimestamp(const CcDateTime& oTimestamp)
  { m_oTimestamp = oTimestamp; }
  //! @brief Set timestamp of this image to system time
  void setTimestampNow();

  /**
   * @brief Write image to file
   * @param sPathToFile: Path to file to write to.
   * @return Status of operation.
   */
  CcStatus saveToFile(const CcString& sPathToFile);

protected:
  EImageType  m_eType = EImageType::Unknown;  //!< Type of Image actually stored in Buffer
  CcByteArray m_oBuffer;                      //!< Buffer for whole Image
  CcDateTime  m_oTimestamp;                   //!< Image creation time
};
