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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CImagePixmap
 */
#pragma once

#include "CcBase.h"
#include "CcMedia.h"
#include "IImageConverter.h"

namespace NImage
{

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CImagePixmap : public IImageConverter
{
public:
  /**
   * @brief Constructor
   */
  CImagePixmap();
  virtual ~CImagePixmap();

  virtual bool checkType(EImageType eType) override;
  virtual EImageType checkFormat(const CcByteArray& oToCheck) override;
  virtual CcImageRaw convertToRaw(const CcByteArray& oInput) override;
  virtual CcByteArray convertFromRaw(const CcImageRaw& oInput) override;
};

} // namespace NImage
