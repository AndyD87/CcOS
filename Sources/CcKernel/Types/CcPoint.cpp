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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcPoint
 */
#include "CcPoint.h"


CcPoint CcPoint::operator+(const CcPoint& oToAdd) const
{
  CcPoint oNewPoint(*this);
  oNewPoint.m_X += oToAdd.m_X;
  oNewPoint.m_Y += oToAdd.m_Y;
  return oNewPoint;
}

CcPoint CcPoint::operator-(const CcPoint& oToSub) const
{
  CcPoint oNewPoint(*this);
  oNewPoint.m_X -= oToSub.m_X;
  oNewPoint.m_Y -= oToSub.m_Y;
  return oNewPoint;
}

void CcPoint::operator+=(const CcPoint& oToAdd)
{
  m_X += oToAdd.m_X;
  m_Y += oToAdd.m_Y;
}

void CcPoint::operator-=(const CcPoint& oToSub)
{
  m_X -= oToSub.m_X;
  m_Y -= oToSub.m_Y;
}

bool CcPoint::operator<(const CcPoint& oToCompare) const
{
  if (getX() < oToCompare.getX() &&
    getY() < oToCompare.getY())
  {
    return true;
  }
  return false;
}

bool CcPoint::operator>(const CcPoint& oToCompare) const
{
  if (getX() < oToCompare.getX() &&
    getY() < oToCompare.getY())
  {
    return true;
  }
  return false;
}

bool CcPoint::operator<=(const CcPoint& oToCompare) const
{
  if (getX() <= oToCompare.getX() &&
      getY() <= oToCompare.getY())
  {
    return true;
  }
  return false;
}

bool CcPoint::operator>=(const CcPoint& oToCompare) const
{
  if (getX() >= oToCompare.getX() &&
      getY() >= oToCompare.getY())
  {
    return true;
  }
  return false;
}
