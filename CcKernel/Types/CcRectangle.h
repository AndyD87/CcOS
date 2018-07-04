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
 * @subpage   CcRectangle
 *
 * @page      CcRectangle
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRectangle
 **/
#ifndef _CcRectangle_H_
#define _CcRectangle_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcPoint.h"
#include "CcSize.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcRectangle :public CcPoint, public CcSize
{
public:
  /**
   * @brief Constructor
   */
  CcRectangle(void)
  {}

  /**
   * @brief Constructor
   */
  CcRectangle(const CcPoint& toAssign) : CcPoint(toAssign), CcSize(0, 0)
  {
  }

  /**
   * @brief Constructor
   */
  CcRectangle(const CcSize& toAssign) : CcPoint(0,0), CcSize(toAssign)
  {
  }

  /**
   * @brief Constructor
   */
  CcRectangle(const CcPoint& oPointToAssign, const CcSize& oSizeToAssign) : CcPoint(oPointToAssign), CcSize(oSizeToAssign)
  {
  }

  /**
   * @brief Constructor
   */
  CcRectangle(int32 uiPosX, int32 uiPosY, int32 uiWidtt, int32 uiHeight) : CcPoint(uiPosX, uiPosY), CcSize(uiWidtt, uiHeight)
  {}

  /**
   * @brief Destructor
   */
  ~CcRectangle(void)
    {}

  bool checkPoint(const CcPoint& oPoint) const;
  bool checkPoint(const CcRectangle& oRectangle) const;
  bool isInside(const CcRectangle& oRectangle) const;

  void addBorderSize(int iSize);

  CcPoint getTopLeftCorner() const;
  CcPoint getTopRightCorner() const;
  CcPoint getBottomLeftCorner() const;
  CcPoint getBottomRightCorner() const;

  CcRectangle operator+(const CcRectangle& toAdd) const;
  CcRectangle operator+(const CcSize& toAdd) const;
  CcRectangle operator+(const CcPoint& toAdd) const;
  CcRectangle operator-(const CcRectangle& toAdd) const;
  CcRectangle operator-(const CcSize& toAdd) const;
  CcRectangle operator-(const CcPoint& toAdd) const;
  inline void operator+=(const CcRectangle& toAdd)
    {setX(getX() + toAdd.getX()); setY(getY() + toAdd.getY());}
  inline void operator+=(const CcSize& toAdd)
    {setSize(getSize() + toAdd);}
  inline void operator+=(const CcPoint& toAdd)
    {setPoint(getPoint() + toAdd);}
  inline void operator-=(const CcRectangle& toAdd)
    {setX(getX() - toAdd.getX()); setY(getY() - toAdd.getY());}
  inline void operator-=(const CcSize& toAdd)
    {setSize(getSize() - toAdd);}
  inline void operator-=(const CcPoint& toAdd)
    {setPoint(getPoint() - toAdd);}
  inline CcSize& operator=(const CcSize& toAssign) 
    { setSize(toAssign); return *this; }
  inline CcSize& operator=(const CcPoint& toAssign)
    { setPoint(toAssign); return *this; }
  inline bool operator==(const CcRectangle& oToCompare) const
    { return getSize() == oToCompare.getSize() && getPoint() == oToCompare.getPoint(); }
  inline bool operator==(const CcSize& oToCompare) const
    { return getSize() == oToCompare; }
  inline bool operator==(const CcPoint& oToCompare) const
    { return getPoint() == oToCompare; }
  inline bool operator!=(const CcRectangle& oToCompare) const
    { return getSize() != oToCompare.getSize() || getPoint() != oToCompare.getPoint(); }
  inline bool operator!=(const CcSize& oToCompare) const
    { return getSize() != oToCompare; }
  inline bool operator!=(const CcPoint& oToCompare) const
    { return getPoint() != oToCompare; }
};

#endif /* _CcRectangle_H_ */
