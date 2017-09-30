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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcXmlUtil
 */
#include "CcXml/CcXmlUtil.h"
#include "CcGlobalStrings.h"
#include "CcString.h"

bool CcXmlUtil::getBoolFromNodeValue(const CcXmlNode& oNode, bool bSetIfInvalid, bool* pbOk)
{
  bool bRet;
  bool bOk;
  if (pbOk == nullptr)
  {
    pbOk = &bOk;
  }
  CcString sValue = oNode.getValue();
  if (sValue.compare(CcGlobalStrings::True, ESensitivity::CaseInsensitiv))
  {
    bRet = true;
  }
  else if (sValue.compare(CcGlobalStrings::False, ESensitivity::CaseInsensitiv))
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

const CcString& CcXmlUtil::getStringFromBool(bool bSetIfInvalid)
{
  if (bSetIfInvalid)
    return CcGlobalStrings::True;
  else
    return CcGlobalStrings::False;
}

