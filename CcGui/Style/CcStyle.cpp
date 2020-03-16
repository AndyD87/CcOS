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
#include "Style/CcStyle.h"

uint16  const CcStyle::TitlebarHeight               (30);
CcColor const CcStyle::WindowBackgroundColor        (0x0f, 0xff, 0x0f);

CcColor const CcStyle::WidgetBackgroundColor        (0x20, 0x20, 0x20);
CcColor const CcStyle::WidgetForegroundColor        (0xff, 0xff, 0xff);
CcColor const CcStyle::WidgetBorderColor            (0x00, 0x00, 0x00);
uint16  const CcStyle::WidgetBorderSize             (0);
CcSizeRelative const CcStyle::WidgetSizeRelative    (1.0, 1.0);

CcColor const CcStyle::ButtonBackgroundColor        (0xe0, 0xe0, 0xe0);
CcColor const CcStyle::ButtonForegroundColor        (0x00, 0x00, 0x00);
CcColor const CcStyle::ButtonBorderColor            (0x00, 0x00, 0x00);
uint16  const CcStyle::ButtonBorderSize             (1);
CcSizeRelative const CcStyle::ButtonSizeRelative    (0.9, 0.9);
CcColor const CcStyle::ButtonHoverBackgroundColor   (0xf0, 0xf0, 0xf0);
CcColor const CcStyle::ButtonHoverForegroundColor   (0x00, 0x00, 0x00);
CcColor const CcStyle::ButtonHoverBorderColor       (0x00, 0x00, 0x00);
uint16  const CcStyle::ButtonHoverBorderSize        (1);
CcColor const CcStyle::ButtonActiveBackgroundColor  (0x80, 0x80, 0x80);
CcColor const CcStyle::ButtonActiveForegroundColor  (0xff, 0xff, 0xff);
CcColor const CcStyle::ButtonActiveBorderColor      (0xff, 0xff, 0xff);
uint16  const CcStyle::ButtonActiveBorderSize       (1);

