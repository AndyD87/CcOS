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
 * @brief     Class IShell
 */
#pragma once

#include "CcBase.h"
#include "CcStringList.h"

class IShell;

/**
 * @brief Basic shell application.
 */
class CcKernelSHARED IShellCommand
{
public:
  /**
   * @brief Create thread instantce with name.
   * @param sName: Target name of thread
   */
  IShellCommand(const CcString& sCommand);

  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to EThreadState::Stopped
   */
  virtual ~IShellCommand();

  virtual CcStatus exec(IShell& oBasicShell, const CcStringList& oArguments) = 0;
  virtual CcStatus stop()
  { return CcStatus(); }

  const CcString& getCommand()
  { return m_sCommand; }

private:
  CcString m_sCommand;
};
