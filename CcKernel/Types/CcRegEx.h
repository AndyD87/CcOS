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
 * @page      CcRegEx
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcRegEx
 *
 * @todo std::regex support does not work on every platform, write an own
 */
#ifndef CcRegEx_H_
#define CcRegEx_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

/**
 * @brief Regular Expression Class
 */
class CcKernelSHARED  CcRegEx {
public:
  /**
   * @brief Constructor
   */
  CcRegEx(void);

  /**
  * @brief Create an Regular Expression Class with ExpressionString
   * @param String: String Regular Expression has to work with
  */
  CcRegEx(const CcString& String);

  /**
   * @brief Destructor
   */
  virtual ~CcRegEx( void );

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

#endif /* CcRegEx_H_ */
