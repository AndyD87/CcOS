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
 * @page      CcHttp
 * @subpage   CcHttpCookies
 *
 * @page      CcHttpCookies
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpCookies
 */
#ifndef H_CcHttpCookies_H_
#define H_CcHttpCookies_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcMapCommon.h"

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpCookies
{
private:
  class CCookie
  {
  public:
    void parseValue(const CcString& sValue);
    static size_t findEqualSign(const CcString& sValue);

    CcString sKey;
    CcString sValue;
    bool bSecure = false;
    bool bHttpOnly = false;
  };
public:
  void parseLine(const CcString& sHeader);
  CcString getCookieLine();
  CcString getSetCookieLine();
  size_t size()
  { return m_oCookies.size(); }

  bool exists(const CcString& sKey);
  bool remove(const CcString& sKey);

private:
  CcString getNextValue(const CcString& sHeader, size_t& uiPosition);

private:
  CcList<CCookie> m_oCookies;
};

#endif // H_CcHttpCookies_H_
