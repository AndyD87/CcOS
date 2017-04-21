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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcButton
 */
#ifndef CCBUTTON_H_
#define CCBUTTON_H_

#include "CcBase.h"
#include "CcGui.h"
#include "Devices/CcTouch.h"
#include "CcWidget.h"
#include "Types/CcPoint.h"

/**
 * @brief Basic Button Class for GUI Applications.
 *        It handles all Click Events for Buttons.
 */
class CcGuiSHARED CcButton : public CcWidget{
public:
  /**
   * @brief Constructor
   */
  CcButton( CcWidget* parent);

  /**
   * @brief Destructor
   */
  virtual ~CcButton();
  
  inline bool getFocus() const
    { return m_bIsFocused; }
  inline bool getFocusable() const
    { return m_bIsFocusable; }
  inline bool getActive() const
    { return m_bIsActive; }
  inline bool getHovered() const
    { return m_bIsHovered; }

  virtual void draw()override;

  virtual void onMouseHover(const CcPoint& oPosition)
    {CCUNUSED(oPosition);}
  virtual void onMouseLeave()
    {}
  virtual void onMouseClick(const CcPoint& oPosition)
    {CCUNUSED(oPosition);}
  virtual void onMouseDoubleClick(const CcPoint& oPosition)
    {CCUNUSED(oPosition);}
  
  inline void setHoverColor(const CcColor& oColor)
    { m_oHoverColor = oColor; }
  inline const CcColor& getHoverColor(void)
    { return m_oHoverColor; }
protected:
  void onMouseHover(CcMouseEvent* pParam);
  void onMouseLeave(CcMouseEvent* pParam);
  void onMouseClick(CcMouseEvent* pParam);
  void onMouseDoubleClick(CcMouseEvent* pParam);

private:
  bool m_bIsHovered = false;
  bool m_bIsActive = false;
  bool m_bIsFocused = false;
  bool m_bIsFocusable = false;
  CcColor m_oHoverColor = CcStyle::ButtonHoverBackgroundColor;
};

#endif /* CCBUTTON_H_ */
