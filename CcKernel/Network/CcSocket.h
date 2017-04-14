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
 * @page      CcSocket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSocket
 */
#ifndef CcSocket_H_
#define CcSocket_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcSocketAbstract.h"
#include "CcIODevice.h"
#include "CcTypes.h"

class CcSocketAddressInfo;
class CcString;
class CcDateTime;

/**
* @brief Button for GUI Applications
*/
class CcKernelSHARED CcSocket : public CcSocketAbstract
{
public:
  /**
   * @brief Constructor
   */
  CcSocket(ESocketType type = ESocketType::TCP);

  /**
   * @brief Destructor
   */
  virtual ~CcSocket(void);

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual bool bind(uint16 Port) = 0;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual bool connect(const CcSocketAddressInfo& oAddressInfo) = 0;

  /**
   * @brief Socket becomes a Host and listen on Port
   * @param Port: Value of Port-Address
   * @return true if port is successfully initiated.
   */
  virtual bool listen(void) = 0;

  /**
   * @brief Waiting for an incoming connection.
   * @return Valid socket if connection established, otherwise 0.
   */
  virtual CcSocket* accept(void) = 0;

  virtual void setTimeout(const CcDateTime& uiTimeValue) = 0;

  virtual CcSocketAddressInfo getHostByName(const CcString& hostname) = 0;

  virtual const CcSocketAddressInfo& getPeerInfo(void);

  virtual void setPeerInfo(const CcSocketAddressInfo& oPeerInfo);

  /**
  * @brief connect to Host with known Name in Network and Port
  *        can be overloaded if System can connect by name.
  * @param hostName: Name of Host to connect to
  * @param hostPort: Port where host ist waiting for connection
  * @return true if connection was successfully established
  */
  virtual bool connect(const CcString& hostName, const CcString& hostPort);

  virtual SOCKETFD getSocketFD() { return 0; }
};

#endif /* CcSocket_H_ */
