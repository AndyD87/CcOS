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
 * @subpage   CcButton
 *
 * @page      CcButton
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcButton
 */
#ifndef H_CCBUTTON_H_
#define H_CCBUTTON_H_

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
  CcButton( const CcWidgetHandle& rParent);

  /**
   * @brief Destructor
   */
  virtual ~CcButton();

  virtual bool setPixelArea(const CcRectangle& oRectangle) override;
  virtual void draw(bool bDoFlush = true) override;
  virtual void drawPixel(const CcColor& oColor, uint64 uiNumber = 1) override;

  bool isHovered() const
    { return m_bIsHoverd; }
  
  CcStyleButton* getStyle()
    {return static_cast<CcStyleButton*>(CcWidget::getStyle());}
  const CcStyleButton* getStyle() const
    {return static_cast<const CcStyleButton*>(CcWidget::getStyle());}

protected:

  void initStyle();
  void initSubSystem();
  virtual void onEvent(EGuiEvent eEvent, void *pMouseEvent) override;
  virtual void onMouseEvent(EGuiEvent eEvent, CcMouseEvent* pMouseEvent) override;
  virtual void onKeyEvent(EGuiEvent eEvent, CcKeyEvent* pMouseEvent) override;
  virtual void onWindowEvent(EGuiEvent eEvent) override;
  virtual void setCustomPainting(bool bEnable) override;

  virtual void onMouseHover(CcMouseEvent* pParam);
  virtual void onMouseLeave(CcMouseEvent* pParam);
  virtual void onMouseClick(CcMouseEvent* pParam);
  virtual void onMouseDoubleClick(CcMouseEvent* pParam);

  virtual void onRectangleChanged() override;
  virtual void onBackgroundChanged() override;
  virtual void onForegroundChanged() override;

private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate;
  bool      m_bIsHoverd = false;
};

#endif // H_CcBUTTON_H_
