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
 * @subpage   IHtmlImpl
 *
 * @page      IHtmlImpl
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IHtmlImpl
 **/
#ifndef H_IHtmlImpl_H_
#define H_IHtmlImpl_H_

#include "CcHtmlNode.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED IHtmlImpl
{
public:
  /**
   * @brief Constructor
   */
  IHtmlImpl(CcHtmlNode& rNode) : m_rNode(rNode)
    {}
  /**
   * @brief Constructor
   */
  IHtmlImpl(CcHtmlNode& rNode, const CcString& sName);

  CcHtmlNode& getNode()
    { return m_rNode; }

  bool operator==(const IHtmlImpl& oToCompare) const;
  inline bool operator!=(const IHtmlImpl& oToCompare) const
    { return !operator==(oToCompare); }

  inline void setIdAttribute(const CcString& sId)
    { m_rNode.setIdAttribute(sId); }

private:
  CcHtmlNode& m_rNode;
};

#endif /* H_IHtmlImpl_H_ */
