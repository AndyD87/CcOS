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
 * @brief     Implemtation of class CcStyle
 */
#include "Styles/CcStyles.h"

const CcColor CcStyle::WindowBackgroundColor = CcColor(0x0f, 0xff, 0x0f);

const CcColor CcStyle::WidgetBackgroundColor = CcColor(0x20, 0x20, 0x20);
const CcColor CcStyle::WidgetForegroundColor = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::WidgetBorderColor     = CcColor(0xff, 0xff, 0xff);
const uint16  CcStyle::WidgetBorderSize      = 0;

const CcColor CcStyle::ButtonBackgroundColor        = CcColor(0x20, 0x20, 0x20);
const CcColor CcStyle::ButtonForegroundColor        = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::ButtonBorderColor            = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::ButtonHoverBackgroundColor   = CcColor(0x40, 0x40, 0x40);
const CcColor CcStyle::ButtonHoverForegroundColor   = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::ButtonHoverBorderColor       = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::ButtonActiveBackgroundColor  = CcColor(0x80, 0x80, 0x80);
const CcColor CcStyle::ButtonActiveForegroundColor  = CcColor(0xff, 0xff, 0xff);
const CcColor CcStyle::ButtonActiveBorderColor      = CcColor(0xff, 0xff, 0xff);
