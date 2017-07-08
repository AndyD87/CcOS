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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @file      SyncClientSvc.h
 * @brief     Class SyncClientSvc
 *
 *  Implementation of Main Application
 */

#include "SyncClientSvc.h"
#include "CcKernel.h"
#include "CcAppKnown.h"

SyncClientSvc::SyncClientSvc(const CcArguments& oArguments) :
  m_oArguments(oArguments),
  CcApp(CcAppKnown::CcSyncClientName, CcAppKnown::CcSyncClientUuid)
{
}

SyncClientSvc::~SyncClientSvc() {
}

void SyncClientSvc::run()
{
  m_poSyncClient = new CcSyncClient("");
  CCMONITORNEW(m_poSyncClient);
  if (m_poSyncClient->setupLocations())
  {
    while (getThreadState() == EThreadState::Running)
    {
      m_poSyncClient->resetQueues();
      // Try to login to server
      if (m_poSyncClient->login())
      {
        // Compare locale filelist with server filelist
        m_poSyncClient->checkForServerUpdates();
        // Process Data if available
        m_poSyncClient->doQueue();
        // Check if local filesystem has changed
        m_poSyncClient->scanDirectories();
        // Process Data if available
        m_poSyncClient->doQueue();
        // Close connection to save ressources
        m_poSyncClient->close();
      }
      else
      {
        CCDEBUG("Unable to login to server, stop client");
        setExitCode((int32) EError::UserLoginFailed);
      }
      CcKernel::delayS(30);
    }
  }
  else
  {
    CCDEBUG("Error in Directory Locations, stop progress");
    setExitCode((int32) EError::FSDirNotFound);
  }

  CCMONITORDELETE(m_poSyncClient);
  delete m_poSyncClient;
}