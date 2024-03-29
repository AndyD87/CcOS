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
 * @brief     Class CcApp
 */
#include "CcApp.h"
#include "CcKernel.h"
#include "CcEvent.h"
#include "CcEventHandler.h"
#include "CcVersion.h"
#include "CcConsole.h"

CcApp::CcApp() 
{
  initApp();
}

/**
 * @brief Constructor
 */
CcApp::CcApp(const CcString& sAppName, const CcVersion& oVersion, const CcUuid& oUuid) :
  IThread(sAppName),
  m_oVersion(oVersion),
  m_oId(oUuid)
{
  initApp();
}

CcApp::~CcApp()
{
  CcKernel::deregisterShutdownHandler(this);
}

CcStatus CcApp::exec()
{
  return startOnCurrent();
}

const CcVersion& CcApp::getVersion() const
{
  // return default version
  return m_oVersion;
}

void CcApp::initApp()
{
// GENERIC OS will not shutdown at the moment, so save ressources!
#ifndef GENERIC
  if(CcKernel::getEnvironmentVariableExists(CcGlobalStrings::EnvVars::AppNoIoBuffering))
  {
    CcConsole::disableBuffering();
  }
#endif
}
