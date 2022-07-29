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
 * @brief     Implementation of Class CcProcess
 */
#include "CcProcess.h"
#include "CcKernel.h"
#include "IIo.h"
#include "CcSharedPointer.h"
#include "IThread.h"
#include  "CcByteArray.h"

CcProcess::CcProcess()
{
}

CcProcess::CcProcess( const CcString& sApplication) :
  m_sApplication(sApplication)
{
}

CcProcess::~CcProcess()
{
  stop();
}

CcStatus CcProcess::start()
{
  CCVERBOSE("CcProcess::start");
  stop();
  return CcKernel::createProcess(*this);
}

void CcProcess::stop()
{
  if(m_pThreadHandle != nullptr)
  {
    m_pThreadHandle->stop();
    m_pThreadHandle.deleteCurrent();
  }
}

CcStatus CcProcess::exec(const CcDateTime& oTimeout)
{
  CcStatus oStatus = start();
  if(waitForStarted(oTimeout))
  {
    oStatus = waitForExit(oTimeout);
    if(oStatus == EStatus::TimeoutReached)
    {
      CCDEBUG("Process run timeout");
      stop();
    }
    else if(oStatus)
    {
      oStatus = getExitCode();
    }
  }
  else
  {
    CCDEBUG("Process start timeout");
  }
  return oStatus;
}

CcStatus CcProcess::waitForState(EThreadState eState, const CcDateTime& oTimeout)
{
  CcStatus oRet(EStatus::InvalidHandle);
  switch(eState)
  {
    case EThreadState::Starting:
    case EThreadState::Stopping:
      if (m_pThreadHandle != nullptr)
        oRet = m_pThreadHandle->waitForState(eState, oTimeout);
      break;
    case EThreadState::Stopped:
      oRet = waitForExit(oTimeout);
      break;
    case EThreadState::Running:
      if (m_pThreadHandle != nullptr)
      {
        oRet = m_pThreadHandle->waitForState(eState, oTimeout);
        if(oRet)
        {
          CcDateTime oTimeWaiting;
          while (!m_pThreadHandle->isProcessStarted() &&
                 oRet)
          {
            if (oTimeout.getTimestampUs() != 0 && oTimeout < oTimeWaiting)
            {
              oRet = EStatus::TimeoutReached;
            }
            else
            {
              CcKernel::delayMs(10);
              oTimeWaiting.addMSeconds(10);
            }
          }
        }
      }
      break;
    default:
      if (m_pThreadHandle != nullptr)
      {
        oRet = m_pThreadHandle->waitForState(eState, oTimeout);
      }
  }
  return oRet;
}

CcStatus CcProcess::waitForStarted(const CcDateTime& oTimeout)
{
  CcStatus oRet(EStatus::AllOk);
  CcDateTime oTimeWaiting;
  while (m_pThreadHandle->getThreadState() == EThreadState::Starting &&
         oRet)
  {
    if (oTimeout.getTimestampUs() != 0 && oTimeout < oTimeWaiting)
    {
      oRet = EStatus::TimeoutReached;
    }
    else
    {
      CcKernel::delayMs(10);
      oTimeWaiting.addMSeconds(10);
    }
  }
  return oRet;
}

CcStatus CcProcess::waitForExit(const CcDateTime& oTimeout)
{
  CcStatus oStatus(EStatus::InvalidHandle);
  if(m_pThreadHandle != nullptr)
  {
    oStatus = m_pThreadHandle->waitForState(EThreadState::Stopped, oTimeout);
    if (oStatus) oStatus = getExitCode();
  }
  else
  {
    oStatus = EStatus::AllOk;
  }
  return oStatus;
}

void CcProcess::setApplication(const CcString& sApplication)
{
  m_sApplication = sApplication;
}

void CcProcess::setArguments(const CcString& sArguments)
{
  m_Arguments.clear();
  m_Arguments.parseArguments(sArguments);
}

void CcProcess::setArguments(const CcStringList& slArguments)
{
  m_Arguments = slArguments;
}

CcString& CcProcess::getApplication()
{
  return m_sApplication;
}

const CcString& CcProcess::getApplication() const
{
  return m_sApplication;
}

CcStringList& CcProcess::getArguments()
{
  return m_Arguments;
}

EThreadState CcProcess::getCurrentState()
{
  if(m_pThreadHandle != nullptr)
  {
    return m_pThreadHandle->getThreadState();
  }
  else
  {
    return EThreadState::Stopped;
  }
}

IIo& CcProcess::pipe()
{
  return *m_pPipe;
}

bool CcProcess::hasExited()
{
  if (m_pThreadHandle != nullptr)
  {
    return m_pThreadHandle->getThreadState() == EThreadState::Stopped;
  }
  return false;
}

CcStatus CcProcess::exec(const CcString& sExecutable,
                         const CcStringList& oParams,
                         const CcString &sWorkingDir,
                         bool bDoWait,
                         const CcDateTime& oTimeout,
                         CcString* pOutput
)
{
  CcStatus oStatus;
  CcProcess oProc(sExecutable);
  oProc.setArguments(oParams);
  if(sWorkingDir.length())
    oProc.setWorkingDirectory(sWorkingDir);
  if(bDoWait)
  {
    oStatus = oProc.exec(oTimeout);
    if(pOutput)
    {
      *pOutput = oProc.pipe().readAll();
    }
  }
  else
    oStatus = oProc.start();
  return oStatus;
}


const CcStringList& CcProcess::getArguments() const
{
  return m_Arguments;
}

void CcProcess::addArgument(const CcString& sArgument)
{
  m_Arguments.add(sArgument);
}

void CcProcess::clearArguments()
{
  m_Arguments.clear();
}

void CcProcess::setThreadHandle(CcProcessThread* pThreadHandle)
{
  m_pThreadHandle = pThreadHandle;
  m_pThreadHandle->registerOnStateChange(NewCcEvent(this, CcProcess::onThreadStateChange));
}

void CcProcess::setPipe(IIo* pInput)
{
  m_pPipe = pInput;
}
