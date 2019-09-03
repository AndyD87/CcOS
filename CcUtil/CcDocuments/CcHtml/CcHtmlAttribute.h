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
 * @page      CcHtml
 * @subpage   CcHtmlAttribute
 *
 * @page      CcHtmlAttribute
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlAttribute
 */
#ifndef H_CcHtmlAttribute_H_
#define H_CcHtmlAttribute_H_

#include "CcHtml.h"
#include "CcString.h"

/**
 * @brief Example Class implementation
 */
class CcDocumentsSHARED CcHtmlAttribute
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlAttribute();

  CcHtmlAttribute(const CcString& sName, const CcString& sValue = "");

  CcString &getName()
    { return m_sName; }
  CcString &getValue()
    { return m_sValue; }
  const CcString &getName() const
    { return m_sName; }
  const CcString &getValue() const
    { return m_sValue; }
  void setName (const CcString& sName )
    { m_sName  =sName; }
  void setValue(const CcString& sValue)
    { m_sValue =sValue; }

  bool operator==(const CcHtmlAttribute& oToCompare) const;
  inline bool operator!=(const CcHtmlAttribute& oToCompare) const
    { return !operator==(oToCompare); }

  inline bool isNull()
    { return this == nullptr; }
  inline bool isNotNull()
    { return this != nullptr; }

private:
  CcString m_sName;
  CcString m_sValue;
};

#endif /* H_CcHtmlAttribute_H_ */
