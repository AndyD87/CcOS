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
 * @brief     Implemtation of class CcArguments
 */
#include "CcArguments.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"
#include "IIo.h"

CcArguments::CcArguments(int argc, char **argv)
{
  parse(argc, argv);
}

CcArguments::CcArguments(const CcString& sArgline)
{
  parse(sArgline);
}

CcArguments::CcArguments(const CcArguments& oToCopy) :
  CcStringList(oToCopy)
{
  operator=(oToCopy);
}

CcArguments::CcArguments(CcArguments&& oToMove) :
  CcStringList(CCMOVE(oToMove))
{
  operator=(CCMOVE(oToMove));
}

CcArguments& CcArguments::operator=(CcArguments&& oToMove)
{
  if(this != &oToMove)
  {
    CcStringList::operator=(CCMOVE(oToMove));
    m_oVariables = CCMOVE(oToMove.m_oVariables);
  }
  return *this;
}

CcArguments& CcArguments::operator=(const CcArguments& oToCopy)
{
  CcStringList::operator=(oToCopy);
  m_oVariables = oToCopy.m_oVariables;
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
  return at(uiIndex);
}

bool CcArguments::parse(int argc, char **argv)
{
  for (int i = 0; i < argc; i++)
  {
    append(argv[i]);
  }
  return parse();
}

CcString CcArguments::getPath() const
{
  CcString sPath;
  if (size() > 0)
  {
    sPath.setOsPath(at(0));
  }
  return sPath;
}

CcString CcArguments::getApplication() const
{
  CcString sPath;
  if (size() > 0)
  {
    sPath.setOsPath(at(0));
    sPath = CcStringUtil::getFilenameFromPath(sPath);
  }
  return sPath;
}

CcString CcArguments::getDirectory() const
{
  CcString sPath;
  if (size() > 0)
  {
    sPath.setOsPath(at(0));
    sPath = CcStringUtil::getDirectoryFromPath(sPath);
  }
  return sPath;
}

CcString CcArguments::getLine() const
{
  CcString sLine;
  bool bFirst = true;
  for (const CcString& sValue : *this)
  {
    if (!bFirst) sLine << CcGlobalStrings::Seperators::Space;
    else         bFirst = false;
    if (sValue.contains(' '))
    {
      sLine << CcGlobalStrings::Seperators::Quote << sValue << CcGlobalStrings::Seperators::Quote;
    }
    else
    {
      sLine << sValue;
    }
  }
  return sLine;
}

bool CcArguments::parse(const CcString& sLine)
{
  const uint8 _STATE_NO_STATE = 0;
  const uint8 _STATE_IN_QUOTE = 1;
  m_eValidity = true;
  clear();
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
          append(sCurrentArgument);
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
        append(sCurrentArgument);
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
    append(sCurrentArgument);
  }
  return parse();
}

bool CcArguments::contains(const CcString& sKey)
{
  for (const CcStringVariantPair& sArgument : m_oVariablesParsed)
  {
    if (sArgument.getKey().compareInsensitve(sKey))
    {
      return true;
    }
  }
  return false;
}

void CcArguments::printHelp(IIo& oOutput)
{
  oOutput.writeString("Arguments");
  for (const CVariableDefinition& sArgument : m_oVariables)
  {
    oOutput.writeString("  ");
    oOutput.writeString(sArgument.sName);
    oOutput.writeString(CcGlobalStrings::EolOs);
    oOutput.writeString("    ");
    oOutput.writeString(sArgument.sDescription);
    oOutput.writeString(CcGlobalStrings::EolOs);
  }
}

CcVariant::EType CcArguments::getType(const CcString& sName)
{
  CcVariant::EType eType = CcVariant::EType::NoType;
  for (CVariableDefinition& oDef : m_oVariables)
  {
    if (sName == oDef.sName)
    {
      eType = oDef.eType;
      break;
    }
  }
  return eType;
}

bool CcArguments::parse()
{
  for (size_t uiPos = 0; m_eValidity && uiPos < size(); uiPos++)
  {
    CcString sLowerValue(at(uiPos));
    sLowerValue.toLower();
    CcVariant::EType eType = getType(sLowerValue);
    if (eType != CcVariant::EType::NoType &&
        uiPos+1 < size())
    {
      uiPos++;
      CcVariant oValue(at(uiPos));
      m_eValidity = oValue.convert(eType);
      if (m_eValidity)
      {
        if (m_oVariablesParsed.containsKey(sLowerValue))
        {
          m_oVariablesParsed[sLowerValue] = oValue;
        }
        else
        {
          m_oVariablesParsed.append(sLowerValue, oValue);
        }
      }
    }
    else if (eType == CcVariant::EType::NoType)
    {
      m_oUnparsed.append(at(uiPos));
    }
    else
    {
      // Missing value, we are at the end
      m_eValidity = false;
    }
  }
  return m_eValidity;
}
