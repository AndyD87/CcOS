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
 * @subpage   CcTaskbar
 *
 * @page      CcTaskbar
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTaskbar
 */
#ifndef H_CCTASKBAR_H_
#define H_CCTASKBAR_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcWidget.h"

class CcMenu;
class CcButton;

/**
 * @brief A Taskbar which can hold A Menu and Tray Icons
 */
class CcGuiSHARED CcTaskbar : public CcWidget 
{
public:
  /**
   * @brief Constructor
   */
  CcTaskbar(CcWidget* rParent);

  /**
   * @brief Destructor
   */
  virtual ~CcTaskbar();

  /**
   * @brief Set Menue showed frist in Taskbar
   * @param menu: Menu with filled menu-tree
   */
  CcMenu * createMenu();

  /**
   * @brief Draw menue beginning left
   */
  void drawMenu();

  /**
   * @brief Remove Menu from display
   */
  void hideMenu();

  /**
   * @brief Draw a dummy window to center, to receive callback for closing menue
   */
  void drawDummy();

  /**
   * @brief Remove Dummy from display
   */
  void hideDummy();

  /**
   * @brief Draw tray icons on right side
   */
  void drawTray();

  /**
   * @brief set target window for applications
   * @param center: Pointer to window of target
   */
  void setCenterWindow(CcWidget* center);

  /**
   * @brief Delete all Draws outside of Taskbar.
   *      Function gets called from callback.
   */
  void DeleteDraws(void* pParam);
private:
  CcButton      *m_DummyCenter = nullptr;//!< Dummycenter to overlap Target Window.
  CcMenu        *m_Menu        = nullptr;       //!< Menu tree to show at first.
  CcWidget* m_Center;     //!< Target window for all elements.
};

#endif // H_CcTASKBAR_H_
