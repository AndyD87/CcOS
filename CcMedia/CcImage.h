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
#ifndef H_CcImage_H_
#define H_CcImage_H_

#include "CcBase.h"
#include "CcMedia.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "Types/CcImageData.h"

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CcImage : public CcImageData
{
public:

  /**
   * @brief Constructor
   */
  CcImage();
  CcImage(const CcImage &oToCopy);

  /**
   * @brief Destructor
   */
  virtual ~CcImage();

  /**
   * @brief Read an Imagefile to Buffer
   * @param sPathToFile: Path to File
   * @return true if Image so successfully loaded
   */
  bool loadFile(const CcString& sPathToFile);

  /**
   * @brief Write image to file.
   * @param sPathToFile: Path to File
   * @return true if Image so successfully loaded
   */
  bool writeFile(const CcString& sPathToFile);

  /**
   * @brief Convert current Image
   * @param Type: Type Image has to be converted to.
   * @param Settings: Setting of target Image to convert to
   * @return true if conversion succeeded.
   */
  bool convert(EImageType Type, void* Settings);
};

#endif // H_CcImage_H_
