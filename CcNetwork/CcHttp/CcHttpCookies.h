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
 * @brief     Class CcHttpCookies
 */
#pragma once

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
  class CcHttpSHARED CCookie
  {
  public:
    void parseValue(const CcString& sValue);
    static size_t findEqualSign(const CcString& sValue);

    CcString sKey;
    CcString sValue;
    bool bSecure = false;
    bool bHttpOnly = false;
  };

  typedef CcList<CcHttpCookies::CCookie> CCookieList;
  #ifdef _MSC_VER
    class CcHttpSHARED CCookieList;
  #endif

public:
  /**
   * @brief Parse header line to get all cookies
   * @param sHeader: Cookie line from http header
   */
  void parseLine(const CcString& sHeader);

  /**
   * @brief Generate header line for send cookies
   * @return String with cookie informations for server
   */
  CcString getCookieLine();

  /**
   * @brief Generate header line for send cookies
   * @return String with cookie infromations for client
   */
  CcString getSetCookieLine();

  /**
   * @brief Get number of cookies stored
   * @return Number of cookies
   */
  size_t size()
  { return m_oCookies.size(); }

  /**
   * @brief Check if cookie already exists
   * @param sKey: Cookie to query for
   * @return True if cookie is existing.
   */
  bool exists(const CcString& sKey);

  /**
   * @brief Remove cookie from current list.
   * @param sKey: Cookie to remove
   * @return True if cookie was existing and removed.
   */
  bool remove(const CcString& sKey);

private:
  CcString getNextValue(const CcString& sHeader, size_t& uiPosition);

private:
  CCookieList m_oCookies;
};
