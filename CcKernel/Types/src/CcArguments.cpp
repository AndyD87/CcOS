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
#include "CcStringUtil.h"

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

CcArguments& CcArguments::operator=(const CcArguments& oToCopy)
{
  m_oArguments = oToCopy.m_oArguments;
  m_oVariables = oToCopy.m_oVariables;
  m_sOperators = oToCopy.m_sOperators;
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

CcString CcArguments::getPath() const
{
  CcString sPath;
  if (m_oArguments.size() > 0)
  {
    sPath.setOsPath(m_oArguments[0]);
  }
  return sPath;
}

CcString CcArguments::getApplication() const
{
  CcString sPath;
  if (m_oArguments.size() > 0)
  {
    sPath.setOsPath(m_oArguments[0]);
    sPath = CcStringUtil::getFilenameFromPath(sPath);
  }
  return sPath;
}

CcString CcArguments::getDirectory() const
{
  CcString sPath;
  if (m_oArguments.size() > 0)
  {
    sPath.setOsPath(m_oArguments[0]);
    sPath = CcStringUtil::getDirectoryFromPath(sPath);
  }
  return sPath;
}

void CcArguments::parseLine(const CcString& sLine)
{
  const uint8 _STATE_NO_STATE = 0;
  const uint8 _STATE_IN_QUOTE = 1;
  m_oArguments.clear();
  CcString sCurrentArgument;
  uint8 uiState = _STATE_NO_STATE;
  for(size_t i = 0; i < sLine.length(); i++)
  {
    if (uiState == _STATE_NO_STATE)
    {
      if (CcStringUtil::isWhiteSpace(sLine[i]))
      {
        if (sCurrentArgument.length() > 0)
        {
          m_oArguments.append(sCurrentArgument);
          sCurrentArgument.clear();
        }
      }
      else if (sLine[i] == '\\')
      {
        if (sLine.length() > i + 1)
        {
          i++;
          sCurrentArgument.append(sLine[i]);
        }
      }
      else if (sLine[i] == '"')
      {
        uiState = _STATE_IN_QUOTE;
      }
      else
      {
        sCurrentArgument.append(sLine[i]);
      }
    }
    else if (uiState == _STATE_IN_QUOTE)
    {
      if (sLine[i] == '\\')
      {
        if (sLine.length() > i + 1)
        {
          i++;
          sCurrentArgument.append(sLine[i]);
        }
      }
      else if (sLine[i] == '"')
      {
        m_oArguments.append(sCurrentArgument);
        sCurrentArgument.clear();
        uiState = _STATE_NO_STATE;
      }
      else
      {
        sCurrentArgument.append(sLine[i]);
      }
    }
  }
  if (sCurrentArgument.length() > 0)
  {
    m_oArguments.append(sCurrentArgument);
  }
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
