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
 * @page      Widgets
 * @subpage   CcTableWidgetColumnData
 *
 * @page      CcTableWidgetColumnData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTableWidgetColumnData
 **/
#ifndef H_CcTableWidgetColumnData_H_
#define H_CcTableWidgetColumnData_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcString.h"
#include "CcColor.h"

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcTableWidgetColumnData
{
public:
  bool operator==(const CcTableWidgetColumnData& oData) const;

  uint32 getWidth()
  { return m_uiWidth; }
  float getWidthRelative()
  { return m_fWidthRelative; }

  void setWidth(uint32 uiWidth)
  { m_uiWidth = uiWidth;  m_fWidthRelative = 0.0;}
  void setWidth(float fWidth)
  { m_fWidthRelative = fWidth; m_uiWidth = 0; }

private:
  uint32    m_uiWidth = 0;
  float     m_fWidthRelative = 1.0;
  CcString  sName;
  CcColor   uiBorderColor;
};

#endif // H_CcTableWidgetColumnData_H_
