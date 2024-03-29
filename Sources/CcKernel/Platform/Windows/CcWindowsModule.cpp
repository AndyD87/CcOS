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
* @brief     Implementation of Class CCcWindowsModule
*/
#include "CcWindowsModule.h"
#include "CcDirectory.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcWindowsFile.h"
#include "CcSystem.h"

CcWindowsModule::~CcWindowsModule()
{
  if(m_pModule)
    unloadModule(m_pModule);
  if (m_pInstance)
  {
    CCMONITORDELETE(m_pInstance);
    FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
    resetHandles();
  }

}

CcStatus CcWindowsModule::loadModule(const CcString& sName, const IKernel& oKernel)
{
  m_sName = sName;
  CcStatus oStatus(false);
  CcString sFoundPath;
  // Check current working dir
  if(CcFile::exists(sName))
  {
    sFoundPath = sName;
  }
  // Check dir of current application
  if (sFoundPath.size() == 0)
  {
    DWORD dwSize = 0;
    wchar_t* pwcBaseDir = nullptr;
    do
    {
      dwSize += MAX_PATH;
      CCDELETEARRAY(pwcBaseDir);
      CCNEWARRAY(pwcBaseDir, wchar_t, dwSize);
    } while (GetLastError() == ERROR_INSUFFICIENT_BUFFER);

    if(pwcBaseDir)
    {
      if (dwSize > GetModuleFileNameW(nullptr, pwcBaseDir, dwSize) &&
          GetLastError() == ERROR_SUCCESS)
      {
        CcWString wsPath(pwcBaseDir);
        CcString sPath = wsPath.getString();
        sPath.appendPath("..");
        sPath.appendPath(sName);
        sPath.normalizePath();
        if (CcFile::exists(sPath))
        {
          sFoundPath = sPath;
        }

      }
      CCDELETEARRAY(pwcBaseDir);
    }
  }
  // Check in PATH
  if(sFoundPath.size() == 0)
  {
    CcString sPath = CcKernel::getEnvironmentVariable("PATH");
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
    CcWString sWindowsPAth = CcWindowsFile::toWindowsPath(sFoundPath);
    m_pInstance = LoadLibraryW(sWindowsPAth.getWcharString());
    CCMONITORNEW(m_pInstance);
    if (!m_pInstance)
    {
    }
    else
    {
      m_pRemove = reinterpret_cast<IModule_RemoveFunction>((void(*)())GetProcAddress(reinterpret_cast<HMODULE>(m_pInstance), IModule_RemoveFunctionName));
      m_pCreate = reinterpret_cast<IModule_CreateFunction>((void(*)())GetProcAddress(reinterpret_cast<HMODULE>(m_pInstance), IModule_CreateFunctionName));
      if(m_pCreate && m_pRemove)
      {
        m_pModule = (*m_pCreate)();
        if(m_pModule)
        {
          m_pModule->setKernel(oKernel);
          oStatus = m_pModule->init();
          m_pModule->registerOnUnload(NewCcEvent(this, CcWindowsModule::unloadModule));
          if(!oStatus)
          {
            unloadModule(m_pModule);
          }
        }
        else
        {
          oStatus = EStatus::CommandNotImplemented;
          CCMONITORDELETE(m_pInstance);
          FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
          resetHandles();
        }
      }
      else
      {
        oStatus = EStatus::CommandNotImplemented;
        CCMONITORDELETE(m_pInstance);
        FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
        resetHandles();
      }
    }
  }
  // Check with extension if not already done
  if (!oStatus && sFoundPath.size() == 0 &&
      !sName.endsWith(CcGlobalStrings::Extensions::System::DynamicLibraryWindows) )
  {
    oStatus = loadModule(sName + CcGlobalStrings::Seperators::Dot + CcGlobalStrings::Extensions::System::DynamicLibraryWindows, oKernel);
  }
  return oStatus;
}

void CcWindowsModule::unloadModule(void* pModule)
{
  m_pModule->deregisterOnUnload(this);
  if(m_pModule == pModule)
  {
    m_pModule->deinit();
    // VS2015 unloads the module earlier than this method may be called
    // To avoid crahes thie Version will be ignored
    //#if ( (_MSC_VER < 1900 && _MSC_VER >= 1910) || defined(DEBUG) )
      (*m_pRemove)(m_pModule);
    //#else
    //  CCMONITORDELETE(m_pModule);
    //#endif
    m_pModule = nullptr;
  }
  if(m_pInstance)
  {
    CCMONITORDELETE(m_pInstance);
    FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
    m_pInstance = nullptr;
  }
  resetHandles();
}

void CcWindowsModule::resetHandles()
{
  m_pCreate = nullptr;
  m_pRemove = nullptr;
  m_pModule = nullptr;
  m_pInstance = nullptr;
}

void CcWindowsModule::marker(){}
