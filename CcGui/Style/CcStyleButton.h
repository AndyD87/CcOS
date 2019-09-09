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
 * @page      Style
 * @subpage   CcStyleButton
 *
 * @page      CcStyleButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStyleButton
 **/
#ifndef H_CcStyleButton_H_
#define H_CcStyleButton_H_

#include "CcBase.h"
#include "CcStyleWidget.h"

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcStyleButton : public CcStyleWidget
{
public:
  /**
   * @brief Constructor
   */
  CcStyleButton();
  
  /**
   * @brief Constructor
   */
  CcStyleButton(const CcColor& oBackgroundColor,
                const CcColor& oForegroundColor,
                const CcColor& oBorderColor,
                const uint16  uBorderSize,
                const CcColor& oHoverBackgroundColor, 
                const CcColor& oHoverForegroundColor, 
                const CcColor& oHoverBorderColor,
                const uint16  uHoverBorderSize) :
    CcStyleWidget(oBackgroundColor, oForegroundColor, oBorderColor,uBorderSize),
    HoverBackgroundColor(oHoverBackgroundColor),
    HoverForegroundColor(oHoverForegroundColor),
    HoverBorderColor(oHoverBorderColor),
    HoverBorderSize(uHoverBorderSize)
  {}
  
  /**
   * @brief Constructor
   */
  CcStyleButton(const CcStyleWidget& rWidgetStyle);

  /**
   * @brief Destructor
   */
  virtual ~CcStyleButton();

  CcColor HoverBackgroundColor;
  CcColor HoverForegroundColor;
  CcColor HoverBorderColor;
  uint16 HoverBorderSize;

public:
  static CcStyleButton Default;
};

#endif // H_CcStyleButton_H_
