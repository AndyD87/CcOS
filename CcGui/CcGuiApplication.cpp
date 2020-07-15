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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcGuiApplication
 */
#include "CcGuiApplication.h"
#include "CcKernel.h"
#include "CcEventHandler.h"

CcGuiApplication::CcGuiApplication()
{
  CcKernel::initGUI();
  CCNEW(m_oWindow, CcWindow);
  m_oWindow->init();
}

CcGuiApplication::CcGuiApplication(const CcString& sAppName):
  CcApp(sAppName)
{
  CCNEW(m_oWindow, CcWindow);
  m_oWindow->init();
}

CcGuiApplication::CcGuiApplication(const CcString& sAppName, const CcUuid& oUuid) :
  CcApp(sAppName, oUuid)
{
  CCNEW(m_oWindow, CcWindow);
  m_oWindow->setTitle(getTitle());
  m_oWindow->init();
}

CcGuiApplication::~CcGuiApplication() 
{
}

void CcGuiApplication::run()
{
  m_oWindow->setTitle(getName());
  m_oWindow->getCloseHandler() += __NewCcEventType(CcGuiApplication, void, CcGuiApplication::eventWindowClose, this);
  m_oWindow->draw();
  // Call user application with available window
  onWindowLoaded();

  // Show
  m_oWindow->show();

  // Start event loop
  m_oWindow->loop();
  CCDEBUG("Window ended");
}

void CcGuiApplication::onWindowLoaded()
{
}

void CcGuiApplication::close()
{
  enterState(EThreadState::Stopping);
}

void CcGuiApplication::eventWindowClose(void* pParam)
{
  CCUNUSED(pParam);
  close();
}

void CcGuiApplication::onStop()
{
  m_oWindow->setWindowState(EWindowState::Close);
}
