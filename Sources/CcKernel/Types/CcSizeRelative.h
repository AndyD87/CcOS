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
 * @brief     Class CcSizeRelative
 */
#pragma once

#include "CcBase.h"

//! Default value for 0.0 to avoid magic numbers
#define CcSizeRelative_DefaultValue 0.0

class CcRectangle;

/**
 * @brief Define a relative size with height and width float values.
 *        To generate a real size a second Size element is required
 *        to multiply this values to.
 */
class CcKernelSHARED CcSizeRelative
{
public:
  CcSizeRelative() = default;
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcSizeRelative)
  ~CcSizeRelative() = default;

  /**
   * @brief Initialize size with default values.
   * @param iWidth:   Start width
   * @param iHeight:  Start height
   */
  CcSizeRelative(float iWidth, float iHeight) : m_fWidth(iWidth), m_fHeight(iHeight)
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
   inline void setSize(float iWidth, float iHeight)
  { m_fWidth = iWidth; m_fHeight = iHeight;}

  //! @param oSize:  set new size value
  inline void setSize(const CcSizeRelative& oSize)
  { m_fWidth = oSize.m_fWidth; m_fHeight = oSize.m_fHeight;}
public:
  //! @return Get width const
  inline float getWidth() const
  { return m_fWidth; }
  //! @return Get height const
  inline float getHeight() const
  { return m_fHeight; }
  //! @return Get this const
  inline const CcSizeRelative& getSize() const
  { return *this; }
  //! @return Get size of area
  inline float getAreaSize() const
  { return m_fWidth*m_fHeight; }
  //! @param iHeight: Set new height value
  inline void setHeight(float iHeight)
  { m_fHeight = iHeight; }
  //! @param iHeight: Add value to current height value
  inline void addHeight(float iHeight)
  { m_fHeight += iHeight; }
  //! @param iWidth: Set new width value
  inline void setWidth(float iWidth)
  { m_fWidth = iWidth; }
  //! @param iWidth: Add value to current width value
  inline void addWidth(float iWidth)
  { m_fWidth += iWidth; }

  //! @copydoc CDoxygenCommon::operator=()
  CcSizeRelative& operator=(const CcSizeRelative& oToCopy)
  { setSize(oToCopy); return *this;}

  //! @copydoc CDoxygenCommon::operator+()
  CcSizeRelative operator+(const CcSizeRelative& oToAdd) const;
  //! @copydoc CDoxygenCommon::operator-()
  CcSizeRelative operator-(const CcSizeRelative& oToSub) const;
  //! @copydoc CDoxygenCommon::operator+=()
  void operator+=(const CcSizeRelative& oToAdd);
  //! @copydoc CDoxygenCommon::operator-=()
  void operator-=(const CcSizeRelative& oToSub);
  //! @copydoc CDoxygenCommon::operator<()
  bool operator<(const CcSizeRelative& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator>()
  bool operator>(const CcSizeRelative& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator<=()
  bool operator<=(const CcSizeRelative& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator>()
  bool operator>=(const CcSizeRelative& oToCompare) const;
  //! @copydoc CDoxygenCommon::operator==()
  bool operator==(const CcSizeRelative& oToCompare) const
  { return m_fWidth == oToCompare.m_fWidth && m_fHeight == oToCompare.m_fHeight; }
  //! @copydoc CDoxygenCommon::operator!=()
  bool operator!=(const CcSizeRelative& oToCompare) const
  { return m_fWidth != oToCompare.m_fWidth || m_fHeight != oToCompare.m_fHeight; }

  //!  @return True if current values are default values
  bool isDefault()
  { return m_fHeight == CcSizeRelative_DefaultValue && m_fWidth == CcSizeRelative_DefaultValue; }
private:
  float m_fWidth = CcSizeRelative_DefaultValue;
  float m_fHeight = CcSizeRelative_DefaultValue;
};
