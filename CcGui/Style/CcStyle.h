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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStyle
 **/
#ifndef H_CcStyle_H_
#define H_CcStyle_H_

#include "CcBase.h"
#include "CcGui.h"
#include "Types/CcColor.h"
#include "CcSizeRelative.h"

/**
 * @brief Class implementation
 */
class CcStyle 
{
public:
  static const uint16 TitlebarHeight;
  static const CcColor WindowBackgroundColor;

  static const CcColor WidgetBackgroundColor;
  static const CcColor WidgetForegroundColor;
  static const CcColor WidgetBorderColor;
  static const uint16  WidgetBorderSize;
  static const CcSizeRelative  WidgetSizeRelative;

  static const CcColor ButtonBackgroundColor;
  static const CcColor ButtonForegroundColor;
  static const CcColor ButtonBorderColor;
  static const uint16  ButtonBorderSize;
  static const CcSizeRelative  ButtonSizeRelative;
  static const CcColor ButtonHoverBackgroundColor;
  static const CcColor ButtonHoverForegroundColor;
  static const CcColor ButtonHoverBorderColor;
  static const uint16  ButtonHoverBorderSize;
  static const CcColor ButtonActiveBackgroundColor;
  static const CcColor ButtonActiveForegroundColor;
  static const CcColor ButtonActiveBorderColor;
  static const uint16  ButtonActiveBorderSize;
};

#endif // H_CcStyle_H_
