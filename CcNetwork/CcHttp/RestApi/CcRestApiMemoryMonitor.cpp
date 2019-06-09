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
 * @par       Web:      http://coolcow.de/projects/CcOS
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
    oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
    oData.sendHeader();
    CcJsonDocument oDoc;
    CcJsonArray& rRootNode = oDoc.getJsonData().setJsonArray();

    std::list<CcMemoryMonitor::CItem> oMemoryList = CcMemoryMonitor::getAllocationList();
    size_t uiCount = 0;
    for(CcMemoryMonitor::CItem& oMemory : oMemoryList)
    {
      uiCount++;
      if(uiCount > 100)
      {
        const char* pString = (const char*)oMemory.pBuffer;
        CcString sString(pString);
        bSuccess = true;
      }
      CcJsonArray oNode;
      oNode.append(CcJsonData("", CcString(oMemory.pFile)));
      oNode.append(CcJsonData("", CcString::fromNumber(oMemory.iLine)));
      rRootNode.add(oNode);
    }

    oData.writeChunked(oDoc.getDocument());
    return bSuccess;
  }

};

CcRestApiMemoryMonitor::CcRestApiMemoryMonitor(IRestApi *pParent) :
  IRestApi(pParent, "memory_monitor")
{
  m_pPrivate = new CPrivate(this);
  CCMONITORNEW(m_pPrivate);
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

  rRootNode.append(CcJsonData("BufferCount: ", CcString::fromNumber(CcMemoryMonitor::getAllocationCount())));

  oData.writeChunked(oDoc.getDocument());
  return bSuccess;
}
