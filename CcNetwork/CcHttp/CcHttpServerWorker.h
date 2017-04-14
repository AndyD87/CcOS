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
 * @page      CcHttpServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpServerWorker
 */
#ifndef CcHttpServerWorker_H_
#define CcHttpServerWorker_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcHttpRequest.h"
#include "CcWorker.h"

class CcHttpServer;

class CcHttpSHARED CcHttpServerWorker : public CcWorker
{
public:
  CcHttpServerWorker(CcHttpServer* Server, CcSocket *socket);
  virtual ~CcHttpServerWorker();

  void run(void) override;

  bool chkReadBuf(void);

  bool done;
  CcHttpRequest m_Header;
  CcSocket     *m_Socket;
  CcHttpServer *m_Server;
  CcByteArray   m_InBuf;
};

#endif /* CcHttpServerWorker_H_ */
