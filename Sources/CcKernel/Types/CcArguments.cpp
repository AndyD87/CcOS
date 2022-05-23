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
#include "../CcLog.h"


const CcString& CcArguments::CVariableDefinition::getName() const
{
  if (oNames.size())
  {
    return oNames[0];
  }
  else return CcGlobalStrings::Empty;
}

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

void CcArguments::writeHelp(IIo& oOutput)
{
  oOutput.writeString("Commands");
  oOutput.writeString(CcGlobalStrings::EolOs);
  for (const CVariableDefinition& sArgument : m_oVariables)
  {
    oOutput.writeString(" ");
    oOutput.writeString(sArgument.getName());
    oOutput.writeString(CcGlobalStrings::EolOs);
    oOutput.writeString("  ");
    oOutput.writeString(sArgument.sDescription);
    oOutput.writeString(CcGlobalStrings::EolOs);
    if (sArgument.oRequired.size())
    {
      oOutput.writeString("  Required Parameters:");
      oOutput.writeString(CcGlobalStrings::EolOs);
      for (const CVariableDefinition& oParam : sArgument.oRequired)
      {
        oOutput.writeString("   ");
        oOutput.writeString(oParam.getName());
        oOutput.writeString(CcGlobalStrings::EolOs);
        oOutput.writeString("    ");
        oOutput.writeString(oParam.sDescription);
        oOutput.writeString(CcGlobalStrings::EolOs);
      }
    }
    if (sArgument.oRequired.size())
    {
      oOutput.writeString("  Optional Parameters:");
      oOutput.writeString(CcGlobalStrings::EolOs);
      for (const CVariableDefinition& oParam : sArgument.oRequired)
      {
        oOutput.writeString("   ");
        oOutput.writeString(oParam.getName());
        oOutput.writeString(CcGlobalStrings::EolOs);
        oOutput.writeString("    ");
        oOutput.writeString(oParam.sDescription);
        oOutput.writeString(CcGlobalStrings::EolOs);
      }
    }
    oOutput.writeString(CcGlobalStrings::EolOs);
  }
}

CcArguments::CVariableDefinition* CcArguments::findVariableDefinition(CVariableDefinitionList& oActiveList, const CcString& sName)
{
  CVariableDefinition* pDef = nullptr;
  for (CVariableDefinition& oDef : oActiveList)
  {
    if (sName == oDef.getName())
    {
      pDef = &oDef;
      break;
    }
  }
  return pDef;
}

bool CcArguments::parse()
{
  size_t uiPos = 0;
  m_oUnparsed = *this;
  return parse(m_oVariables, uiPos);
}

bool CcArguments::parse(CVariableDefinitionList& oActiveList, size_t uiPos)
{
  while (m_eValidity && uiPos < m_oUnparsed.size())
  {
    CcString sLowerValue(m_oUnparsed.at(uiPos));
    sLowerValue.toLower();
    CVariableDefinition* pDef = findVariableDefinition(oActiveList, sLowerValue);
    if (pDef &&
        pDef->eType == CcVariant::EType::Switch)
    {
      m_oUnparsed.remove(uiPos);
      pDef->uiCounter++;
      CcVariant oValue(true);
      m_oVariablesParsed.append(sLowerValue, oValue);
    }
    else if (pDef &&
        pDef->eType != CcVariant::EType::NoType &&
        uiPos+1 < size())
    {
      m_oUnparsed.remove(uiPos);
      CcVariant oValue(m_oUnparsed.at(uiPos));
      bool bSuccess = oValue.convert(pDef->eType);
      if (bSuccess)
      {
        m_oUnparsed.remove(uiPos);
        pDef->uiCounter++;
        if (m_oVariablesParsed.containsKey(sLowerValue))
        {
          m_oVariablesParsed[sLowerValue] = oValue;
        }
        else
        {
          m_oVariablesParsed.append(sLowerValue, oValue);
        }
        if (pDef->oRequired.size() > 0)
        {
          parse(pDef->oRequired, uiPos);
          for (CVariableDefinition& oDef : pDef->oRequired)
          {
            if (oDef.uiCounter == 0)
            {
              // Missing value, we are at the end
              m_eValidity = false;
              m_sErrorMessage = CcLog::formatErrorMessage("Required parameter missing: " + oDef.getName());
              break;
            }
          }
        }
        if (pDef->oOptional.size() > 0)
          parse(pDef->oOptional, uiPos);
      }
      else
      {
        // Missing value, we are at the end
        m_eValidity = false;
      }
    }
    else if (pDef ==  nullptr ||
             (pDef && pDef->eType == CcVariant::EType::NoType))
    {
      uiPos++;
    }
    else if(pDef)
    {
      // Missing value, we are at the end
      m_sErrorMessage = CcLog::formatErrorMessage("No value defined for: " + pDef->getName());
      m_eValidity = false;
    }
  }
  return m_eValidity;
}

void CcArguments::clear()
{
  m_sErrorMessage.clear();
  CcList<CcString>::clear();
  clear(m_oVariables);
}

void CcArguments::clear(CVariableDefinitionList& oActiveList)
{
  for (CVariableDefinition& oDef : oActiveList)
  {
    oDef.uiCounter = 0;
    clear(oDef.oRequired);
    clear(oDef.oOptional);
  }
}
