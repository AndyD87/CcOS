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
 * @brief     Class CcTitlebar
 **/
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"
#include "Style/CcStyleWidget.h"

class CcCloseButton;
class CcMinimizeButton;
class CcMaximizeButton;

/**
 * @brief Class implementation
 */
class CcTitlebar : public CcWidget
{
public:
  /**
   * @brief Constructor
   */
  CcTitlebar(CcWidget* pParent );

  /**
   * @brief Destructor
   */
  virtual ~CcTitlebar();

  //! @return True if minimize button is set
  inline bool getMinimizeButton()
  { return m_oMinimizeButton != NULL; }
  //! @return True if maximize button is set
  inline bool getMaximizeButton()
  { return  m_oMaximizeButton != NULL; }
  //! @return True if close button is enabled
  inline bool getCloseButton()
  { return  m_oCloseButton != NULL; }
  //! @return Get height of titlebar
  int32 getHeight()
  { return m_uiHeight;}

  //! @param bEnable: True enables Minimize button
  void setMinimizeButton(bool bEnable);
  //! @param bEnable: True enables Maximize button
  void setMaximizeButton(bool bEnable);
  //! @param bEnable: True enables Close button
  void setCloseButton(bool bEnable);
  //! @param uiSize: Set size of top border if wanted
  void setTopLineSize(int32 uiSize)
  { m_uiTopLineSize = uiSize; }
  //! @param uiHeight: Set height of title bar
  void setHeight(int32 uiHeight);

private:
  virtual void draw(bool bDoFlush = true) override;
  void drawTopLine();
  virtual void onRectangleChanged() override;
  void onMouseLeftDown(CcMouseEvent* MouseEvent);
  void onMouseLeftUp(CcMouseEvent* MouseEvent);
  void onMouseMove(CcMouseEvent* MouseEvent);
  void onMouseLeave(CcMouseEvent* MouseEvent);
private:
  int32            m_uiHeight = CcStyle::TitlebarHeight;
  CcCloseButton*    m_oCloseButton    = NULL;
  CcMinimizeButton* m_oMinimizeButton = NULL;
  CcMaximizeButton* m_oMaximizeButton = NULL;
  int32            m_uiTopLineSize = 3;
  bool              m_bMouseDown = false;
  CcPoint           m_oMouseDownPoint;
  CcRectangle       m_oOriginalRect;
  CcStyleWidget     m_oTitlebarStyle;
};
