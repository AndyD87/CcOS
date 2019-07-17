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
 * @brief     Implementation of Class CcHttpRestApiProvider
 */
#include "CcHttpRestApiProvider.h"
#include "CcMemoryMonitor.h"
#include "CcGlobalStrings.h"

CcHttpRestApiProvider::CcHttpRestApiProvider(const CcString& sRootPath) :
  IHttpPathProvider(sRootPath),
  IRestApi(nullptr, CcGlobalStrings::Empty)
{
  setCanStartWith(true);
}

CcHttpRestApiProvider::~CcHttpRestApiProvider()
{
}

CcStatus CcHttpRestApiProvider::exec(CcHttpWorkData& oData)
{
  CcStatus oStatus;
  CcString sPath = oData.getRequest().getPath().substr(IHttpPathProvider::getPath().length(), oData.getRequest().getPath().length() - IHttpPathProvider::getPath().length());
  CcStringList oPath = sPath.split(CcGlobalStrings::Seperators::Path, false);
  oStatus = IRestApi::execPath(oPath, oData);
  return oStatus;
}
