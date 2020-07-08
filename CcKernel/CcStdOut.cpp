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
 * @brief     Implementation of Class CcStdOut
 */

#include "CcStdOut.h"
#include "stdio.h"
#include "CcWString.h"

size_t CcStdOut::read(void* pBuffer, size_t uSize)
{
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
  return SIZE_MAX;
}

size_t CcStdOut::write(const void* pBuffer, size_t uSize)
{
#ifdef WINDOWS
  CcWString ucString(static_cast<const char*>(pBuffer), uSize);
  wprintf(L"%ls", ucString.getWcharString());
#elif defined(GENERIC)
  CCUNUSED(pBuffer);
#else
  printf("%.*s", (int)uSize, static_cast<const char*>(pBuffer));
#endif
  return uSize;
}

CcStatus CcStdOut::open(EOpenFlags eOpenFlags)
{
  CCUNUSED(eOpenFlags);
  return false;
}

CcStatus CcStdOut::close()
{
  return false;
}

void CcStdOut::disableBuffer()
{
#if !defined(GENERIC)
  setvbuf(stdout, nullptr, _IONBF, 0);
#endif
}

