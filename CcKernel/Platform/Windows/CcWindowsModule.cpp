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
      m_pCreate = reinterpret_cast<IModule_CreateFunction>(GetProcAddress(reinterpret_cast<HMODULE>(m_pInstance), IModule_CreateFunctionName));
      m_pRemove = reinterpret_cast<IModule_RemoveFunction>(GetProcAddress(reinterpret_cast<HMODULE>(m_pInstance), IModule_RemoveFunctionName));
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
          CCMONITORDELETE(m_pInstance);
          FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
          resetHandles();
        }
      }
      else
      {
        CCMONITORDELETE(m_pInstance);
        FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
        resetHandles();
      }
    }
  }
  return oStatus;
}

CcStatus CcWindowsModule::unloadModule()
{
  if(m_pModule)
  {
    m_pModule->deinit();
    (*m_pRemove)(m_pModule);
  }
  if(m_pInstance)
  {
    CCMONITORDELETE(m_pInstance);
    FreeLibrary(reinterpret_cast<HMODULE>(m_pInstance));
  }
  resetHandles();
  return true;
}

void CcWindowsModule::resetHandles()
{
  m_pCreate = nullptr;
  m_pRemove = nullptr;
  m_pModule = nullptr;
  m_pInstance = nullptr;
}

void CcWindowsModule::marker(){}
