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
 * @brief     Implemtation of class CcArguments
 */
#include "CcArguments.h"

CcArguments::CcArguments( void )
{
}

CcArguments::CcArguments(int argc, char **argv)
{
  for (int i = 0; i < argc; i++)
  {
    m_oArguments.append(argv[i]);
  }
}

CcArguments::CcArguments(const CcString& sArgline)
{
  parseLine(sArgline);
}

CcArguments::CcArguments( const CcArguments& oToCopy )
{
  operator=(oToCopy);
}

CcArguments::CcArguments( CcArguments&& oToMove )
{
  operator=(std::move(oToMove));
}

CcArguments::~CcArguments( void )
{
}

CcArguments& CcArguments::operator=(const CcArguments& oToCopy)
{
  m_oArguments = oToCopy.m_oArguments;
  m_oVariables = oToCopy.m_oVariables;
  m_sOperators = oToCopy.m_sOperators;
  return *this;
}

CcArguments& CcArguments::operator=(CcArguments&& oToMove)
{
  if(this != &oToMove)
  {
    m_oArguments = std::move(oToMove.m_oArguments);
    m_oVariables = std::move(oToMove.m_oVariables);
    m_sOperators = std::move(oToMove.m_sOperators);
  }
  return *this;
}

bool CcArguments::operator==(const CcArguments& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcArguments::operator!=(const CcArguments& oToCompare) const
{
  return !operator==(oToCompare);
}

const CcString& CcArguments::operator[](size_t uiIndex) const
{
  return m_oArguments.at(uiIndex);
}

void CcArguments::parseLine(const CcString& sLine)
{
  m_oArguments = sLine.split(" ");
}

bool CcArguments::contains(const CcString& sKey)
{
  for (const CcString& sArgument : m_oArguments)
  {
    if (sArgument == sKey)
    {
      return true;
    }
  }
  return false;
}
