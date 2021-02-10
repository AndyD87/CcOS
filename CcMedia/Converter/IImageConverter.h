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
 * @brief     Class IImageConverter
 */
#pragma once

#include "CcBase.h"
#include "CcMedia.h"
#include "CcByteArray.h"
#include "CcImageData.h"
#include "CcImageRaw.h"

namespace NImage
{

/**
 * @brief Interface class for image converter.
 *        Minimum required interface to support this mechanism is defined here
 *        and must be implemented from each inheriting object.
 */
class CcMediaSHARED IImageConverter
{
public:
  virtual ~IImageConverter() = default;

  /**
   * @brief Check if this converter can generate queried type
   * @param eType: Type to query for
   * @return True if this converter supports @p eType
   */
  virtual bool checkType(EImageType eType) = 0;

  /**
   * @brief Check if data could be read from this converter and
   *        a valid EImageType could be found.
   * @param oToCheck: Data process
   * @return Found type or EImageType::Unknown if not
   */
  virtual EImageType checkFormat(const CcByteArray& oToCheck) = 0;

  /**
   * @brief Convert incoming data to raw image.
   * @param oInput: Input data to process
   * @return Converted raw image or empty image on error.
   */
  virtual CcImageRaw convertToRaw(const CcByteArray& oInput) = 0;

  /**
   * @brief Conver raw image to formated image.
   * @param oInput: Incomming raw image to convert
   * @return Converted binary formated image
   */
  virtual CcByteArray convertFromRaw(const CcImageRaw& oInput) = 0;

  /**
   * @brief Register this converter in global image instance
   */
  void registerConverter();

  /**
   * @brief Remove this registered converter from global image instance
   */
  void deregisterConverter();
};

} // namespace NImage
