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
 * @brief     Implementation of Class CcHttpDefaultProvider
 */
#include "CcHttpDefaultProvider.h"

CcString CcHttpDefaultProvider::m_s404Msg("<h1>Welcome to the CcOS HTTP - Webserver</h1>\nThis Page is default set on Webserver and indicates no installed WebpageProvider");

CcHttpDefaultProvider::CcHttpDefaultProvider()
{
}

CcHttpDefaultProvider::~CcHttpDefaultProvider()
{
}

void CcHttpDefaultProvider::setWorkingDir(const CcString& sWD)
{
  m_sWorkingDir = sWD;
}

CcHttpResponse CcHttpDefaultProvider::execGet(CcHttpRequest &Data)
{
  CcHttpResponse cRet(true);
  CCUNUSED(Data);
  cRet.m_oContent = m_s404Msg.getByteArray();
  return cRet;
}
