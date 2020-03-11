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
 * @subpage   CcImageRaw
 *
 * @page      CcImageRaw
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcImageRaw
 */
#ifndef H_CcImageRaw_H_
#define H_CcImageRaw_H_

#include "CcBase.h"
#include "CcMedia.h"
#include "CcByteArray.h"
#include "CcSize.h"

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CcImageRaw : protected CcSize
{
public:
  typedef union
  {
    uint32 uiValue;
    uint8  pValue[4];
    struct
    {
      uint8 R;
      uint8 G;
      uint8 B;
      uint8 A;
    } RGBA;
  } CPixel;

  /**
   * @brief Constructor
   */
  CcImageRaw();
  CcImageRaw(uint32 uiX, uint32 uiY);
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcImageRaw)
  ~CcImageRaw();

  CcImageRaw& operator=(const CcImageRaw& oToCopy);
  CcImageRaw& operator=(CcImageRaw&& oToCopy);

  void resize(uint32 uiX, uint32 uiY);

  CPixel& getPixel(uint32 uiX, uint32 uiY);
  uint8& getPixelR(uint32 uiX, uint32 uiY) const;
  uint8& getPixelG(uint32 uiX, uint32 uiY) const;
  uint8& getPixelB(uint32 uiX, uint32 uiY) const;
  uint8& getPixelA(uint32 uiX, uint32 uiY) const;
  void setPixel(uint32 X, uint32 Y, uint8 R, uint8 G, uint8 B, uint8 A);

  inline uint32 getX() const
    { return CcSize::getWidth(); }
  inline uint32 getY() const
    { return CcSize::getHeight(); }
  inline uint32 getWidth() const
    { return CcSize::getWidth(); }
  inline uint32 getHeight() const
    { return CcSize::getHeight(); }

  uint32 getBufferSize()
  { return m_uiBufferSize; }

  void clear();
private:
  uint32      m_uiBufferSize = 0;
  CPixel*     m_pData = nullptr;
};

#endif // H_CcImageRaw_H_
