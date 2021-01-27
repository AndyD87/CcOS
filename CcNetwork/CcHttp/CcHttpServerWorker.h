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
 * @brief     Class CcHttpServerWorker
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcHttpWorkData.h"
#include "IWorker.h"

class CcHttpServer;

/**
 * @brief Worker class of http server.
 *        The Worker will be started from server on incomming transmissions.
 */
class CcHttpSHARED CcHttpServerWorker : public IWorker
{
public:
  /**
   * @brief Create worker with handle to starting server and communication socket.
   * @param oServer: Parent server.
   * @param oSocket: Accepted connection from server
   */
  CcHttpServerWorker(CcHttpServer& oServer, const CcSocket& oSocket);
  virtual ~CcHttpServerWorker();

  /**
   * @brief For generic devices, setup stack size for reducing memory consumption.
   * @return Number of bytes required for handling all requests.
   */
  virtual size_t getStackSize() override
  { return 4096; }

  /**
   * @brief Main loop of worker.
   *        It will receive header informations, and forward request to server provider.
   */
  void run() override;

  /**
   * @brief Check if buffer contains the header end tag.
   * @param sInputData:       Already read data.
   * @param uiContentOffset:  Current offset wich was already read.
   * @return
   */
  CcStatus chkReadBuf(const CcString& sInputData, size_t& uiContentOffset);

  /**
   * @brief Finish an incomming transmission.
   *        It will send last data if some are left and close connections.
   */
  void finish();

  /**
   * @brief Will be called if error occured for printing errors on commandline or log.
   */
  void error();
private:
  CcHttpWorkData m_oData;
};
