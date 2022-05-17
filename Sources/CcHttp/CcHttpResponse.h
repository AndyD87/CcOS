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
 * @brief     Class CcHttpResponse
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcBufferList.h"
#include "CcHttpTransferEncoding.h"
#include "CcHttpGlobals.h"
#include "CcStringList.h"
#include "CcHttpCookies.h"

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

  //! @return Generate header from data
  CcString getHeader();

  /**
   * @brief Parse Http header and get all known informations.
   * @param Parse: String with http header to parse
   */
  void parse(const CcString& Parse);

  /**
   * @brief Append a raw header line to list
   * @param sKey:   Key of header value
   * @param sValue: Value of pair
   */
  void appendHeaderLine(const CcString& sKey, const CcString& sValue);

  //! @return Get content as binary data
  CcBufferList& getContent()
  { return m_oContent; }
  //! @return Get current content type
  const CcString& getContentType()
  { return m_sContentType; }
  //! @return Get content length
  uint64 getContentLength();
  //! @return Get http code with status informations
  uint16 getHttpCode()
  { return m_uiHttpCode; }
  //! @return Get current transfer encoding
  CcHttpTransferEncoding getTransferEncoding()
  { return m_oTransferEncoding;}

  //! @param sRange: Set accepted range
  void setAcceptRanges(const CcString& sRange);
  //! @param sAllowed: Set allow
  void setAllow(const CcString& sAllowed);
  //! @param sConnection: Set connection
  void setConnection(const CcString& sConnection);
  //! @param sContentType: Set connection type
  void setContentType(const CcString& sContentType)
  { m_sContentType = sContentType; }
  //! @param sHttpsState: Set http state
  void setHttp(const CcString& sHttpsState);
  //! @param sServer: Set server address
  void setServer(const CcString& sServer);
  //! @param eError: Set error by enum
  void setError(CcHttpGlobals::EError eError);
  //! @param uiFlags: Set transfer encoding as flags
  void setTransferEncoding(uint32 uiFlags)
  { m_oTransferEncoding.setFlag(uiFlags);}
  //! @param sType: Set authentication type
  void setWwwAuthenticate(const CcString& sType);
  //! @param pCookies: Set cookiehandler for responses
  void setCookies(CcHttpCookies* pCookies)
  { m_pCookies = pCookies; }

private:
  size_t findKey(const CcString& sKey);
  void parseLine(const CcString& Parse);
  void addTransferEncoding(CcString& sHeader);

public:
  CcStringList            m_oHeaderLines;             //!< Header lines in string list
  CcHttpTransferEncoding  m_oTransferEncoding;        //!< Transfer encoding as enum
  uint16                  m_uiHttpCode = UINT16_MAX;  //!< Error code of http response
private:
  CcHttpCookies*          m_pCookies = nullptr;
  CcBufferList m_oContent;
  CcString m_sContentType;
};
