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
 * @brief     Implementation of class CcIpFrame
 */
#include "Network/CcIpFrame.h"
#include "CcStringList.h"

CcIpFrame::CcIpFrame()
{
}

CcIpFrame::~CcIpFrame()
{
}

CcIpFrame& CcIpFrame::operator=(CcIpFrame&& oToMove)
{
  if (&oToMove != this)
  {

  }
  return *this;
}

CcIpFrame& CcIpFrame::operator=(const CcIpFrame& oToCopy)
{

  return *this;
}

bool CcIpFrame::operator==(const CcIpFrame& oToCompare) const
{
  bool bRet = false;
  if (false)
  {
    bRet = true;
  }
  return bRet;
}
