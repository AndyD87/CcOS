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
 * @brief     Implemtation of class CcStatus
 */
#include "CcStatus.h"

CcStatus& CcStatus::operator=(const CcStatus& oError)
{
  m_eError = oError.m_eError;
  return *this;
}

CcStatus& CcStatus::operator=(EStatus eError)
{
  m_eError = eError;
  return *this;
}

CcStatus& CcStatus::operator=(bool bTrueFalse)
{
  if (bTrueFalse)
    m_eError = EStatus::AllOk;
  else
    m_eError = EStatus::Error;
  return *this;
}

CcStatus& CcStatus::operator=(int iErrorCode)
{
  m_eError = static_cast<EStatus>(iErrorCode);
  return *this;
}

void operator&=(bool& bLeft, const CcStatus& oStatus )
{
  bLeft = bLeft & (oStatus.getError() == EStatus::AllOk);
}

void operator&=(int& iLeft, const CcStatus& oStatus )
{
  iLeft = iLeft & static_cast<int>(oStatus.getError());
}

