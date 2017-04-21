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
 * @subpage   CcHttpRequest
 *
 * @page      CcHttpRequest
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpRequest
 */
#ifndef CcHttpRequest_H_
#define CcHttpRequest_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcMapCommon.h"
#include "CcUrl.h"


enum class EHttpRequestType : uint8
{
  Get,
  Head,
  PostUrlEnc,
  PostMultip
};

#define HTTP_GET          0x01
#define HTTP_HEAD         0x02
#define HTTP_POST_URLENC  0x03
#define HTTP_POST_MULTIP  0x04

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpRequest
{
public:
  /**
   * @brief Constructor
   */
  CcHttpRequest(const CcString& Parse);
  CcHttpRequest(const EHttpRequestType eType);

  CcHttpRequest();

  /**
   * @brief Destructor
   */
  virtual ~CcHttpRequest( void );

  CcString getHeader(void);

  inline CcUrl& url()
    { return m_Url; }
  inline const CcUrl& getUrl() const
    { return m_Url; }
  inline void setUrl(const CcUrl& oUrl)
    { m_Url = oUrl; }

  void parse(const CcString& Parse);

  void setPath(const CcString& sPath);
  void setHost(const CcString& Host);
  void setUserAgent(const CcString& Host);
  void setContentType(const CcString& additional);
  void setContentSize(size_t size);
  inline void setUsername(const CcString& sUsername)
    { m_sUsername = sUsername; }
  inline void setPassword(const CcString& sPassword)
    { m_sPassword = sPassword; }
  inline void setAuthorization(const CcString& sAuthorization)
    { m_Header.Authorization = sAuthorization; }
  inline void setRequestType(EHttpRequestType eType)
    {m_Header.RequestType = eType; }

  void setMozillaAgent(void);
private:
  void parseLine(const CcString& Parse);
public:
  CcUrl m_Url;
  struct sHeaderData
  {
    EHttpRequestType RequestType;
    CcString Path;
    CcString HTTPTarget; //!< URL-Path
    CcString HTTPMethod; //!< Calling Method like GET POST HEADER
    CcString Accept;
    CcString AcceptCharset;
    CcString AcceptEncoding;
    CcString AcceptLanguage;
    CcString Authorization;
    CcString CacheControl;
    CcString Connection;
    CcString Cookie;
    CcString ContentLength;
    CcString ContentType;
    CcString Host;
    CcString Range;
    CcString Referer;
    CcString TransferEncoding;
    CcString UserAgent;
  } m_Header;
  CcString m_sUsername;
  CcString m_sPassword;
  struct sContentData{
    CcStringMap IncomeData;
    CcStringList Files;
  } m_Data;
};

#endif /* CcHttpRequest_H_ */
