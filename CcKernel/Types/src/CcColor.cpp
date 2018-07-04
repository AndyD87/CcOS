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
 * @brief     Class CcColor
 */
#include "Types/CcColor.h"

CcColor::CcColor(uint16 color, eColorType type)
{
  setColor(color,type);
}

CcColor::CcColor(uint32 color, eColorType type)
{
  setColor(color,type);
}

CcColor::CcColor(uint8 R, uint8 G, uint8 B, uint8 A)
{
  setColor(R, G, B, A);
}

CcColor::~CcColor()
{

}

CcColor& CcColor::operator=(const CcColor& rColor)
{
  m_A = rColor.m_A;
  m_R = rColor.m_R;
  m_G = rColor.m_G;
  m_B = rColor.m_B;
  return *this;
}

void CcColor::setColor(uint16 color, eColorType type)
{
  switch(type)
  {
    case eColorARGB:
    case eColorRGB:
      m_R = 0xff & (color >> 9);
      m_G = 0xff & (color >> 2);
      m_B = 0xff & (color << 3);
      m_A = 0;
      break;
    case eColorBGR:
      m_B = 0xff & (color >> 9);
      m_G = 0xff & (color >> 2);
      m_R = 0xff & (color << 3);
      m_A = 0;
      break;
  }
}
void CcColor::setColor(uint32 color, eColorType type)
{
  switch(type)
  {
    case eColorARGB:
      m_A = 0xff & (color >> 24);
      m_R = 0xff & (color >> 16);
      m_G = 0xff & (color >>  8);
      m_B = 0xff & color;
      break;
    case eColorRGB:
      m_R = 0xff & (color >> 16);
      m_G = 0xff & (color >> 8);
      m_B = 0xff & (color);
      m_A = 0;
      break;
    case eColorBGR:
      m_R = 0xff & (color);
      m_G = 0xff & (color >> 8);
      m_B = 0xff & (color >> 16);
      m_A = 0;
      break;
  }
}

void CcColor::setColor(uint8 R, uint8 G, uint8 B, uint8 A)
{
  setColorR(R);
  setColorG(G);
  setColorB(B);
  setColorA(A);
}


uint16 CcColor::getColor16Bit( eColorType )
{
  uint16 uiRet;
  uiRet  = 0x001F & ( m_B >> 3 );
  uiRet |= 0x07E0 & ( m_G << 2 );
  uiRet |= 0xf800 & ( m_R << 9 );
  return uiRet;
}

uint32 CcColor::getColor32Bit( eColorType )
{
  uint32 uiRet;
  uiRet  = 0x000000ff & ( m_B ) ;
  uiRet |= 0x0000ff00 & ( m_G <<  8 );
  uiRet |= 0x00ff0000 & ( m_R << 16 );
  uiRet |= 0xff000000 & ( m_A << 24 );
  return uiRet;
}
