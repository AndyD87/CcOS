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
 * @page      CcRemoteDeviceServer
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
  CcRemoteDeviceServer(NRemoteDeviceServer::CConfig* pConfig = nullptr, bool bNoUi = false);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServer();

  virtual void run() override;
  virtual void onStop() override;
  virtual size_t getStackSize() override;

  void initConfigDefaults();
  void setupWebserver();
  void setupWlan();
  NRemoteDeviceServer::CConfig& getConfig()
  { return *m_pConfig; }

private:
  class CPrivate;
private:
  CPrivate*                     m_pPrivate = nullptr;  //!< Private data for applicaton.
  NRemoteDeviceServer::CConfig* m_pConfig = nullptr;
  bool                          m_bConfigOwner = false;
  bool                          m_bUi = false;
  CcAppDirectories              m_oDirectories;
  CcRemoteDeviceBoardSupport    m_oBoardSupport;
};

#endif // H_CcRemoteDeviceServer_H_
