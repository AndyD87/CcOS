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
 * @brief     Class CcScpiServerWorker
 */
#pragma once

#include "CcBase.h"
#include "CcScpi.h"
#include "CcUser.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "IWorker.h"
#include "CcScpiServerConfig.h"

/**
 * @brief Worker-Class for FTP. Each incomming Connection to Server
 *        will create this Object.
 *        It is getting automaticaly closed if connection is stopped or
 *        interrupted.
 */
class CcScpiSHARED CcScpiServerWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   * @param inData:  Incoming received data.
   * @param oSocket: Socket opend from Server to work with.
   * @param hServerConfig: Pointer to Server to access for example UserList
   */
  CcScpiServerWorker(CcByteArray* inData, CcSocket oSocket, CcScpiServerConfigHandle hServerConfig);

  /**
   * @brief Destructor
   */
  virtual ~CcScpiServerWorker();

  /**
   * @brief Main Loop for Worker, afert leaving this, the Worker is getting deleted
   */
  void run();

private: // Methods
private: // Member
  CcSocket     m_pSocket; //!< Socket received from Server
  CcByteArray  *m_InData = nullptr; //!< Temporary Input Buffer for operating..
  CcScpiServerConfigHandle m_hServerConfig; //!< Pointer to Server which was creating
};
