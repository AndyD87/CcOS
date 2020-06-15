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
 * @subpage   CcSizeRelative
 *
 * @page      CcSizeRelative
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSizeRelative
 */
#ifndef H_CcSizeRelative_H_
#define H_CcSizeRelative_H_

#include "CcBase.h"
#include "CcBase.h"

#define CcRectangle_DefaultValue 0.0

class CcRectangle;

class CcKernelSHARED CcSizeRelative
{
public:
  CcSizeRelative() = default;
  CcSizeRelative(float iWidth, float iHeight) : m_fWidth(iWidth), m_fHeight(iHeight)
    {}
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcSizeRelative);
  ~CcSizeRelative() = default;

  bool isInside(const CcRectangle& oRectangle);

public:
  inline float getWidth() const
  { return m_fWidth; }
  inline float getHeight() const
  { return m_fHeight; }
  inline const CcSizeRelative& getSize() const
  { return *this; }
  inline void setSize(const CcSizeRelative& oSize)
  { m_fWidth = oSize.m_fWidth; m_fHeight = oSize.m_fHeight;}
  inline void setSize(float iWidth, float iHeight)
  { m_fWidth = iWidth; m_fHeight = iHeight;}
  inline void setHeight(float iHeight)
  { m_fHeight = iHeight; }
  inline void addHeight(float iHeight)
  { m_fHeight += iHeight; }
  inline void setWidth(float iWidth)
  { m_fWidth = iWidth; }
  inline void addWidth(float iWidth)
  { m_fWidth += iWidth; }
  inline float getAreaSize() const
  { return m_fWidth*m_fHeight; }

  CcSizeRelative operator+(const CcSizeRelative& oToAdd) const;
  CcSizeRelative operator-(const CcSizeRelative& oToAdd) const;
  void operator+=(const CcSizeRelative& oToAdd);
  void operator-=(const CcSizeRelative& oToAdd);
  bool operator<(const CcSizeRelative& oToCompare) const;
  bool operator>(const CcSizeRelative& oToCompare) const;
  bool operator<=(const CcSizeRelative& oToCompare) const;
  bool operator>=(const CcSizeRelative& oToCompare) const;
  CcSizeRelative& operator=(const CcSizeRelative& toAssign)
  { setSize(toAssign); return *this;}
  bool operator==(const CcSizeRelative& oToCompare) const
  { return m_fWidth == oToCompare.m_fWidth && m_fHeight == oToCompare.m_fHeight; }
  bool operator!=(const CcSizeRelative& oToCompare) const
  { return m_fWidth != oToCompare.m_fWidth || m_fHeight != oToCompare.m_fHeight; }

  bool isDefault()
  { return m_fHeight == CcRectangle_DefaultValue && m_fWidth == CcRectangle_DefaultValue; }
private:
  float m_fWidth = CcRectangle_DefaultValue;
  float m_fHeight = CcRectangle_DefaultValue;
};

#endif // H_CcSizeRelative_H_
