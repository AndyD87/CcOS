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
 * @brief     Implementation of Class CcHttpProvider
 */
#include "CcHttpProvider.h"

CcHttpProvider::CcHttpProvider( const CcString& sPath):
  m_sPath(sPath)
{
}

CcHttpProvider::~CcHttpProvider()
{
}

CcHttpResponse &CcHttpProvider::getHeader()
{
  return m_Header;
}

void CcHttpProvider::setOutput(CcSocket *Socket)
{
  m_Output = Socket;
}

CcHttpResponse CcHttpProvider::execGet(CcHttpRequest &Data)
{
  return execHead(Data);
}

CcHttpResponse CcHttpProvider::execPost(CcHttpRequest &Data)
{
  return execGet(Data);
}

CcHttpResponse CcHttpProvider::execHead(CcHttpRequest &Data)
{
  CCUNUSED(Data);
  CcHttpResponse caRet(true);
  return caRet;
}


void CcHttpProvider::setPath(const CcString& sPath)
{
  m_sPath = sPath;
}

CcString CcHttpProvider::getPath()
{
  return m_sPath;
}

bool CcHttpProvider::pregMatch(const CcString& toCompare)
{
  return m_sPath == toCompare;
}

bool CcHttpProvider::write(CcByteArray &data)
{
  size_t iRet;
  iRet = m_Output->write(data.getArray(), data.size());
  if (iRet != SIZE_MAX && iRet == data.size())
    return true;
  return false;
}

bool CcHttpProvider::writeHeader()
{
  CcByteArray cCa = getHeader().getHeader();
  return write(cCa);
}
