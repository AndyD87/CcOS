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

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CcImageRaw
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
  CcImageRaw(size_t uiX, size_t uiY);
  ~CcImageRaw();

  void resize(size_t uiX, size_t uiY);

  CPixel& getPixel(size_t uiX, size_t uiY);
  uint8& getPixelR(size_t uiX, size_t uiY) const;
  uint8& getPixelG(size_t uiX, size_t uiY) const;
  uint8& getPixelB(size_t uiX, size_t uiY) const;
  uint8& getPixelA(size_t uiX, size_t uiY) const;
  void setPixel(size_t X, size_t Y, uint8 R, uint8 G, uint8 B, uint8 A);

  size_t getBufferSize()
  { return m_uiBufferSize; }

private:
  size_t      m_uiBufferSize = 0;
  size_t      m_uiX = 0;
  size_t      m_uiY = 0;
  CPixel*     m_pData = nullptr;
};

#endif // H_CcImageRaw_H_
