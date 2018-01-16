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
#ifndef _CcHtmlAttribute_H_
#define _CcHtmlAttribute_H_

#include "CcBase.h"
#include "CcHtml.h"
#include "CcString.h"
#include "CcHtmlTypes.h"

/**
 * @brief Example Class impelmentation
 */
class CcDocumentsSHARED CcHtmlAttribute
{
  //! Known types
  typedef enum{
    eUnknown = 0,
    eName,
    eId,
    eClass,
  }CcHtmlAttributeType;
public:
  /**
   * @brief Constructor
   */
  CcHtmlAttribute( void );

  /**
   * @brief Destructor
   */
  ~CcHtmlAttribute( void );
  CcString &getName()
    { return m_sName; }
  CcString &getValue()
    { return m_sValue; }
  void setName (const CcString& sName )
    { m_sName  =sName; }
  void setValue(const CcString& sValue)
    { m_sValue =sValue; }

private:
  CcString m_sName;
  CcString m_sValue;
};

#endif /* _CcHtmlAttribute_H_ */
