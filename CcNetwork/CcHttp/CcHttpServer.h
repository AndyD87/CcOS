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
 * @page      CcHttp
 * @subpage   CcHttpServer
 *
 * @page      CcHttpServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpServer
 */
#ifndef H_CcHttpServer_H_
#define H_CcHttpServer_H_

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
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpServer : public CcApp
{
public: // types
  enum class EState
  {
    Stopped = 0,
    Starting,
    Listening
  };
public:
  /**
   * @brief Constructor
   */
  CcHttpServer(uint16 Port = CcCommonPorts::InvalidPort);
  CcHttpServer(CcHttpServerConfig* pConfig);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpServer();

  virtual void run() override;
  virtual void onStop() override;

  void registerProvider(const CcHandle<IHttpProvider> &toAdd);
  bool deregisterProvider(const CcHandle<IHttpProvider> &toRemove);
  const CcHandle<IHttpProvider> findProvider(const CcHttpWorkData &oData) const;
  const CcList<CcHandle<IHttpProvider>>& getReceiverList();

  EState getState() const
    { return m_eState; }
  void decWorker()
  { if(m_uiWorkerCount > 0) m_uiWorkerCount--;}
  void setPort(uint16 uiPort)
    { m_pConfig->getAddressInfo().setPort(uiPort);}
  CcHttpServerConfig& getConfig()
    { return *m_pConfig; }
  const CcHttpServerConfig& getConfig() const
    { return *m_pConfig; }
  uint64 getRequestCount() const
    { return m_uiRequestsCount; }
  uint16 getWorkerCount()
    { return m_uiWorkerCount; }

  void setConfig(CcHttpServerConfig* pConfig);

protected:
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

#endif // H_CcHttpServer_H_
