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
 * @page      CcUtil
 * @subpage   CcHtmlRoot
 *
 * @page      CcHtmlRoot
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHtmlRoot
 **/
#ifndef H_CcHtmlRoot_H_
#define H_CcHtmlRoot_H_

#include "CcHtml.h"
#include "CcHtmlNode.h"
#include "CcHtmlHead.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcHtmlRoot : public CcHtmlNode
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlRoot();

  /**
   * @brief Destructor
   */
  virtual ~CcHtmlRoot();

  CcHtmlHead& getHeader()
    { return m_oHeader; }
  CcHtmlNode& getBody()
    { return m_oBody; }
  void setLanguage(const CcString& sLanguage);

private:
  CcHtmlNode m_oDocType;
  CcHtmlHead m_oHeader;
  CcHtmlNode m_oBody;
};

#endif /* H_CcHtmlRoot_H_ */
