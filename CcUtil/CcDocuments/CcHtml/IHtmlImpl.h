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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IHtmlImpl
 **/
#pragma once
#include "CcHtmlNode.h"

/**
 * @brief Basic implementation for html manipulating objects.
 */
class CcDocumentsSHARED IHtmlImpl
{
public:
  /**
   * @brief Constructor
   * @param rNode: Node to work on.
   */
  IHtmlImpl(CcHtmlNode& rNode) : m_rNode(rNode)
    {}
  /**
   * @brief Constructor
   */
  IHtmlImpl(CcHtmlNode& rNode, const CcString& sName);

  /**
   * @brief Get assigned node of this html tag
   * @return
   */
  CcHtmlNode& getNode()
  { return m_rNode; }

  /**
   * @brief Compare assigned not of this and compared object.
   * @param oToCompare: Object to compare to.
   * @return True if nodes are same.
   */
  bool operator==(const IHtmlImpl& oToCompare) const;

  /**
   * @brief Add an id to html node
   * @param sId: Id to add on node
   */
  inline void setIdAttribute(const CcString& sId)
  { m_rNode.setIdAttribute(sId); }

private:
  CcHtmlNode& m_rNode; //!< Basic node to work on.
};
