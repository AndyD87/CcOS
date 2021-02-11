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
 * @brief     Class CcRectangle
 **/
#pragma once

#include "CcBase.h"
#include "CcPoint.h"
#include "CcSize.h"

/**
 * @brief Class implementation
 */
class CcKernelSHARED CcRectangle :public CcPoint, public CcSize
{
public:
  /**
   * @brief Constructor
   */
  CcRectangle() = default;

  /**
   * @brief Initialize values by copy from another rectangle
   * @param toAssign: Rectangle to copy
   */
  CcRectangle(const CcRectangle& toAssign) : CcPoint(toAssign), CcSize(toAssign)
    {}

  /**
   * @brief Initialize values by copy from Point, Size will be initialized as default.
   * @param toAssign: Point to copy
   */
  CcRectangle(const CcPoint& toAssign) : CcPoint(toAssign), CcSize(0, 0)
    {}

  /**
   * @brief Initialize values by copy from Size, Point will be initialized as default.
   * @param toAssign: Size to copy
   */
  CcRectangle(const CcSize& toAssign) : CcPoint(0,0), CcSize(toAssign)
    {}

  /**
   * @brief Initialize values by copy from Size and Point
   * @param oPointToAssign: Point to copy
   * @param oSizeToAssign: Size to copy
   */
  CcRectangle(const CcPoint& oPointToAssign, const CcSize& oSizeToAssign) : CcPoint(oPointToAssign), CcSize(oSizeToAssign)
    {}

  /**
   * @brief Initialize values by defining rectangle dimensions direct
   * @param uiPosX:   Start point X value.
   * @param uiPosY:   Start point y value.
   * @param uiWidth:  Width of rectangle
   * @param uiHeight: Height of rectangle.
   */
  CcRectangle(int32 uiPosX, int32 uiPosY, int32 uiWidth, int32 uiHeight) : CcPoint(uiPosX, uiPosY), CcSize(uiWidth, uiHeight)
    {}

  /**
   * @brief Destructor
   */
  ~CcRectangle(){}

  /**
   * @brief Check if point is inside of rectangle
   * @param oPoint: Point to verify
   * @return True if point is inside rectangle
   */
  bool checkPoint(const CcPoint& oPoint) const;

  /**
   * @brief Check if another rectangle start within this
   * @param oRectangle: Rectangle to verify
   * @return True if rectangle start point is in this
   */
  bool checkPoint(const CcRectangle& oRectangle) const;

  /**
   * @brief Check if another rectangle is fully part of this
   * @param oRectangle: Rectangle to verify
   * @return True if rectangle completly fits into this.
   */
  bool isInside(const CcRectangle& oRectangle) const;

  /**
   * @brief A border value will be assigned to rectangle.
   *        It will reduced on all sides by this value.
   *        Start point will be moved to keep position.
   * @param iSize: Size of border.
   */
  void addBorderSize(int iSize);

  //! @return Get point of top left corner
  CcPoint getTopLeftCorner() const;
  //! @return Get point of top right corner
  CcPoint getTopRightCorner() const;
  //! @return Get point of bottom left corner
  CcPoint getBottomLeftCorner() const;
  //! @return Get point of botom right corner
  CcPoint getBottomRightCorner() const;

  /**
   * @brief Update rectangle dimensions
   * @param uiPosX:   Start point X value.
   * @param uiPosY:   Start point y value.
   * @param uiWidth:  Width of rectangle
   * @param uiHeight: Height of rectangle.
   */
  void set(int32 uiPosX, int32 uiPosY, int32 uiWidth, int32 uiHeight)
  { setPoint(uiPosX, uiPosY); setSize(uiWidth, uiHeight); }

