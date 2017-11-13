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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSocketAbstract
 */
#ifndef CcSocketAbstract_H_
#define CcSocketAbstract_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"
#include "CcTypes.h"
#include "CcString.h"
#include "Network/CcSocketAddressInfo.h"

enum class ESocketType {
  TCP = 0,
  UDP,
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
  CcSocketAbstract(ESocketType type = ESocketType::TCP): m_SockType(type){}

  /**
   * @brief Destructor
   */
  virtual ~CcSocketAbstract(void){};

  /**
   * @brief connect to Host with known IP-Address and Port
   * @param ipAdress: IpAddress of Host
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus bind(uint16 Port) = 0;

  /**
   * @brief connect to Host with known Name in Network and Port
   * @param hostName: Name of Host to connect to
   * @param Port:     Port where host ist waiting for connection
   * @return true if connection was successfully established
   */
  virtual CcStatus connect(const CcSocketAddressInfo& oAddressInfo) = 0;

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

  virtual CcSocketAddressInfo getHostByName(const CcString& hostname) = 0;
protected:
  ESocketType m_SockType;
  CcSocketAddressInfo m_oConnectionInfo;
  CcSocketAddressInfo m_oPeerInfo;
};

#endif /* CcSocketAbstract_H_ */
