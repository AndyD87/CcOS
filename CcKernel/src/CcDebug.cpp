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
 * @brief     Class CcDebug
 */

#include "CcBase.h"
#include "CcDebug.h"
#include "CcString.h"
#include "CcConsole.h"

#ifdef VERBOSE
  CcDebug::ELevel CcDebug::c_eLevel = CcDebug::ELevel::Verbose;
#else
  #ifdef DEBUG
    CcDebug::ELevel CcDebug::c_eLevel = CcDebug::ELevel::Debug;
  #else
    CcDebug::ELevel CcDebug::c_eLevel = CcDebug::ELevel::Error;
  #endif
#endif

bool CcDebug::c_bVerboseEnabled   = false;

void CcDebug::writeDebug(const CcString& sMsg)
{
  CcString sOutMsg("[dbg ] " + sMsg + "\n");
  CcConsole::writeString(sOutMsg);
}

void CcDebug::writeVerbose(const CcString& sMsg)
{
  if (c_bVerboseEnabled)
  {
    CcString sOutMsg("[vbs ] " + sMsg + "\n");
    CcConsole::writeString(sOutMsg);
  }
}

void CcDebug::writeInfo(const CcString& sMsg)
{
  if (c_eLevel >= ELevel::Info)
  {
    CcString sOutMsg("[info] " + sMsg + "\n");
    CcConsole::writeString(sOutMsg);
  }
}

void CcDebug::writeWarning(const CcString& sMsg)
{
  if (c_eLevel >= ELevel::Warning)
  {
    CcString sOutMsg("[warn] " + sMsg + "\n");
    CcConsole::writeString(sOutMsg);
  }
}

void CcDebug::writeError(const CcString& sMsg)
{
  if (c_eLevel >= ELevel::Error)
  {
    CcString sOutMsg("[err ] " + sMsg + "\n");
    CcConsole::writeString(sOutMsg);
  }
}

void CcDebug::setLevel(ELevel eLevel)
{
  c_eLevel = eLevel;
}

void CcDebug::setVerbose(bool bEnable)
{
  c_bVerboseEnabled = bEnable;
}
