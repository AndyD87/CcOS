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
 * @brief     Implementation of class CcSizeRelative
 */
#include "CcSizeRelative.h"
#include "CcRectangle.h"

bool CcSizeRelative::isInside(const CcRectangle& oRectangle)
{
  bool bIsIn = false;
  if(oRectangle.getBottomRightCorner().getX() < getWidth() &&
     oRectangle.getBottomRightCorner().getY() < getHeight() )
  {
    bIsIn = true;
  }
  return bIsIn;
}

CcSizeRelative CcSizeRelative::operator+(const CcSizeRelative& oToAdd) const
{
  CcSizeRelative oNewPoint(*this);
  oNewPoint.m_fWidth += oToAdd.m_fWidth;
  oNewPoint.m_fHeight += oToAdd.m_fHeight;
  return oNewPoint;
}

CcSizeRelative CcSizeRelative::operator-(const CcSizeRelative& oToAdd) const
{
  CcSizeRelative oNewPoint(*this);
  oNewPoint.m_fWidth -= oToAdd.m_fWidth;
  oNewPoint.m_fHeight -= oToAdd.m_fHeight;
  return oNewPoint;
}

bool CcSizeRelative::operator<(const CcSizeRelative& oToCompare) const
{
  if (getWidth() < oToCompare.getWidth() &&
      getHeight() < oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSizeRelative::operator>(const CcSizeRelative& oToCompare) const
{
  if (getWidth() < oToCompare.getWidth() &&
    getHeight() < oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSizeRelative::operator<=(const CcSizeRelative& oToCompare) const
{
  if (getWidth() <= oToCompare.getWidth() &&
    getHeight() <= oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSizeRelative::operator>=(const CcSizeRelative& oToCompare) const
{
  if (getWidth() >= oToCompare.getWidth() &&
      getHeight() >= oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

void CcSizeRelative::operator+=(const CcSizeRelative& oToAdd)
{
  m_fWidth += oToAdd.m_fWidth;
  m_fHeight += oToAdd.m_fHeight;
}

void CcSizeRelative::operator-=(const CcSizeRelative& oToAdd)
{
  m_fWidth -= oToAdd.m_fWidth;
  m_fHeight -= oToAdd.m_fHeight;
}
