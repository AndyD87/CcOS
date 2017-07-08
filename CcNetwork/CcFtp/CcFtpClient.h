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
 * @page      CcFtp
 * @subpage   CcFtpClient
 *
 * @page      CcFtpClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcFtpClient
 */
#ifndef CcFtpClient_H_
#define CcFtpClient_H_

#include "CcBase.h"
#include "CcFtp.h"
#include "CcThreadObject.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"

/**
 * @brief Button for GUI Applications
 */
class CcFtpSHARED CcFtpClient : public CcThreadObject
{
public:
  /**
   * @brief Constructro
   */
  CcFtpClient( void );

  /**
   * @brief Destructor
   */
  virtual ~CcFtpClient( void );
private:
  CcSocket *m_Socket; //!< Pointer to Socket for communication
  bool m_Done;        //!< Set to true if operation has ended
};

#endif /* CcFtpClient_H_ */
