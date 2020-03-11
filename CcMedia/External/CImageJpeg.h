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
 * @subpage   CImageJpeg
 *
 * @page      CImageJpeg
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CImageJpeg
 */
#ifndef H_CImageJpeg_H_
#define H_CImageJpeg_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcMedia.h"

namespace NImage
{

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CImageJpeg
{
public:
  /**
   * @brief Constructor
   */
  CImageJpeg();

  /**
   * @brief Destructor
   */
  virtual ~CImageJpeg();

};

} // namespace NImage

#endif // H_CImageJpeg_H_
