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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcStdIn
 */

#include "CcStdIn.h"
#include "CcUCString.h"
#include <stdio.h>
#include <cstring>

CcStdIn::CcStdIn(void)
{
}

CcStdIn::~CcStdIn() {

}

size_t CcStdIn::read(char* buffer, size_t size)
{
  size_t iRet = 0;
#ifdef WIN32
  CcUCString ucString(size);
  if (fgetws(ucString.getWcharString(), (int) ucString.length(), stdin) != nullptr)
  {
    ucString.resize(wcslen(ucString.getWcharString()));
    m_sTemporaryBackup.append( ucString.getString());
    if (m_sTemporaryBackup.length() > size)
    {
      memcpy(buffer, m_sTemporaryBackup.getCharString(), size);
      iRet = size;
      m_sTemporaryBackup.remove(0, size);
    }
    else
    {
      memcpy(buffer, m_sTemporaryBackup.getCharString(), m_sTemporaryBackup.length());
      iRet = m_sTemporaryBackup.length();
      m_sTemporaryBackup.clear();
    }
  }
  else
  {
    m_sTemporaryBackup.clear();
  }
#else
  if (fgets(buffer, (int) size, stdin) != nullptr)
  {
    iRet = strlen(buffer);
  }
#endif
  return iRet;
}

size_t CcStdIn::write(const char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}

bool CcStdIn::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

bool CcStdIn::close()
{
  return false;
}
