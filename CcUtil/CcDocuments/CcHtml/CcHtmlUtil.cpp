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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CcHtmlUtil
 */
#include "CcHtml/CcHtmlUtil.h"
#include "CcHtml/CcHtmlNodeList.h"

CcHtmlNode& CcHtmlUtil::findNodeByAttributes(CcHtmlNode& pNodes, const CcString& sName, CcVector<CcHtmlAttribute*>& Attributes, size_t nr)
{
  CcHtmlNode* pRet = nullptr;
  size_t nrCnt = 0;;
  for (CcHtmlNode& pNode : pNodes)
  {
    if (pNode.getName() == sName)
    {
      bool bFound = true;
      for (size_t i = 0; i < Attributes.size(); i++)
      {
        CcHtmlAttribute& tempNodeAttr = pNode.getAttribute(Attributes.at(i)->getName());
        if (
          CCISNULLREF(tempNodeAttr) == false ||
          tempNodeAttr.getValue() != Attributes.at(i)->getValue())
        {
          bFound = false;
        }
      }
      if (bFound == true)
      {
        pRet = &pNode;
        if (nrCnt < nr)
        {
          nrCnt++;
          continue;
        }
        else
          break;
      }
    }
    else
    {
      pRet = &findNodeByAttributes(pNode, sName, Attributes, nr);
      if (pRet != nullptr)
      {
        if (nrCnt < nr)
        {
          nrCnt++;
          continue;
        }
        else
          break;
      }
    }
  }
  return *pRet;
}