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
 * @page      CcGui
 * @subpage   CcCross
 *
 * @page      CcCross
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcCross
 */
#ifndef CCCROSS_H
#define CCCROSS_H

#include "CcBase.h"
#include "CcGui.h"

class CcGuiSHARED CcCross
{
public:
  CcCross();
  CcCross(uint16 width, uint16 height, uint16 thickness);
  virtual ~CcCross();
  void setValues(uint16 width, uint16 height, uint16 thickness);
  inline void setDiagonal(bool bDiagonal) { m_bDiagonal = bDiagonal;}
  uint16 m_width  = 0;
  uint16 m_height = 0;
  uint16 m_thick  = 0;
  bool m_bDiagonal = false;
};

#endif // CCCROSS_H
