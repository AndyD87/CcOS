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
 * @brief     Class CcButton
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "Devices/ITouch.h"
#include "CcWidget.h"
#include "Types/CcPoint.h"
#include "Style/CcStyleButton.h"

/**
 * @brief Basic Button Class for GUI Applications.
 *        It handles all Click Events for Buttons.
 */
class CcGuiSHARED CcButton : public CcWidget
{
public:
  /**
   * @brief Constructor
   */
  CcButton( CcWidget* rParent);

  /**
   * @brief Destructor
   */
  virtual ~CcButton();

  virtual bool setPixelArea(const CcRectangle& oRectangle) override;
  virtual void draw(bool bDoFlush = true) override;
  virtual void drawPixel(const CcColor& oColor, uint64 uiNumber = 1) override;

  //! @return True if mouse hovers this button
  bool isHovered() const;

  /**
   * @brief Set style widget style on hover.
   * @param bActive:          True for setting style on hover when activated or not
   * @param oForegroundColor: Target Foreground color
   * @param oBackgroundColor: Target Background color
   * @param oBorderColor:     Target BorderColor color
   * @param uiBorderSize:     Target BorderSize
   */
  void setHoverStyle(bool bActive, const CcColor &oForegroundColor, const CcColor &oBackgroundColor, const CcColor &oBorderColor, uint16 uiBorderSize);

  //! @return Get current style of button
  CcStyleButton& getStyle();
  //! @return Get current style of button
  const CcStyleButton& getStyle() const;
  //! @return Get widget style of button
  inline CcStyleWidget& getWidgetStyle()
  { return CcWidget::getStyle(); }
  //! @return Get widget style of buttwon
  inline const CcStyleWidget& getWidgetStyle() const
  { return CcWidget::getStyle(); }

  /**
   * @brief Set text displayed in the middle of the button
   * @param sString: String containing text to be displayed
   */
  void setText( const CcString& sString );

  /**
   * @brief get reference to stored String
   * @return pointer to String
   */
  const CcString& getText();

protected:
  virtual void onEvent(CcInputEvent* pEventData) override;
  virtual void onMouseEvent(CcMouseEvent* pEventData) override;
  virtual void onKeyEvent(CcKeyEvent* pEventData) override;
  virtual void onWindowEvent(CcInputEvent* pEventData) override;
  virtual void setCustomPainting(bool bEnable) override;

  //! @param pParam: Mouse event for hover received
  virtual void onMouseHover(CcMouseEvent* pParam);
  //! @param pParam: Mouse event for hover end received
  virtual void onMouseLeave(CcMouseEvent* pParam);
  //! @param pParam: Mouse event for click received
  virtual void onMouseClick(CcMouseEvent* pParam);
  //! @param pParam: Mouse event for double click received
  virtual void onMouseDoubleClick(CcMouseEvent* pParam);
  //! @brief Called on size or position changes.
  virtual void onRectangleChanged() override;

public: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
};
