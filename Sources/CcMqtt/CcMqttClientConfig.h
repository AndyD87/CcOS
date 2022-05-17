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
 * @brief     Class CcMqttClientConfig
 */
#pragma once

#include "CcBase.h"
#include "CcMqtt.h"
#include "CcString.h"
#include "CcPassword.h"
#include "Network/CcSocketAddressInfo.h"

/**
 * @brief Mqtt client configuration
 */
class CcMqttSHARED CcMqttClientConfig
{
public:
  /**
   * @brief Constructor
   * @param bUseSsl: Use secured connection if true
   */
  CcMqttClientConfig(bool bUseSsl = true);

  /**
   * @brief Destructor
   */
  ~CcMqttClientConfig();

  //! @return Get username from config
  const CcString& getUsername()
  { return m_sUsername; }
  //! @return Get password from config
  const CcPassword& getPassword()
  { return m_oPassword; }
  //! @return True if ssl is enabled
  bool isSsl()
  { return m_bSsl; }
  //! @return Get connection port
  uint16 getPort()
  { return m_oAddressInfo.getPort(); }
  //! @return Get connection info
  const CcSocketAddressInfo& getAddressInfo()
  { return m_oAddressInfo; }

  //! @param sHost: Set host for connection.
  void setHost(const CcString& sHost);

private: // Member
  CcSocketAddressInfo       m_oAddressInfo;
  bool                      m_bSsl;
  CcString                  m_sUsername;
  CcPassword                m_oPassword;
};
