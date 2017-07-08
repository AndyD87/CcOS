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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcStdErr
 */
#include "CcStdErr.h"
#include "CcWString.h"
#include "stdio.h"

CcStdErr::CcStdErr(void)
{
}

CcStdErr::~CcStdErr() {

}
size_t CcStdErr::size(void)
{
  return SIZE_MAX;
}

size_t CcStdErr::read(char* buffer, size_t size)
{
  CCUNUSED(buffer);
  CCUNUSED(size);
  return 0;
}

size_t CcStdErr::write(const char* buffer, size_t size)
{
#ifdef WIN32
  CcWString ucString(buffer, size);
  fwprintf(stderr, L"%.*ws", (int) ucString.length(), ucString.getWcharString());
#else
  fprintf(stderr, "%.*s", (int) size, buffer);
#endif
  return size;
}

bool CcStdErr::open(EOpenFlags flags)
{
  CCUNUSED(flags);
  return false;
}

bool CcStdErr::close()
{
  return false;
}

bool CcStdErr::setFilePointer(size_t pos)
{
  CCUNUSED(pos);
  return false;
}

