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
 * @brief     Implemtation of class CcStyleButton
 */
#include "Style/CcStyleButton.h"
#include "Style/CcStyle.h"

CcStyleWidget CcStyleButton::oDefaultWidgetStyle(
  CcStyle::ButtonBackgroundColor,
  CcStyle::ButtonForegroundColor,
  CcStyle::ButtonBorderColor,
  CcStyle::ButtonBorderSize,
  CcStyle::ButtonSizeRelative
);

CcStyleButton::CcStyleButton() :
  oHoverStyle(  CcStyle::ButtonHoverBackgroundColor,
                CcStyle::ButtonHoverForegroundColor,
                CcStyle::ButtonHoverBorderColor,
                CcStyle::ButtonHoverBorderSize,
                CcStyle::ButtonSizeRelative)
{
}
