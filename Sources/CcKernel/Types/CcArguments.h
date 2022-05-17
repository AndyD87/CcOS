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
 * @brief     Class CcArguments
 **/
#pragma once

#include "CcBase.h"
#include "CcList.h"
#include "CcStringList.h"
#include "CcMapCommon.h"

/**
 * @brief Class for parsing and working with arguments from a commandline
 */
class CcKernelSHARED CcArguments : public CcStringList
{
public:
  class CcKernelSHARED CVariableDefinition
  {
  public:
    CVariableDefinition(CcStringList oNames = {""},
                        CcVariant::EType eType = CcVariant::EType::NoType, 
                        const CcString& sDefault = "", 
                        const CcString& sDescription = "", 
                        CcList<CVariableDefinition> oRequiredParameters={},
                        CcList<CVariableDefinition> oOptionalParameters={}
    ) :
      oNames(oNames),
      eType(eType),
      sDefault(sDefault),
      sDescription(sDescription),
      oRequired(oRequiredParameters),
      oOptional(oOptionalParameters)
    {}

    bool operator==(const CVariableDefinition& toCompare) const
    {
      return oNames == toCompare.oNames && eType == toCompare.eType &&
        sDefault == toCompare.sDefault && sDescription == toCompare.sDescription;
    }

    const CcString& getName() const;

    typedef class CcKernelSHARED CcList<CVariableDefinition> CList;

    CcStringList      oNames;
    CcVariant::EType  eType;
    CcString          sDefault;
    CcString          sDescription;
    CList             oRequired;
    CList             oOptional;
    size_t            uiCounter = 0;
  };

  typedef class CcKernelSHARED CcList<CVariableDefinition> CVariableDefinitionList;

  CcArguments() = default;
  ~CcArguments() = default;

  /**
   * @brief Cunstructor with arguments coming from a typically main routine
   * @param iArgc:  Number of Arguments stored in argv
   * @param ppArgv: Arguments in a array of char strings.
   */
  CcArguments(int iArgc, char **ppArgv);

  /**
   * @brief Constructor with argument line to parse initialy
   * @param sArgline: Line to parse for arguments
   */
  CcArguments(const CcString& sArgline);

  /**
   * @brief Constructor with argument line to parse initialy
   * @param sArgline: Line to parse for arguments
   */
  CcArguments(const CVariableDefinitionList& oVariables)
  { setVariablesList(oVariables); }

  /**
   * @brief CopyConstructor
   * @param oToCopy: Object to copy
   */
  CcArguments( const CcArguments& oToCopy );

  /**
   * @brief MoveConstructor
   * @param oToMove: Object to move
   */
  CcArguments( CcArguments&& oToMove );

  /**
   * @brief Move content from another Object
   */
  CcArguments& operator=(CcArguments&& oToMove);

  /**
   * @brief Copy content from another Object
   */
  CcArguments& operator=(const CcArguments& oToCopy);

  /**
   * @brief Compare if contents of this Arguments object are the same
   *        as the object to compare to.
   * @param oToCompare: Object to compare to
   * @return true if same
   */
  bool operator==(const CcArguments& oToCompare) const;

  /**
   * @brief Compare if contents of this Arguments object are not the same
   *        as the object to compare to.
   * @param oToCompare: Object to compare to
   * @return true if not same
   */
  bool operator!=(const CcArguments& oToCompare) const;

  /**
   * @brief Get Argument on a specific location
   * @param uiIndex: Number of argument in list.
   * @return Location at given Index, or nullreference if uiIndex is not available.
   */
  const CcString& operator[](size_t uiIndex) const;

  /**
   * @brief Parse Arguments with arguments in a format like main will get.
   * @param argc: Number of Arguments stored in argv
   * @param argv: Arguments in a array of char strings.
   */
  bool parse(int argc, char **argv);

  /**
   * @brief Parse a line with arguments
   * @param sLine: Line to parse
   */
  bool parse(const CcString& sLine);

  /**
   * @brief Get Arguments in a line
   * @return Collaps all arguments quoted in a line
   */
  CcString getLine() const;

  /**
   * @brief Get Path to Binary if arguments came from a typical main
   * @return Path as String or empty String if not found
   */
  CcString getPath() const;

  /**
   * @brief Get application name from getPath();
   * @return Name as String or empty String if not found
   */
  CcString getApplication() const;

  /**
   * @brief Get last directory from getPath();
   * @return Path as String or empty String if not found
   */
  CcString getDirectory() const;

  /**
   * @brief Check if sKey is part of arguments in list.
   * @param sKey: Name to search for
   * @return true if found
   */
  bool contains(const CcString& sKey);

  void writeHelp(IIo& oOutput);

  /**
   * @brief Get all variables the parser is using.
   * @return Paires of values and variables as Map
   */
  const CVariableDefinitionList& getVariablesList() const
  { return m_oVariables; }

  //! @return Get last error message from last parse execution.
  const CcString& getErrorMessage() const
  { return m_sErrorMessage; }

  //! @param oVariables: List of varibles the parser should look for.
  void setVariablesList(const CVariableDefinitionList& oVariables)
  { m_oVariables = oVariables; }

  /**
   * @brief Get all variables and their values wich was parsed
   * @return Paires of values and variables as Map
   */
  const CcVariantMap& getSetVariables() const
  { return m_oVariablesParsed; }

  //! @return All unused/undefined variables from commandline
  const CcStringList& getUnparsed() const
  { return m_oUnparsed; }

  const CcVariant& getValue(const CcString& sKey)
  { return m_oVariablesParsed.getValue(sKey); }

  //! @return Get last parsing validation information.
  CcStatus isValid() const
  { return m_eValidity; }
  
private:
  //! @return Get Type of variable if found, otherwise CcVariant::EType::NoType.
  static CcArguments::CVariableDefinition* findVariableDefinition(CVariableDefinitionList& oActiveList, const CcString& sName);
  bool parse();
  bool parse(CVariableDefinitionList& oActiveList, size_t uiPos);
  void clear();
  void clear(CVariableDefinitionList& oActiveList);

private:
  CcStatus                  m_eValidity;
  CVariableDefinitionList   m_oVariables;
  CcVariantMap              m_oVariablesParsed;
  CcStringList              m_oUnparsed;
  CcString                  m_sErrorMessage;
};

template class CcList<CcArguments::CVariableDefinition>;