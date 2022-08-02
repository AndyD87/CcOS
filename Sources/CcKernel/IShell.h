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
#include "CcApp.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcVector.h"
#include "CcMapCommon.h"
#include "CcMutex.h"

class IIo;
class IShellCommand;
class CcKernelShutdownEvent;
class CcProcess;

#ifdef _MSC_VER
template class CcKernelSHARED CcVector<IShellCommand*>;
#endif

/**
 * @brief Basic shell application.
 */
class CcKernelSHARED IShell : public CcApp
{
public:
  /**
   * @brief Create thread instantce with name.
   * @param sName: Target name of thread
   */
  IShell();

  /**
   * @brief Destroy Object and waiting until @ref getThreadState is set to EThreadState::Stopped
   */
  virtual ~IShell();

  virtual void run() override;
  
  void init(IIo* pIoStream);
  void initDefaultCommands();
  CcStatus changeDirectory(const CcString& sPath);
  const CcString& getWorkingDirectory()
  { return m_sWorkingDir; }
  const CcStringMap& getEnvironmentVariables()
  { return m_oEnvironmentVariables; }
  void setEnvironmentVariable(const CcString& sName, const CcString& sValue)
  { m_oEnvironmentVariables.set(sName, sValue); }

  size_t writeLine(const CcString& sLine);
  void setEcho(bool bOnOff)
  { m_bEchoInput = bOnOff; }
  void setStream(IIo* pIoStream)
  { m_pIoStream =  pIoStream; }

protected:
  CcStatus execLine(CcStringList& oArguments);
  CcStatus isConnectionActive();

private:
  size_t readLine();
  void updatePrefix();
  virtual void onStop() override;
  void onKernelShutdown(CcKernelShutdownEvent* pEvent);
  void cancelOnStop(IThread* pProcess);

private:
  IIo*                      m_pIoStream;
  CcByteArray               m_oTransferBuffer;
  CcByteArray               m_oReadLineBuffer;
  bool                      m_bEchoInput = true;
  CcMutex                   m_oActiveCommandLock;
  IShellCommand*            m_pActiveCommand = nullptr;
  CcProcess*                m_pActiveProcess = nullptr;

  size_t    m_uiLastRead = 0;
  size_t    m_uiLastWritten = 0;
  CcString  m_sRead;
  CcString  m_sWorkingDir;
  CcString  m_sPrefix;
  CcStringMap m_oEnvironmentVariables;

  CcVector<IShellCommand*> m_oCommands;
  CcVector<IShellCommand*> m_oCreatedCommands;
};
