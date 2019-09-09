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
 * @page      Types
 * @subpage   CcArguments
 *
 * @page      CcArguments
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcArguments
 **/
#ifndef H_CcExampleClassAllOp_H_
#define H_CcExampleClassAllOp_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcStringList.h"
#include "CcMapCommon.h"

/**
 * @brief Class for parsing and working with arguments from a commandline
 */
class CcKernelSHARED CcArguments : public CcStringList
{
public:
  /**
   * @brief Constructor
   */
  CcArguments() = default;

  /**
   * @brief Cunstructor with arguments coming from a typically main routine
   * @param argc: Number of Arguments stored in argv
   * @param argv: Arguments in a array of char strings.
   */
  CcArguments(int argc, char **argv);

  /**
   * @brief Constructor with argument line to parse initialy
   * @param sArgline: Line to parse for arguments
   */
  CcArguments(const CcString& sArgline);

  /**
   * @brief CopyConstructor
   */
  CcArguments( const CcArguments& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcArguments( CcArguments&& oToMove );

  /**
   * @brief Destructor
   */
  ~CcArguments() = default;

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
   * @brief Initialize Arguments with arguments in a format like main will get.
   * @param argc: Number of Arguments stored in argv
   * @param argv: Arguments in a array of char strings.
   */
  void init(int argc, char **argv);

  /**
   * @brief Parse a line with arguments
   * @param sLine: Line to parse
   */
  void parseLine(const CcString& sLine);

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

  /**
   * @brief Get all variables and their values wich was parsed
   * @return Paires of values and variables as Map
   */
  const CcStringMap& getVarList() const
    { return m_oVariables; }

private:
  CcStringMap  m_oVariables;
  CcString     m_sOperators;
};

#endif // H_CcExampleClassAllOp_H_
