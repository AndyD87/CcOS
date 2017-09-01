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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLog
 */
#include "CcLog.h"
#include "stdio.h"
#include "CcGlobalStrings.h"

CcLog::CcLog(const CcString& sOutputFile):
  m_oOutputFile(sOutputFile)
{
  bool bSuccess = true;
  if (!m_oOutputFile.isFile())
  {
    if (!m_oOutputFile.open(EOpenFlags::Write))
    {
      bSuccess = false;
    }
  }
  if (bSuccess)
  {
    if (m_oOutputFile.open(EOpenFlags::Append))
      m_bFileValid = true;
  }
}

CcLog::~CcLog() {
  // todo Auto-generated destructor stub
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
  CcString sOutString(sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}

void CcLog::writeDebug(const CcString& sMsg)
{
  CcString sOutString("[dbg ] " + sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}

void CcLog::writeVerbose(const CcString& sMsg)
{
  CcString sOutString("[vbs ] " + sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}

void CcLog::writeInfo(const CcString& sMsg)
{
  CcString sOutString("[info] " + sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}

void CcLog::writeWarning(const CcString& sMsg)
{
  CcString sOutString("[warn] " + sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}

void CcLog::writeError(const CcString& sMsg)
{
  CcString sOutString("[err] " + sMsg + CcGlobalStrings::EolOs);
  write(sOutString);
}
