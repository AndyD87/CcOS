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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcProcess
 */
#include "CcProcess.h"
#include "CcKernel.h"
#include "CcIODevice.h"
#include "CcSharedPointer.h"
#include "CcThreadObject.h"

class CcProcessPrivate
{
public:
  CcSharedPointer<CcIODevice> m_pPipe = nullptr;
  CcSharedPointer<CcThreadObject> m_pThreadHandle = nullptr;
};

CcProcess::CcProcess(void)
{
  m_pPrivate = new CcProcessPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcProcess::CcProcess( const CcString& sApplication) :
  m_sApplication(sApplication)
{
  m_pPrivate = new CcProcessPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcProcess::~CcProcess(void )
{
  CCDELETE(m_pPrivate);
}

void CcProcess::start()
{
  CcKernel::createProcess(*this);
}

void CcProcess::waitForExit()
{
  if (m_pPrivate->m_pThreadHandle != nullptr)
  {
    m_pPrivate->m_pThreadHandle->waitForExit();
  }
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

CcString& CcProcess::getApplication(void)
{
  return m_sApplication;
}

const CcString& CcProcess::getApplication(void) const
{
  return m_sApplication;
}

CcStringList& CcProcess::getArguments(void)
{
  return m_Arguments;
}

CcIODevice& CcProcess::pipe()
{
  return *m_pPrivate->m_pPipe;
}

bool CcProcess::hasExited()
{
  if (m_pPrivate->m_pThreadHandle != nullptr)
  {
    return m_pPrivate->m_pThreadHandle->getThreadState() == EThreadState::Stopped;
  }
  return false;
}

const CcStringList& CcProcess::getArguments(void) const
{
  return m_Arguments;
}

void CcProcess::addArgument(const CcString& sArgument)
{
  m_Arguments.add(sArgument);
}

void CcProcess::clearArguments(void)
{
  m_Arguments.clear();
}

void CcProcess::setThreadHandle(CcThreadObject* pThreadHandle)
{
  m_pPrivate->m_pThreadHandle = pThreadHandle;
}

void CcProcess::setPipe(CcIODevice* pInput)
{
  m_pPrivate->m_pPipe = pInput;
}
