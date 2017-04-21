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
 * @subpage   CcStringList
 *
 * @page      CcStringList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcStringList
 **/
#ifndef CCSTRINGLIST_H_
#define CCSTRINGLIST_H_

#include "CcKernelBase.h"
#include "CcString.h"
#include "CcList.h"

#ifdef WIN32
template class CcKernelSHARED CcList<CcString>;
#endif

/**
 * @brief Manage a List of Strings
 */
class CcKernelSHARED  CcStringList : public CcList<CcString>
{
public:
  /**
   * @brief Constructor
   */
  CcStringList();

  /**
  * @brief Copy-Constructor
  */
  CcStringList(const CcStringList& toAssign);

  /**
   * @brief Destructor
   */
  virtual ~CcStringList();

  /**
   * @brief Parse an Commandline input into the Stringlist and return the argument
   * @param Line: Line to parse;
   * @return First argument in line
   */
  CcString parseArguments(const CcString& Line);

  /**
   * @brief Create and
   * @param seperator
   * @return
   */
  CcString collapseList(const CcString& seperator) const;

  /**
   * @brief Find Position of String in List
   * @param toFind: String to find
   * @param offset: Start Position for searching in List
   * @return position of String or SIZE_MAX if not found
   */
  size_t findString(const CcString& toFind, size_t offset = 0) const;
};

#endif /* CCSTRINGLIST_H_ */
