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
 * @brief     Implementation of Class CcHttpJqueryProvider
 */
#include "CcHttpJqueryProvider.h"
#include "CcMemoryMonitor.h"
#include "CcGlobalStrings.h"
#include "Resources/jquery-3.4.1.min.js.h"

CcHttpJqueryProvider::CcHttpJqueryProvider(const CcString& sRootPath) :
  IHttpPathProvider(sRootPath)
{
  setCanStartWith(true);
}

CcHttpJqueryProvider::~CcHttpJqueryProvider()
{
}

CcStatus CcHttpJqueryProvider::execGet(CcHttpWorkData& oData)
{
  oData.getResponse().setTransferEncoding(CcHttpTransferEncoding::Chunked);
  oData.sendHeader();
  size_t uiTransfered = 0;
  while (uiTransfered < g_Jquery_3_4_1_Min_Size)
  {
    size_t uiNextTransfer = CCMIN(1400, g_Jquery_3_4_1_Min_Size - uiTransfered);
    oData.writeChunked(g_Jquery_3_4_1_Min + uiTransfered, uiNextTransfer);
    uiTransfered += uiNextTransfer;
  }
  return true;
}