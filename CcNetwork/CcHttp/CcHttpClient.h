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
 * @brief     Class CcHttpClient
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "IThread.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcHttpRequest.h"
#include "CcHttpResponse.h"
#include "CcMapCommon.h"

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpClient : public IThread
{
public:
  /**
   * @brief Create client, all values have to be set later
   */
  CcHttpClient();

  /**
   * @brief Create client with url
   * @param Url: Set url for next request
   */
  CcHttpClient(const CcUrl& Url);

  CcHttpClient(const CcHttpClient&)
  { /* Do not Copy*/ }

  /**
   * @brief Destructor
   */
  virtual ~CcHttpClient();

  //! @return Get request header
  inline CcHttpRequest& headerRequest()
  { return m_HeaderRequest; }
  //! @return Get response header
  inline CcHttpResponse& headerResponse()
  { return m_HeaderResponse; }

  //! @return Get data received
  CcByteArray& getByteArray();
  //! @return Get cookie object
  CcHttpCookies& getCookies()
  { return m_oCookies; }
  //! @return Get current url for request
  inline const CcUrl& getUrl() const
  { return m_oUrl; }

  //! @return True if Request was done
  bool isDone();

  /**
   * @brief Add data for post request
   * @param sName:  Name of value
   * @param sValue: Value of data
   */
  void addData(const CcString& sName, const CcString& sValue);

  /**
   * @brief Add file to read for post request
   * @param sFilePath:  Path to file for upload
   * @param sFileName:  Name of file for upload
   */
  void addFiles(const CcString& sFilePath, const CcString& sFileName);

  //! @param sRequestString: Post data to set for transfer
  inline void setRequestString(const CcString& sRequestString)
  { m_sRequestString = sRequestString; }
  //! @param Url: Set url for next request
  void setUrl(const CcUrl& Url);
  //! @param Retries: Set maximum number of retries until cancel request
  inline void setRetries(uint16 Retries)
  { m_uiRetries = Retries; }
  //! @param output: Forward output to IO Device and do not store result.
  void setOutputDevice(IIo* output);

  /**
   * @brief Execute Get request with current data set
   * @return Result of request
   * @todo add post-data to header
   */
  bool execGet();

  /**
   * @brief Execute Head request with current data set
   * @return Result of request
   */
  bool execHead();

  /**
   * @brief Execute Post request with current data set
   * @return Result of request
   */
  bool execPost();

  /**
   * @brief Execute Post request with multipart/form-data format set
   * @return Result of request
   */
  bool execPostMultipart();

  virtual void run() override;
private: //methods
  bool connectSocket();
  void closeSocket();
  bool readHeader();
  bool receiveChunked();

private:
  uint16 m_uiRetries;

private:
  CcSocket m_Socket;
  CcString m_WD;
  CcString m_sRequestString;
  CcStringMap m_oRequestData;
  CcStringMap m_oRequestFiles;
  IIo *m_Output = nullptr;
  CcHttpCookies   m_oCookies;
  CcHttpRequest   m_HeaderRequest;
  CcHttpResponse  m_HeaderResponse;
  bool m_Done;
  CcString    m_sHeader;
  CcByteArray m_oBuffer;
  CcUrl m_oUrl;
private:
  static uint16 s_uiRetries;  //!< Default retries to get a valid connection and HTTP result lower than 300
};
