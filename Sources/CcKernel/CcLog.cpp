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
 * @brief     Class CcLog
 */
#include "CcLog.h"
#include "stdio.h"
#include "CcGlobalStrings.h"
#include "CcConsole.h"
#include "CcMessageFormat.h"

CcLog::CcLog(const CcString& sOutputFile)
{
  setFilePath(sOutputFile);
}

CcLog::~CcLog()
{
  if(m_bFileValid) m_oOutputFile.close();
}

size_t CcLog::write(const void* pBuffer, size_t uSize)
{
  if (m_bFileValid)
  {
    m_oOutputFile.write(pBuffer, uSize);
    //m_oOutputFile.flush();
  }
  if(m_bWriteToConsole)
  {
    CcConsole::write(pBuffer, uSize);
  }
  return uSize;
}

bool CcLog::setFilePath(const CcString& sOutputFile)
{
  m_oOutputFile.setFilePath(sOutputFile);
  if (m_oOutputFile.isFile())
  {
    if (m_oOutputFile.open(EOpenFlags::Append | EOpenFlags::ShareRead))
    {
      m_bFileValid = true;
    }
  }
  else
  {
    if (m_oOutputFile.open(EOpenFlags::Write | EOpenFlags::ShareRead))
      m_bFileValid = true;
  }
  return m_bFileValid;
}

void CcLog::write(const CcString& sMsg)
{
  write(sMsg.getCharString(), sMsg.length());
}

void CcLog::writeLine(const CcString& sMsg)
{
  write(sMsg);
  write(CcGlobalStrings::EolOs);
}

void CcLog::writeDebug(const CcString& sMsg)
{
  writeLine(CcMessageFormat::formatDebugMessage(sMsg));
}

void CcLog::writeVerbose(const CcString& sMsg)
{
  writeLine(CcMessageFormat::formatVerboseMessage(sMsg));
}

void CcLog::writeInfo(const CcString& sMsg)
{
  writeLine(CcMessageFormat::formatInfoMessage(sMsg));
}

void CcLog::writeWarning(const CcString& sMsg)
{
  writeLine(CcMessageFormat::formatWarningMessage(sMsg));
}

void CcLog::writeError(const CcString& sMsg)
{
  writeLine(CcMessageFormat::formatErrorMessage(sMsg));
}
