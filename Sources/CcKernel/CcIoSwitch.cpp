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
 * @brief     Implementation of Class CcIoSwitch
 */

#include "CcIoSwitch.h"

size_t CcIoSwitch::read(void* pBuffer, size_t uSize)
{
  size_t iRet = SIZE_MAX;
  if (m_pReadStream)
  {
    iRet = m_pReadStream->read(pBuffer, uSize);
  }
  return iRet;
}

size_t CcIoSwitch::write(const void* pBuffer, size_t uSize)
{
  size_t iRet = SIZE_MAX;
  if (m_pWriteStream)
  {
    iRet = m_pWriteStream->write(pBuffer, uSize);
  }
  return iRet;
}

CcStatus CcIoSwitch::open(EOpenFlags flags)
{
  CcStatus oStatus;
  if (m_pReadStream)
  {
    oStatus = m_pReadStream->open(flags);
  }
  if (oStatus && m_pWriteStream)
  {
    oStatus = m_pWriteStream->open(flags);
  }
  return oStatus;
}

CcStatus CcIoSwitch::cancel()
{
  CcStatus oStatus;
  if (m_pReadStream)
  {
    oStatus = m_pReadStream->cancel();
  }
  if (oStatus && m_pWriteStream)
  {
    oStatus = m_pWriteStream->cancel();
  }
  return oStatus;
}

CcStatus CcIoSwitch::close()
{
  CcStatus oStatus;
  if (m_pReadStream)
  {
    oStatus = m_pReadStream->close();
  }
  if (oStatus && m_pWriteStream)
  {
    oStatus = m_pWriteStream->close();
  }
  return oStatus;
}
