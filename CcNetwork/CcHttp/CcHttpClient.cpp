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
 * @brief     Implementation of Class CcHttpClient
 */
#include "CcHttpClient.h"
#include "CcKernel.h"
#include "CcHttpResponse.h"
#include "CcHttpGlobalStrings.h"
#include "CcFile.h"
#include "CcDateTime.h"

#ifdef CCSSL_ENABLED
  #include "CcSslSocket.h"
#endif

//! Max transfer buffer for Http transfer
#define MAX_TRANSER_BUFFER 10240

uint16 CcHttpClient::s_uiRetries = 5;

CcHttpClient::CcHttpClient() :
  IThread("CcHttpClient"),
  m_uiRetries(s_uiRetries),
  m_Socket(nullptr),
  m_Output(nullptr),
  m_Done(false)
{
  m_WD = CcKernel::getWorkingDir();
  m_HeaderRequest.setCookies(&m_oCookies);
  m_HeaderResponse.setCookies(&m_oCookies);
}

CcHttpClient::CcHttpClient(const CcUrl& Url) :
  IThread("CcHttpClient"),
  m_uiRetries(s_uiRetries),
  m_Socket(nullptr),
  m_Output(nullptr),
  m_Done(false)
{
  m_WD = CcKernel::getWorkingDir();
  setUrl(Url);
  m_HeaderRequest.clear(true);
  m_HeaderRequest.setCookies(&m_oCookies);
  m_HeaderResponse.setCookies(&m_oCookies);
}

CcHttpClient::~CcHttpClient()
{
}

void CcHttpClient::setUrl(const CcUrl& Url)
{
  m_oUrl = Url;
  // Check for Port-Settings
  if (m_oUrl.getPort()== 0)
  {
    // Search for default ports
    if (m_oUrl.getProtocol() == "http")
    {
      m_oUrl.setPort(80);
    }
    else if (m_oUrl.getProtocol() == "https")
    {
      m_oUrl.setPort(443);
    }
    else
    {
      // No known protocol found, use default port 80
      m_oUrl.setPort(80);
    }
  }
}

void CcHttpClient::addData(const CcString& sName, const CcString& sValue)
{
  m_oRequestData.add(sName, sValue);
}

void CcHttpClient::addFiles(const CcString& sFilePath, const CcString& sFileName)
{
  m_oRequestFiles.add(sFilePath, sFileName);
}

bool CcHttpClient::exec(EHttpRequestType eRequestType, const CcByteArray& oData)
{
  CcStatus oStatus = false;
  // Check if url is a real url
  if (m_oUrl.isUrl())
  {
    // create a request package
    m_HeaderRequest.setRequestType(eRequestType, m_oUrl.getPath());
    if (eRequestType != EHttpRequestType::Get)
    {
      m_HeaderRequest.setContentLength(oData.size());
    }
    // set target host from url
    m_HeaderRequest.setHost(m_oUrl.getHostname());

    // get generated request back to String
    CcString httpRequest(m_HeaderRequest.getHeader());
    // Start connection to host
    uint16 retryCounter = 0;

    do
    {
      m_oBuffer.clear();
      retryCounter++;
      // check if connection is working
      if (connectSocket())
      {
        // set status to true, it will be set to fale on any error
        oStatus = true;
        if (oStatus)
        {
          if (m_Socket.write(httpRequest.getCharString(), httpRequest.length()) != httpRequest.length())
          {
            CCDEBUG("Failed to write header");
            oStatus = false;
          }
          if (oStatus && oData.size())
          {
            if (!m_Socket.writeArray(oData))
            {
              CCDEBUG("Failed to write additional data");
              oStatus = false;
            }
          }
        }

        if (oStatus)
        {
          if (readHeader() == false)
          {
            CCDEBUG("Failed to read header");
            oStatus = false;
          }
        }
        if (m_HeaderResponse.getHttpCode() >= 400)
        {
          oStatus = EStatus::NetworkError;
        }
        if (oStatus)
        {
          size_t rec;
          if (m_HeaderResponse.getTransferEncoding().isChunked())
          {
            oStatus = receiveChunked();
          }
          else
          {
            CcByteArray oBuffer(MAX_TRANSER_BUFFER);
            uint64 uiDataSize = m_HeaderResponse.getContentLength();
            while (m_oBuffer.size() < uiDataSize)
            {
              // receive next data
              rec = m_Socket.readArray(oBuffer, false);
              if (rec && rec != SIZE_MAX)
              {
                // append data to buffer
                m_oBuffer.append(oBuffer.getArray(), rec);
              }
            }
            oStatus = true;
          }
        }
        closeSocket();
      }
    } while (oStatus == false &&
      (retryCounter < m_uiRetries || retryCounter == 0) &&
      CcKernel::sleep(100));
  }
  else
  {
    oStatus = EStatus::CommandInvalidParameter;
  }
  m_HeaderRequest.clear(true);
  return oStatus;
}

