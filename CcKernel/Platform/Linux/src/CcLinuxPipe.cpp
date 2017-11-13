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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcLinuxPipe
 */
#include "CcLinuxPipe.h"
#include "CcKernel.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

CcLinuxPipe::CcLinuxPipe()
{
  if(pipe(m_iPipes) != 0)
  {
    CCDEBUG("Error on creating pipes");
  }
}

CcLinuxPipe::~CcLinuxPipe(void)
{
  if(m_iPipes[0] >= 0)
    ::close(m_iPipes[0]);
  if(m_iPipes[0] >= 0)
    ::close(m_iPipes[1]);
}

size_t CcLinuxPipe::read(void *buffer, size_t size)
{
  return ::read(m_iPipes[0], buffer, size);
}

size_t CcLinuxPipe::write(const void* buffer, size_t size)
{
  return ::write(m_iPipes[1], buffer, size);
}

CcStatus CcLinuxPipe::open(EOpenFlags)
{
  return true;
}

CcStatus CcLinuxPipe::close()
{
  return true;
}

CcStatus CcLinuxPipe::cancel()
{
  return true;
}