  /**
   * @brief Create new rectangle from this and add values from another
   * @param toAdd: Rectangle to add with this
   * @return Created rectangle
   */
  CcRectangle operator+(const CcRectangle& toAdd) const;
  /**
   * @brief Create new rectangle from this and increase size
   * @param toAdd: Size to add with this
   * @return Created rectangle
   */
  CcRectangle operator+(const CcSize& toAdd) const;
  /**
   * @brief Create new rectangle from this and increase start point
   * @param toAdd: Point to add with this
   * @return Created rectangle
   */
  CcRectangle operator+(const CcPoint& toAdd) const;
  /**
   * @brief Create new rectangle from this and add values from another
   * @param toAdd: Rectangle to add with this
   * @return Created rectangle
   */
  CcRectangle operator-(const CcRectangle& toAdd) const;
  /**
   * @brief Create new rectangle from this and reduce size
   * @param toAdd: Size to add with this
   * @return Created rectangle
   */
  CcRectangle operator-(const CcSize& toAdd) const;
  /**
   * @brief Create new rectangle from this and reduce start point
   * @param toAdd: Point to add with this
   * @return Created rectangle
   */
  CcRectangle operator-(const CcPoint& toAdd) const;

  //! @param toAdd: Increase this rectangle with that values
  inline void operator+=(const CcRectangle& toAdd)
  {setX(getX() + toAdd.getX()); setY(getY() + toAdd.getY());}
  //! @param toAdd: Increase this rectangle with that values
  inline void operator+=(const CcSize& toAdd)
  {setSize(getSize() + toAdd);}
  //! @param toAdd: Increase this rectangle with that values
  inline void operator+=(const CcPoint& toAdd)
  {setPoint(getPoint() + toAdd);}
  //! @param toAdd: Shrink this rectangle with that values
  inline void operator-=(const CcRectangle& toAdd)
  {setX(getX() - toAdd.getX()); setY(getY() - toAdd.getY());}
  //! @param toAdd: Shrink this rectangle with that values
  inline void operator-=(const CcSize& toAdd)
  {setSize(getSize() - toAdd);}
  //! @param toAdd: Shrink this rectangle with that values
  inline void operator-=(const CcPoint& toAdd)
  {setPoint(getPoint() - toAdd);}
  /**
   * @brief Resize this
   * @param toAssign: New size to set
   * @return Handle to this
   */
  inline CcRectangle& operator=(const CcSize& toAssign)
  { setSize(toAssign); return *this; }
  /**
   * @brief Move this
   * @param toAssign: New point to set
   * @return Handle to this
   */
  inline CcRectangle& operator=(const CcPoint& toAssign)
  { setPoint(toAssign); return *this; }
  /**
   * @brief Resize and move this
   * @param toAssign: New values to set
   * @return Handle to this
   */
  inline CcRectangle& operator=(const CcRectangle& toAssign)
  { setPoint(toAssign); setSize(toAssign); return *this; }
  /**
   * @brief Compare two rectangle if they are same.
   * @param oToCompare: Rectangle to compare with
   * @return True if all values matches
   */
  inline bool operator==(const CcRectangle& oToCompare) const
  { return getSize() == oToCompare.getSize() && getPoint() == oToCompare.getPoint(); }
  /**
   * @brief Compare with size if it matches with this.
   * @param oToCompare: Size to compare with
   * @return True if size is same
   */
  inline bool operator==(const CcSize& oToCompare) const
  { return getSize() == oToCompare; }
  /**
   * @brief Compare with point if it starts with this point.
   * @param oToCompare: Point to compare with
   * @return True if start point is same
   */
  inline bool operator==(const CcPoint& oToCompare) const
  { return getPoint() == oToCompare; }
  /**
   * @brief Compare two rectangle if they are not same.
   * @param oToCompare: Rectangle to compare with
   * @return True if at least one value does not match
   */
  inline bool operator!=(const CcRectangle& oToCompare) const
  { return getSize() != oToCompare.getSize() || getPoint() != oToCompare.getPoint(); }
  /**
   * @brief Compare with size if it does not match with this.
   * @param oToCompare: Size to compare with
   * @return True if size is not same
   */
  inline bool operator!=(const CcSize& oToCompare) const
  { return getSize() != oToCompare; }
  /**
   * @brief Compare with point if this rectangle does not start at this point.
   * @param oToCompare: Point to compare with
   * @return True if start point is not same
   */
  inline bool operator!=(const CcPoint& oToCompare) const
  { return getPoint() != oToCompare; }
};
