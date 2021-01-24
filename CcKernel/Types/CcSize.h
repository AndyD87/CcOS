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
 * @brief     Class CcSize
 */
#pragma once

#include "CcBase.h"

//! Default value for 0 to avoid magic numbers
#define CcSize_DefaultValue 0

class CcRectangle;

/**
 * @brief Define a size with height and width integer values
 */
class CcKernelSHARED CcSize
{
public:
  CcSize() = default;
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcSize)
  ~CcSize() = default;

  /**
   * @brief Initialize size with default values.
   * @param iWidth:   Start width
   * @param iHeight:  Start height
   */
  CcSize(int32 iWidth, int32 iHeight) : m_iWidth(iWidth), m_iHeight(iHeight)
  {}

  /**
   * @brief Check if this size, defined as rectangle starting by 0.0, is enable to be placed
   *        in target rectangle
   * @param oRectangle: Rectangle to check if size could be placed in.
   * @return True if rectangle is big enough.
   */
  bool isInside(const CcRectangle& oRectangle);

  /**
   * @brief Set size with width and height.
   * @param iWidth:   New width to set
   * @param iHeight:  New height to set
   */
  inline void setSize(int32 iWidth, int32 iHeight)
  { m_iWidth = iWidth; m_iHeight = iHeight;}

  //! @return Get width const
  inline int32 getWidth() const
  {return m_iWidth;}
  //! @return Get height const
  inline int32 getHeight() const
  {return m_iHeight;}
  //! @return Get this const
  inline const CcSize& getSize() const
  {return *this;}
  //! @return Get size of area
  inline int32 getAreaSize() const
  { return m_iWidth*m_iHeight;}
  //! @param oSize:  set new size value
  inline void setSize(const CcSize& oSize)
  { m_iWidth = oSize.m_iWidth; m_iHeight = oSize.m_iHeight;}
  //! @param iHeight: Set new height value
  inline void setHeight(int32 iHeight)
  { m_iHeight = iHeight; }
  //! @param iHeight: Add value to current height value
  inline void addHeight(int32 iHeight)
  { m_iHeight += iHeight; }
  //! @param iWidth: Set new width value
  inline void setWidth(int32 iWidth)
  { m_iWidth = iWidth; }
  //! @param iWidth: Add value to current width value
  inline void addWidth(int32 iWidth)
  { m_iWidth += iWidth; }

  //! @copydoc CDoxygenCommon::operator=()
  CcSize& operator=(const CcSize& oToCopy) { setSize(oToCopy); return *this;}

  //! @copydoc CDoxygenCommon::operator+()
  CcSize operator+(const CcSize& oToAdd) const;
  //! @copydoc CDoxygenCommon::operator-()
  CcSize operator-(const CcSize& oToSub) const;
  //! @copydoc CDoxygenCommon::operator+=()
  void operator+=(const CcSize& oToAdd);
  //! @copydoc CDoxygenCommon::operator-=()
  void operator-=(const CcSize& oToSub);
  //! @copydoc CDoxygenCommon::operator<()
  bool operator<(const CcSize& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator>()
  bool operator>(const CcSize& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator<=()
  bool operator<=(const CcSize& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator>()
  bool operator>=(const CcSize& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator==()
  bool operator==(const CcSize& oToCompare) const
  { return m_iWidth == oToCompare.m_iWidth && m_iHeight == oToCompare.m_iHeight; }
  //! @copydoc CDoxygenCommon::operator!=()
  bool operator!=(const CcSize& oToCompare) const
  { return m_iWidth != oToCompare.m_iWidth || m_iHeight != oToCompare.m_iHeight; }
private:
  int32 m_iWidth 	= CcSize_DefaultValue;
  int32 m_iHeight 	= CcSize_DefaultValue;
};
