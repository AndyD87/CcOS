/**
 * @copyright  Andreas Dirmeier (C) 2022
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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class SystemdService
 */
#include "SystemdService.h"
#include "CcKernel.h"
#include "CcByteArray.h"
#include "CcFile.h"
#include "CcDirectory.h"
#include "CcProcess.h"
#include "CcStringUtil.h"

const char SYSTEMD_CONFIG_PATH[]       = "/etc/systemd/system/";
const char SYSTEMD_SERVICE_EXTENSION[] = ".service";

SystemdService::SystemdService(CcService& oService) :
  m_oService(oService),
  m_sServiceFile(SYSTEMD_CONFIG_PATH + oService.getName() + SYSTEMD_SERVICE_EXTENSION)
{
  readFile();
  checkBasicData();
}

SystemdService::~SystemdService()
{

}

CcStatus SystemdService::readFile()
{
  CcStatus oStatus = m_oServiceFile.readFile(m_sServiceFile);
  return oStatus;
}

CcStatus SystemdService::writeFile()
{
  return m_oServiceFile.writeFile(m_sServiceFile);
}

CcStatus SystemdService::create()
{
  return writeFile();
}

CcStatus SystemdService::remove()
{
  CcProcess::exec("systemctl", {"disable", m_oService.getName()+SYSTEMD_SERVICE_EXTENSION},true);
  CcStatus oStatus = CcFile::remove(m_sServiceFile);
  return oStatus;
}

CcStatus SystemdService::stop()
{
  return CcProcess::exec("systemctl", {"stop", m_oService.getName()+SYSTEMD_SERVICE_EXTENSION},true);
}

CcStatus SystemdService::start()
{
  return CcProcess::exec("systemctl", {"start", m_oService.getName()+SYSTEMD_SERVICE_EXTENSION},true);
}

CcStatus SystemdService::setArguments(const CcArguments& oArguments)
{
  m_oArguments = oArguments;
  setupExecPath();
  return writeFile();
}

CcStatus SystemdService::setAutoStart(bool bOnOff)
{
  if(bOnOff)
    return CcProcess::exec("systemctl", {"enable", m_oService.getName()+SYSTEMD_SERVICE_EXTENSION},true);
  else
    return CcProcess::exec("systemctl", {"disable", m_oService.getName()+SYSTEMD_SERVICE_EXTENSION},true);
}

CcStatus SystemdService::setExectuable(const CcString& sExePath)
{
  m_sExecutable = sExePath;
  setupExecPath();
  return writeFile();
}

CcStatus SystemdService::setWorkingDir(const CcString& sWorkingDir)
{
  CcStatus oStatus(false);
  CcIniFile::CSection& oServiceSection = m_oServiceFile.createSection("Service");
  if(oServiceSection.isValid())
  {
    oServiceSection.setValue("WorkingDirectory", sWorkingDir);
    oStatus = true;
  }
  return oStatus;
}

void SystemdService::checkBasicData()
{
  CcIniFile::CSection& oUnitSection = m_oServiceFile.createSection("Unit");
  if(oUnitSection.isValid())
  {
    if(!oUnitSection.keyExists("Description"))
    {
      oUnitSection.setValue("Description", m_oService.getName());
    }
  }
  CcIniFile::CSection& oServiceSection = m_oServiceFile.createSection("Service");
  if(oServiceSection.isValid())
  {
    if(!oServiceSection.keyExists("Type"))
    {
      oServiceSection.setValue("Type", "simple");
    }
    if(!oServiceSection.keyExists("ExecStart"))
    {
      m_sExecutable = CcKernel::getCurrentExecutablePath();
      oServiceSection.setValue("ExecStart", m_sExecutable);
    }
    else
    {
      CcString sCurrentPath = oServiceSection.getValue("ExecStart");
      CcString sEscaped = CcStringUtil::escapeString(sCurrentPath);
      m_oArguments.parseArguments(sEscaped);
      if (m_oArguments.size() > 0)
      {
        m_sExecutable = m_oArguments[0];
        m_oArguments.remove(0);
      }
      else
      {
        m_sExecutable = "";
      }
    }
  }
  CcIniFile::CSection& oInstallSection = m_oServiceFile.createSection("Install");
  if(oInstallSection.isValid())
  {
    oInstallSection.setValue("WantedBy", "multi-user.target");
  }
}

void SystemdService::setupExecPath()
{
  CcIniFile::CSection& oServiceSection = m_oServiceFile.createSection("Service");
  if(oServiceSection.isValid())
  {
    CcString sExeArgPath = m_sExecutable;
    if (m_oArguments.size())
      sExeArgPath << CcGlobalStrings::Seperators::Space << m_oArguments.getLine();
    oServiceSection.setValue("ExecStart", sExeArgPath);
  }
}

CcStringList SystemdService::getAllServices()
{
  CcStringList oList;
  CcFileInfoList oFileList = CcDirectory::getFileList(SYSTEMD_CONFIG_PATH);
  for(CcFileInfo& oInfo : oFileList)
  {
    if(oInfo.isFile() && oInfo.getName().endsWith(SYSTEMD_SERVICE_EXTENSION))
    {
      oList.append(oInfo.getName().substr(0, oInfo.getName().length() - (sizeof(SYSTEMD_SERVICE_EXTENSION) - 1)));
    }
  }
  return oList;
}
