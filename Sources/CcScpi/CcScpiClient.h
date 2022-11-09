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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcScpiClient
 */
#pragma once

#include "CcBase.h"
#include "CcScpi.h"
#include "IThread.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"

#include "Network/CcSocket.h"

/**
 * @brief Button for GUI Applications
 */
class CcScpiSHARED CcScpiClient : public IThread
{
public:
  /**
   * @brief Constructro
   */
  CcScpiClient();

  /**
   * @brief Destructor
   */
  virtual ~CcScpiClient();

  CcStatus connect();
  bool isConnected()
  { return m_oSocket.isValid(); }

  CcStatus send(const CcString& sSend);
  CcStatus query(const CcString& sQuery, CcString& sResult);

  virtual void run() override
  {}

  void setServer(const CcString& sServer)
  { m_sServer = sServer; }
  
  void setPort(const CcString& sPort)
  { m_sPort = sPort; }


private:
  CcStatus m_oStatus;
  CcSocket m_oSocket;
  CcString m_sServer;
  CcString m_sPort;
};
