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
#ifndef CcRemoteDeviceServer_H_
#define CcRemoteDeviceServer_H_

#include "CcRemoteDevice.h"
#include "CcRemoteDeviceConfigServer.h"
#include "CcAppDirectories.h"
#include "Applications/RestApiWebframework/CcHttpWebframework.h"

/**
 * @brief CcRemoteDeviceServer impelmentation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CcRemoteDeviceServer : public CcHttp::Application::RestApiWebframework::CcHttpWebframework
{
public:
  /**
   * @brief Constructor
   */
  CcRemoteDeviceServer(CcRemoteDeviceConfigServer* pConfig = nullptr, bool bNoUi = false);

  /**
   * @brief Destructor
   */
  virtual ~CcRemoteDeviceServer();

  virtual void run() override;

private:
  class CPrivate;
private:
  CPrivate*             m_pPrivate = nullptr;  //!< Private data for applicaton.
  CcAppDirectories      m_oDirectories;
  CcRemoteDeviceConfigServer* m_pConfig = nullptr;
  bool                  m_bConfigOwner = false;
};

#endif /* CcRemoteDeviceServer_H_ */