bool CcHttpClient::execGet()
{
  return exec(EHttpRequestType::Get);
}

bool CcHttpClient::execHead()
{
  bool bRet = false;
  if (m_oUrl.isUrl())
  {
    m_HeaderRequest.setRequestType(EHttpRequestType::Head, m_oUrl.getPath());
    m_HeaderRequest.setHost(m_oUrl.getHostname());
    CcString httpRequest(m_HeaderRequest.getHeader());

    // Start connection to host
    uint16 retryCounter = 0;
    // check if job is done and limit is not reached
    while (bRet == false && (retryCounter < m_uiRetries || retryCounter == 0))
    {
      retryCounter++;
      // check if connection is working
      if (connectSocket())
      {
        m_Socket.write(httpRequest.getCharString(), httpRequest.length());
        size_t rec;
        char receive[MAX_TRANSER_BUFFER];
        rec = m_Socket.read(receive, sizeof(receive));
        while (rec && rec != SIZE_MAX)
        {
          m_oBuffer.append(receive, rec);
          rec = m_Socket.read(receive, sizeof(receive));
        }
        rec = m_oBuffer.find(CcHttpGlobalStrings::EOLSeperator);
        if (rec != SIZE_MAX)
        {
          CcString HeaderString;
          HeaderString.append(m_oBuffer, 0, rec);
          m_oBuffer.remove(0, rec + 2);
          if (m_oBuffer.at(0) == '\r')
            m_oBuffer.remove(0, 2);
          m_HeaderResponse.parse(HeaderString);
          size_t contentLength = static_cast<size_t>(m_HeaderResponse.getContentLength());
          if (contentLength > m_oBuffer.size() && contentLength > 0)
          {
            m_oBuffer.remove(contentLength, m_oBuffer.size() - contentLength);
          }
        }
        if (m_HeaderResponse.getHttpCode() == 200)
        {
          bRet = true;
        }
        closeSocket();
      }
    }
  }
  m_HeaderRequest.clear(true);
  return bRet;
}

