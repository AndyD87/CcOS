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
 * @brief     Implementation of Class CcRestApiApplicationFooter
 */
#include "CcRestApiApplicationFooter.h"
#include "CcHttpWorkData.h"
#include "CcJson/CcJsonDocument.h"
#include "CcJson/CcJsonArray.h"
#include "CcJson/CcJsonObject.h"
#include "CcKernel.h"
#include "CcSystem.h"
#include "CcVersion.h"
#include "CcRestApiApplication.h"

CcRestApiApplicationFooter::CcRestApiApplicationFooter(CcRestApiApplication *pParent) :
  IRestApi(pParent, "footer")
{
}

CcRestApiApplicationFooter::~CcRestApiApplicationFooter()
{
}

bool CcRestApiApplicationFooter::get(CcHttpWorkData& oData)
{
  CCUNUSED(oData);
  bool bSuccess = false;
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  CcJsonDocument oDoc;
  CcJsonObject& rRootNode = oDoc.getJsonData().setJsonObject();
  rRootNode.append(CcJsonData("Name", CcKernel::getSystem().getName()));
  rRootNode.append(CcJsonData("Version", CcKernel::getSystem().getVersion().getVersionString()));
  rRootNode.append(CcJsonData("CcOSVersion", CcKernel::getVersion().getVersionString()));
  oData.writeChunked(oDoc.getDocument());
  return bSuccess;
}
