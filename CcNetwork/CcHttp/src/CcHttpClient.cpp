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
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcHttpClient
 */
#include "CcHttpClient.h"
#include "CcKernel.h"
#include "CcHttpResponse.h"
#include "CcHttpConstStrings.h"
#ifdef CCSSL_ENABLED
  #include "CcSslSocket.h"
#endif

#define MAX_TRANSER_BUFFER 10000

CcHttpClient::CcHttpClient(void) :
  m_uiRetries(s_Retries),
  m_Socket(0),
  m_Output(0),
  m_Done(false)
{
  m_WD = CcKernel::getWorkingDir();
}

CcHttpClient::~CcHttpClient( void ) 
{
}


void CcHttpClient::setUrl(const CcUrl& Url)
{
  m_HeaderRequest.url() = Url;
  // Check for Port-Settings
  if (m_HeaderRequest.url().getPort()== 0)
  {
    // Search for default ports
    if (m_HeaderRequest.url().getProtocol() == "http")
    {
      m_HeaderRequest.url().setPort(80);
    }
    else if (m_HeaderRequest.url().getProtocol() == "https")
    {
      m_HeaderRequest.url().setPort(443);
    }
    else
    {
      // No known protocol found, use default port 80
      m_HeaderRequest.url().setPort(80);
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
  if (m_HeaderRequest.url().isUrl())
  {
    // create a request package
    m_HeaderRequest.setPath(m_HeaderRequest.url().getPath());
    // set target host from url
    m_HeaderRequest.setHost(m_HeaderRequest.url().getHostname());
    // get generated request back to String
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
        // write request to host
        m_Socket->write(httpRequest.getCharString(), httpRequest.length());
        size_t rec;
        size_t posDataBegin = SIZE_MAX;
        char receive[MAX_TRANSER_BUFFER];
        // Receive Server-Data
        rec = m_Socket->read(receive, sizeof(receive));
        // while no more to read, read from server
        while (rec && rec != SIZE_MAX)
        {
          // append data to buffer
          m_Buffer.append(receive, rec);
          // receive next data
          rec = m_Socket->read(receive, MAX_TRANSER_BUFFER);
        }
        // search for Header End
        rec = m_Buffer.find(CcHttpConstStrings::EOLSeperator);
        if (rec == SIZE_MAX)
        {
          rec = m_Buffer.find(CcHttpConstStrings::EOLSeperatorCompatible);
          if (rec != SIZE_MAX)
            posDataBegin = rec + 4;
        }
        else{
          posDataBegin = rec + 2;
        }
        // check if Header end was found and Data beginning point is found
        if (posDataBegin < m_Buffer.size())
        {
          CcString HeaderString;
          // Extract HeaderString
          HeaderString.append(m_Buffer, 0, rec);
          // Remove Header from Buffer to get raw Data String
          m_Buffer.remove(0, posDataBegin);
          // Parse Header to get info if request was successfull
          m_HeaderResponse.parse(HeaderString);
        }
        if (m_HeaderResponse.m_Header.HTTP.toUint16() < 300)
        {
          bRet = true;
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
  if (m_HeaderRequest.url().isUrl())
  {
    m_HeaderRequest.setPath(m_HeaderRequest.url().getPath());
    m_HeaderRequest.setHost(m_HeaderRequest.url().getHostname());
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
        m_Socket->write(httpRequest.getCharString(), httpRequest.length());
        size_t rec;
        char receive[1024];
        rec = m_Socket->read(receive, 1024);
        while (rec && rec != SIZE_MAX)
        {
          m_Buffer.append(receive, rec);
          rec = m_Socket->read(receive, 1024);
        }
        rec = m_Buffer.find(CcHttpConstStrings::EOLSeperator);
        if (rec == SIZE_MAX)
        {
          rec = m_Buffer.find(CcHttpConstStrings::EOLSeperatorCompatible);
        }
        if (rec != SIZE_MAX)
        {
          CcString HeaderString;
          HeaderString.append(m_Buffer, 0, rec);
          m_Buffer.remove(0, rec + 2);
          if (m_Buffer.at(0) == '\r')
            m_Buffer.remove(0, 2);
          m_HeaderResponse.parse(HeaderString);
          size_t contentLength = m_HeaderResponse.m_Header.ContentLength.toUint32();
          if (contentLength > m_Buffer.size() && contentLength > 0)
          {
            m_Buffer.remove(contentLength, m_Buffer.size() - contentLength);
          }
        }
        if (m_HeaderResponse.m_Header.HTTP.toUint16() == 200)
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
  m_HeaderRequest.setPath(m_HeaderRequest.url().getPath());
  m_HeaderRequest.setHost(m_HeaderRequest.url().getHostname());
  m_HeaderRequest.setRequestType(EHttpRequestType::PostUrlEnc);
  // @todo add post-data to header


  if (connectSocket())
  {
    CcByteArray oData;
    if (m_sRequestString.length() > 0)
    {
      oData.append( m_sRequestString.getByteArray() );
    }
    m_HeaderRequest.setContentSize(oData.size());
    CcString sRequest(m_HeaderRequest.getHeader());
    sRequest << CcHttpConstStrings::EOL;
    sRequest << oData;
    m_Socket->writeArray(sRequest.getByteArray());
    CcByteArray oHeaderData;
    size_t uiPos      = 0;
    size_t uiReadData = 0;
    do
    {
      CcByteArray oBuffer(1024); // @todo magic number
      uiReadData = m_Socket->readArray(oBuffer);
      uiPos = oBuffer.find(CcHttpConstStrings::EOLSeperator);
      size_t uiSeperatorSize = 0;
      if (uiPos == SIZE_MAX)
      {
        uiPos = oBuffer.find(CcHttpConstStrings::EOLSeperatorCompatible);
        if(uiPos != SIZE_MAX)
          uiSeperatorSize = CcHttpConstStrings::EOLSeperatorCompatible.length();
      }
      else
      {
        uiSeperatorSize = CcHttpConstStrings::EOLSeperator.length();
      }
      if (uiPos == SIZE_MAX)
      {
        oHeaderData.append(oBuffer);
      }
      else
      {
        oHeaderData.append(oBuffer.getArray(), uiPos);
        oBuffer.remove(0, uiPos + uiSeperatorSize);
        m_Buffer.append(oBuffer);
      }
    } while (uiReadData > 0 && uiPos < SIZE_MAX);     // @todo remove SIZE_MAX with a max transfer size
    while (uiReadData > 0 && uiReadData < SIZE_MAX)   // @todo remove SIZE_MAX with a max transfer size
    {
      CcByteArray oBuffer(1024); // @todo magic number
      uiReadData = m_Socket->readArray(oBuffer);
      m_Buffer.append(oBuffer);
    }
    return true;
  }
  return false;
}

bool CcHttpClient::execPostMultipart()
{
  bool bRet = false;
  m_HeaderRequest.setPath(m_HeaderRequest.url().getPath());
  m_HeaderRequest.setHost(m_HeaderRequest.url().getHostname());
  CcString BoundaryName     ("----------ABCDEFG");
  CcString BoundaryNameBegin("--" + BoundaryName + CcHttpConstStrings::EOLSeperator);
  CcString BoundaryNameEnd  ("--" + BoundaryName + "--" + CcHttpConstStrings::EOL);
  m_HeaderRequest.setRequestType(EHttpRequestType::PostMultip);
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
    sFileContent.append("\"" + CcHttpConstStrings::EOL);
    sFileContent.append("Content-Type: application/octet-stream" + CcHttpConstStrings::EOL + CcHttpConstStrings::EOL);
    fileContent.append(sFileContent);
    contentSize += sFileContent.length()+2;
  }
  for (const CcStringPair& sRequesData : m_oRequestData)
  {
    CcString sParam(BoundaryNameBegin);
    sParam.append("Content-Disposition: form-data; name=\"");
    sParam.append(sRequesData.getKey());
    sParam.append("\"" + CcHttpConstStrings::EOL + CcHttpConstStrings::EOL);
    sParam.append(sRequesData.getValue());
    postParam.append(sParam);
    contentSize += sParam.length()+2;
  }
  postParam.append(BoundaryNameEnd);
  contentSize += BoundaryNameEnd.length();
  m_HeaderRequest.setContentSize(contentSize);
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
      if (m_Socket->connect(m_HeaderRequest.url().getHostname(), m_HeaderRequest.url().getPortString()))
      {
        m_Socket->write(Header.getCharString(), Header.length());
      }
      for (size_t i = 0; i < m_oRequestFiles.size(); i++)
      {
        m_Socket->write(fileContent.at(i).getCharString(), fileContent.at(i).length());
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
              readLeft = m_Socket->write(buf, read);
              while ((read != SIZE_MAX) && (readLeft != read))
                readLeft += m_Socket->write(buf, read - readLeft);
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
        m_Socket->write(CcHttpConstStrings::EOL.getCharString(), CcHttpConstStrings::EOL.length());
      }
      size_t read;
      for (size_t i = 0; i < postParam.size(); i++)
      {
        m_Socket->write(postParam.at(i).getCharString(), postParam.at(i).length());
        m_Socket->write(CcHttpConstStrings::EOL.getCharString(), CcHttpConstStrings::EOL.length());
      }
      bool bDone = false;
      char buf[1024];
      while (!bDone)
      {
        read = m_Socket->read(buf, 1024);
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

bool CcHttpClient::isDone(void)
{
  return m_Done;
}

void CcHttpClient::setOutputDevice(CcIODevice*output)
{
  m_Output = output;
}

CcByteArray& CcHttpClient::getByteArray(void)
{
  return m_Buffer;
}

void CcHttpClient::run(void)
{
}

bool CcHttpClient::connectSocket(void)
{
  CcString sPort = m_HeaderRequest.url().getPortString();
  if (m_HeaderRequest.url().getProtocol() == "https")
  {
    if (sPort.length() == 0)
      sPort = "443";
#ifdef CCSSL_ENABLED
    m_Socket = new CcSslSocket();
    static_cast<CcSslSocket*>(m_Socket)->initClient();
#else
    m_Socket = CcKernel::getSocket(ESocketType::TCP);
#endif
  }
  else
  {
    if (sPort.length() == 0)
      sPort = "80";
    m_Socket = CcKernel::getSocket(ESocketType::TCP);
  }
  if(m_Socket)
    return m_Socket->connect(m_HeaderRequest.url().getHostname(), sPort);
  return true;
}

void CcHttpClient::closeSocket(void)
{
  if (m_Socket)
  {
    m_Socket->close();
    m_Socket = nullptr;
  }
}
