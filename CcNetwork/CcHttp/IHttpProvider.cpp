/*
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @brief     Implementation of Class IHttpProvider
 */
#include "IHttpProvider.h"

IHttpProvider::IHttpProvider()
{
}

IHttpProvider::~IHttpProvider()
{
}

CcStatus IHttpProvider::exec(CcHttpWorkData& oData)
{
  CcStatus oReturn;
  if (oData.getRequest().getRequestType() == EHttpRequestType::Get)
  {
    oReturn = execGet(oData);
  }
  else if (oData.getRequest().getRequestType() == EHttpRequestType::PostMultip)
  {
    oReturn = execPost(oData);
  }
  else if (oData.getRequest().getRequestType() == EHttpRequestType::Head)
  {
    oReturn = execHead(oData);
  }
  else
  {
    oData.getResponse().setError(CcHttpGlobals::EError::ErrorMethodNotAllowed);
  }
  return oReturn;
}

CcStatus IHttpProvider::execPost(CcHttpWorkData& Data)
{
  return execGet(Data);
}

CcStatus IHttpProvider::execHead(CcHttpWorkData& Data)
{
  return execGet(Data);
}
