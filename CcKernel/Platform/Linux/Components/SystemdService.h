/*
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
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class SystemdService
 */
#pragma once

#include "CcBase.h"
#include "CcService.h"
#include "CcArguments.h"
#include "Documents/CcIniFile.h"

/**
 * @brief Device interface for linux pins in /sys/class/gpio/
 */
class SystemdService
{
public:
  /**
   * @brief Initialize interface with predefined pin index in /sys/class/gpio/
   * @param uiPinNr: Target pin number
   */
  SystemdService(CcService& oService);
  ~SystemdService();

  CcStatus readFile();
  CcStatus writeFile();

  CcStatus  create();
  CcStatus  remove();
  CcStatus  stop();
  CcStatus  start();
  CcStatus  setArguments(const CcArguments& oArguments);
  CcStatus  setAutoStart(bool bOnOff);
  CcStatus  setExectuable(const CcString& sExePath);
  CcStatus  setWorkingDir(const CcString& sWorkingDir);

  static CcStringList getAllServices();

private:
  void checkBasicData();
  void setupExecPath();

private:
  CcService&    m_oService;
  CcString      m_sServiceFile;
  CcIniFile     m_oServiceFile;

  CcString      m_sExecutable;
  CcArguments   m_oArguments;
};
