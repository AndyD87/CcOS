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
 * @page      CcDhcp
 * @subpage   CcDhcpClient
 *
 * @page      CcDhcpClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpClient
 **/
#ifndef _CcDhcpClient_H_
#define _CcDhcpClient_H_

#include "CcBase.h"
#include "CcDhcp.h"

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpClient {
public:
  /**
   * @brief Constructor
   */
  CcDhcpClient( void );

  /**
   * @brief Destructor
   */
  ~CcDhcpClient( void );

private:
  static bool s_bIsInitialized;
};
#endif /* _CcDhcpClient_H_ */
