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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpRequest
 */
#ifndef _CcHttpRequest_H_
#define _CcHttpRequest_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcMapCommon.h"
#include "CcUrl.h"
#include "CcByteArray.h"
#include "CcHttpTransferEncoding.h"


enum class EHttpRequestType : uint8
{
  Unknown,
  Get,
  Head,
  Post,
  Put,
  List,
  Delete,
  Patch,
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

  CcHttpRequest();

  /**
   * @brief Destructor
   */
  ~CcHttpRequest();

  CcString getHeader();

  void parse(const CcString& Parse);

  const CcString& getPath() const
    { return m_sPath; }
  EHttpRequestType getRequestType() const
    { return m_eRequestType; }

  void setAccept(const CcString& sAccept);
  void setAcceptCharset(const CcString& sAcceptCharset);
  void setAcceptEncoding(const CcString& sAcceptEncoding);
  void setAcceptLanguage(const CcString& sAcceptLanguage);
  void setConnection(const CcString& sConnection);
  void setHost(const CcString& Host);
  void setUserAgent(const CcString& Host);
  void setContentEncoding(const CcString& additional);
  void setContentType(const CcString& additional);
  void setContentLength(size_t size);
  void setAuthorization(const CcString& sUsername, const CcString& sPassword);
  void setAuthorization(const CcString& sAuthorization);
  void setRequestType(EHttpRequestType eType, const CcString& sPath);
  void addLine(const CcString& sName, const CcString& sValue);

  void setMozillaAgent();
private:
  void parseLine(const CcString& Parse);
  void addTransferEncoding();

private:
  CcStringList m_oHeaderLines;
  CcHttpTransferEncoding m_oTransferEncoding;
  CcStringList Files;
  CcByteArray m_oContent;
  EHttpRequestType m_eRequestType = EHttpRequestType::Unknown;
  CcString m_sPath;
};

#endif /* _CcHttpRequest_H_ */
