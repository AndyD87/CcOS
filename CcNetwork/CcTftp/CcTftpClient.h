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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcTftpClient
 */
#ifndef _CcTftpClient_H_
#define _CcTftpClient_H_

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
  CcTftpClient();

  /**
   * @brief Destructor
   */
  virtual ~CcTftpClient();
};

#endif /* _CcTftpClient_H_ */
