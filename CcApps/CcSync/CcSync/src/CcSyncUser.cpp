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
* @brief     Implemtation of class CcSyncUserClientCommand
*/
#include "CcSyncUser.h"

CcSyncUser::~CcSyncUser(void)
{
}

CcSyncUser& CcSyncUser::operator=(const CcSyncUser& oToCopy)
{
  m_sToken          = oToCopy.m_sToken        ;
  m_pClientConfig   = oToCopy.m_pClientConfig ;
  m_pAccountConfig  = oToCopy.m_pAccountConfig;
  m_pDatabase       = oToCopy.m_pDatabase     ;
  m_bIsValid        = oToCopy.m_bIsValid      ;
  return *this;
}

CcSyncUser& CcSyncUser::operator=(CcSyncUser&& oToMove)
{
  m_sToken          = std::move(oToMove.m_sToken        );
  m_pClientConfig   = std::move(oToMove.m_pClientConfig );
  m_pAccountConfig  = std::move(oToMove.m_pAccountConfig);
  m_pDatabase       = std::move(oToMove.m_pDatabase     );
  m_bIsValid        = std::move(oToMove.m_bIsValid      );
  return *this;
}