bool CcHttpClient::execPost()
{
  bool bSuccess = false;
  m_oBuffer.clear();
  m_HeaderRequest.setRequestType(EHttpRequestType::Post, m_oUrl.getPath());
  m_HeaderRequest.setHost(m_oUrl.getHostname());

  if(m_HeaderRequest.getContentType().length() == 0)
    m_HeaderRequest.setContentType("application/x-www-form-urlencoded");

  if (connectSocket())
  {
    CcByteArray oData;
    if (m_sRequestString.length() > 0)
    {
      oData.append( m_sRequestString.getByteArray() );
    }
    m_HeaderRequest.setContentLength(oData.size());
    CcString sRequest(m_HeaderRequest.getHeader());
    sRequest << oData;
    m_Socket.writeArray(sRequest.getByteArray());
    CcByteArray oHeaderData;
    size_t uiPos      = 0;
    size_t uiReadData = 0;
    CcByteArray oBuffer(10240); // @todo magic number
    do
    {
      uiReadData = m_Socket.readArray(oBuffer, false);
      uiPos = oBuffer.find(CcHttpGlobalStrings::EOLSeperator);
      if (uiPos != SIZE_MAX)
      {
        oHeaderData.append(oBuffer.getArray(), uiPos);
        uiPos += CcHttpGlobalStrings::EOLSeperator.length();
        m_oBuffer.append(oBuffer.getArray() + uiPos, uiReadData - uiPos );
      }
      else
      {
        oHeaderData.append(oBuffer);
      }
    } while (uiReadData > 0 && uiPos == SIZE_MAX);     // @todo remove SIZE_MAX with a max transfer size
    m_HeaderResponse.parse(oHeaderData);

    if (m_HeaderResponse.getTransferEncoding().isChunked())
    {
      receiveChunked();
    }
    else
    {
      while (uiReadData > 0 && uiReadData < oBuffer.size() &&
        m_oBuffer.size() < m_HeaderResponse.getContentLength())   // @todo remove SIZE_MAX with a max transfer size
      {
        uiReadData = m_Socket.readArray(oBuffer, false);
        if(uiReadData < oBuffer.size())
          m_oBuffer.append(oBuffer, uiReadData);
      }
    }
    bSuccess = true;
  }
  m_HeaderRequest.clear(true);
  return bSuccess;
}

bool CcHttpClient::execPostMultipart()
{
  bool bRet = false;
  m_HeaderRequest.setRequestType(EHttpRequestType::Post, m_oUrl.getPath());
  m_HeaderRequest.setHost(m_oUrl.getHostname());
  CcString BoundaryName     ("----------ABCDEFG");
  CcString BoundaryNameBegin("--" + BoundaryName + CcHttpGlobalStrings::EOLSeperator);
  CcString BoundaryNameEnd  ("--" + BoundaryName + "--" + CcHttpGlobalStrings::EOL);
  m_HeaderRequest.setContentType("multipart/form-data; boundary=" + BoundaryName);
  size_t contentSize = 0;
  CcStringList fileContent;
  CcStringList postParam;
  for (const CcStringPair& sRequesFile : m_oRequestFiles)
  {
    CcString sTemp(m_WD); 
    sTemp.appendPath(sRequesFile.getValue());
    CcFile file(sTemp);
    contentSize += file.size();
    CcString sFileContent = BoundaryNameBegin;
    sFileContent.append("Content-Disposition: form-data; filename=\"");
    CcString temp(sRequesFile.getValue().extractFilename());
    sFileContent.append(temp);
    sFileContent.append("\"; ");
    sFileContent.append("name=\"");
    sFileContent.append(sRequesFile.getKey());
    sFileContent.append("\"" + CcHttpGlobalStrings::EOL);
    sFileContent.append("Content-Type: application/octet-stream" + CcHttpGlobalStrings::EOL + CcHttpGlobalStrings::EOL);
    fileContent.append(sFileContent);
    contentSize += sFileContent.length()+2;
  }
  for (const CcStringPair& sRequesData : m_oRequestData)
  {
    CcString sParam(BoundaryNameBegin);
    sParam.append("Content-Disposition: form-data; name=\"");
    sParam.append(sRequesData.getKey());
    sParam.append("\"" + CcHttpGlobalStrings::EOL + CcHttpGlobalStrings::EOL);
    sParam.append(sRequesData.getValue());
    postParam.append(sParam);
    contentSize += sParam.length()+2;
  }
  postParam.append(BoundaryNameEnd);
  contentSize += BoundaryNameEnd.length();
  m_HeaderRequest.setContentLength(contentSize);
  CcString Header = m_HeaderRequest.getHeader();
  // Start connection to host
  uint16 retryCounter = 0;
  // check if job is done and limit is not reached
  while (bRet == false && (retryCounter < m_uiRetries || retryCounter == 0))
  {
    retryCounter++;
    // check if connection is working
    if (connectSocket())
    {
      if (m_Socket.connect(m_oUrl.getHostname(), m_oUrl.getPortString()))
      {
        m_Socket.write(Header.getCharString(), Header.length());
      }
      for (size_t i = 0; i < m_oRequestFiles.size(); i++)
      {
        m_Socket.write(fileContent.at(i).getCharString(), fileContent.at(i).length());
        CcString sTemp(m_WD); 
        sTemp.appendPath(m_oRequestFiles.at(i).getValue());
        CcFile file(sTemp);
        if (file.isFile() && file.open(EOpenFlags::Read))
        {
          size_t read, readLeft;
          char buf[MAX_TRANSER_BUFFER];
          while (!bRet)
          {
            read = file.read(buf, sizeof(buf));
            if (read != SIZE_MAX && read != 0)
            {
              readLeft = m_Socket.write(buf, read);
              while ((read != SIZE_MAX) && (readLeft != read))
                readLeft += m_Socket.write(buf, read - readLeft);
              if (readLeft == SIZE_MAX)
              {
                bRet = true;
              }
            }
            else if (read == SIZE_MAX)
            {
              bRet = true;
            }
            else{
              bRet = true;
            }
          }
          file.close();
        }
        m_Socket.write(CcHttpGlobalStrings::EOL.getCharString(), CcHttpGlobalStrings::EOL.length());
      }
      size_t read;
      for (size_t i = 0; i < postParam.size(); i++)
      {
        m_Socket.write(postParam.at(i).getCharString(), postParam.at(i).length());
        m_Socket.write(CcHttpGlobalStrings::EOL.getCharString(), CcHttpGlobalStrings::EOL.length());
      }
      bool bDone = false;
      char buf[MAX_TRANSER_BUFFER];
      while (!bDone)
      {
        read = m_Socket.read(buf, sizeof(buf));
        if (m_Output != 0)
        {
          m_Output->write(buf, read);
        }
        if (read == SIZE_MAX || read == 0)
        {
          bDone = true;
        }
      }
      closeSocket();
    }
  }
  return bRet;
}

