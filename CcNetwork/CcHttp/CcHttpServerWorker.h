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
#ifndef H_CcHttpServerWorker_H_
#define H_CcHttpServerWorker_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcHttpWorkData.h"
#include "IWorker.h"

class CcHttpServer;

class CcHttpSHARED CcHttpServerWorker : public IWorker
{
public:
  CcHttpServerWorker(CcHttpServer& oServer, const CcSocket& oSocket);
  virtual ~CcHttpServerWorker();

  virtual size_t getStackSize() override
    { return 4096; }

  void run() override;
  CcStatus chkReadBuf(const CcString& sInputData, size_t& uiContentOffset);
  void finish();
  void error();
private:
  CcHttpWorkData m_oData;
};

#endif // H_CcHttpServerWorker_H_
