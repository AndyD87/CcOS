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
 * @page      CcGui
 * @subpage   CcCrossButton
 *
 * @page      CcCrossButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcCrossButton
 */
#ifndef CCCROSSBUTTON_H
#define CCCROSSBUTTON_H

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcButton.h"

class CcGuiSHARED CcCrossButton : public CcButton
{
public:
  CcCrossButton(CcWidget* parent);
  virtual ~CcCrossButton();

  void setCrossColor(uint8 R, uint8 G, uint8 B);
  void setCrossSize(uint8 size);
  void drawCross( void );

private:
  uint8 m_R;
  uint8 m_G;
  uint8 m_B;
  uint8 m_CrossSize;
};

#endif // CCCROSSBUTTON_H
