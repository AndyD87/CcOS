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
#include "CcGui.h"
#include "CcColor.h"

/**
 * @brief Class implementation
 */
class CcGuiSHARED CcStyleButton
{
public:
  /**
   * @brief Constructor
   */
  CcStyleButton();
  
  /**
   * @brief Constructor
   */
  CcStyleButton(const CcColor& oHoverBackgroundColor, 
                const CcColor& oHoverForegroundColor, 
                const CcColor& oHoverBorderColor,
                const uint16  uHoverBorderSize) :
    oHoverBackgroundColor(oHoverBackgroundColor),
    oHoverForegroundColor(oHoverForegroundColor),
    oHoverBorderColor(oHoverBorderColor),
    uiHoverBorderSize(uHoverBorderSize)
  {}
  
  /**
   * @brief Destructor
   */
  ~CcStyleButton() = default;

  CcColor oHoverBackgroundColor;
  CcColor oHoverForegroundColor;
  CcColor oHoverBorderColor;
  uint16  uiHoverBorderSize;
  bool    bHoverActive = false;
};

#endif // H_CcStyleButton_H_
