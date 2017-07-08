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
 * @brief     Implementation of class CcSize
 */
#include "Types/CcSize.h"


CcSize CcSize::operator+(const CcSize& oToAdd) const
{
  CcSize oNewPoint(*this);
  oNewPoint.m_uiWidth += oToAdd.m_uiWidth;
  oNewPoint.m_uiHeight += oToAdd.m_uiHeight;
  return oNewPoint;
}

CcSize CcSize::operator-(const CcSize& oToAdd) const
{
  CcSize oNewPoint(*this);
  oNewPoint.m_uiWidth -= oToAdd.m_uiWidth;
  oNewPoint.m_uiHeight -= oToAdd.m_uiHeight;
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
  m_uiWidth += oToAdd.m_uiWidth;
  m_uiHeight += oToAdd.m_uiHeight;
}

void CcSize::operator-=(const CcSize& oToAdd)
{
  m_uiWidth -= oToAdd.m_uiWidth;
  m_uiHeight -= oToAdd.m_uiHeight;
}