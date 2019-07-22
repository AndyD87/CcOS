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
 * @brief     Implementation of Class CcVBoxIfc
 */
#include "CcVBoxIfc.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "Hash/CcSha256.h"
#include "CcFile.h"
#include "CcVersion.h"
#include "CcProcess.h"
#include "CcFileSystem.h"

class CcVBoxIfc::CPrivate
{
public:
  static const CcString sCommonExecutable;
};

const CcString CcVBoxIfc::CPrivate::sCommonExecutable("vboxmanage");

CcVBoxIfc::CcVBoxIfc(const CcString& sPath)
{
  setExecutable(sPath);
}

CcVBoxIfc::~CcVBoxIfc()
{
}

CcVersion CcVBoxIfc::getVersion(bool* bOk)
{
  CcVersion oVersion;
  CcString sVersion = exec(CcString("-v"), bOk);
  if(sVersion.length())
  {
    oVersion.setVersionString(sVersion);
  }
  return oVersion;
}

CcVBoxIfc::CVmInfoList CcVBoxIfc::getVmList(bool* bOk)
{
  CVmInfoList oList;
  CcStringList oArguments;
  oArguments.append("list");
  oArguments.append("vms");
  CcString sVms = exec(oArguments, bOk);
  CcStringList oVms = sVms.splitLines(false);
  for (CcString sVm : oVms)
  {
    CcStringList sSplitted = sVm.split(CcGlobalStrings::Seperators::Space, false);
    if (sSplitted.size() == 2)
    {
      CVmInfoListItem oVmList;
      oVmList.sName = sSplitted[0];
      oVmList.oUuId.setUuid(sSplitted[1]);
      oList.append(oVmList);
    }
  }
  return oList;
}

bool CcVBoxIfc::setExecutable(const CcString& sPath)
{
  if (sPath.length() > 0)
  {
    m_sExePath = sPath;
    getVersion(&m_bExeFound);
    m_bExeFound = true;
    if (!m_bExeFound)
    {
      m_sExePath = "";
      m_bExeFound = false;
    }
  }
  else
  {
    CcString sFile = CcFileSystem::findExecutable(CPrivate::sCommonExecutable);
    if (sFile.length())
    {
      m_sExePath = sFile;
      m_bExeFound = true;
      getVersion(&m_bExeFound);
      if (!m_bExeFound)
      {
        m_sExePath = "";
        m_bExeFound = false;
      }
    }
    else
    {
#ifdef WINDOWS
      sFile = CcFileSystem::findExecutable(CPrivate::sCommonExecutable + ".exe");
      if (sFile.length())
      {
        m_sExePath = sFile;
        m_bExeFound = true;
        getVersion(&m_bExeFound);
        if (!m_bExeFound)
        {
          m_sExePath = "";
          m_bExeFound = false;
        }
      }
      else
      {
#endif

#ifdef WINDOWS
      }
#endif

    }
  }
  return m_bExeFound;
}

CcString CcVBoxIfc::exec(const CcStringList& sArgs, bool* bOk)
{
  CcString sResponse;
  if(isValid())
  {
    CcProcess oProc(m_sExePath);
    oProc.setArguments(sArgs);
    oProc.exec();
    sResponse = oProc.pipe().readAll();
    if (bOk != nullptr)
    {
      if(oProc.getExitCode())
        *bOk = true;
      else
        *bOk = false;
    }
  }
  else if (bOk != nullptr)
  {
    *bOk = false;
  }
  return sResponse;
}
