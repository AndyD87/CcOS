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
 * @brief     Class CcHttpWorkData
 */
#pragma once

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
   * @brief Initialize workset with
   * @param oServer: Handle to server
   * @param oSocket: Socket for transfer
   */
  CcHttpWorkData(CcHttpServer& oServer, const CcSocket& oSocket) :
    m_oServer(oServer),
    m_oSocket(oSocket),
    m_oRequest(false)
  { }

  ~CcHttpWorkData();

  /**
   * @brief Send header informations, data will follow later
   * @return True if header was send successfully
   */
  bool sendHeader();

  /**
   * @brief Read all content left from socket
   * @return Number of bytes read or SIZE_MAX on error
   */
  size_t readAllContent();

  /**
   * @brief Write current content in chunk mode
   * @return Number of bytes written or SIZE_MAX on error
   */
  size_t writeAllChunked();

  /**
   * @brief Write string to socket
   * @param sData: String to write to socket
   * @return Number of bytes written or SIZE_MAX on error
   */
  size_t write(const CcString& sData)
  { return write(sData.getCharString(), sData.length()); }

  /**
   * @brief Write ByteArray to socket
   * @param oData: Data to write
   * @return Number of bytes written or SIZE_MAX on error
   */
  size_t write(const CcByteArray& oData)
  { return write(oData.getArray(), oData.size()); }

  virtual size_t write(const void* pData, size_t uiLength) override;
  virtual size_t read(void* pData, size_t uiSize) override
  { return m_oSocket.read(pData, uiSize); }
  virtual CcStatus open(EOpenFlags) override
  { return false; }
  virtual CcStatus close() override
  { return false; }
  virtual CcStatus cancel() override
  { return false; }

  /**
   * @brief Split query line of path
   * @param sPath: Path to parse
   * @return Query part of @p sPath
   */
  static CcString splitQueryLine(CcString& sPath);

  /**
   * @brief Parse query line and extract key and value pairs
   * @param sData: Query to parse
   * @return Mapt with key and value pairs
   */
  static CcStringMap parseQueryLine(const CcString& sData);

  /**
   * @brief Generate query line for map
   * @param oData: Map to transfer to line
   * @return Generated line
   */
  static CcString generateQueryLine(const CcStringMap& oData);

  /**
   * @brief Direct convert query part of path to key and value map
   * @param sPath: Path to parse
   * @return Map parsed from path
   */
  static CcStringMap splitAndParseQueryLine(CcString& sPath)
  { return parseQueryLine(splitQueryLine(sPath)); }


  //! @return Get current request
  CcHttpRequest& getRequest()
  { return m_oRequest; }
  //! @return Get current request
  const CcHttpRequest& getRequest() const
  { return m_oRequest; }
  //! @return Get target response
  CcHttpResponse& getResponse()
  { return m_oResponse; }
  //! @return Get server for configurations
  CcHttpServer& getServer()
  { return m_oServer; }
  //! @return Get current socket
  CcSocket& getSocket()
  { return m_oSocket; }
  //! @return True if header was already send
  bool isHeaderSend()
  { return m_bHeaderSend; }
  //! @return Get current type of request as known enum
  EHttpRequestType getRequestType()
  { return m_oRequest.getRequestType(); }

public:
  CcHttpServer&       m_oServer;              //!< Incomming server
  CcSocket            m_oSocket;              //!< Communication socket from server
  CcHttpRequest       m_oRequest;             //!< Request parsed from incomming data
  CcHttpResponse      m_oResponse;            //!< Response generated from execution
  bool                m_bHeaderSend = false;  //!< Flag if header was send
};
