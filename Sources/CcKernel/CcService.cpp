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
 * @brief     Implementation of Class CcService
 */
#include "CcService.h"
#include "CcKernel.h"
#include "CcEventHandler.h"
#include "CcServiceSystem.h"
#ifdef GENERIC
  #include "CcGenericThreadManager.h"
#endif

CcService::CcService(CcSharedPointer<CcApp> pApplication) :
  m_oApplication(pApplication)
{
}

CcService::CcService(const CcString& sServiceName, CcSharedPointer<CcApp> pApplication) :
  m_oApplication(pApplication),
  m_sServiceName(sServiceName)
{
}

CcService::~CcService()
{
}

void CcService::eventStart()
{
}

void CcService::eventPause()
{
}

void CcService::eventStop()
{
}

CcStatus CcService::exec()
{
  CcStatus oRunStatus = CcKernel::getServiceSystem().init(*this);
  CcKernel::getServiceSystem().deinit(*this);
  return oRunStatus;
}

void CcService::stop()
{
  if(m_oApplication.isValid())
  {
    m_oApplication->stop();
    m_oApplication->waitForExit();
  }
}

CcStatus CcService::getExitCode() const
{
  if (m_oApplication.isValid())
  {
    return m_oApplication->getExitCode();
  }
  return EStatus::NotFound;
}

void CcService::idle()
{
  // short delay
  CcKernel::sleep(1);
}

CcStatus CcService::run()
{
  #ifdef GENERIC
    CcGenericThreadManager::getInstance()->start();
  #endif

  if (m_oApplication.isValid())
  {
    m_oApplication->start();
    m_oApplication->waitForRunning();
  }
  eventStart();
  CCDEBUG("Service started");
  loop();
  CCDEBUG("Service stopped");
  eventStop();

  #ifdef GENERIC
    CcGenericThreadManager::getInstance()->stop();
  #endif
  return m_oApplication->getExitCode();
}

size_t CcService_uiCount = 0;

bool CcService::onLoop()
{
  CcService_uiCount++;
  idle();
  #ifdef GENERIC
    bool bKeepLooping = CcGenericThreadManager::getInstance()->idle();
  #else
    bool bKeepLooping = m_oApplication->isRunning();
  #endif
  return bKeepLooping;
}
