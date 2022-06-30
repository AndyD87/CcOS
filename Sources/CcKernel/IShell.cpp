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
#include "IShell.h"
#include "Shell/IShellCommand.h"
#include "CcKernel.h"
#include "IIo.h"
#include "CcGlobalStrings.h"
#include "CcStringUtil.h"
#include "CcDirectory.h"

IShell::IShell()
{
  m_sWorkingDir = CcKernel::getWorkingDir();
  m_oTransferBuffer.resize(256);
  CcKernel::registerShutdownHandler(NewCcEvent(this, IShell::onKernelShutdown));
}

IShell::~IShell()
{
  for(IShellCommand* pCommand : m_oCreatedCommands)
  {
    CCDELETE(pCommand);
  }
  m_oCreatedCommands.clear();
  onStop();
}

void IShell::run()
{
  size_t uiReceived = 0;
  while(isRunning())
  {
    if(m_pIoStream)
    {
      // Start with next line
      updatePrefix();
      m_sRead.clear();
      if(uiReceived != SIZE_MAX)
      m_pIoStream->writeString(m_sPrefix);

      uiReceived = readLine();
      // Handle the line
      CcStringList oArguments = CcStringUtil::getArguments(m_sRead);
      if(oArguments.size() > 0)
      {
        bool bCommandFound = false;
        for(IShellCommand* pCommand : m_oCommands)
        {
          if(pCommand->getCommand() == oArguments[0])
          {
            bCommandFound = true;
            oArguments.remove(0);

            m_oActiveCommandLock.lock();
            m_pActiveCommand = pCommand;
            m_oActiveCommandLock.unlock();
            setExitCode(m_pActiveCommand->exec(*this, oArguments));
            m_oActiveCommandLock.lock();
            m_pActiveCommand = nullptr;
            m_oActiveCommandLock.unlock();

            break;
          }
        }
        if(bCommandFound == false)
        {
          writeLine("Unknown command: " + oArguments[0]);
        }
      }
    }
  }
}

void IShell::init(IIo* pIoStream)
{
  m_pIoStream = pIoStream;
  m_oEnvironmentVariables = CcKernel::getEnvironmentVariables();
}

#include "Shell/CcShellCd.h"
#include "Shell/CcShellLs.h"
#include "Shell/CcShellGetEnv.h"
#include "Shell/CcShellExit.h"
#include "Shell/CcShellExport.h"

#define CREATE_AND_APPEND(COMMAND)                    \
  m_oCreatedCommands.append(CCNEW_INLINE(COMMAND)); \
  m_oCommands.append(m_oCreatedCommands.last());      

void IShell::initDefaultCommands()
{
  CREATE_AND_APPEND(CcShellCd);
  CREATE_AND_APPEND(CcShellLs);
  CREATE_AND_APPEND(CcShellGetEnv);
  CREATE_AND_APPEND(CcShellExit);
  CREATE_AND_APPEND(CcShellExport);
}

CcStatus IShell::changeDirectory(const CcString& sPath)
{
  CcStatus oStatus;
  CcString sNewPath = m_sWorkingDir;
  CcStringUtil::setPath(sNewPath, sPath);
  if(CcDirectory::exists(sNewPath))
  {
    m_sWorkingDir = sNewPath;
  }
  else
  {
    oStatus = EStatus::FSFileNotFound;
  }
  return oStatus;
}

void IShell::writeLine(const CcString& sLine)
{
  if(m_pIoStream)
  {
    m_pIoStream->writeLine(sLine, IIo::ELineEnding::CRNL);
  }
}

size_t IShell::readLine()
{
  size_t uiReceived = 0;
  if(m_oReadLineBuffer.size())
  {
    uiReceived = CCMIN(m_oReadLineBuffer.size(), m_oTransferBuffer.size());
    m_oTransferBuffer.write(m_oReadLineBuffer.getArray(), uiReceived);
    m_oReadLineBuffer.remove(0, uiReceived);
  }
  else
  {
    uiReceived = m_pIoStream->read(m_oTransferBuffer.getArray(), m_oTransferBuffer.size());
  }
  bool bEndOfLine = false;
  while(bEndOfLine == false && uiReceived != SIZE_MAX)
  {
    // echo read data

    // Interpret read data
    for(size_t uiPos =0 ; uiPos < uiReceived && bEndOfLine == false; uiPos++)
    {
      switch (static_cast<uint8>(m_oTransferBuffer[uiPos]))
      {
        case 127: // del pressed
          if(m_sRead.size() > 0)
          {
            m_sRead.remove(m_sRead.size()-1);
            if(m_bEchoInput) m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
          }
          break;
        case '\r':
          CCFALLTHROUGH;
        case '\n':
          uiPos++; // Ignore this char
          if(uiReceived > uiPos)
          {
            m_oReadLineBuffer.append(m_oTransferBuffer.getArray(uiPos), uiReceived - uiPos);
            uiPos = uiReceived;
          }
          else
          {
            bEndOfLine = true;
          }
          if (m_bEchoInput) m_pIoStream->writeString(CcGlobalStrings::EolLong);
          break;
        default:
          if(static_cast<uint8>(m_oTransferBuffer[uiPos]) < 32)
          {
            // Instruction signs, do not display or save
          }
          else
          {
            m_sRead.append(m_oTransferBuffer[uiPos]);
            if (m_bEchoInput) m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
          }
          break;
      }
    }
    if(bEndOfLine == false)
    {
      uiReceived = m_pIoStream->read(m_oTransferBuffer.getArray(), m_oTransferBuffer.size());
      CcKernel::sleep(10);
    }
  }
  return uiReceived;
}

void IShell::updatePrefix()
{
  m_sPrefix = m_sWorkingDir;
  m_sPrefix << CcGlobalStrings::Seperators::Colon << CcGlobalStrings::Seperators::Space;
}

void IShell::onStop()
{
  if (m_pIoStream)
  {
    m_pIoStream->cancel();
  }
}

void IShell::onKernelShutdown(CcKernelShutdownEvent* pEvent)
{
  switch (pEvent->eReason)
  {
    case CcKernelShutdownEvent::EReason::UserRequest:
      m_oActiveCommandLock.lock();
      if (m_pActiveCommand)
      {
        pEvent->bContinueShutdown = false;
        if (m_pActiveCommand->stop())
        {
        }
      }
      else
      {
        stop();
      }
      m_oActiveCommandLock.unlock();
      break;
    default:
      stop();
      waitForState(EThreadState::Stopped, CcDateTimeFromMSeconds(1000));
  }
}