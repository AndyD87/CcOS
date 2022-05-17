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
 * @brief     Implemtation of class CcXmlUtil
 */
#include "Xml/CcXmlUtil.h"
#include "CcGlobalStrings.h"
#include "CcString.h"
#include "CcStringUtil.h"

bool CcXmlUtil::getBoolFromNodeValue(const CcXmlNode& oNode, bool bSetIfInvalid, bool* pbOk)
{
  bool bRet;
  bool bOk;
  if (pbOk == nullptr)
  {
    pbOk = &bOk;
  }
  CcString sValue = oNode.innerText();
  if (CcStringUtil::cmpWithLower(CcGlobalStrings::True, sValue))
  {
    bRet = true;
  }
  else if (CcStringUtil::cmpWithLower(CcGlobalStrings::False, sValue))
  {
    bRet = false;
  }
  else
  {
    int64 iNumber = sValue.toInt64(pbOk);
    if (*pbOk)
    {
      if (iNumber != 0)
        bRet = true;
      else
        bRet = false;
    }
    else
    {
      bRet = bSetIfInvalid;
    }
  }
  return bRet;
}

const CcString& CcXmlUtil::getStringFromBool(bool bTrueFalse)
{
  if (bTrueFalse)
    return CcGlobalStrings::True;
  else
    return CcGlobalStrings::False;
}

