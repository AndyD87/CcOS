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
 * @subpage   CcHttpWorkData
 *
 * @page      CcHttpWorkData
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpWorkData
 */
#ifndef H_CcHttpWorkData_H_
#define H_CcHttpWorkData_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcHttpRequest.h"
#include "CcHttpResponse.h"
#include "Network/CcSocket.h"
#include "CcMapCommon.h"

class CcHttpServer;

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpWorkData : public IIo
{
public:
  /**
   * @brief Constructor
   */
  CcHttpWorkData(CcHttpServer& oServer, CcSocket oSocket) :
    m_oServer(oServer),
    m_oSocket(oSocket),
    m_oRequest(false)
  { }

  /**
   * @brief Destructor
   */
  ~CcHttpWorkData();

  CcHttpRequest& getRequest()
    { return m_oRequest; }
  const CcHttpRequest& getRequest() const
    { return m_oRequest; }
  CcHttpResponse& getResponse()
    { return m_oResponse; }
  CcHttpServer& getServer()
    { return m_oServer; }
  CcSocket& getSocket()
    { return m_oSocket; }
  bool isHeaderSend()
    { return m_bHeaderSend; }
  bool sendHeader();

  size_t readAllContent();
  size_t writeAllChunked();
  size_t writeChunked(const void* pData, size_t uiLength);
  size_t writeChunked(const CcString& sData)
    { return writeChunked(sData.getCharString(), sData.length()); }
  virtual size_t write(const void* pData, size_t uiSize) override
    { return writeChunked(pData, uiSize); }
  virtual size_t read(void* pData, size_t uiSize) override
    { return m_oSocket.read(pData, uiSize); }
  virtual CcStatus open(EOpenFlags) override
    { return false; }
  virtual CcStatus close() override
    { return false; }
  virtual CcStatus cancel() override
    { return false; }

  EHttpRequestType getRequestType()
    { return m_oRequest.getRequestType(); }
  static CcString splitQueryLine(CcString& sPath);
  static CcStringMap parseQueryLine(const CcString& sData);
  static CcString generateQueryLine(const CcStringMap& oData);
  static CcStringMap splitAndParseQueryLine(CcString& sPath)
    { return parseQueryLine(splitQueryLine(sPath)); }

public:
  CcHttpServer&       m_oServer;
  CcSocket            m_oSocket;
  CcHttpRequest       m_oRequest;
  CcHttpResponse      m_oResponse;
  bool                m_bHeaderSend = false;
};

#endif // H_CcHttpWorkData_H_
