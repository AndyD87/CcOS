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

CcStatus CcLinuxModule::loadModule(const CcString& sName, const IKernel& oKernel)
{
  CcStatus oStatus(false);
  CcString sFoundPath;
  if(CcFile::exists(sName))
  {
    sFoundPath = sName;
  }
  if(sFoundPath.size() == 0)
  {
    Dl_info DlInfo;
    if(dladdr((void*)CcLinuxModule::marker, &DlInfo))
    {
      CcString sCurrentDir = DlInfo.dli_fname;
      sCurrentDir = CcStringUtil::getDirectoryFromPath(sCurrentDir);
      sCurrentDir.appendPath(sName);
      if(CcFile::exists(sCurrentDir))
      {
        sFoundPath = sCurrentDir;
      }
      else
      {
        sCurrentDir = DlInfo.dli_fname;
        sCurrentDir = CcStringUtil::getDirectoryFromPath(sCurrentDir);
        sCurrentDir.appendPath("../lib");
        sCurrentDir.appendPath(sName);
        sCurrentDir.normalizePath();
        if(CcFile::exists(sCurrentDir))
        {
          sFoundPath = sCurrentDir;
        }
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
    m_pHandle = dlopen(sFoundPath.getCharString(), RTLD_LAZY);
    CCMONITORNEW(m_pHandle);
    if (!m_pHandle)
    {
        CcString sError(dlerror());
        CCDEBUG(sError);
    }
    else
    {
      m_pCreate = reinterpret_cast<IModule_CreateFunction>(dlsym(m_pHandle, IModule_CreateFunctionName));
      m_pRemove = reinterpret_cast<IModule_RemoveFunction>(dlsym(m_pHandle, IModule_RemoveFunctionName));
      if(m_pCreate && m_pRemove)
      {
        m_pModule = (*m_pCreate)(oKernel);
        if(m_pModule)
        {
          oStatus = m_pModule->init();
          if(!oStatus)
          {
            unloadModule();
          }
        }
        else
        {
          CCMONITORDELETE(m_pHandle);
          dlclose(m_pHandle);
          resetHandles();
        }
      }
      else
      {
        CCMONITORDELETE(m_pHandle);
        dlclose(m_pHandle);
        resetHandles();
      }
    }
  }
  // Check with extension if not already done
  if (!oStatus && sFoundPath.size() == 0 &&
      !sName.endsWith(CcGlobalStrings::Extensions::System::DynamicLibraryCommon) )
  {
    oStatus = loadModule(CcGlobalStrings::Prefixes::Lib + sName + CcGlobalStrings::Seperators::Dot + CcGlobalStrings::Extensions::System::DynamicLibraryCommon, oKernel);
  }
  return oStatus;
}

CcStatus CcLinuxModule::unloadModule()
{
  if(m_pModule)
  {
    m_pModule->deinit();
    (*m_pRemove)(m_pModule);
  }
  if(m_pHandle)
  {
    CCMONITORDELETE(m_pHandle);
    dlclose(m_pHandle);
  }
  resetHandles();
  return true;
}

void CcLinuxModule::resetHandles()
{
  m_pCreate = nullptr;
  m_pRemove = nullptr;
  m_pModule = nullptr;
  m_pHandle = nullptr;
}

void CcLinuxModule::marker(){}
