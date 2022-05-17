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
 * @brief     Implemtation of class CcDhcpLeaseList
 */
#include "CcDhcpLeaseList.h"

CcDhcpLeaseList::CcDhcpLeaseList()
{
}

CcDhcpLeaseList::~CcDhcpLeaseList()
{
}

size_t CcDhcpLeaseList::findMacAddress(const CcMacAddress& oMacAddres) const
{
  size_t uiRet = SIZE_MAX;
  size_t uiPos = 0;
  for (const CcDhcpLeaseItem& oLease : *this)
  {
    if (oLease.getMac() == oMacAddres)
    {
      uiRet = uiPos;
      break;
    }
    uiPos++;
  }
  return uiRet;
}

size_t CcDhcpLeaseList::findIpAddress(const CcIp& oIpAddres) const
{
  size_t uiPos = 0;
  for (const CcDhcpLeaseItem& oLease : *this)
  {
    if (oLease.getIp() == oIpAddres)
    {
      break;
    }
    uiPos++;
  }
  uiPos = SIZE_MAX;
  return uiPos;
}

CcIp CcDhcpLeaseList::getNextFree(const CcIp& oBegin, const CcIp& oEnd)
{
  CcIp oIp;
  if (size() > 0)
  {
    oIp = last().getIp();
    oIp++;
    if (oIp == oEnd)
    {
      oIp = oBegin;
    }
  }
  else
  {
    oIp = oBegin;
  }
  return oIp;
}