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
#ifndef _CcExampleClassAllOp_H_
#define _CcExampleClassAllOp_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcStringList.h"
#include "CcMapCommon.h"

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcArguments {
public:
  /**
   * @brief Constructor
   */
  CcArguments( void );

  /**
   * @brief Constructor
   */
  CcArguments(int argc, char **argv);

  /**
   * @brief Constructor
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
  ~CcArguments( void );

  void parseLine(const CcString& sLine);

  CcArguments& operator=(CcArguments&& oToMove);
  CcArguments& operator=(const CcArguments& oToCopy);
  bool operator==(const CcArguments& oToCompare) const;
  bool operator!=(const CcArguments& oToCompare) const;
  const CcString& operator[](size_t uiIndex) const;

  CcString getPath() const;
  CcString getApplication() const;
  CcString getDirectory() const;

  bool contains(const CcString& sKey);
  size_t size() const
    { return m_oArguments.size(); }

  const CcStringMap& getVarList() const
    { return m_oVariables; }
  const CcStringList& getArgumentList() const
    { return m_oArguments; }

private:
  CcStringList m_oArguments;
  CcStringMap  m_oVariables;
  CcString     m_sOperators;
};

#endif /* _CcExampleClassAllOp_H_ */
