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
  * @subpage   CcHtmlNodeList
  *
  * @page      CcHtmlNodeList
  * @copyright Andreas Dirmeier (C) 2017
  * @author    Andreas Dirmeier
  * @par       Web:      https://coolcow.de/projects/CcOS
  * @par       Language: C++11
  * @brief     Class CcHtmlNodeList
  */
#ifndef H_CcHtmlNodeList_H_
#define H_CcHtmlNodeList_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcMacros.h"
#include "CcList.h"
#include "CcHtmlNode.h"

#ifdef _MSC_VER
template class CcDocumentsSHARED CcList<CcHtmlNode>;
#endif

/**
 * @brief Example Class implementation
 */
class CcDocumentsSHARED CcHtmlNodeList : public CcList<CcHtmlNode>
{
};

CcNewClassInheritance(CcHtmlNodeListIterator, CcHtmlNodeList::iterator);

#endif // H_CcHtmlNodeList_H_
