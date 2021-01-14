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
 * @brief     Class CcMqttClientConfig
 */
#ifndef H_CcMqttClientConfig_H_
#define H_CcMqttClientConfig_H_

#include "CcBase.h"
#include "CcMqtt.h"
#include "CcString.h"
#include "CcPassword.h"
#include "Network/CcSocketAddressInfo.h"

/**
 * @brief Button for GUI Applications
 */
class CcMqttSHARED CcMqttClientConfig
{
public:
  /**
   * @brief Constructro
   */
  CcMqttClientConfig(bool bUseSsl = true);

  /**
   * @brief Destructor
   */
  ~CcMqttClientConfig();

  const CcString& getUsername()
    { return m_sUsername; }
  const CcPassword& getPassword()
    { return m_oPassword; }
  bool isSsl()
    { return m_bSsl; }
  uint16 getPort()
    { return m_oAddressInfo.getPort(); }
  const CcSocketAddressInfo& getAddressInfo()
    { return m_oAddressInfo; }

  void setHost(const CcString& sHost);

private: // Member
  CcSocketAddressInfo       m_oAddressInfo;
  bool                      m_bSsl;
  CcString                  m_sUsername;
  CcPassword                m_oPassword;
};

#endif /* H_CcMqttClientConfig_H_ */
