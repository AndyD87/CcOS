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
 * @subpage   IPixMap
 *
 * @page      IPixMap
 * @par       Language: C++11
 * @brief     Class IPixMap
 */

#ifndef H_IPixMap_H_
#define H_IPixMap_H_

#include "CcBase.h"
#include "CcBase.h"
#include "IDevice.h"
#include "CcSize.h"

class CcColor;
class CcRectangle;

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED IPixMap : public IDevice
{
public:
  enum class EType
  {
    GrayScale,
    Color,
  };

  virtual EType getType() const = 0;
  virtual bool setPixelArea(const CcRectangle& oArea) = 0;
  virtual const CcSize& getSize() const = 0;
  virtual void drawPixel(int32 uiX, int32 uiY, const CcColor& oValue) = 0;

  inline int32 getWidth()
    { return getSize().getWidth(); }
  inline int32 getHeight()
    { return getSize().getHeight(); }
};

#endif // _IPixMap_H_
