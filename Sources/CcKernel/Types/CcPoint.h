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
 * @brief     Class CcPoint
 */
#pragma once

#include "CcBase.h"

/**
 * @brief Object to describe a 2 dimensional point.
 */
class CcKernelSHARED CcPoint
{
public:
  CcPoint() = default;
  /**
   * @brief Create point and copy data from an other point.
   * @param oToCopy: Object to copy coordinates from
   */
  CcPoint(const CcPoint& oToCopy)  : m_X(oToCopy.m_X), m_Y(oToCopy.m_Y)
  {}             
  /**
   * @brief Create point directly with coordinates
   * @param x:  X-Coordinate of point
   * @param y:  Y-Coordinate of point
   */
  CcPoint(int32 x, int32 y) : m_X(x), m_Y(y)
  {}
  ~CcPoint() = default;

public:
  //! @return Get reference of X value
  inline int32& x()
  {return m_X;}
  //! @return Get reference of Y value
  inline int32& y()
  {return m_Y;}
  //! @return Get copy of X value
  inline int32 getX() const
  {return m_X;}
  //! @return Get copy of Y value
  inline int32 getY() const
  {return m_Y;}
  //! @return Get this point as point
  inline const CcPoint& getPoint() const
  {return *this;}

  //! @param oPoint: Set point by copy data from another point
  inline void setPoint(const CcPoint& oPoint)
  { m_X = oPoint.m_X; m_Y = oPoint.m_Y;}
  //! @param x: Set new X coordinate
  //! @param y: Set new Y coordinate
  inline void setPoint(int32 x, int32 y) 
  { m_X = x; m_Y = y;}
  //! @param x: Set new X coordinate
  inline void setX(int32 x)
  { m_X = x; }
  //! @param x: Add value to X coordinate
  inline void addX(int32 x) 
  { m_X += x; }
  //! @param y: Set new Y coordinate
  inline void setY(int32 y)
  { m_Y = y;}
  //! @param y: Add value to Y coordinate
  inline void addY(int32 y)
  { m_Y += y;}

  /**
   * @brief Add two points and return result
   * @param oToAdd: Point to add to this
   * @return New point of addition
   */
  CcPoint operator+(const CcPoint& oToAdd) const;

  /**
   * @brief Subtrac a point from this and return result
   * @param oToSub: Point to substract from this
   * @return New point of substraction
   */
  CcPoint operator-(const CcPoint& oToSub) const;

  /**
   * @brief Add and update a point to this
   * @param oToAdd: Point to add to this
   */
  void operator+=(const CcPoint& oToAdd);

  /**
   * @brief Substract and update a point from this
   * @param oToSub: Point to substract from this
   */
  void operator-=(const CcPoint& oToSub);

  /**
   * @brief Check if X **AND** Y coordinates from this point is lower than
   *        from another point.
   * @param oToCompare: Object to compare to.
   * @return True if both coordinates are lower
   */
  bool operator<(const CcPoint& oToCompare) const;

  /**
   * @brief Check if X **AND** Y coordinates from this point is greater than
   *        from another point.
   * @param oToCompare: Object to compare to.
   * @return True if both coordinates are greater
   */
  bool operator>(const CcPoint& oToCompare) const;

  /**
   * @brief Check if X **AND** Y coordinates from this point is lower than
   *        or equal to another point.
   * @param oToCompare: Object to compare to.
   * @return True if both coordinates are lower or equal
   */
  bool operator<=(const CcPoint& oToCompare) const;

  /**
   * @brief Check if X **AND** Y coordinates from this point is greater than
   *        or equal to another point.
   * @param oToCompare: Object to compare to.
   * @return True if both coordinates are greater or equal
   */
  bool operator>=(const CcPoint& oToCompare) const;

  /**
   * @brief Copy operator to assign value from another point to this.
   * @param toAssign: Object to copy from
   * @return Handle to this point
   */
  CcPoint& operator=(const CcPoint& toAssign) 
  { setPoint(toAssign); return *this; }

  /**
   * @brief Check if X **AND** Y coordinates from this point is equal to another point.
   * @param oToCompare: Object to compare to.
   * @return True if both coordinates are equal
   */
  bool operator==(const CcPoint& oToCompare) const
  { return m_X == oToCompare.m_X && m_Y == oToCompare.m_Y; }

  /**
   * @brief Check if X **OR** Y coordinates from this point is not equal to another point.
   * @param oToCompare: Object to compare to.
   * @return True if at least one coordinate is not equal
   */
  bool operator!=(const CcPoint& oToCompare) const
  { return m_X != oToCompare.m_X || m_Y != oToCompare.m_Y; }
private:
  int32 m_X = 0;
  int32 m_Y = 0;
};
