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
 * @page      CcMqtt
 * @subpage   CcMqttClient
 *
 * @page      CcMqttClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMqttClient
 */
#ifndef H_CcMqttClient_H_
#define H_CcMqttClient_H_

#include "CcBase.h"
#include "CcMqtt.h"
#include "CcApp.h"
#include "Network/CcSocket.h"
#include "CcString.h"

/**
 * @brief Button for GUI Applications
 */
class CcMqttSHARED CcMqttClient : public CcApp
{
public:
  /**
   * @brief Constructro
   */
  CcMqttClient();

  /**
   * @brief Destructor
   */
  virtual ~CcMqttClient();

  virtual void run() override;

  bool connect(const CcString& sUsername, const CcString& sPassword);
private: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};

#endif /* H_CcMqttClient_H_ */
