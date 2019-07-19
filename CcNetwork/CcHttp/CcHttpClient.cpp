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
 * @par       Web:      http://coolcow.de/projects/CcOS
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

#define MAX_TRANSER_BUFFER 10000

uint16 CcHttpClient::s_uiRetries = 5;

CcHttpClient::CcHttpClient() :
  IThread("CcHttpClient"),
  m_uiRetries(s_uiRetries),
  m_Socket(0),
  m_Output(0),
  m_Done(false)
{
  m_WD = CcKernel::getWorkingDir();
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


bool CcHttpClient::execGet()
{
  // @todo add post-data to header
  bool bRet = false;
  // Check if url is a real url
  if (m_oUrl.isUrl())
  {
    // create a request package
    m_HeaderRequest.setRequestType(EHttpRequestType::Get, m_oUrl.getPath());
    // set target host from url
    m_HeaderRequest.setHost(m_oUrl.getHostname());
    // get generated request back to String
    CcString httpRequest(m_HeaderRequest.getHeader());
    // Start connection to host
    uint16 retryCounter = 0;
    // check if job is done and limit is not reached
    while (bRet == false && (retryCounter < m_uiRetries || retryCounter == 0))
    {
      m_Buffer.clear();
      retryCounter++;
      // check if connection is working
      if (connectSocket())
      {
        CcStatus oStatus;
        if (oStatus)
        {
          if (m_Socket.write(httpRequest.getCharString(), httpRequest.length()) != httpRequest.length())
          {
            CCDEBUG("Failed to write header");
            oStatus = false;
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

        if (oStatus)
        {
          size_t rec;
          if (m_HeaderResponse.getTransferEncoding().isChunked())
          {
            receiveChunked();
          }
          else
          {
            CcByteArray oBuffer(MAX_TRANSER_BUFFER);
            uint64 uiDataSize = m_HeaderResponse.getContentLength();
            while (m_Buffer.size() < uiDataSize)
            {
              // receive next data
              rec = m_Socket.readArray(oBuffer, false);
              if (rec && rec != SIZE_MAX)
              {
                // append data to buffer
                m_Buffer.append(oBuffer.getArray(), rec);
              }
            }
          }
          if (m_HeaderResponse.getHttpCode() < 400)
          {
            bRet = true;
          }
        }
        closeSocket();
      }
    }
  }
  return bRet;
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
        char receive[1024];
        rec = m_Socket.read(receive, 1024);
        while (rec && rec != SIZE_MAX)
        {
          m_Buffer.append(receive, rec);
          rec = m_Socket.read(receive, 1024);
        }
        rec = m_Buffer.find(CcHttpGlobalStrings::EOLSeperator);
        if (rec != SIZE_MAX)
        {
          CcString HeaderString;
          HeaderString.append(m_Buffer, 0, rec);
          m_Buffer.remove(0, rec + 2);
          if (m_Buffer.at(0) == '\r')
            m_Buffer.remove(0, 2);
          m_HeaderResponse.parse(HeaderString);
          size_t contentLength = static_cast<size_t>(m_HeaderResponse.getContentLength());
          if (contentLength > m_Buffer.size() && contentLength > 0)
          {
            m_Buffer.remove(contentLength, m_Buffer.size() - contentLength);
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
  return bRet;
}

bool CcHttpClient::execPost()
{
  m_Buffer.clear();
  m_HeaderRequest.setRequestType(EHttpRequestType::Post, m_oUrl.getPath());
  m_HeaderRequest.setHost(m_oUrl.getHostname());
  // @todo add post-data to header


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
        m_Buffer.append(oBuffer.getArray() + uiPos, uiReadData - uiPos );
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
      while (uiReadData > 0 && uiReadData < SIZE_MAX &&
        m_Buffer.size() < m_HeaderResponse.getContentLength())   // @todo remove SIZE_MAX with a max transfer size
      {
        uiReadData = m_Socket.readArray(oBuffer, false);
        m_Buffer.append(oBuffer, uiReadData);
      }
    }
    return true;
  }
  return false;
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
          char buf[1024];
          while (!bRet)
          {
            read = file.read(buf, 1024);
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
      char buf[1024];
      while (!bDone)
      {
        read = m_Socket.read(buf, 1024);
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
  return false;
}

bool CcHttpClient::isDone()
{
  return m_Done;
}

void CcHttpClient::setOutputDevice(IIoDevice*output)
{
  m_Output = output;
}

CcByteArray& CcHttpClient::getByteArray()
{
  return m_Buffer;
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
    CCNEW(m_Socket, CcSslSocket);
    static_cast<CcSslSocket*>(m_Socket.getRawSocket())->initClient();
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
  bool bSuccess = false;
  size_t rec;
  // write request to host
  size_t posDataBegin = 0;
  m_sHeader.clear();
  // while no more to read, read from server
  do
  {
    m_sHeader.reserve(MAX_TRANSER_BUFFER);
    // receive next data
    rec = m_Socket.read(m_sHeader.getCharString(), m_sHeader.getBufferSize());
    if (rec && rec != SIZE_MAX)
    {
      size_t uiPos = m_sHeader.find(CcHttpGlobalStrings::EOLSeperator);
      if (uiPos != SIZE_MAX)
      {
        posDataBegin = uiPos + CcHttpGlobalStrings::EOLSeperator.length();
      }
    }
  } while (rec && rec != SIZE_MAX && posDataBegin == 0);

  // check if Header end was found and Data beginning point is found
  if (posDataBegin > 0 &&
      posDataBegin <= m_sHeader.length())
  {
    bSuccess = true;
    m_Buffer.clear();
    m_Buffer.append(m_sHeader.getCharString() + posDataBegin, rec - posDataBegin);

    m_sHeader.remove(posDataBegin, m_sHeader.length() - posDataBegin);

    // Parse Header to get info if request was successfull
    m_HeaderResponse.parse(m_sHeader);
  }
  return bSuccess;
}

bool CcHttpClient::receiveChunked()
{
  bool bRet = false;
  CcByteArray oBuffer;
  if (m_Buffer.size() > 0)
  {
    oBuffer.append(m_Buffer);
    m_Buffer.clear();
  }
  size_t uiLeftLine = 0;
  bool bDone = false;
  do
  {
    if (uiLeftLine == 0)
    {
      size_t uiPos = oBuffer.find(CcHttpGlobalStrings::EOL);
      if (uiPos != SIZE_MAX)
      {
        CcString sLength(oBuffer.getArray(), uiPos);
        oBuffer.remove(0, uiPos + CcHttpGlobalStrings::EOL.length());
        bool bOk;
        uiLeftLine = static_cast<size_t>(sLength.toUint64(&bOk, 16));
        if (uiLeftLine == 0)
        {
          uiLeftLine = 100;
          bDone = true;
        }
      }
      else
      {
        oBuffer.resize(MAX_TRANSER_BUFFER);
        m_Socket.readArray(oBuffer);
      }
    }
    else
    {
      if (oBuffer.size() <= uiLeftLine)
      {
        m_Buffer.append(oBuffer);
        uiLeftLine -= oBuffer.size();
        oBuffer.clear();
        oBuffer.resize(MAX_TRANSER_BUFFER);
        m_Socket.readArray(oBuffer);
      }
      else
      {
        m_Buffer.append(oBuffer, uiLeftLine);
        oBuffer.remove(0, uiLeftLine);
        uiLeftLine = 0;
        if (oBuffer.find(CcHttpGlobalStrings::EOL) == 0)
        {
          oBuffer.remove(0, CcHttpGlobalStrings::EOL.length());
          if (oBuffer.size() == 0)
          {
            oBuffer.resize(MAX_TRANSER_BUFFER);
            m_Socket.readArray(oBuffer);
          }
        }
        else if (oBuffer.size() == 1)
        {
          oBuffer.resize(MAX_TRANSER_BUFFER);
          m_Socket.readArray(oBuffer);
          oBuffer.remove(0);
        }
        else
        {
          bDone = true;
          bRet = false;
        }
      }
    }
  } while (bDone == false);
  return bRet;
}
