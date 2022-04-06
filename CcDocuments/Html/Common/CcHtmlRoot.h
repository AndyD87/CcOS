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
 * @brief     Class CcHtmlRoot
 **/
#pragma once

#include "Html/CcHtml.h"
#include "Html/IHtmlImpl.h"
#include "CcHtmlHead.h"

/**
 * @brief Root node of html document
 */
class CcDocumentsSHARED CcHtmlRoot : public IHtmlImpl
{
public:
  /**
   * @brief Create object on existing html node
   * @param rNode: Node to convert
   */
  CcHtmlRoot(CcHtmlNode& rNode);
  virtual ~CcHtmlRoot();

  //! @return Get header node
  CcHtmlHead& getHeader()
  { return m_oHeader; }
  //! @return Get body node
  CcHtmlNode& getBody()
  { return m_oBody; }

  //! @param sLanguage: Set doctype with language
  void setLanguage(const CcString& sLanguage);

private:
  CcHtmlNode m_oDocType;
  CcHtmlHead m_oHeader;
  CcHtmlNode m_oBody;
};
