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
 * @file      SyncClientCli.h
 * @brief     Class SyncClientCli
 *
 *  Implementation of Main Application
 */

#include "SyncClientCli.h"
#include "CcKernel.h"
#include "CcAppKnown.h"
#include "CcSyncConsole.h"

namespace Strings
{
static const CcString Login        ("login");
static const CcString LoginDesc    ("Login to account.");
static const CcString Logout       ("logout");
static const CcString AccountInfo  ("accountinfo");
static const CcString Reset        ("reset");
static const CcString Sync         ("sync");
static const CcString SyncServer   ("server");
static const CcString SyncClient   ("client");
static const CcString DoQueueShort ("do");
static const CcString DoQueue      ("doqueue");
static const CcString Exit         ("exit");
static const CcString Help         ("help");
static const CcString HelpDesc     ("This help Message");
}

SyncClientCli::SyncClientCli(const CcArguments& oArguments) :
  CcApp(CcAppKnown::CcSyncClientName, CcAppKnown::CcSyncClientUuid),
  m_oArguments(oArguments)
{
}

SyncClientCli::~SyncClientCli() {
}

void SyncClientCli::run()
{
  // @todo add accountname from commandline, or current user
  m_poSyncClient = new CcSyncClient(""); 
  CCMONITORNEW(m_poSyncClient);
  if (m_poSyncClient->setupLocations())
  {
    bool bCommandlineLoop = true;
    while (bCommandlineLoop)
    {
      CcString sCommandLine = CcSyncConsole::clientQuery();
      CcArguments oArguments(sCommandLine);
      if (oArguments[0].compareInsensitve(Strings::Login))
      {
        if (oArguments.size() > 1)
        {
          if (m_poSyncClient->selectAccount(oArguments[1]))
          {
            CcSyncConsole::writeLine("Account selcted: " + m_poSyncClient->getAccountName());
          }
          else
          {
            CcSyncConsole::writeLine("No account found");
          }
        }
        else
        {
          if (m_poSyncClient->selectAccount(""))
          {
            CcSyncConsole::writeLine("First account selcted: " + m_poSyncClient->getAccountName());
          }
          else
          {
            CcSyncConsole::writeLine("No account found");
          }
        }
        if (m_poSyncClient->login())
        {
          CcSyncConsole::writeLine("Login succeeded");
        }
        else
        {
          CcSyncConsole::writeLine("Login failed");
        }
      }
      else if (oArguments[0].compareInsensitve(Strings::Logout))
      {
        m_poSyncClient->logout();
        CcSyncConsole::writeLine("Logout done, connection closed");
      }
      else if (oArguments[0].compareInsensitve(Strings::AccountInfo))
      {
        CcSyncConsole::writeLine(m_poSyncClient->getAccountData());
      }
      else if (oArguments[0].compareInsensitve(Strings::Reset))
      {
        m_poSyncClient->resetQueues();
      }
      else if (oArguments[0].compareInsensitve(Strings::Sync))
      {
        if (oArguments.size() > 1)
        {
          if (oArguments[1].compareInsensitve(Strings::SyncClient))
          {
            CcSyncConsole::writeLine("Start local sync");
            m_poSyncClient->scanDirectories();
            CcSyncConsole::writeLine("Local sync done");
          }
          else if (oArguments[1].compareInsensitve(Strings::SyncServer))
          {
            CcSyncConsole::writeLine("Start remote sync");
            m_poSyncClient->checkForServerUpdates();
            CcSyncConsole::writeLine("Remote sync done");
          }
          else
          {
            CcSyncConsole::writeLine("Please select Server or Client as secondary paramter, or none for both");
          }
        }
        else
        {
          CcSyncConsole::writeLine("Reset Queue");
          m_poSyncClient->resetQueues();
          CcSyncConsole::writeLine("Remote sync: start");
          m_poSyncClient->checkForServerUpdates();
          CcSyncConsole::writeLine("Remote sync: done");
          CcSyncConsole::writeLine("Local sync: start");
          m_poSyncClient->scanDirectories();
          CcSyncConsole::writeLine("Local sync: done");
        }
      }
      else if (oArguments[0].compareInsensitve(Strings::DoQueue)      ||
               oArguments[0].compareInsensitve(Strings::DoQueueShort) )
      {
        m_poSyncClient->doQueue();
      }
      else if (oArguments[0].compareInsensitve(Strings::Exit))
      {
        CcSyncConsole::writeLine("Bye :)");
        bCommandlineLoop = false;
      }
      else if (oArguments[0].compareInsensitve(Strings::Help))
      {
        CcSyncConsole::writeLine("  " + Strings::Login   + "  " + Strings::LoginDesc);
        CcSyncConsole::writeLine("  " + Strings::Logout  + "  " + Strings::HelpDesc);
        CcSyncConsole::writeLine("  " + Strings::Sync    + "  " + Strings::HelpDesc);
        CcSyncConsole::writeLine("  " + Strings::DoQueue + "/" + Strings::DoQueueShort + "  " + Strings::HelpDesc);
        CcSyncConsole::writeLine("  " + Strings::Reset   + "  " + Strings::HelpDesc);
        CcSyncConsole::writeLine("  " + Strings::Exit    + "  " + Strings::HelpDesc);
        CcSyncConsole::writeLine("  " + Strings::Help    + "  " + Strings::HelpDesc);
      }
      else
      {
        CcSyncConsole::writeLine("Unknown Command");
      }
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