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

CcService* CcService::s_pInstance(nullptr);

CcService::CcService(CcSharedPointer<CcApp> pApplication) :
  m_oApplication(pApplication)
{
  if (m_oApplication.isValid())
    m_sServiceName = m_oApplication->getName();
  if (s_pInstance == nullptr)
    s_pInstance = this;
}

CcService::CcService(const CcString& sServiceName, CcSharedPointer<CcApp> pApplication) :
  m_oApplication(pApplication),
  m_sServiceName(sServiceName)
{
  if (s_pInstance == nullptr)
    s_pInstance = this;
}

CcService::~CcService()
{
  if (s_pInstance == this)
    s_pInstance = nullptr;
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
  CcStatus oStatus;
  if (m_oApplication.isValid())
    m_oApplication->start();
  loop();
  if (m_oApplication.isValid())
    oStatus = m_oApplication->getExitCode();
  return oStatus;

}

void CcService::idle()
{
}

void CcService::run()
{
  eventStart();
  loop();
  eventStop();
}

bool CcService::onLoop()
{
  idle();
  return m_eThreadState == EThreadState::Running;
}

void CcService::onStop()
{
  if(m_oApplication.isValid())
    m_oApplication->stop();
}
