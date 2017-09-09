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
 * @subpage   CcGuiApplication
 *
 * @page      CcGuiApplication
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcGuiApplication
 */
#ifndef CcGuiApplication_H_
#define CcGuiApplication_H_

#include "CcBase.h"
#include "CcGui.h"
#include "CcApp.h"
#include "CcWindow.h"
#include "CcList.h"

/**
 * @brief Menue-Tree start point with settings for display
 */
class CcGuiSHARED CcGuiApplication : public CcApp {
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
  virtual ~CcGuiApplication(void);

  virtual void run(void) override;

  void close();

  CcWindowHandle getWindow()
    { return m_oWindow.handle(); }

  inline const CcString& getTitle()
    { return m_sTitle; }
  inline void setTitle(const CcString& sTitle)
    { m_sTitle = sTitle; }
private:
  void eventWindowClose(void* pParam);
  void onStopped() override;

private:
  CcWindowPointer m_oWindow;
  CcString m_sTitle;
};

#endif /* CcGuiApplication_H_ */
