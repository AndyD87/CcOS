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
#include "IThread.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcVector.h"
#include "CcMapCommon.h"

class IIo;
class IShellCommand;

/**
 * @brief Basic shell application.
 */
class IShell : public IThread
{
public:
  /**
   * @brief Create thread instantce with name.
   * @param sName: Target name of thread
   */
  CcKernelSHARED IShell();

  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to EThreadState::Stopped
   */
  CcKernelSHARED virtual ~IShell();

  CcKernelSHARED virtual void run() override;
  
  CcKernelSHARED void init(IIo* pIoStream);
  CcKernelSHARED void initDefaultCommands();
  CcKernelSHARED CcStatus changeDirectory(const CcString& sPath);
  CcKernelSHARED const CcString& getWorkingDirectory()
  { return m_sWorkingDir; }
  const CcStringMap& getEnvironmentVariables()
  { return m_oEnvironmentVariables; }
  void setEnvironmentVariable(const CcString& sName, const CcString& sValue)
  { m_oEnvironmentVariables.set(sName, sValue); }

  CcKernelSHARED void writeLine(const CcString& sLine);

private:
  void readLine();
  void updatePrefix();

private:
  IIo*          m_pIoStream;
  CcByteArray   m_oTransferBuffer;
  CcByteArray   m_oReadLineBuffer;

  CcString  m_sRead;
  CcString  m_sWorkingDir;
  CcString  m_sPrefix;
  CcStringMap m_oEnvironmentVariables;

  CcVector<IShellCommand*> m_oCommands;
  CcVector<IShellCommand*> m_oCreatedCommands;
};
