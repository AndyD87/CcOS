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
 * @subpage   CcHttpClient
 *
 * @page      CcHttpClient
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpClient
 */
#ifndef CcHttpClient_H_
#define CcHttpClient_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcThreadObject.h"
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
class CcHttpSHARED CcHttpClient : public CcThreadObject
{
public:
  /**
   * @brief Constructor
   */
  CcHttpClient( void );

  /**
   * @brief Destructor
   */
  virtual ~CcHttpClient( void );

  void setUrl(const CcUrl& Url);
  inline const CcUrl& getUrl() const
    { return m_HeaderRequest.getUrl(); }
  inline CcHttpRequest& headerRequest()
    { return m_HeaderRequest; }
  inline CcHttpResponse& headerResponse()
    { return m_HeaderResponse; }
  void addData(const CcString& sName, const CcString& sValue);
  void addFiles(const CcString& sFilePath, const CcString& sFileName);
  inline void setRequestString(const CcString& sRequestString)
    { m_sRequestString = sRequestString; }
  bool execGet();
  bool execHead();
  bool execPost();
  bool execPostMultipart();

  inline void setRetries(uint16 Retries)
  { m_uiRetries = Retries; }

  CcByteArray& getByteArray(void);

  void setOutputDevice(CcIODevice* output);

  bool isDone(void);

  void run();
private: //methods
  bool connectSocket(void);
  void closeSocket(void);

private:
  uint16 m_uiRetries;

private:
  CcSocket m_Socket;
  CcString m_WD;
  CcString m_sRequestString;
  CcStringMap m_oRequestData;
  CcStringMap m_oRequestFiles;
  CcIODevice *m_Output = nullptr;
  CcHttpRequest m_HeaderRequest; 
  CcHttpResponse m_HeaderResponse;
  bool m_Done;
  CcByteArray m_Buffer;
private:
  uint16 s_Retries = 5;  //!< Default retries to get a valid connection and HTTP result lower than 300
};

#endif /* CcHttpClient_H_ */
