/*
 * This file is part of CcRemoteDeviceServer.
 *
 * CcRemoteDeviceServer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcRemoteDeviceServer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcRemoteDeviceServer.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRemoteDeviceServer
 **/
#ifndef H_CcRemoteDeviceServer_H_
#define H_CcRemoteDeviceServer_H_

#include "CcRemoteDevice.h"
#include "Server/CConfig.h"
#include "CcAppDirectories.h"
#include "CcRemoteDeviceBoardSupport.h"
#include "CcApp.h"
#include "Network/CcSocket.h"
#include "Devices/IWlan.h"
#include "RestApi/CcRestApiApplicationStatus.h"
#include "Applications/RestApiWebframework/CcHttpWebframework.h"

class CcRemoteDeviceJsProvider;
class CcRemoteDeviceCssProvider;
class CcRestApiDevice;

#ifdef _MSC_VER
template class CcRemoteDeviceSHARED CcVector<CcRestApiApplicationStatus::IPublisher*>;
template class CcRemoteDeviceSHARED CcVector<CcRestApiDevice*>;
template class CcRemoteDeviceSHARED CcHandle<IWlan>;
#endif

/**
 * @brief CcRemoteDeviceServer implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceServer : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceServer(NRemoteDevice::Server::CConfig* pConfig, bool bNoUi);
  CcRemoteDeviceServer();

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServer();

  virtual void run() override;
  virtual void onStop() override;
  virtual size_t getStackSize() override;

  void init();
  void initConfigDefaults();
  void setupWebserver();
  void setupWlan();
  NRemoteDevice::Server::CConfig& getConfig()
  { return *m_pConfig; }

private:
  NRemoteDevice::Server::CConfig* m_pConfig       = nullptr;
  bool                            m_bConfigOwner  = false;
  bool                            m_bUi           = true;
  CcAppDirectories                m_oDirectories;
  CcRemoteDeviceBoardSupport      m_oBoardSupport;
  CcSocket                        m_oSocket;

  // Devices
  CcHandle<IWlan>                 m_pWlanDevice = nullptr;

  // Webserver
  CcRemoteDeviceJsProvider*       m_pJsProvider = nullptr;
  CcRemoteDeviceCssProvider*      m_pCssProvider = nullptr;
  CcVector<CcRestApiDevice*>      m_oAllocatedRestApiDevices;
  CcVector<CcRestApiApplicationStatus::IPublisher*> m_oStatusPublisher;
  NHttp::Application::RestApiWebframework::CcHttpWebframework* m_pHttpServer = nullptr;
};

#endif // H_CcRemoteDeviceServer_H_
