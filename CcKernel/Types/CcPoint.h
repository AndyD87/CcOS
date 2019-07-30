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
 * @page      Types
 * @subpage   CcPoint
 *
 * @page      CcPoint
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcPoint
 */
#ifndef H_CcPoint_H_
#define H_CcPoint_H_

#include "CcBase.h"
#include "CcKernelBase.h"

class CcKernelSHARED CcPoint
{
public:
  CcPoint() = default;
  CcPoint(int32 x, int32 y) : m_X(x), m_Y(y)
    {}
  ~CcPoint() = default;

public:
  inline int32 getX() const
    {return m_X;}
  inline int32 getY() const
    {return m_Y;}
  inline const CcPoint& getPoint() const
    {return *this;}
  inline void setPoint(const CcPoint& oPoint)
    { m_X = oPoint.m_X; m_Y = oPoint.m_Y;}
  inline void setPoint(int32 x, int32 y) 
    { m_X = x; m_Y = y;}
  inline void setX(int32 x) 
    { m_X = x; }
  inline void addX(int32 x) 
    { m_X += x; }
  inline void setY(int32 y)
    { m_Y = y;}
  inline void addY(int32 y)
    { m_Y += y;}

  CcPoint operator+(const CcPoint& oToAdd) const;
  CcPoint operator-(const CcPoint& oToAdd) const;
  void operator+=(const CcPoint& oToAdd);
  void operator-=(const CcPoint& oToAdd);
  bool operator<(const CcPoint& oToCompare) const;
  bool operator>(const CcPoint& oToCompare) const;
  bool operator<=(const CcPoint& oToCompare) const;
  bool operator>=(const CcPoint& oToCompare) const;
  CcPoint& operator=(const CcPoint& toAssign) 
    { setPoint(toAssign); return *this; }
  bool operator==(const CcPoint& oToCompare) const
    { return m_X == oToCompare.m_X && m_Y == oToCompare.m_Y; }
  bool operator!=(const CcPoint& oToCompare) const
    { return m_X != oToCompare.m_X || m_Y != oToCompare.m_Y; }
private:
  int32 m_X = 0;
  int32 m_Y = 0;
};

#endif / H_CcPoint_H_
