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
#include "CcStyleWidget.h"

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
   * @brief Destructor
   */
  ~CcStyleButton() = default;

  CcStyleWidget oHoverStyle;
  bool          bHoverActive = true;

  static CcStyleWidget oDefaultWidgetStyle;

};

#endif // H_CcStyleButton_H_
