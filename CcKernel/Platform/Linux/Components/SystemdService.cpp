/**
 * @copyright  Andreas Dirmeier (C) 2015
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

const char SYSTEMD_CONFIG_PATH[] = "/etc/systemd/system/";

SystemdService::SystemdService(CcService& oService) :
  m_oService(oService),
  m_sServiceFile(SYSTEMD_CONFIG_PATH + oService.getName() + ".service")
{
}

SystemdService::~SystemdService()
{

}

void SystemdService::readFile()
{
  m_oServiceFile.readFile(m_sServiceFile);
}

void SystemdService::writeFile()
{
  m_oServiceFile.writeFile(m_sServiceFile);
}

CcStatus SystemdService::create()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus SystemdService::remove()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus SystemdService::stop()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus SystemdService::start()
{
  CcStatus oStatus(false);
  return oStatus;
}

CcStatus SystemdService::setArguments(const CcArguments& oArguments)
{
  CcStatus oStatus(false);
  CCUNUSED(oArguments);
  return oStatus;
}

CcStatus SystemdService::setAutoStart(bool bOnOff)
{
  CcStatus oStatus(false);
  CCUNUSED(bOnOff);
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
      CcString sCurrentExecutable = CcKernel::getCurrentExecutablePath();
      oUnitSection.setValue("ExecStart", sCurrentExecutable);
    }
  }
}
