/**
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
* @author    Andreas Dirmeier
* @par       Language: C++11
* @brief     Implementation of Class CCcLinuxModule
*/
#include "CcLinuxModule.h"
#include "CcDirectory.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"

#include <errno.h>
#include <dlfcn.h>

class CcLinuxModule::CPrivate
{
public:
  static void marker()
  {}
  void* pSharedObject = nullptr;
};

CcLinuxModule::CcLinuxModule(const CcString& sName)
{
  CCNEW(m_pContext, CcLinuxModule::CPrivate);

  CcString sFoundPath;
  if(CcFile::exists(sName))
  {
    sFoundPath = sName;
  }
  if(sFoundPath.size() == 0)
  {
    Dl_info DlInfo;
    if(dladdr((void*)CcLinuxModule::CPrivate::marker, &DlInfo))
    {
      CcString sCurrentDir = DlInfo.dli_fname;
      sCurrentDir = CcStringUtil::getDirectoryFromPath(sCurrentDir);
      sCurrentDir.appendPath(sName);
      if(CcFile::exists(sCurrentDir))
      {
        sFoundPath = sCurrentDir;
      }
    }
  }
  if(sFoundPath.size() == 0)
  {
    CcString sPath = CcKernel::getEnvironmentVariable("LD_LIBRARY_PATH");
    sPath += ";" + CcKernel::getEnvironmentVariable("LIBRARY_PATH");
    sPath += ";" + CcKernel::getEnvironmentVariable("PATH");
    CcStringList oLibPaths = sPath.split(CcGlobalStrings::Seperators::Semicolon, false);
    for(CcString& sLibPath : oLibPaths)
    {
      CcFileInfoList oFileList = CcDirectory::getFileList(sLibPath);
      for(CcFileInfo& oFile : oFileList)
      {
        if(oFile.getName() == sPath)
        {
          sFoundPath = sPath;
        }
      }
    }
  }
  if(sFoundPath.size())
  {
    void* pModule = dlopen(sFoundPath.getCharString(), RTLD_LAZY);
    CCDEBUG("Found at " + CcString::fromNumber(reinterpret_cast<uintptr>(pModule)));
  }
}

CcLinuxModule::~CcLinuxModule()
{
  CcLinuxModule::CPrivate* pPrivate = static_cast<CcLinuxModule::CPrivate*>(m_pContext);
  CCDELETE(pPrivate);
}

CcStatus CcLinuxModule::init()
{
  return false;
}

CcStatus CcLinuxModule::deinit()
{
  return false;
}