bool CcHttpClient::isDone()
{
  return m_Done;
}

void CcHttpClient::setOutputDevice(IIo*output)
{
  m_Output = output;
}

CcByteArray& CcHttpClient::getByteArray()
{
  return m_oBuffer;
}

void CcHttpClient::run()
{
}

bool CcHttpClient::connectSocket()
{
  bool bRet = false;
  CcString sPort = m_oUrl.getPortString();
  if (m_oUrl.getProtocol() == "https")
  {
    if (sPort.length() == 0)
      sPort = "443";
#ifdef CCSSL_ENABLED
    CCNEWTYPE(pSocket, CcSslSocket);
    pSocket->initClient();
    m_Socket = pSocket;
#else
    m_Socket = CcKernel::getSocket(ESocketType::TCP);
#endif
  }
  else
  {
    if (sPort.length() == 0)
      sPort = "80";
    m_Socket = CcSocket(ESocketType::TCP);
  }
  m_Socket.setTimeout(CcDateTimeFromSeconds(5));
  bRet = m_Socket.connect(m_oUrl.getHostname(), sPort);
  if (bRet)
  {
    m_Socket.setTimeout(CcDateTimeFromSeconds(10));
  }
  return bRet;
}

void CcHttpClient::closeSocket()
{
  m_Socket.close();
}

bool CcHttpClient::readHeader()
{
  bool bSuccess = true;
  size_t uiReadSize;

  //Clear buffers
  m_sHeader.clear();
  m_oBuffer.clear();

  // while no more to read, read from server
  CcByteArray oData(MAX_TRANSER_BUFFER);
  do
  {
    // receive next data
    uiReadSize = m_Socket.readArray(oData, false);
    if (uiReadSize && uiReadSize != SIZE_MAX)
    {
      size_t uiPos = oData.find(CcHttpGlobalStrings::EOLSeperator);
      if (uiPos < uiReadSize)
      {
        // append all data to header up to found seperator
        m_sHeader.append(oData, 0, uiPos);
        // Append the rest to internal buffer
        size_t uiPosData = uiPos + CcHttpGlobalStrings::EOLSeperator.length();
        uiReadSize -= uiPosData;
        m_oBuffer.append(oData.getArray(uiPosData), uiReadSize);
      }
      else
      {
        // append all data to header
        m_sHeader.append(oData, 0, uiReadSize);
      }
    }
    else
    {
      m_sHeader.clear();
      m_oBuffer.clear();
      bSuccess = false;
    }
  } while ( bSuccess &&
            uiReadSize &&
            uiReadSize != SIZE_MAX && 
            m_oBuffer.size() == 0);

  // check if Header end was found and Data beginning point is found
  if (bSuccess)
  {
    // Parse Header to get info if request was successfull
    m_HeaderResponse.parse(m_sHeader);
  }
  return bSuccess;
}

