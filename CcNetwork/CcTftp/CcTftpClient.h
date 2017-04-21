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
 * @page      CcTftp
 * @subpage   CcTftpClient
 *
 * @page      CcTftpClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcTftpClient
 */
#ifndef CcTftpClient_H_
#define CcTftpClient_H_

#include "CcBase.h"
#include "CcTftp.h"
#include "CcThreadObject.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"

/**
 * @brief Button for GUI Applications
 */
class CcTftpSHARED CcTftpClient : public CcThreadObject
{
public:
  /**
   * @brief Constructro
   */
  CcTftpClient( void );

  /**
   * @brief Destructor
   */
  virtual ~CcTftpClient( void );
private:
  CcSocket *m_Socket; //!< Pointer to Socket for communication
  bool m_Done;        //!< Set to true if operation has ended
};

#endif /* CcTftpClient_H_ */
