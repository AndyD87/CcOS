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
 * @brief     Implementation of Class CcStdErr
 */
#include "CcStdErr.h"
#include "CcWString.h"
#include "stdio.h"

size_t CcStdErr::size(void)
{
  return SIZE_MAX;
}

size_t CcStdErr::read(void* pBuffer, size_t uSize)
{
  CCUNUSED(pBuffer);
  CCUNUSED(uSize);
  return 0;
}

size_t CcStdErr::write(const void* pBuffer, size_t uSize)
{
#ifdef WIN32
  CcWString ucString(static_cast<const char*>(pBuffer), uSize);
  fwprintf(stderr, L"%.*ws", (int) ucString.length(), ucString.getWcharString());
#else
  fprintf(stderr, "%.*s", (int) uSize, static_cast<const char*>(pBuffer));
#endif
  return uSize;
}

CcStatus CcStdErr::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

CcStatus CcStdErr::close()
{
  return false;
}
