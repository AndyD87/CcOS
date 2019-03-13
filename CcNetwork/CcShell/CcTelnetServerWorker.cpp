/**
 * @copyright  Andreas Dirmeier (C) 2017
 *
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
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 * @brief     Implementation of Class CcTelnetServerWorker
 */
#include "CcTelnetServerWorker.h"
#include "CcKernel.h"

CcTelnetServerWorker::CcTelnetServerWorker(CcSocket Socket) :
  m_Socket(Socket)
{
}

CcTelnetServerWorker::~CcTelnetServerWorker()
{
}

void CcTelnetServerWorker::run()
{
  CCDEBUG("Telnet Session started");
  while (getThreadState() == EThreadState::Running)
  {
    if (m_Socket != nullptr)
    {
      m_Shell.setInput(&m_Socket);
      m_Shell.setOutput(&m_Socket);
      m_Shell.start();
      while (m_Shell.getThreadState() != EThreadState::Stopped)
      {
        CcKernel::delayMs(100);
      }
    }
  }
}
