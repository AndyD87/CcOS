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
 * @subpage   CcPushButton
 *
 * @page      CcPushButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcPushButton
 */
#ifndef _CCPUSHBUTTON_H_
#define _CCPUSHBUTTON_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "CcText.h"
#include "CcButton.h"

#define CCPUSHBUTTON_DEFAULT_BORDERSIZE        1
#define CCPUSHBUTTON_DEFAULT_BORDERCOLOR_R     0xff
#define CCPUSHBUTTON_DEFAULT_BORDERCOLOR_G     0xff
#define CCPUSHBUTTON_DEFAULT_BORDERCOLOR_B     0xff

class CcPushButtonPrivate;

/**
 * @brief Push-Button with containing Text for GUI
 */
class CcGuiSHARED CcPushButton : public CcButton 
{
public:
  /**
   * @brief Constructor
   */
  CcPushButton(const CcWidgetHandle& rParent);

  /**
   * @brief Destructor
   */
  virtual ~CcPushButton();

  /**
   * @brief draw all data
   */
  void draw(void);

  /**
   * @brief publish Button on Display
   */
  void drawButton(void);

  /**
   * @brief Set text displayed in the middle of the button
   * @param sString: String containing text to be displayed
   */
  void setText( const CcString& sString );

  /**
   * @brief get reference to stored String
   * @return pointer to String
   */
  const CcString& getString(void);

private:
  CcPushButtonPrivate * m_pPrivate;
};

#endif /* _CCPUSHBUTTON_H_ */
