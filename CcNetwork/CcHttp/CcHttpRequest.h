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
   */
  CcHttpRequest(const CcString& Parse);

  CcHttpRequest(bool bInitValues = true);

  /**
   * @brief Destructor
   */
  ~CcHttpRequest();

  CcString getHeader();

  void parse(const CcString& Parse);

  void appendHeaderLine(const CcString& sKey, const CcString& sValue);

  const CcString& getPath() const
    { return m_sPath; }
  EHttpRequestType getRequestType() const
    { return m_eRequestType; }
  CcString getContentType();
  CcByteArray& getContent()
    { return m_oContent; }
  uint64 getContentLength();
  CcHttpTransferEncoding getTransferEncoding()
    { return m_oTransferEncoding;}
  CcStringMap getQuery()
    { return m_oQuery; }

  void setAccept(const CcString& sAccept);
  void setAcceptCharset(const CcString& sAcceptCharset);
  void setAcceptEncoding(const CcString& sAcceptEncoding);
  void setAcceptLanguage(const CcString& sAcceptLanguage);
  void setConnection(const CcString& sConnection);
  void setHost(const CcString& Host);
  void setUserAgent(const CcString& Host);
  void setContentEncoding(const CcString& additional);
  void setContentType(const CcString& additional);
  void setContentLength(uint64 uiSize);
  void setAuthorization(const CcString& sUsername, const CcString& sPassword);
  void setAuthorization(const CcString& sAuthorization);
  void setRequestType(EHttpRequestType eType, const CcString& sPath);
  void appendContent(const void* pData, size_t uiLen);

  void setMozillaAgent();
private:
  void parseFirstLine(const CcString& Parse);
  void parseLine(const CcString& Parse);
  void addTransferEncoding();

private:
  CcStringList            m_oHeaderLines;
  CcHttpTransferEncoding  m_oTransferEncoding;
  CcStringList            Files;
  CcByteArray             m_oContent;
  EHttpRequestType        m_eRequestType = EHttpRequestType::Unknown;
  CcString                m_sPath;
  CcStringMap             m_oQuery;
};

#endif /* H_CcHttpRequest_H_ */
