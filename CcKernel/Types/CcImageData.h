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
 * @subpage   CcImageData
 *
 * @page      CcImageData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcImageData
 */
#ifndef H_CcImageData_H_
#define H_CcImageData_H_

#include "CcBase.h"
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

  /**
   * @brief Constructor
   */
  CcImageData() = default;

  CcImageData(const CcByteArray &oBuffer, EImageType eType);

  /**
   * @brief Destructor
   */
  ~CcImageData() = default;

  /**
   * @brief Fill Buffer with raw Data
   * @param oToCopy:  Buffer with image data
   * @param Type: Set type of Image the raw data are from
   */
  void setBuffer(const CcByteArray &oToCopy, EImageType eType = EImageType::Unknown);

  /**
   * @brief Get Image Data as Array
   * @param Type: Type of Image to return, eNoImage will return image in stored format
   * @return Buffer with Image
   */
  const CcByteArray& getBuffer() const
  { return m_oBuffer; }
  CcByteArray& getBuffer()
  { return m_oBuffer; }

  const CcString& getFileExtension()
  { return getFileExtension(m_eType); }

  static const CcString& getFileExtension(EImageType eType);

  /**
   * @brief Get Type of Image currently stored in Class
   * @return Returns the current format of picture in Buffer, if no picture is in buffer
   *         type is set to eNoImage;
   */
  EImageType getType()
  { return m_eType; }

  const CcDateTime& getTimestamp()
  { return m_oTimestamp; }
  
  void setType(EImageType eType)
  { m_eType = eType; }

  void setTimestamp(const CcDateTime& oTimestamp)
  { m_oTimestamp = oTimestamp; }

  void setTimestampNow();


  CcStatus saveToFile(const CcString& sPathToFile);

protected:
  EImageType  m_eType = EImageType::Unknown;  //!< Type of Image actually stored in Buffer
  CcByteArray m_oBuffer;                      //!< Buffer for whole Image
  CcDateTime  m_oTimestamp;
};

#endif // H_CcImageData_H_
