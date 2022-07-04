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
 * @brief     Implementation of Class CcLinuxPipe
 */
#include "CcLinuxPipe.h"
#include "CcKernel.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

//! @brief Often called macro for closing an pipe if opened
//! @param PIPE: Pipe to close
#define CCLINUXPIPE_CLOSE(PIPE) \
    if(PIPE >= 0)               \
    { ::close(PIPE); PIPE = -1; }

CcLinuxPipe::CcLinuxPipe()
{
  if(pipe(m_iPipes[0]) != 0)
  {
    CCDEBUG("Error on creating pipes");
  }
  if(pipe(m_iPipes[1]) != 0)
  {
    CCDEBUG("Error on creating pipes");
  }
}

CcLinuxPipe::~CcLinuxPipe()
{
  CCLINUXPIPE_CLOSE(m_iPipes[0][0]);
  CCLINUXPIPE_CLOSE(m_iPipes[0][1]);
  CCLINUXPIPE_CLOSE(m_iPipes[1][0]);
  CCLINUXPIPE_CLOSE(m_iPipes[1][1]);
  CCLINUXPIPE_CLOSE(m_iPipes[2][0]);
  CCLINUXPIPE_CLOSE(m_iPipes[2][1]);
}

#include <sys/ioctl.h>

size_t CcLinuxPipe::read(void *pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if(m_iPipes[1][0] >= 0)
    uiRead = ::read(m_iPipes[1][0], pBuffer, uSize);
  return uiRead;
}

size_t CcLinuxPipe::write(const void* pBuffer, size_t uSize)
{
  if(m_iPipes[0][1] >= 0)
    return ::write(m_iPipes[0][1], pBuffer, uSize);
  else
    return SIZE_MAX;
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
  CCLINUXPIPE_CLOSE(m_iPipes[1][0]);
  return true;
}

void CcLinuxPipe::closePipe(int iPipeDirection, int iPipenumber)
{
  CCLINUXPIPE_CLOSE(m_iPipes[iPipeDirection][iPipenumber]);
}

void CcLinuxPipe::closeChild()
{
  CCLINUXPIPE_CLOSE(m_iPipes[0][1]);
  CCLINUXPIPE_CLOSE(m_iPipes[1][0]);
}

void CcLinuxPipe::closeParent()
{
  CCLINUXPIPE_CLOSE(m_iPipes[0][0]);
  CCLINUXPIPE_CLOSE(m_iPipes[1][1]);
  CCLINUXPIPE_CLOSE(m_iPipes[2][1]);
}
