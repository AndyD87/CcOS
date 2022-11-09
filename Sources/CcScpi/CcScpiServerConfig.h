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
 * @brief     Class CcScpiServerConfig
 */
#pragma once

#include "CcBase.h"
#include "CcScpi.h"
#include "CcHandle.h"

/**
 * @brief Button for GUI Applications
 */
class CcScpiSHARED CcScpiServerConfig
{
public:
  /**
   * @brief Constructor
   *        Default Port is set to 69
   */
  CcScpiServerConfig();

  /**
   * @brief Destructor
   */
  ~CcScpiServerConfig();

  //! @return Get port of tftp server
  uint16 getPort() const
  { return m_uiPort; }

  //! @param uiPort: Set port number for server to listen at
  void setPort(uint16 uiPort)
  { m_uiPort = uiPort; }
private:
  uint16      m_uiPort;   //!< Port where Socket is listen on.
};

#ifdef _MSC_VER
template class CcScpiSHARED CcHandle<CcScpiServerConfig>;
#endif

//! Handle for tftp config
typedef CcHandle<CcScpiServerConfig> CcScpiServerConfigHandle;
