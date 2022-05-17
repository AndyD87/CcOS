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
 * @brief     Class CcHtmlAttribute
 */
#pragma once

#include "CcHtml.h"
#include "CcMapCommon.h"

/**
 * @brief Example Class implementation
 */
class CcDocumentsSHARED CcHtmlAttribute : public CcStringPair
{
public:
  CcHtmlAttribute() = default;
  ~CcHtmlAttribute() = default;

  /**
   * @brief Create attribute with name and value
   * @param sName:  Name of attribute
   * @param sValue: Value of this attribute
   */
  CcHtmlAttribute(const CcString& sName, const CcString& sValue = "");

  //! @return Get name of attribute
  CcString &getName()
  { return getKey(); }
  //! @return Get name of attribute
  const CcString& getName() const
  { return getKey(); }
  //! @param sName: Set name of attribute
  void setName (const CcString& sName )
  { setKey(sName); }

  //! @return True if this attribute is null attribute
  inline bool isNull() const
  { return &c_rNullattribute == this; }
  //! @return True if this attribute is not null attribute
  inline bool isNotNull() const
  { return &c_rNullattribute != this; }

private:
  static const CcHtmlAttribute& c_rNullattribute;
};
