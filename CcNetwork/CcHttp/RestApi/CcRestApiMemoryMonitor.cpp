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
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcRestApiMemoryMonitor
 */
#include "CcRestApiMemoryMonitor.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcMemoryMonitor.h"

class CcRestApiMemoryMonitor::CPrivate : public IRestApi
{
public:
  CPrivate(IRestApi* pParent) : IRestApi(pParent, "buffers")
  {}
  virtual bool get(CcHttpWorkData& oData) override
  {
    CCUNUSED(oData);
    bool bSuccess = false;
    oData.sendHeader();

#ifdef MEMORYMONITOR_ENABLED
    CcMemoryMonitor::printLeft(&oData);
#endif
    return bSuccess;
  }

};

CcRestApiMemoryMonitor::CcRestApiMemoryMonitor(IRestApi *pParent) :
  IRestApi(pParent, "monitor")
{
  CCNEW(m_pPrivate, CPrivate, this);
}

CcRestApiMemoryMonitor::~CcRestApiMemoryMonitor()
{
  CCDELETE(m_pPrivate);
}

bool CcRestApiMemoryMonitor::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonArray& rRootNode = oDoc.getJsonData().setJsonArray();

#ifdef MEMORYMONITOR_ENABLED
  rRootNode.append(CcJsonNode("BufferCount: ", CcString::fromSize(CcMemoryMonitor::getAllocationCount())));
#else
  rRootNode.append(CcJsonNode("BufferCount: ", CcString::fromSize(0)));
#endif

  oData.write(oDoc.getDocument());
  return bSuccess;
}
