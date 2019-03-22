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
 * @subpage   CcHttpResponse
 *
 * @page      CcHttpResponse
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpResponse
 */
#ifndef _CcHttpResponse_H_
#define _CcHttpResponse_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcBufferList.h"
#include "CcHttpTransferEncoding.h"
#include "CcHttpGlobals.h"
#include "CcStringList.h"

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpResponse
{
public:
  /**
   * @brief Constructor
   */
  CcHttpResponse(const CcString& Parse);

  /**
   * @brief Create a Response Header with possbaly filled data
   * @param init: If true is set or empty, Header will be filled with default information
   *              otherwise if false, no values are set and Header-Info is Blank
   */
  CcHttpResponse(bool init = true);

  /**
   * @brief Destructor
   */
  ~CcHttpResponse();

  CcString getHeader();

  void parse(const CcString& Parse);

  CcString getContentType();
  uint64 getContentLength();
  uint16 getHttpCode()
    { return m_uiHttpCode; }
  CcHttpTransferEncoding getTransferEncoding()
    { return m_oTransferEncoding;}

  void setAcceptRanges(const CcString& sRange);
  void setAllow(const CcString& sAllowed);
  void setConnection(const CcString& sConnection);
  void setContentType(const CcString& getContentType);
  void setHttp(const CcString& sHttpsState);
  void setServer(const CcString& sServer);
  void setError(CcHttpGlobals::EError eError);

private:
  void parseLine(const CcString& Parse);
  void addTransferEncoding();

public:
  CcStringList m_oHeaderLines;
  CcHttpTransferEncoding m_oTransferEncoding;
  uint16 m_uiHttpCode = UINT16_MAX;
  CcBufferList m_oContent;
};

#endif /* _CcHttpResponse_H_ */
