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
 * @brief     Class CcCrossButton
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcButton.h"

/**
 * @brief Create a button with a cross as display
 */
class CcGuiSHARED CcCrossButton : public CcButton
{
public:
  /**
   * @brief Create Crossbutton
   * @param rParent: Parent widget to assign to, or nullptr
   */
  CcCrossButton(CcWidget* rParent);
  virtual ~CcCrossButton();

  /**
   * @brief Set Color of cross
   * @param uiR:  Red value
   * @param uiG:  Blue value
   * @param uiB:  Grean value
   */
  void setCrossColor(uint8 uiR, uint8 uiG, uint8 uiB);
  //! @param uiSize: Set thickness of cross
  void setCrossSize(uint8 uiSize);
  //! @brief Draw cross to gui
  void drawCross();

private:
  uint8 m_uiR;
  uint8 m_uiG;
  uint8 m_uiB;
  uint8 m_uiSize;
};
