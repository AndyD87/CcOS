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
 * @page      Devices
 * @subpage   IGrayscaleMap
 *
 * @page      IGrayscaleMap
 * @par       Language: C++11
 * @brief     Class IGrayscaleMap
 */

#ifndef H_IGrayscaleMap_H_
#define H_IGrayscaleMap_H_

#include "CcBase.h"
#include "CcBase.h"
#include "IPixMap.h"

class CcColor;

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED IGrayscaleMap : public IPixMap
{
public:
  virtual EType getType() const override;
  virtual void drawPixel(int32 uiX, int32 uiY, const CcColor& oValue) override;

  virtual void drawPixel(int32 uiX, int32 uiY, uint8 uiValue) = 0;
};

#endif // _IGrayscaleMap_H_
