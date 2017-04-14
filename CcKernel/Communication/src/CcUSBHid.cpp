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
 * @file      CcUSBHid
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Create Access to an USB-Hid-Device
 */

#include "Communication/CcUSBHid.h"

CcUSBHid::CcUSBHid( void )
{
}

CcUSBHid::~CcUSBHid( void )
{
}

bool CcUSBHid::setDevice( uint32 vid, uint32 pid, uint32 usage )
{
  bool bRet(false);
  m_Info.vid = vid;
  m_Info.pid = pid;
  m_Info.usage = usage;
  bRet = connect();
  return bRet;
}

size_t CcUSBHid::getReportInputSize(void)
{
  return m_Info.m_uiReportInputSize;
}

size_t CcUSBHid::getReportOutputSize(void)
{
  return m_Info.m_uiReportOutputSize;
}
