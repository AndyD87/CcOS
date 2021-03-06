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
 * @brief     Implementation of class CcRectangle
 */
#include "CcRectangle.h"


bool CcRectangle::checkPoint(const CcPoint& oPoint) const
{
  if (oPoint >= getPoint() &&
      oPoint <= getBottomRightCorner())
  {
    return true;
  }
  return false;
}

bool CcRectangle::checkPoint(const CcRectangle& oRectangle) const
{
  if (oRectangle.getPoint() >= getPoint() &&
    oRectangle.getBottomRightCorner() <= getBottomRightCorner())
  {
    return true;
  }
  return false;
}

bool CcRectangle::isInside(const CcRectangle& oRectangle) const
{
  if( checkPoint(oRectangle) &&
      checkPoint(oRectangle.getBottomRightCorner()))
  {
    return true;
  }
  return false;
}

void CcRectangle::addBorderSize(int iSize)
{
  setX(getX() + iSize);
  setY(getY() + iSize);
  setWidth(getWidth() - (iSize*2));
  setHeight(getHeight() - (iSize*2));
}

CcPoint CcRectangle::getTopLeftCorner() const
{
  return getPoint();
}
CcPoint CcRectangle::getTopRightCorner() const
{
  return CcPoint(getX() + getWidth() - 1, getY());
}
CcPoint CcRectangle::getBottomLeftCorner() const
{
  return CcPoint(getX(), getY() + getHeight() - 1);
}
CcPoint CcRectangle::getBottomRightCorner() const
{
  return CcPoint(getX() + getWidth() - 1, getY() + getHeight() - 1);
}

CcRectangle CcRectangle::operator+(const CcRectangle& oToAdd) const
{
  CcRectangle oNewPoint(*this);
  oNewPoint.setSize(getSize() + oToAdd.getSize());
  return oNewPoint;
}

CcRectangle CcRectangle::operator+(const CcSize& oToAdd) const
{
  CcRectangle oNewSize(*this);
  oNewSize.setSize(getSize() + oToAdd);
  return oNewSize;
}

CcRectangle CcRectangle::operator+(const CcPoint& oToAdd) const
{
  CcRectangle oNewPoint(*this);
  oNewPoint.setPoint( getPoint() + oToAdd);
  return oNewPoint;
}

CcRectangle CcRectangle::operator-(const CcRectangle& oToAdd) const
{
  CcRectangle oNewPoint(*this);
  oNewPoint.setSize(getSize() - oToAdd.getSize());
  return oNewPoint;
}

CcRectangle CcRectangle::operator-(const CcSize& oToAdd) const
{
  CcRectangle oNewSize(*this);
  oNewSize.setSize(getSize() - oToAdd);
  return oNewSize;
}

CcRectangle CcRectangle::operator-(const CcPoint& oToAdd) const
{
  CcRectangle oNewPoint(*this);
  oNewPoint.setPoint(getPoint() - oToAdd);
  return oNewPoint;
}
