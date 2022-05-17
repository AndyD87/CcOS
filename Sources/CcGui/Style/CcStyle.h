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
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "Types/CcColor.h"
#include "CcSizeRelative.h"

/**
 * @brief Default style for widgets
 */
class CcStyle 
{
public:
  static const uint16 TitlebarHeight;               //!< Height of titlebar
  static const CcColor WindowBackgroundColor;       //!< Background color for window

  static const CcColor WidgetBackgroundColor;       //!< Background color for widget
  static const CcColor WidgetForegroundColor;       //!< Foreground color for widget
  static const CcColor WidgetBorderColor;           //!< Color of border for widget
  static const uint16  WidgetBorderSize;            //!< Size of border for widget
  static const CcSizeRelative  WidgetSizeRelative;  //!< Widget size relative to parent

  static const CcColor ButtonBackgroundColor;       //!< Background color for button
  static const CcColor ButtonForegroundColor;       //!< Foreground color for button
  static const CcColor ButtonBorderColor;           //!< Color of border for button
  static const uint16  ButtonBorderSize;            //!< Size of border for button
  static const CcSizeRelative  ButtonSizeRelative;  //!< Widget size relative to parent for button
  static const CcColor ButtonHoverBackgroundColor;  //!< Background color for hoverd button
  static const CcColor ButtonHoverForegroundColor;  //!< Foreground color for hoverd button
  static const CcColor ButtonHoverBorderColor;      //!< Color of border for hoverd button
  static const uint16  ButtonHoverBorderSize;       //!< Size of border for hoverd button
  static const CcColor ButtonActiveBackgroundColor; //!< Background color for active button
  static const CcColor ButtonActiveForegroundColor; //!< Foreground color for active button
  static const CcColor ButtonActiveBorderColor;     //!< Color of border for active button
  static const uint16  ButtonActiveBorderSize;      //!< Size of border for active button
};
