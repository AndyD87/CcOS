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
 * @brief     Implementation of class CcSize
 */
#include "Types/CcSize.h"
#include "Types/CcRectangle.h"

bool CcSize::isInside(const CcRectangle& oRectangle)
{
  bool bIsIn = false;
  if(oRectangle.getBottomRightCorner().getX() < getWidth() &&
     oRectangle.getBottomRightCorner().getY() < getHeight() )
  {
    bIsIn = true;
  }
  return bIsIn;
}

CcSize CcSize::operator+(const CcSize& oToAdd) const
{
  CcSize oNewPoint(*this);
  oNewPoint.m_iWidth += oToAdd.m_iWidth;
  oNewPoint.m_iHeight += oToAdd.m_iHeight;
  return oNewPoint;
}

CcSize CcSize::operator-(const CcSize& oToAdd) const
{
  CcSize oNewPoint(*this);
  oNewPoint.m_iWidth -= oToAdd.m_iWidth;
  oNewPoint.m_iHeight -= oToAdd.m_iHeight;
  return oNewPoint;
}

bool CcSize::operator<(const CcSize& oToCompare) const
{
  if (getWidth() < oToCompare.getWidth() &&
      getHeight() < oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSize::operator>(const CcSize& oToCompare) const
{
  if (getWidth() < oToCompare.getWidth() &&
    getHeight() < oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSize::operator<=(const CcSize& oToCompare) const
{
  if (getWidth() <= oToCompare.getWidth() &&
    getHeight() <= oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

bool CcSize::operator>=(const CcSize& oToCompare) const
{
  if (getWidth() >= oToCompare.getWidth() &&
      getHeight() >= oToCompare.getHeight())
  {
    return true;
  }
  return false;
}

void CcSize::operator+=(const CcSize& oToAdd)
{
  m_iWidth += oToAdd.m_iWidth;
  m_iHeight += oToAdd.m_iHeight;
}

void CcSize::operator-=(const CcSize& oToAdd)
{
  m_iWidth -= oToAdd.m_iWidth;
  m_iHeight -= oToAdd.m_iHeight;
}