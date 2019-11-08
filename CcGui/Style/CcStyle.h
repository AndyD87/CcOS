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
 * @page      CcGui
 * @subpage   Style
 *
 * @page      Style
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStyle
 **/
#ifndef H_CcStyle_H_
#define H_CcStyle_H_

#include "CcBase.h"
#include "CcGui.h"
#include "Types/CcColor.h"

/**
 * @brief Class implementation
 */
class CcStyle 
{
public:
  enum class EType : size_t
  {
    None = 0,
    BackgroundColor,
    ForegroundColor,
    BorderStyle,
    HoverColor,
  };

  static uint16 TitlebarHeight;
  static CcColor WindowBackgroundColor;

  static CcColor WidgetBackgroundColor;
  static CcColor WidgetForegroundColor;
  static CcColor WidgetBorderColor;
  static uint16  WidgetBorderSize;

  static CcColor ButtonBackgroundColor;
  static CcColor ButtonForegroundColor;
  static CcColor ButtonBorderColor;
  static uint16  ButtonBorderSize;
  static CcColor ButtonHoverBackgroundColor;
  static CcColor ButtonHoverForegroundColor;
  static CcColor ButtonHoverBorderColor;
  static uint16 ButtonHoverBorderSize;
  static CcColor ButtonActiveBackgroundColor;
  static CcColor ButtonActiveForegroundColor;
  static CcColor ButtonActiveBorderColor;
};

#endif // H_CcStyle_H_
