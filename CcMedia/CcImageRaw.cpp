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
#include "CcStatic.h"
#include "Private/IImageConverter.h"

CcImageRaw::CcImageRaw()
{
}

CcImageRaw::CcImageRaw(uint32 uiX, uint32 uiY) :
  CcSize(uiX, uiY)
{
  resize(uiX, uiY);
}

CcImageRaw::~CcImageRaw()
{
  CCDELETEARR(m_pData);
}

CcImageRaw& CcImageRaw::operator=(const CcImageRaw& oToCopy)
{
  resize(oToCopy.getX(), oToCopy.getY());
  CcStatic::memcpy(m_pData, oToCopy.m_pData, m_uiBufferSize);
  return *this;
}

CcImageRaw& CcImageRaw::operator=(CcImageRaw&& oToCopy)
{
  if (this != &oToCopy)
  {
    setSize(oToCopy.getX(), oToCopy.getY());
    m_pData = oToCopy.m_pData;
    oToCopy.setSize(0, 0);
    oToCopy.m_pData = nullptr;
  }
  return *this;
}

void CcImageRaw::resize(uint32 uiX, uint32 uiY)
{
  CCDELETEARR(m_pData);
  CCNEWARRAY(m_pData, CPixel, uiX*uiY);
  m_uiBufferSize = uiX * uiY * sizeof(CPixel);
  setSize(uiX, uiY);
#ifdef DEBUG
  CcStatic::memset(m_pData, 0xff, m_uiBufferSize);
#endif
}

CcImageRaw::CPixel& CcImageRaw::getPixel(uint32 uiX, uint32 uiY)
{
  return m_pData[uiX + (uiY * getX())];
}

uint8& CcImageRaw::getPixelR(uint32 uiX, uint32 uiY) const
{
  return m_pData[uiX + (uiY * getX())].RGBA.R;
}

uint8& CcImageRaw::getPixelG(uint32 uiX, uint32 uiY) const
{
  return m_pData[uiX + (uiY * getX())].RGBA.G;
}

uint8& CcImageRaw::getPixelB(uint32 uiX, uint32 uiY) const
{
  return m_pData[uiX + (uiY * getX())].RGBA.B;
}

uint8& CcImageRaw::getPixelA(uint32 uiX, uint32 uiY) const
{
  return m_pData[uiX + (uiY * getX())].RGBA.A;
}

void CcImageRaw::setPixel(uint32 X, uint32 Y, uint8 R, uint8 G, uint8 B, uint8 A)
{
  uint32 uiPos = X + (Y * getX());
  m_pData[uiPos].RGBA.R = R;
  m_pData[uiPos].RGBA.G = G;
  m_pData[uiPos].RGBA.B = B;
  m_pData[uiPos].RGBA.A = A;
}

void CcImageRaw::clear()
{
  CCDELETEARR(m_pData);
  setSize(0, 0);
}