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
 * @brief     Class CcHttpServer
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcHandle.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcHttpRequest.h"
#include "CcHttpResponse.h"
#include "CcApp.h"
#include "CcHttpServerWorker.h"
#include "IHttpProvider.h"
#include "HttpProvider/CcHttpDefaultProvider.h"
#include "Network/CcCommonPorts.h"
#include "CcHttpServerConfig.h"

#ifdef _MSC_VER
template class CcHttpSHARED CcVector<IHttpProvider*>;
template class CcHttpSHARED CcHandle<IHttpProvider>;
template class CcHttpSHARED CcList<CcHandle<IHttpProvider>>;
#endif

/**
 * @brief Http server application
 */
class CcHttpSHARED CcHttpServer : public CcApp
{
public: // types
  //! Running state of server
  enum class EState
  {
    Stopped = 0,
    Starting,
    Listening
  };
public:
  /**
   * @brief Create simple server on specified port, and configure later.
   * @param Port: Target port
   */
  CcHttpServer(uint16 Port = CcCommonPorts::InvalidPort);

  /**
   * @brief Creat server by passing initial configuration to server
   * @param pConfig: Server configuration to use
   */
  CcHttpServer(CcHttpServerConfig* pConfig);

  virtual ~CcHttpServer();

  virtual void run() override;
  virtual void onStop() override;

  /**
   * @brief Register provider for incomming requests
   * @param toAdd: Provider to add to list
   */
  void registerProvider(const CcHandle<IHttpProvider> &toAdd);

  /**
   * @brief Remove existing provider from incoming requests
   * @param toRemove: Provider to remove
   * @return True if provider was available and is now removed
   */
  bool deregisterProvider(const CcHandle<IHttpProvider> &toRemove);

  /**
   * @brief Find provieder for http request workset
   * @param oData: Workset from request
   * @return Found provider or invalid handle if none found
   */
  const CcHandle<IHttpProvider> findProvider(const CcHttpWorkData &oData) const;

  //! Decrement current worker count
  void decWorker()
  { if (m_uiWorkerCount > 0) m_uiWorkerCount--; }
  //! Increment current worker count
  void incWorker()
  { m_uiWorkerCount++; }

  //! @return Get list of all provider
  const CcList<CcHandle<IHttpProvider>>& getReceiverList();
  //! @return Get current app state
  EState getState() const
  { return m_eState; }
  //! @return Get server configuration
  CcHttpServerConfig& getConfig()
  { return *m_pConfig; }
  //! @return Get server config as reference
  const CcHttpServerConfig& getConfig() const
  { return *m_pConfig; }
  //! @return Get request counter
  uint64 getRequestCount() const
  { return m_uiRequestsCount; }
  //! @return Get number of workers active
  uint16 getWorkerCount()
  { return m_uiWorkerCount; }

  //! @return Set port to bind before startup
  void setPort(uint16 uiPort)
  { m_pConfig->getAddressInfo().setPort(uiPort);}
  //! @return Set http configuration before startup
  void setConfig(CcHttpServerConfig* pConfig);

protected:
  //! Internal init
  void init();

private:
  uint16                            m_uiWorkerCount     = 0;
  uint64                            m_uiRequestsCount   = 0;
  CcHttpServerConfig*               m_pConfig           = nullptr;
  bool                              m_bConfigOwner      = false;
  CcSocket                          m_oSocket;
  CcList<CcHandle<IHttpProvider>>   m_ProviderList;
  IHttpProvider*                    m_pDefaultProvider  = nullptr;
  EState                            m_eState = EState::Stopped;

};
