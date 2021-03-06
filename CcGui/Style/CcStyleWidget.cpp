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
 * @brief     Implemtation of class CcStyleWidget
 */
#include "Style/CcStyleWidget.h"

CcStyleWidget::CcStyleWidget()
{
  oBackgroundColor = CcStyle::WidgetBackgroundColor;
  oForegroundColor = CcStyle::WidgetForegroundColor;
  oBorderColor     = CcStyle::WidgetBorderColor;
  uBorderSize      = CcStyle::WidgetBorderSize;
  oSizeRelative    = CcStyle::WidgetSizeRelative;
}

CcStyleWidget::CcStyleWidget( const CcColor& oBackgroundColor,
                              const CcColor& oForegroundColor,
                              const CcColor& oBorderColor,
                              const uint16  uBorderSize,
                              const CcSizeRelative& oRelative) :
  oBackgroundColor(oBackgroundColor),
  oForegroundColor(oForegroundColor),
  oBorderColor(oBorderColor),
  uBorderSize(uBorderSize),
  oSizeRelative(oRelative)
{
}

CcStyleWidget& CcStyleWidget::operator=(const CcStyleWidget& oStyleSheet)
{
  oBackgroundColor = oStyleSheet.oBackgroundColor;
  oForegroundColor = oStyleSheet.oForegroundColor;
  oBorderColor = oStyleSheet.oBorderColor;
  uBorderSize = oStyleSheet.uBorderSize;
  return*this;
}
