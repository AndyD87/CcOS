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
#ifndef _CcImageData_H_
#define _CcImageData_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcByteArray.h"

/**
 * @brief Type of Image in Buffer
 */
enum class EImageType : uint16
{
  Unknown = 0,
  Raw = 1,
  Jpeg = 2,
  Png = 3,
  Bmp = 4,
  Gif = 5
};

/**
 * @brief Example Class impelmentation
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
  void setBuffer(const CcByteArray &oToCopy, EImageType Type = EImageType::Raw);

  /**
   * @brief Get Image Data as Array
   * @param Type: Type of Image to return, eNoImage will return image in stored format
   * @return Buffer with Image
   */
  virtual const CcByteArray& getBuffer();

  /**
   * @brief Get Type of Image currently stored in Class
   * @return Returns the current format of picture in Buffer, if no picture is in buffer
   *         type is set to eNoImage;
   */
  virtual EImageType getType(void);

private:
  EImageType  m_Type;   //!< Type of Image actually stored in Buffer
  CcByteArray m_Buffer; //!< Buffer for whole Image
};

#endif /* _CcImageData_H_ */
