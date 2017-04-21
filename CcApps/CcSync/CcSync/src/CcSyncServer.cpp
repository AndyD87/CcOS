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
* @par       Web: http://adirmeier.de/CcOS
* @version   0.01
* @date      2016-04
* @par       Language   C++ ANSI V3
* @brief     Implemtation of class CcSyncServer
*/
#include "CcSyncServer.h"
#include "CcSyncGlobals.h"
#include "CcKernel.h"
#include "CcDirectory.h"
#include "Network/CcSocket.h"
#include "CcSyncServerWorker.h"
#include "CcSyncClientConfig.h"
#include "CcSslControl.h"
#include "CcSslSocket.h"
#include "CcFile.h"
#include "Hash/CcMd5.h"

CcSyncServer::CcSyncServer(void)
{
}

CcSyncServer::CcSyncServer(const CcSyncServer& oToCopy)
{
  operator=(oToCopy);
}

CcSyncServer::CcSyncServer(CcSyncServer&& oToMove)
{
  operator=(std::move(oToMove));
}

CcSyncServer::~CcSyncServer(void)
{
}

void CcSyncServer::run()
{
  CcString sConfigFile = m_sDatabaseFile = CcKernel::getConfigDir();
  sConfigFile.appendPath(CcSyncGlobals::ConfigDirName);
  if (!CcDirectory::exists(sConfigFile))
  {
    CCINFO("No configuration Directory on default config-directory");
    CcDirectory::create(sConfigFile);
  }
  else
  {
    if (setupDatabase())
    {
      sConfigFile.appendPath(CcSyncGlobals::Server::ConfigFileName);
      m_oConfig.readConfig(sConfigFile);
    }
    else
      CCERROR("No database file available");
  }
  if(CcFile::exists(m_oConfig.getSslCertFile()) == false ||
     CcFile::exists(m_oConfig.getSslKeyFile()) == false)
  {
    CcSslControl::createCert(m_oConfig.getSslCertFile(), m_oConfig.getSslKeyFile());
  }
  m_pSocket = new CcSslSocket(); 
  CCMONITORNEW(m_pSocket);
  m_pSocket->initServer();
  if (m_pSocket->bind(m_oConfig.getPort())                   &&
      m_pSocket->loadKey(m_oConfig.getSslKeyFile())          &&
      m_pSocket->loadCertificate(m_oConfig.getSslCertFile()) )
  {
    CCDEBUG("Server is listening on: " + CcString::fromNumber(m_oConfig.getPort()));
    while (getThreadState() == EThreadState::Running)
    {
      if (m_pSocket->listen())
      {
        CcSocket* oTemp = m_pSocket->accept();
        if (oTemp != nullptr)
        {
          CCDEBUG("Server recognized an incomming connection, starting thread");
          CcSyncServerWorker* oNewWorker = new CcSyncServerWorker(this, oTemp); 
          CCMONITORNEW(oNewWorker);
          oNewWorker->start();
        }
        else
        {
          CCERROR("Error on accepting connection");
        }
      }
      else
      {
        CCERROR("Error on listening");
      }
    }
    CCDEBUG("Server is going down");
  }
  else
  {
    CCERROR("Unable to bind Socket");
  }
  delete m_pSocket;
}

CcSyncServer& CcSyncServer::operator=(const CcSyncServer& oToCopy)
{
  m_oConfig     = oToCopy.m_oConfig;
  return *this;
}

CcSyncServer& CcSyncServer::operator=(CcSyncServer&& oToMove)
{
  if (this != &oToMove)
  {
    m_oConfig     = std::move(oToMove.m_oConfig);
  }
  return *this;
}

CcSyncUser CcSyncServer::loginUser(const CcString& sAccount, const CcString & sUserName, const CcString & sPassword)
{
  CcString sToken;
  for (const CcSyncServerAccount& rAccount : m_oConfig.getAccountList())
  {
    if (rAccount.getName().compareInsensitve(sAccount))
    {
      // @todo: Check for Additional Users
      if (rAccount.getPassword().getString() == sPassword )
      {
        sToken << rAccount.getName() << CcString::fromNumber(CcKernel::getDateTime().getTimestampUs());
        CcMd5 oTokenGenerator;
        oTokenGenerator.generate(sToken);
        sToken = oTokenGenerator.getHexString();
        if (m_oDatabase.userExistsInDatabase(sAccount, sUserName))
        {
          if (false == m_oDatabase.updateUser(rAccount.getName(), rAccount.getName(), sToken))
          {
            sToken = "";
          }
          else
          {
            break;
          }
        }
        else
        {
          if (false == m_oDatabase.insertUser(rAccount.getName(), rAccount.getName(), sToken))
          {
            sToken = "";
          }
          else
          {
            break;
          }
        }
      }
    }
  }
  return getUserByToken(sToken);
}

CcSyncUser CcSyncServer::getUserByToken(const CcString& sToken)
{
  CcSyncUser oUser;
  CcString sUsername;
  CcString sAccount;
  if (m_oDatabase.getUserByToken(sToken, sAccount, sUsername))
  {
    CcSyncServerAccount* pAccount = m_oConfig.findAccount(sUsername);
    if (pAccount != nullptr)
    {
      CcSyncClientConfig* pClientConfig = pAccount->clientConfig();
      if (pClientConfig != nullptr)
      {
        CcSyncAccountConfig* pAccountConfig = pClientConfig->getAccountConfig(sAccount);
        if (pAccountConfig != nullptr)
        {
          ESyncRights eRights = ESyncRights::Account;
          if (eRights == ESyncRights::Account &&
              pAccount->isAdmin())
          {
            eRights = ESyncRights::Admin;
          }
          CcSyncDbClient* pClientDatabase = pAccount->database();
          if (pClientDatabase != nullptr)
          {
            oUser = CcSyncUser(sToken, pClientConfig, pAccountConfig, pClientDatabase, eRights);
          }
        }
      }
    }
  }
  return oUser;
}

bool CcSyncServer::setupDatabase()
{
  m_sDatabaseFile = CcKernel::getConfigDir();
  m_sDatabaseFile.appendPath(CcSyncGlobals::ConfigDirName);
  m_sDatabaseFile.appendPath(CcSyncGlobals::Server::DatabaseFileName);
  return m_oDatabase.openDatabase(m_sDatabaseFile);
}
