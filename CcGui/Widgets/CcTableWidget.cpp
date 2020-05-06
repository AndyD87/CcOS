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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcTableWidget
 */
#include "Widgets/CcTableWidget.h"

CcTableWidget::CcTableWidget(CcWidget* pParent):
  CcWidget(pParent)
{
}

CcTableWidget::~CcTableWidget()
{
}

CcTableWidgetRow& CcTableWidget::addRow()
{
  append(CcTableWidgetRow(this, m_uiColumnsCount));
  updateSizes();
  return last();
}

void CcTableWidget::addColumn()
{
  for (CcTableWidgetRow& oRow : *this)
  {
    oRow.addColumn();
  }
  m_uiColumnsCount++;
  updateSizes();
}

void CcTableWidget::updateSizes()
{
  CcSize oSizeToCalculate;
  CcSizeRelative oSizeRelativeToCalculate;
  for(CcTableWidgetRow& oRow : *this)
  {
    if(oRow.getHeightRelative() != 0.0)
    {
      oSizeRelativeToCalculate.addHeight(oRow.getHeightRelative());
    }
    else if(oRow.getHeight() != 0)
    {
      oSizeToCalculate.addHeight(oRow.getHeight());
    }
  }
  for(CcTableWidgetColumnData& oCol : m_oColumns)
  {
    if(oCol.getWidthRelative() != 0.0)
    {
      oSizeRelativeToCalculate.addWidth(oCol.getWidthRelative());
    }
    else if(oCol.getWidth() != 0)
    {
      oSizeToCalculate.addWidth(oCol.getWidth());
    }
  }
  CcSize oTableSize = getSize();
  CcSize oTableSizeLeft = oTableSize-oTableSize;
  CcPoint oPosition(0, 0);
  for(CcTableWidgetRow& oRow : *this)
  {
    CcTableWidgetRow::iterator oCell = oRow.begin();
    CcSize oNewSize;
    if(oRow.getHeightRelative() != 0.0)
    {
      float fNewHeight = static_cast<float>(oRow.getHeightRelative()*oTableSizeLeft.getHeight())/(oRow.getHeightRelative()*oTableSizeLeft.getHeight());
      oNewSize.setHeight(static_cast<int32>(fNewHeight));
    }
    else if(oRow.getHeight() != 0)
    {
      oNewSize.setHeight(oRow.getHeight());
    }
    for(CcTableWidgetColumnData& oCol : m_oColumns)
    {
      if(oCol.getWidthRelative() != 0.0)
      {
        float fNewWidth = static_cast<float>(oCol.getWidthRelative()*oTableSizeLeft.getWidth()) / (oSizeRelativeToCalculate.getWidth());
        oNewSize.setWidth(static_cast<int32>(fNewWidth));
      }
      else if(oCol.getWidth() != 0)
      {
        oNewSize.setWidth(oCol.getWidth());
      }
      if(oCell->getChild())
      {
        oCell->getChild()->setPos(oPosition);
        oCell->getChild()->setSize(oNewSize);
      }
      oCell++;
    }
    oPosition.addY(oNewSize.getHeight());
    oPosition.setX(0);
  }
  oSizeToCalculate -= oTableSize;
  draw();
}