bool CcHttpClient::receiveChunked()
{
  bool bRet = false;
  CcByteArray oBuffer;
  size_t uiLeftLine = 0;
  size_t uiLastReadSize = 0;
  if (m_oBuffer.size() > 0)
  {
    oBuffer = CCMOVE(m_oBuffer);
    uiLastReadSize = oBuffer.size();
    m_oBuffer.clear();
  }
  oBuffer.resize(MAX_TRANSER_BUFFER);
  do
  {
    if (uiLeftLine == 0)
    {
      size_t uiPos = oBuffer.find(CcHttpGlobalStrings::EOL, 0, uiLastReadSize);
      if (uiPos < uiLastReadSize)
      {
        CcString sLength(oBuffer.getArray(), uiPos);

        size_t uiLengthPos = uiPos + CcHttpGlobalStrings::EOL.length();
        oBuffer.move( 0, 
                      uiLengthPos,
                      uiLastReadSize - uiLengthPos);

        uiLastReadSize -= uiLengthPos;
        bool bOk;
        uiLeftLine = static_cast<size_t>(sLength.toUint64(&bOk, 16));
        if (uiLeftLine == 0)
        {
          if(uiLastReadSize == 0)
          {
            // We have read 0 from an invalid string, so more data may be available.
            if(sLength.length() != 0)
            {
              uiLastReadSize = m_Socket.readArray(oBuffer, false);
              bRet = false;
              //uiLeftLine     = oBuffer.find(CcHttpGlobalStrings::EOL, 0, uiLastReadSize);
            }
            else
            {
              bRet = true;
            }
          }
        }
      }
      else if(uiLastReadSize > 0)
      {
        size_t uiTempSize = m_Socket.read(oBuffer.getArray() + uiLastReadSize, oBuffer.size()-uiLastReadSize);
        if(uiTempSize <= oBuffer.size()-uiLastReadSize)
        {
          uiLastReadSize += uiTempSize;
        }
        else
        {
          // Escape while and set error return
          uiLastReadSize = SIZE_MAX;
          bRet = false;
        }
      }
      else
      {
        uiLastReadSize = m_Socket.readArray(oBuffer, false);
      }
    }
    else
    {
      if (uiLastReadSize <= uiLeftLine)
      {
        m_oBuffer.append(oBuffer, uiLastReadSize);
        uiLeftLine -= uiLastReadSize;
        uiLastReadSize = m_Socket.readArray(oBuffer, false);
      }
      else
      {
        m_oBuffer.append(oBuffer, uiLeftLine);

        oBuffer.move(0, uiLeftLine, uiLastReadSize - uiLeftLine);

        uiLastReadSize -= uiLeftLine;
        uiLeftLine = 0;
        if (oBuffer.find(CcHttpGlobalStrings::EOL) == 0)
        {
          uiLastReadSize -= CcHttpGlobalStrings::EOL.length();
          oBuffer.move(0, CcHttpGlobalStrings::EOL.length(), uiLastReadSize);
          if (uiLastReadSize == 0)
          {
            uiLastReadSize = m_Socket.readArray(oBuffer, false);
          }
        }
        else
        {
          bRet = false;
        }
      }
    }
  } while ( bRet == false &&
            uiLastReadSize <= oBuffer.size());
  return bRet;
}
