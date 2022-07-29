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
#include "CcFileSystem.h"
#include "CcProcess.h"

class IShellPassthroughThread : public IThread
{
public:
  IShellPassthroughThread(CcProcess* pProcess, IIo* pStream) :
    m_pProcess(pProcess),
    m_pStream(pStream)
  { }

  virtual void run() override
  {
    CcByteArray oReadBuffer;
    oReadBuffer.resize(1024);
    while (m_pStream && m_pProcess &&
            m_pProcess->getCurrentState() != EThreadState::Stopped)
    {
      size_t uiRead = m_pProcess->pipe().readArray(oReadBuffer, false);
      if (uiRead && m_pStream)
        m_pStream->writeArray(oReadBuffer, uiRead);
    }
    m_pStream = nullptr;
    m_pProcess = nullptr;
  }

  virtual void onStop() override
  {
    if(m_pStream)
    {
      m_pStream->cancel();
      m_pStream = nullptr;
    }
    m_pProcess = nullptr;
  }

  CcProcess*  m_pProcess;
  IIo*        m_pStream;
};


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
  writeLine("Welcome to CcOS Basic Shell");
  while(isRunning())
  {
    if(m_pIoStream)
    {
      // Start with next line
      updatePrefix();
      m_sRead.clear();
      if(uiReceived != SIZE_MAX)
        m_pIoStream->writeString(m_sPrefix);

      uiReceived = readLine(nullptr);
      // Handle the line
      CcStringList oArguments = CcStringUtil::getArguments(m_sRead);
      setExitCode(execLine(oArguments));
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
#include "Shell/CcShellEcho.h"

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
  CREATE_AND_APPEND(CcShellEcho);
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

size_t IShell::readLine(IIo* pOutStream)
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
    if(uiReceived > m_oTransferBuffer.size())
      CcKernel::sleep(10);
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
        case '\b': // del pressed
          if (m_sRead.size() > 0)
          {
            m_sRead.remove(m_sRead.size() - 1);
            if (m_bEchoInput)
            {
              m_pIoStream->write("\b \b", 3);
              if (pOutStream) pOutStream->write("\b \b", 3);
            }
          }
          break;
        case 127: // del pressed
          if(m_sRead.size() > 0)
          {
            m_sRead.remove(m_sRead.size()-1);
            if (m_bEchoInput)
            {
              m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
              if (pOutStream) pOutStream->write(&m_oTransferBuffer[uiPos], 1);
            }
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
          if (m_bEchoInput)
          {
            m_pIoStream->writeString(CcGlobalStrings::EolLong);
            if (pOutStream) pOutStream->writeString(CcGlobalStrings::EolLong);
          }
          break;
        default:
          if(static_cast<uint8>(m_oTransferBuffer[uiPos]) < 32)
          {
            // Instruction signs, do not display or save
          }
          else
          {
            m_sRead.append(m_oTransferBuffer[uiPos]);
            if (m_bEchoInput)
            {
              m_pIoStream->write(&m_oTransferBuffer[uiPos], 1);
              if(pOutStream) pOutStream->write(&m_oTransferBuffer[uiPos], 1);
            }
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

CcStatus IShell::execLine(CcStringList& oArguments)
{
  CcStatus oStatus;
  bool bCommandFound = false;
  if (oArguments.size() > 0)
  {
    CcString sCommand = oArguments[0];
    oArguments.remove(0);
    for (IShellCommand* pCommand : m_oCommands)
    {
      if (pCommand->getCommand() == sCommand)
      {
        bCommandFound = true;

        m_oActiveCommandLock.lock();
        m_pActiveCommand = pCommand;
        m_oActiveCommandLock.unlock();
        oStatus = m_pActiveCommand->exec(*this, oArguments);
        m_oActiveCommandLock.lock();
        m_pActiveCommand = nullptr;
        m_oActiveCommandLock.unlock();

        break;
      }
    }
    if (bCommandFound == false)
    {
      // Search for Executable
      CcString sPath = CcFileSystem::findExecutable(sCommand, m_sWorkingDir);
      if (sPath.length())
      {
        bCommandFound = true;

        CCNEW(m_pActiveProcess, CcProcess, sPath);
        IShellPassthroughThread oPassThroughThread(m_pActiveProcess, m_pIoStream);

        m_pActiveProcess->setArguments(oArguments);
        m_pActiveProcess->setWorkingDirectory(getWorkingDirectory());
        m_pActiveProcess->registerOnStateChange(NewCcEvent(this, IShell::cancelOnStop));

        m_pActiveProcess->start();
        oPassThroughThread.start();

        CcByteArray oReadBuffer;
        oReadBuffer.resize(1024);
        while (m_pActiveProcess->getCurrentState() != EThreadState::Stopped)
        {
#ifdef WINDOWS
          m_sRead.clear();
          size_t uiRead = readLine(&m_pActiveProcess->pipe());
#else
          size_t uiRead = m_pIoStream->readArray(oReadBuffer, false);
          if (uiRead &&
              uiRead < oReadBuffer.size() &&
              m_pIoStream)
          {
            m_pActiveProcess->pipe().writeArray(oReadBuffer, uiRead);
          }
#endif
        }
        oPassThroughThread.stop();
        CCDELETE(m_pActiveProcess);
      }
    }
    if (bCommandFound == false)
    {
      writeLine("Unknown command: " + sCommand);
      oStatus = EStatus::CommandUnknown;
    }
  }
  return oStatus;
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
      else if (m_pActiveProcess)
      {
        m_pActiveProcess->stop();
        pEvent->bContinueShutdown = false;
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

void IShell::cancelOnStop(IThread* pProcess)
{
  if( pProcess && 
      pProcess->getThreadState() == EThreadState::Stopped && 
      m_pIoStream)
  {
    m_pIoStream->cancel();
  }
}
