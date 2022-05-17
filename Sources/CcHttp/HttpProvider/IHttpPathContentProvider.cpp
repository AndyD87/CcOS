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
 * @brief     Implementation of Class IHttpPathContentProvider
 */
#include "IHttpPathContentProvider.h"
#include "CcHttpGlobalStrings.h"

IHttpPathContentProvider::IHttpPathContentProvider(const CcString& sPath) :
  IHttpPathProvider(sPath)
{
}

IHttpPathContentProvider::~IHttpPathContentProvider()
{
}

CcStatus IHttpPathContentProvider::execGet(CcHttpWorkData& oData)
{
  oData.getResponse().setContentType(getMimeType());
  oData.sendHeader();
  size_t uiToTransfer;
  const uchar* pData = static_cast<const uchar*>(getContent(uiToTransfer));
  size_t uiTransfered = 0;
  while (uiTransfered < uiToTransfer)
  {
    size_t uiNextTransfer = CCMIN(1400, uiToTransfer - uiTransfered);
    oData.write(pData + uiTransfered, uiNextTransfer);
    uiTransfered += uiNextTransfer;
  }
  return true;
}

const CcString& IHttpPathContentProvider::getMimeType()
{
  return CcHttpGlobalStrings::MimeTypes::ApplicationOctet;
}
