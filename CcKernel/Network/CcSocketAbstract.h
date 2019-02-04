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
 * @page      Network
 * @subpage   CcSocketAbstract
 *
 * @page      CcSocketAbstract
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSocketAbstract
 */
#ifndef _CcSocketAbstract_H_
#define _CcSocketAbstract_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcTypes.h"
#include "CcString.h"
#include "Network/CcSocketAddressInfo.h"

class CcDateTime;

enum class ESocketType 
{
  Unknown = 0,
  TCP,
  UDP,
};

enum class ESocketOption
{
  Reuse,        //<! Send ReuseAddress and ReusePort
  ReuseAddress, //<! Do not block address after crash
  ReusePort,    //<! Do not block port after crash
  Broadcast     //<! This Option enables Broadcast by default, for disable, send int32 with 0 as Data
};

/**
 * @brief Button for GUI Applications
 */
class CcKernelSHARED CcSocketAbstract : public CcIODevice
{
public:
  /**
   * @brief Constructor
   */
  CcSocketAbstract(ESocketType type = ESocketType::TCP): m_eSocketType(type){}

  /**
   * @brief Destructor
   */
  virtual ~CcSocketAbstract(void){};

  virtual CcStatus setAddressInfo(const CcSocketAddressInfo& oAddrInfo) = 0;

  /**
   * @brief connect to Host with known IP-Address and Port
   * @return true if connection was successfully established
   */
  virtual CcStatus bind() = 0;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @return true if connection was successfully established
   */
  virtual CcStatus connect() = 0;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @param Port: Value of Port-Address
   * @return true if port is successfully initiated.
   */
  virtual CcStatus listen(void) = 0;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  virtual CcSocketAbstract* accept(void) = 0;

  virtual void setTimeout(const CcDateTime& uiTimeValue) = 0;

  virtual CcSocketAddressInfo getPeerInfo(void) = 0;

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo) = 0;

  virtual CcSocketAddressInfo getHostByName(const CcString& hostname) = 0;

  virtual CcStatus setOption(ESocketOption eOption, void* pData, size_t uiDataLen) = 0;

  virtual CcStatus setOptionRaw(int iLevel, int iOptName, void* pData, size_t uiDataLen) = 0;

  virtual SOCKETFD getSocketFD() { return 0; }

  inline ESocketType getType() const
    { return m_eSocketType; }

private:
  ESocketType m_eSocketType;
};

#endif /* _CcSocketAbstract_H_ */
