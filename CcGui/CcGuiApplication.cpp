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
  m_oWindow = new CcWindow(); 
  CCMONITORNEW(m_oWindow.getPtr());
  m_oWindow->init();
}

CcGuiApplication::CcGuiApplication(const CcString& sAppName):
  CcApp(sAppName)
{
  m_oWindow = new CcWindow(); 
  CCMONITORNEW(m_oWindow.getPtr());
  m_oWindow->setTitle(getTitle());
  m_oWindow->init();
}

CcGuiApplication::CcGuiApplication(const CcString& sAppName, const CcUuid& oUuid) :
  CcApp(sAppName, oUuid)
{
  m_oWindow = new CcWindow();
  CCMONITORNEW(m_oWindow.getPtr());
  m_oWindow->setTitle(getTitle());
  m_oWindow->init();
}

CcGuiApplication::~CcGuiApplication() 
{
}


void CcGuiApplication::run()
{
  //m_oWindow->setSize(CcSize(400, 300));
  m_oWindow->getCloseHandler() += CcEventHandle(new CcEvent<CcGuiApplication, void>(this, &CcGuiApplication::eventWindowClose));
  m_oWindow->draw();
  m_oWindow->loop();
  CCDEBUG("Window ended");
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

void CcGuiApplication::onStopped()
{
  m_oWindow.deleteCurrent();
}
