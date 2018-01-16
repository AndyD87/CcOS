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
 * @page      CcMedia
 * @subpage   CcImage
 *
 * @page      CcImage
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcImage
 */
#ifndef _CcImage_H_
#define _CcImage_H_

#include "CcBase.h"
#include "CcMedia.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "Types/CcImageData.h"

/**
 * @brief Example Class impelmentation
 */
class CcMediaSHARED CcImage
{
public:

  /**
   * @brief Constructor
   */
  CcImage( void );

  CcImage(const CcImage &copy);

  /**
   * @brief Destructor
   */
  virtual ~CcImage( void );

  /**
   * @brief Read an Imagefile to Buffer
   * @param sPathToFile: Path to File
   * @return true if Image so successfully loaded
   */
  bool loadFile(const CcString& sPathToFile);

  /**
   * @brief Fill Buffer with raw Data
   * @param oToCopy:  Buffer with image data
   * @param Type: Set type of Image the raw data are from
   */
  void fillBuffer(const CcByteArray &oToCopy, EImageType Type = EImageType::Raw);

  /**
   * @brief Fill Buffer with raw Data
   * @param buffer:  Buffer with image data
   * @param bufSize: Size of Buffer to write to image
   * @param Type: Set type of Image the raw data are from
   */
  void fillBuffer(const char *buffer, size_t bufSize, EImageType Type = EImageType::Raw);

  /**
   * @brief Get Image Data as Array
   * @param Type: Type of Image to return, eNoImage will return image in stored format
   * @return Buffer with Image
   */
  virtual CcByteArray getImageBuffer();

  /**
   * @brief Get Type of Image currently stored in Class
   * @return Returns the current format of picture in Buffer, if no picture is in buffer
   *         type is set to eNoImage;
   */
  virtual EImageType getType(void);

  /**
   * @brief Convert current Image
   * @param Type: Type Image has to be converted to.
   * @param Settings: Setting of target Image to convert to
   * @return true if conversion succeeded.
   */
  bool convert(EImageType Type, void* Settings);

  EImageType  m_Type;   //!< Type of Image actually stored in Buffer
  CcByteArray m_Buffer; //!< Buffer for whole Image
};

#endif /* _CcImage_H_ */
