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
 * @subpage   CcRegEx
 *
 * @page      CcRegEx
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRegEx
 *
 * @todo std::regex support does not work on every platform, write an own
 */
#ifndef H_CcRegEx_H_
#define H_CcRegEx_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

/**
 * @brief Regular Expression Class
 */
class CcKernelSHARED  CcRegEx
{
public:
  /**
   * @brief Constructor
   */
  CcRegEx() = default;

  /**
   * @brief Create an Regular Expression Class with ExpressionString
   * @param String: String Regular Expression has to work with
   */
  CcRegEx(const CcString& String);

  /**
   * @brief Destructor
   */
  ~CcRegEx() = default;

  /**
   * @brief Set the Regular Expression-String
   * @param String: String Regular Expression has to work with
   */
  void setRegEx(const CcString& String);

  /**
   * @brief Check if Regular Expression matches a String
   * @param toCompare: String to execute regulare Expression on
   * @return true if regular Expression succeeded.
   */
  bool pregMatch(const CcString& toCompare);

private:
  CcString m_RegEx; //!< Regular-Expresstion String to work with.
};

#endif // H_CcRegEx_H_
