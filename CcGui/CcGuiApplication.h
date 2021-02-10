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
 * @brief     Class CcGuiApplication
 */
#pragma once

#include "CcBase.h"
#include "CcGui.h"
#include "CcApp.h"
#include "CcWindow.h"
#include "CcList.h"

/**
 * @brief Gui application with window
 */
class CcGuiSHARED CcGuiApplication : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcGuiApplication();

  /**
   * @brief Constructor
   */
  CcGuiApplication(const CcString& sAppName);

  /**
   * @brief Constructor
   */
  CcGuiApplication(const CcString& sAppName, const CcUuid& oUuid);

  /**
   * @brief Destructor
   */
  virtual ~CcGuiApplication() override;

  virtual void run() override;

  /**
   * @brief This method will be called if Window was created and setup, but not yet showed.
   */
  virtual void onWindowLoaded();

  /**
   * @brief Close gui application.
   *        Remove all windows and widgets and stop application.
   */
  void close();

  /**
   * @brief Get window of gui application.
   * @return Handle to basic window
   */
  CcWindowPointer& getWindow()
  { return m_oWindow; }
  //! @return Get title of application and window
  inline const CcString& getTitle()
  { return m_sTitle; }
  //! @param sTitle: Set title of application and window
  inline void setTitle(const CcString& sTitle)
    { m_sTitle = sTitle; }

protected:
  /**
   * @brief Event receiver for close event.
   * @param pParam: Parameter of close event, typically nullptr
   */
  void eventWindowClose(void* pParam);
  void onStop() override;

private:
  CcWindowPointer m_oWindow;
  CcString m_sTitle;
};
