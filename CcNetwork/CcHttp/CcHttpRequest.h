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
 * @brief     Class CcHttpRequest
 */
#ifndef H_CcHttpRequest_H_
#define H_CcHttpRequest_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcMapCommon.h"
#include "CcUrl.h"
#include "CcByteArray.h"
#include "CcHttpTransferEncoding.h"
#include "CcHttpCookies.h"

/**
 * @brief Enum of different http request types
 */
enum class EHttpRequestType : uint8
{
  Unknown = 0x00,
  Get = 0x01,
  Head = 0x02,
  Post = 0x03,
  Put,
  List,
  Delete,
  Patch,
};

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpRequest
{
public:
  /**
   * @brief Constructor
   * @param Parse: Direct parse incomming request
   */
  CcHttpRequest(const CcString& Parse);

  /**
   * @brief Constructor
   * @param bInitValues: Outgoing request
   */
  CcHttpRequest(bool bInitValues = true);
  ~CcHttpRequest();

  //! @return Generate header from data
  CcString getHeader();

  /**
   * @brief Cleare values und re initialize if required
   * @param bInitValues: If True, values get resetted to initial values.
   */
  void clear(bool bInitValues = true);

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
  
  //! @return Get current path of request
  const CcString& getPath() const
  { return m_sPath; }
  //! @return Get current type of request as enum
  EHttpRequestType getRequestType() const
  { return m_eRequestType; }
  //! @return Get cookie object
  const CcHttpCookies& getCookies()
  { return m_oCookies; }
  //! @return Get current content type
  CcString getContentType();
  //! @return Get content as binary data
  CcByteArray& getContent()
  { return m_oContent; }
  //! @return Get content length
  uint64 getContentLength();
  //! @return Get current transfer encoding
  CcHttpTransferEncoding getTransferEncoding()
  { return m_oTransferEncoding;}
  //! @return Get query data from path
  CcStringMap getQuery()
  { return m_oQuery; }
  
  //! @param sAccept: Set accept value
  void setAccept(const CcString& sAccept);
  //! @param sAcceptCharset: Set accepted charset
  void setAcceptCharset(const CcString& sAcceptCharset);
  //! @param sAcceptEncoding: Set accepted encoding
  void setAcceptEncoding(const CcString& sAcceptEncoding);
  //! @param sAcceptLanguage: Set accepted language
  void setAcceptLanguage(const CcString& sAcceptLanguage);
  //! @param sConnection: Set accepted connection type
  void setConnection(const CcString& sConnection);
  //! @param oCookies: Set Cookies
  void setCookies(const CcHttpCookies& oCookies)
  { m_oCookies = oCookies; }
  //! @param Host: Set target host
  void setHost(const CcString& Host);
  //! @param Host: Set User agent
  void setUserAgent(const CcString& Host);
  //! @param additional: Set content encoding type
  void setContentEncoding(const CcString& additional);
  //! @param additional: Set content type
  void setContentType(const CcString& additional);
  //! @param uiSize: Set content length
  void setContentLength(uint64 uiSize);
  //! @brief Set basic authorization with username and password
  //! @param sUsername: Username
  //! @param sPassword: Password
  void setAuthorization(const CcString& sUsername, const CcString& sPassword);
  //! @param sAuthorization: Set other authorization format raw.
  void setAuthorization(const CcString& sAuthorization);
  //! @param eType: Type of request as enum
  //! @param sPath: Path for request
  void setRequestType(EHttpRequestType eType, const CcString& sPath);
  //! @brief Set Mozilla as useragent
  void setMozillaAgent();

  /**
   * @brief Append content to request
   * @param pData:  Content data to add as buffer
   * @param uiLen:  size of @p pData
   */
  void appendContent(const void* pData, size_t uiLen);
private:
  size_t findKey(const CcString& sKey);
  void parseFirstLine(const CcString& Parse);
  void parseLine(const CcString& Parse);
  void addTransferEncoding();

private:
  CcStringList            m_oHeaderLines;
  CcHttpTransferEncoding  m_oTransferEncoding;
  CcStringList            Files;
  CcByteArray             m_oContent;
  CcHttpCookies           m_oCookies;
  EHttpRequestType        m_eRequestType = EHttpRequestType::Unknown;
  CcString                m_sPath;
  CcStringMap             m_oQuery;
};

#endif // H_CcHttpRequest_H_
