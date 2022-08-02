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
 * @brief     Implementation of Class CcShell
 */

#include "CcShell.h"
#include "CcConsole.h"
#ifdef WINDOWS
  #include "Shell/CcWinRawConsole.h"
#endif

CcShell::CcShell(IIo* pIoStream) :
  IShell()
{
  if (pIoStream)
  {
    m_oIoStream.setReadStream(pIoStream);
    m_oIoStream.setWriteStream(pIoStream);
  }
  else
  {
    #ifdef WINDOWS
      CCNEW(m_pStreamControl, CcWinRawConsole);
      if (m_pStreamControl->open(EOpenFlags::ReadWrite))
      {
        m_oIoStream.setReadStream(m_pStreamControl);
        m_oIoStream.setWriteStream(m_pStreamControl);
      }
    #else
    #ifndef GENERIC
      setEcho(false);
    #endif // !GENERIC
      m_oIoStream.setReadStream(&CcConsole::getInStream());
      m_oIoStream.setWriteStream(&CcConsole::getOutStream());
    #endif
  }
  init(&m_oIoStream);
  initDefaultCommands();
}

CcShell::~CcShell()
{
  setStream(nullptr);
  CCDELETE(m_pStreamControl);
}
