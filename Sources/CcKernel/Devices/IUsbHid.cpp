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
 * @brief     Create Access to an USB-Hid-Device
 */

#include "IUsbHid.h"

IUsbHid::IUsbHid()
{
}

IUsbHid::~IUsbHid()
{
}

bool IUsbHid::setDevice(uint32 iVid, uint32 iPid, uint32 iUsbUsage )
{
  bool bRet(false);
  m_Info.vid = iVid;
  m_Info.pid = iPid;
  m_Info.usage = iUsbUsage;
  bRet = connect();
  return bRet;
}

size_t IUsbHid::getReportInputSize()
{
  return m_Info.m_uiReportInputSize;
}

size_t IUsbHid::getReportOutputSize()
{
  return m_Info.m_uiReportOutputSize;
}
