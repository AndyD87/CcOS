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
 * @brief     Class CcScpiServer
 */
#pragma once

#include "CcBase.h"
#include "CcScpi.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcApp.h"
#include "CcScpiServerWorker.h"
#include "CcScpiServerConfig.h"
#include "CcUserList.h"

/**
 * @brief Button for GUI Applications
 */
class CcScpiSHARED CcScpiServer : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcScpiServer();

  /**
   * @brief Constructor
   * @param oConfig: Initialize Server with predefined configuration
   */
  CcScpiServer(const CcScpiServerConfig& oConfig);

  /**
   * @brief Destructor
   */
  virtual ~CcScpiServer();

  /**
   * @brief Main Loop of Application-Thread
   */
  virtual void run() override;

  //! @return Current used configuration
  CcScpiServerConfig& config() 
  { return m_oConfig; }
private:
  CcScpiServerConfig m_oConfig;
  CcSocket   m_Socket; //!< Socket where Server is listen on
};
