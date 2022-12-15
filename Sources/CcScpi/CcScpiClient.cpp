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
 * @brief     Implementation of Class CcScpiClient
 */
#include "CcScpiClient.h"
#include "CcKernel.h"

CcScpiClient::CcScpiClient() :
  IThread("CcScpiClient"),
  m_oSocket(nullptr)
{
}

CcScpiClient::~CcScpiClient() {
}

CcStatus CcScpiClient::connect()
{
  m_oSocket = CcSocket(ESocketType::TCP);
  m_oStatus = m_oSocket.connect(m_sServer, m_sPort);
  if (!m_oStatus)
  {
    m_oSocket = nullptr;
  }
  else
  {
    m_oSocket.setTimeout(CcDateTimeFromMSeconds(m_uiTimeoutMs));
  }
  return m_oStatus;
}

CcStatus CcScpiClient::send(const CcString& sSend)
{
  if (isConnected())
  {
    m_oStatus = m_oSocket.writeLine(sSend, IIo::ELineEnding::NL);
  }
  else
  {
    m_oStatus = EStatus::NotConnected;
  }
  return m_oStatus;
}

CcStatus CcScpiClient::query(const CcString& sQuery, CcString& sResult)
{
  if (isConnected())
  {
    m_oStatus = m_oSocket.writeLine(sQuery, IIo::ELineEnding::NL);
    sResult = m_oSocket.readAll();
    if (sResult.endsWith(CcGlobalStrings::EolShort))
    {
      sResult = sResult.substr(0, sResult.length() - CcGlobalStrings::EolShort.length());
    }
  }
  else
  {
    m_oStatus = EStatus::NotConnected;
  }
  return m_oStatus;
}


