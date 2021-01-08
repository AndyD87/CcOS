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
 * @subpage   IImageConverter
 *
 * @page      IImageConverter
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IImageConverter
 */
#ifndef H_IImageConverter_H_
#define H_IImageConverter_H_

#include "CcBase.h"
#include "CcMedia.h"
#include "CcByteArray.h"
#include "CcImageData.h"
#include "CcImageRaw.h"

namespace NImage
{

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED IImageConverter
{
public:
  virtual ~IImageConverter() = default;
  virtual bool checkType(EImageType eType) = 0;
  virtual EImageType checkFormat(const CcByteArray& oToCheck) = 0;
  virtual CcImageRaw convertToRaw(const CcByteArray& oInput) = 0;
  virtual CcByteArray convertFromRaw(const CcImageRaw& oInput) = 0;

  void registerConverter();
  void deregisterConverter();
};

} // namespace NImage

#endif // H_IImageConverter_H_
