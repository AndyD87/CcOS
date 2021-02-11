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
 * @brief     Class CcStyleWidget
 **/
#pragma once

#include "CcBase.h"
#include "CcStyle.h"
#include "CcString.h"
#include "CcRectangle.h"
#include "CcSizeRelative.h"

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcStyleWidget 
{
public:
  CcStyleWidget();
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcStyleWidget);

  /**
   * @brief Create style for widget with specific values
   * @param oBackgroundColor: Background color
   * @param oForegroundColor: Foreground color
   * @param oBorderColor:     Border color
   * @param uBorderSize:      Size of border
   * @param oRelative:        Size relative to parent
   */
  CcStyleWidget(const CcColor& oBackgroundColor,
                const CcColor& oForegroundColor,
                const CcColor& oBorderColor,
                const uint16  uBorderSize,
                const CcSizeRelative &oRelative);
  virtual ~CcStyleWidget() = default;

  /**
   * @brief Copy style form another widget to this
   * @param oStyleSheet: Style to copy from
   * @return Handle to this
   */
  CcStyleWidget& operator=(const CcStyleWidget& oStyleSheet);

  CcRectangle     oRectangle;     //!< Dimensions of widget
  CcString sBackgroundImage;      //!< Background image if exists
  CcColor oBackgroundColor;       //!< Background color
  CcColor oForegroundColor;       //!< Foreground color
  CcColor oBorderColor    ;       //!< Border color
  uint16  uBorderSize     = 1;    //!< Border size
  CcSizeRelative  oSizeRelative;  //!< Relative size if set
};
