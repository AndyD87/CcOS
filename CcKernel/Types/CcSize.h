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
#ifndef _CcSize_H_
#define _CcSize_H_

#include "CcBase.h"
#include "CcKernelBase.h"

class CcKernelSHARED CcSize
{
public:
  CcSize() : m_uiWidth(0), m_uiHeight(0)
    {}
  CcSize(uint16 uiWidth, uint16 uiHeight) : m_uiWidth(uiWidth), m_uiHeight(uiHeight)
    {}
  ~CcSize()
    {}

public:
  inline uint16 getWidth( void ) const 
    {return m_uiWidth;}
  inline uint16 getHeight( void ) const 
    {return m_uiHeight;}
  inline const CcSize& getSize() const
    {return *this;}
  inline void setSize(const CcSize& oSize)
    { m_uiWidth = oSize.m_uiWidth; m_uiHeight = oSize.m_uiHeight;}
  inline void setSize(uint16 uiWidth, uint16 uiHeight)
    { m_uiWidth = uiWidth; m_uiHeight = uiHeight;}
  inline void setHeight(uint16 uiHeight)
   { m_uiHeight = uiHeight; }
  inline void setWidth(uint16 uiWidth)
    { m_uiWidth = uiWidth; }
  inline uint32 getAreaSize() const
    { return m_uiWidth*m_uiHeight;}

  CcSize operator+(const CcSize& oToAdd) const;
  CcSize operator-(const CcSize& oToAdd) const;
  void operator+=(const CcSize& oToAdd);
  void operator-=(const CcSize& oToAdd);
  bool operator<(const CcSize& oToCompare) const;
  bool operator>(const CcSize& oToCompare) const;
  bool operator<=(const CcSize& oToCompare) const;
  bool operator>=(const CcSize& oToCompare) const;
  CcSize& operator=(const CcSize& toAssign) { setSize(toAssign); return *this;}
private:
  uint16 m_uiWidth;
  uint16 m_uiHeight;
};

#endif //_CcSize_H_
