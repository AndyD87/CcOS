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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcImageRaw
 */
#include "CcImageRaw.h"
#include "CcFile.h"
#include "Private/IImageConverter.h"

CcImageRaw::CcImageRaw()
{
}

CcImageRaw::CcImageRaw(size_t uiX, size_t uiY)
{
  resize(uiX, uiY);
}

CcImageRaw::~CcImageRaw()
{
  CCDELETEARR(m_pData);
}

void CcImageRaw::resize(size_t uiX, size_t uiY)
{
  CCDELETEARR(m_pData);
  CCNEWARRAY(m_pData, CPixel, uiX*uiY);
  m_uiX = uiX;
  m_uiY = uiY;
  m_uiBufferSize = uiX * uiY * sizeof(CPixel);
}

CcImageRaw::CPixel& CcImageRaw::getPixel(size_t uiX, size_t uiY)
{
  return m_pData[uiX + (uiY *m_uiX)];
}

uint8& CcImageRaw::getPixelR(size_t uiX, size_t uiY) const
{
  return m_pData[uiX + (uiY *m_uiX)].RGBA.R;
}

uint8& CcImageRaw::getPixelG(size_t uiX, size_t uiY) const
{
  return m_pData[uiX + (uiY *m_uiX)].RGBA.G;
}

uint8& CcImageRaw::getPixelB(size_t uiX, size_t uiY) const
{
  return m_pData[uiX + (uiY *m_uiX)].RGBA.B;
}

uint8& CcImageRaw::getPixelA(size_t uiX, size_t uiY) const
{
  return m_pData[uiX + (uiY *m_uiX)].RGBA.A;
}

void CcImageRaw::setPixel(size_t X, size_t Y, uint8 R, uint8 G, uint8 B, uint8 A)
{
  size_t uiPos = X + (Y *m_uiX);
  m_pData[uiPos].RGBA.R = R;
  m_pData[uiPos].RGBA.G = G;
  m_pData[uiPos].RGBA.B = B;
  m_pData[uiPos].RGBA.A = A;
}
