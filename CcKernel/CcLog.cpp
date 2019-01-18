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
 * @brief     Class CcLog
 */
#include "CcLog.h"
#include "stdio.h"
#include "CcGlobalStrings.h"

CcLog::CcLog()
{
}

CcLog::CcLog(const CcString& sOutputFile)
{
  setFilePath(sOutputFile);
}

CcLog::~CcLog()
{
  if(m_bFileValid) m_oOutputFile.close();
}

bool CcLog::setFilePath(const CcString& sOutputFile)
{
  m_oOutputFile.setFilePath(sOutputFile);
  if (!m_oOutputFile.isFile())
  {
    if (m_oOutputFile.open(EOpenFlags::Append))
    {
      m_bFileValid = true;
    }
  }
  else
  {
    if (m_oOutputFile.open(EOpenFlags::Write))
      m_bFileValid = true;
  }
  return m_bFileValid;
}

void CcLog::write(const CcString& sMsg)
{
  if (m_bFileValid)
  {
    m_oOutputFile.write(sMsg.getCharString(), sMsg.length());
  }
}

void CcLog::writeLine(const CcString& sMsg)
{
  write(sMsg);
  write(CcGlobalStrings::EolOs);
}

void CcLog::writeDebug(const CcString& sMsg)
{
  writeLine(formatDebugMessage(sMsg));
}

void CcLog::writeVerbose(const CcString& sMsg)
{
  writeLine(formatVerboseMessage(sMsg));
}

void CcLog::writeInfo(const CcString& sMsg)
{
  writeLine(formatInfoMessage(sMsg));
}

void CcLog::writeWarning(const CcString& sMsg)
{
  writeLine(formatWarningMessage(sMsg));
}

void CcLog::writeError(const CcString& sMsg)
{
  writeLine(formatErrorMessage(sMsg));
}

CcString CcLog::formatDebugMessage(const CcString& sMsg)
{
  return CcString() << "[dbg ] " << sMsg;
}

CcString CcLog::formatVerboseMessage(const CcString& sMsg)
{
  return CcString() << "[vbs ] " << sMsg;
}

CcString CcLog::formatInfoMessage(const CcString& sMsg)
{
  return CcString() << "[info] " << sMsg;
}

CcString CcLog::formatWarningMessage(const CcString& sMsg)
{
  return CcString() << "[warn] " << sMsg;
}

CcString CcLog::formatErrorMessage(const CcString& sMsg)
{
  return CcString() << "[err ] " << sMsg;
}

