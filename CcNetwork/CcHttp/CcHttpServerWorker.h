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
 * @page      CcHttp
 * @subpage   CcHttpServerWorker
 *
 * @page      CcHttpServerWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpServerWorker
 */
#ifndef _CcHttpServerWorker_H_
#define _CcHttpServerWorker_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcHttpRequest.h"
#include "IWorker.h"

class CcHttpServer;

class CcHttpSHARED CcHttpServerWorker : public IWorker
{
public:
  CcHttpServerWorker(CcHttpServer* Server, CcSocket socket);
  virtual ~CcHttpServerWorker();

  void run() override;

  bool chkReadBuf();

  bool done;
  CcSocket      m_Socket;
  CcHttpServer *m_Server;
  CcHttpRequest m_Header;
  CcByteArray   m_InBuf;
};

#endif /* _CcHttpServerWorker_H_ */
