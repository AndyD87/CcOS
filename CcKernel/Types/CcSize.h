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
 * @subpage   CcSize
 *
 * @page      CcSize
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSize
 */
#ifndef H_CcSize_H_
#define H_CcSize_H_

#include "CcBase.h"
#include "CcBase.h"

class CcRectangle;

class CcKernelSHARED CcSize
{
public:
  CcSize() = default;
  CcSize(int32 iWidth, int32 iHeight) : m_iWidth(iWidth), m_iHeight(iHeight)
    {}
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcSize)
  ~CcSize() = default;

  bool isInside(const CcRectangle& oRectangle);

public:
  inline int32 getWidth() const
    {return m_iWidth;}
  inline int32 getHeight() const
    {return m_iHeight;}
  inline const CcSize& getSize() const
    {return *this;}
  inline void setSize(const CcSize& oSize)
    { m_iWidth = oSize.m_iWidth; m_iHeight = oSize.m_iHeight;}
  inline void setSize(int32 iWidth, int32 iHeight)
    { m_iWidth = iWidth; m_iHeight = iHeight;}
  inline void setHeight(int32 iHeight)
   { m_iHeight = iHeight; }
  inline void addHeight(int32 iHeight)
   { m_iHeight += iHeight; }
  inline void setWidth(int32 iWidth)
    { m_iWidth = iWidth; }
  inline void addWidth(int32 iWidth)
    { m_iWidth += iWidth; }
  inline int32 getAreaSize() const
    { return m_iWidth*m_iHeight;}

  CcSize operator+(const CcSize& oToAdd) const;
  CcSize operator-(const CcSize& oToAdd) const;
  void operator+=(const CcSize& oToAdd);
  void operator-=(const CcSize& oToAdd);
  bool operator<(const CcSize& oToCompare) const;
  bool operator>(const CcSize& oToCompare) const;
  bool operator<=(const CcSize& oToCompare) const;
  bool operator>=(const CcSize& oToCompare) const;
  CcSize& operator=(const CcSize& toAssign) { setSize(toAssign); return *this;}
  bool operator==(const CcSize& oToCompare) const
    { return m_iWidth == oToCompare.m_iWidth && m_iHeight == oToCompare.m_iHeight; }
  bool operator!=(const CcSize& oToCompare) const
    { return m_iWidth != oToCompare.m_iWidth || m_iHeight != oToCompare.m_iHeight; }
private:
  int32 m_iWidth = 0;
  int32 m_iHeight = 0;
};

#endif // H_CcSize_H_
