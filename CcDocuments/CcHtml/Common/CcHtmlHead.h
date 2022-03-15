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
 * @brief     Class CcHtmlHead
 **/
#pragma once

#include "CcHtml/IHtmlImpl.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcHtmlHead : public IHtmlImpl
{
public:
  /**
   * @brief Constructor
   */
  CcHtmlHead(CcHtmlNode& rNode);

  //! @param sTitle: Title for head information
  void setTitle(const CcString& sTitle);
  //! @param sIcon: Icon for head information
  void setIcon(const CcString& sIcon);
  //! @param sCharset: Charset for head information
  void setCharset(const CcString& sCharset);

  /**
   * @brief Add Related canonical link.
   * @param sLink: String of link
   * @return Created node for Head
   */
  CcHtmlNode& addRelCanonical(const CcString& sLink);

  /**
   * @brief Add meta tag to head.
   * @param sName: Name of this meta tag.
   * @param sContent: Content of meta tag to set
   * @return Created node for meta
   */
  CcHtmlNode& addMeta(const CcString& sName, const CcString& sContent = "");

  /**
   * @brief Add stylesheet link.
   * @param sLink: Link to css file
   * @return Created node for style
   */
  CcHtmlNode& addStyleSheet(const CcString& sLink);

  /**
   * @brief Add script link.
   * @param sLink: Link to js file
   * @return Created node for script
   */
  CcHtmlNode& addScript(const CcString& sLink);
};
