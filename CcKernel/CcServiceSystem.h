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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcServiceSystem
 */
#pragma once

#include "CcBase.h"
#include "CcService.h"

class CcStringList;
class CcArguments;

/**
 * @brief Main System class.
 *        This class is the connection from CcOS universe to Hardware or underlying OS.
 *        Every Platform has to implement this methods.
 *        For Example in Windows the File CcWindowsSystem.cpp is doing this,
 *        in Linux it would be done by CcLinuxSystem.com
 */
class CcKernelSHARED CcServiceSystem
{
public:
  //! Create system and all required variables
  CcServiceSystem();
  //! Cleanup system and all required variables
  ~CcServiceSystem();

  /**
   * @brief Initialize current application as service and run in background.
   *        A service supports notifications and signals from system.
   * @return 0 if Service was initialized successfully
   */
  CcStatus  init(CcService& pService);

  CcStatus  deinit(CcService& pService);

  CcStatus  create(CcService& pService);
  CcStatus  remove(CcService& pService);
  CcStatus  stop(CcService& pService);
  CcStatus  start(CcService& pService);
  CcStatus  setExectuable(CcService& pService, const CcString& sExePath);
  CcStatus  setWorkingDir(CcService& pService, const CcString& sWorkingDir);
  CcStatus  setArguments(CcService& pService, const CcArguments& oArguments);
  CcStatus  setAutoStart(CcService& pService, bool bOnOff);
  CcStringList getAllServices();

public: // Types
  class CPrivate;
private: // Member
  CPrivate* m_pPrivate = nullptr;
};
