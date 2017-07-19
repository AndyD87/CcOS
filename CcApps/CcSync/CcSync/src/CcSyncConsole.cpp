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
 * @brief     Implemtation of class CcSyncConsole
 */
#include "CcSyncConsole.h"
#include "CcConsole.h"
#include "CcString.h"

ESyncRights CcSyncConsole::s_eClientRights = ESyncRights::None;

void CcSyncConsole::setClientRights(ESyncRights eRights)
{
  s_eClientRights = eRights;
}

void CcSyncConsole::writeLine(const CcString& sMessage)
{
  CcConsole::writeLine(sMessage);
}

CcString CcSyncConsole::clientQuery()
{
  switch (s_eClientRights)
  {
    case ESyncRights::Account:
      CcConsole::writeString("Account: ");
      break;
    case ESyncRights::Admin:
      CcConsole::writeString("Admin: ");
      break;
    default:
      CcConsole::writeString("(Client): ");
      break;
  }
  return CcConsole::readLine();
}

CcString CcSyncConsole::query(const CcString& sQuery)
{
  CcConsole::writeString(sQuery + " :");
  return CcConsole::readLine();
}
