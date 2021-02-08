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
 * @brief     Class CcTftpServerConfig
 */
#pragma once

#include "CcBase.h"
#include "CcTftp.h"
#include "CcHandle.h"

/**
 * @brief Button for GUI Applications
 */
class CcTftpSHARED CcTftpServerConfig
{
public:
  /**
   * @brief Constructor
   * @param Port: Port on wich the Server should listen.
   *              Default Port is set to 69
   */
  CcTftpServerConfig();

  /**
   * @brief Destructor
   */
  ~CcTftpServerConfig();

  //! @return Get port of tftp server
  uint16 getPort() const
  { return m_uiPort; }
  //! @return Get maximum number of retransmissions before cancel
  size_t getMaxRetransMissions() const
  { return m_uiMaxRetransMissions; }
  //! @return Get root directory for files
  const CcString& getRootDir() const
  { return m_sRootDir; }
  //! @return True if reading files is enabled
  bool isReadEnabled() const
  { return m_bReadEnabled; }
  //! @return True if writing files is enabled
  bool isWriteEnabled() const
  { return m_bWriteEnabled; }

  //! @param bEnable: Set True if reading files should be enabled
  void setReadEnabled(bool bEnable)
  { m_bReadEnabled = bEnable; }
  //! @param bEnable: Set True if writing files should be enabled
  void setWriteEnabled(bool bEnable)
  { m_bWriteEnabled = bEnable; }
  //! @param uiPort: Set port number for server to listen at
  void setPort(uint16 uiPort)
  { m_uiPort = uiPort; }
  //! @param sRootDir: Set new root directory for files
  void setRootDir(const CcString& sRootDir)
  { m_sRootDir = sRootDir; }
  //! @param uiMaxRetransMissions: Set maximum number of retransmissions before cancel
  void setMaxRetransMissions(size_t uiMaxRetransMissions)
  { m_uiMaxRetransMissions = uiMaxRetransMissions; }

private:
  uint16      m_uiPort;   //!< Port where Socket is listen on.
  CcString    m_sRootDir; //!< Root-Dir of Server
  bool        m_bReadEnabled = true; //!< Default enable read
  bool        m_bWriteEnabled = false; //!< Default disable write
  size_t      m_uiMaxRetransMissions = 5; //!< Maximum number of retransmition of a single block.
};

#ifdef _MSC_VER
template class CcTftpSHARED CcHandle<CcTftpServerConfig>;
#endif

//! Handle for tftp config
typedef CcHandle<CcTftpServerConfig> CcTftpServerConfigHandle;
