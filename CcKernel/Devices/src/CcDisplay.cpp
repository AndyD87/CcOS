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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcDisplay
 */
#include "Devices/CcDisplay.h"
#include "Devices/CcDisplayArea.h"

CcDisplay::CcDisplay(const CcSize& oSize) :
  m_oSize(oSize)
{}

CcDisplay::~CcDisplay() {

  // Auto-generated destructor stub
}

//void CcDisplay::resetAddress( void )
//{
//  setAddress( 0, 0, m_SizeX, m_SizeY);
//}

void CcDisplay::nextCursor(void)
{
  if (m_oCursor.getX() < m_DrawSizeX - 1)
  {
    m_oCursor.setX(m_oCursor.getX()+1);
  }
  else{
    m_oCursor.setX(m_DrawStartX);
    if (m_oCursor.getY() < m_DrawSizeY - 1)
    {
      m_oCursor.setY(m_oCursor.getY() +1 );
    }
    else{
      m_oCursor.setY(m_DrawStartY);
    }
  }
}
