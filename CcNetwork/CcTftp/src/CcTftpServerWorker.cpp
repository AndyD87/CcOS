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
 * @brief     Implementation of Class CcTftpServerWorker
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcThreadObject.h"
#include "CcTftpServerWorker.h"
#include "CcTftpServer.h"
#include "ETftpServer.h"
#include "CcFile.h"

uint16 CcTftpServerWorker::s_uiTransferId = 0;

CcTftpServerWorker::CcTftpServerWorker(CcByteArray* inData, CcSocket *oSocket, CcTftpServer *oIncomeServer) :
  m_Socket(oSocket),
  m_InData(inData),
  m_Server(oIncomeServer)
{
}

CcTftpServerWorker::~CcTftpServerWorker(void)
{
  if (m_Socket != nullptr)
  {
    CCMONITORDELETE(m_Socket); 
    delete m_Socket;
  }
  if (m_InData != nullptr)
  {
    CCMONITORDELETE(m_InData);
    delete m_InData;
  }
}

void CcTftpServerWorker::run()
{
  if (m_InData->size() > 1)
  {
    ETftpServerCommands uiOpCode = static_cast<ETftpServerCommands>((*m_InData)[0] << 8 | (*m_InData)[1]);
    m_InData->remove(0, 2);
    switch (uiOpCode)
    {
      case ETftpServerCommands::RRQ:
        CCDEBUG("TftpServer Read Request");
        runFileUpload();
        break;
      case ETftpServerCommands::WRQ:
        CCDEBUG("TftpServer Write Request");
        runFileUpload();
        break;
      case ETftpServerCommands::DATA:
        CCDEBUG("TftpServer Data");
        parseRequest(*m_InData);
        break;
      case ETftpServerCommands::ACK:
        CCDEBUG("TftpServer Acknowledged");
        parseRequest(*m_InData);
        break;
      case ETftpServerCommands::ERROR:
        CCDEBUG("TftpServer Error");
        parseRequest(*m_InData);
        break;
      case ETftpServerCommands::OACK:
        CCDEBUG("TftpServer Option acknowledged");
        parseRequest(*m_InData);
        break;
      default:
        CCERROR("Wront TFTP-Command received");
    }
  }
}

bool CcTftpServerWorker::parseRequest(const CcString& sRequest)
{
  bool bRet = false;
  uint16 uiOptionsToAck = 0;
  CcByteArray oSendOACK;
  CcStringList oCommandList(sRequest.split('\0'));
  if (oCommandList.size() > 1)
  {
    bRet = true;
    m_sFileName = m_Server->config().getRootDir();
    oSendOACK.append('\0');
    oSendOACK.append((char) ETftpServerCommands::OACK);
    CcString sTempPath;
    sTempPath.setOsPath(oCommandList[0].trim());
    if (sTempPath.startsWith("/"))
      sTempPath = sTempPath.substr(1);
    m_sFileName.appendPath(sTempPath);
    CCDEBUG("File for upload: " + m_sFileName);
    if (oCommandList[1].compare("octet", ESensitivity::CaseInsensitiv))
    {
      m_eTransferType = ETftpServerTransferType::octet;
    }
    for (size_t i = 0; i < oCommandList.size(); i++)
    {
      if (oCommandList[i].compare("blksize", ESensitivity::CaseInsensitiv))
      {
        if (i < oCommandList.size())
        {
          uiOptionsToAck++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          i++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_uiBlockSize = oCommandList[i].toUint16();
        }
      }

      else if (oCommandList[i].compare("tsize", ESensitivity::CaseInsensitiv))
      {
        if (i < oCommandList.size())
        {
          CcFile oFile(m_sFileName);
          if (oFile.exists())
          {
            uiOptionsToAck++;
            m_uiTSize = oFile.size();
            CcString sFileSize = CcString::fromNumber(m_uiTSize);
            oSendOACK.append(oCommandList[i].getByteArray());
            oSendOACK.append('\0');
            oSendOACK.append(sFileSize.getByteArray());
            oSendOACK.append('\0');
            i++;
          }
        }
      }

      else if (oCommandList[i].compare("timeout", ESensitivity::CaseInsensitiv))
      {
        if (i + 1 < oCommandList.size())
        {
          uiOptionsToAck++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          i++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_uiTimeout = oCommandList[i].toUint16();
        }
      }
    }
  }
  if (uiOptionsToAck > 0)
  {
    bRet = false;
    m_Socket->writeArray(oSendOACK);
    CcByteArray oArray(1024);
    m_Socket->readArray(oArray, true);
    if (oSendOACK.size() > 2)
    {
      ETftpServerCommands uiOpCode = static_cast<ETftpServerCommands>(oArray[0] << 8 | oArray[1]);
      if (uiOpCode != ETftpServerCommands::ACK)
      {
        CCDEBUG("Options not acked");
      }
      else
      {
        bRet = true;
      }
    }
    else
    {
      CCDEBUG("Options read failed");
    }
  }
  return bRet;
}

uint16 CcTftpServerWorker::getNewTransferId()
{
  return s_uiTransferId++;
}

void CcTftpServerWorker::runFileDownload()
{
  parseRequest(*m_InData);
  m_uiTransferId = getNewTransferId();
}

void CcTftpServerWorker::runFileUpload()
{
  if (parseRequest(*m_InData))
  { 
    m_uiTransferId = getNewTransferId();
    CcFile oFile(m_sFileName);
    CCERROR("TftpServer: FileUpload: " + m_sFileName);
    if (oFile.open(EOpenFlags::Read | EOpenFlags::ShareRead))
    {
      CcByteArray oData(m_uiBlockSize);
      oFile.readArray(oData, true);
      while (oData.size() != 0)
      {
        if (!sendBlock(oData))
        {
          break;
        }
        oData.resize(m_uiBlockSize);
        oFile.readArray(oData, true);
      }
      oFile.close();
    }
    else
    {
      sendError(ETftpServerErrors::FileNotFound);
    }
    m_Socket->close();
  }
  else
  {
    CCERROR("TftpServer: Parsing Data failed");
  }
}


void CcTftpServerWorker::sendError(ETftpServerErrors eErrorCode)
{
  CcByteArray oSendData;
  switch (eErrorCode)
  {
    case ETftpServerErrors::FileNotFound:
      CCERROR("TftpServer: FileNotFound");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::AccessViolation:
      CCERROR("TftpServer: AccessViolation");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::AccessViolation));
      oSendData.append(STftpServerErrorMessages::AccessViolation.getCharString(), STftpServerErrorMessages::AccessViolation.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::DiskFull:
      CCERROR("TftpServer: DiskFull");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::DiskFull));
      oSendData.append(STftpServerErrorMessages::DiskFull.getCharString(), STftpServerErrorMessages::DiskFull.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::IllegalOperation:
      CCERROR("TftpServer: IllegalOperation");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::IllegalOperation));
      oSendData.append(STftpServerErrorMessages::IllegalOperation.getCharString(), STftpServerErrorMessages::IllegalOperation.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::UnknownTransfer:
      CCERROR("TftpServer: UnknownTransfer");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::UnknownTransfer));
      oSendData.append(STftpServerErrorMessages::UnknownTransfer.getCharString(), STftpServerErrorMessages::UnknownTransfer.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::FileAlreadyExists:
      CCERROR("TftpServer: FileAlreadyExists");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileAlreadyExists));
      oSendData.append(STftpServerErrorMessages::FileAlreadyExists.getCharString(), STftpServerErrorMessages::FileAlreadyExists.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::NoSuchUser:
      CCERROR("TftpServer: NoSuchUser");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::NoSuchUser));
      oSendData.append(STftpServerErrorMessages::NoSuchUser.getCharString(), STftpServerErrorMessages::NoSuchUser.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::Termination:
      CCERROR("TftpServer: NoSuchUser");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::Termination));
      oSendData.append(STftpServerErrorMessages::Termination.getCharString(), STftpServerErrorMessages::Termination.length());
      oSendData.append('\0');
      break;
    default: // ETftpServerErrors::Termination and rest.
      CCERROR("TftpServer: Unknown");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::Unknown));
      oSendData.append(STftpServerErrorMessages::Unknown.getCharString(), STftpServerErrorMessages::Unknown.length());
      oSendData.append('\0');
      break;
  }
  m_Socket->writeArray(oSendData);
}

bool CcTftpServerWorker::sendBlock(const CcByteArray& oData)
{
  bool bRet = false;
  CcByteArray oHeaderData;
  oHeaderData.append('\0');
  oHeaderData.append(static_cast<char>(ETftpServerCommands::DATA));
  oHeaderData.append(static_cast<char>((m_uiBlockNr & 0xff00) >> 8));
  oHeaderData.append(static_cast<char>( m_uiBlockNr & 0x00ff));
  oHeaderData.append(oData);
  //CCDEBUG("Write Data with size: " + CcString::fromNumber(oHeaderData.size()));
  m_Socket->writeArray(oHeaderData);
  bRet = true;
  m_Socket->readArray(oHeaderData, true);
  //CCDEBUG("Received Data with size: " + CcString::fromNumber(oHeaderData.size()));
  if (oHeaderData.size() > 3)
  {
    ETftpServerCommands uiOpCode = static_cast<ETftpServerCommands>(oHeaderData[1]);
    uint16 uiBlockNr = ((0xff & static_cast<uint16>(oHeaderData[2])) << 8) | (0x00ff & static_cast<uint16>(oHeaderData[3]));
    if (uiOpCode != ETftpServerCommands::ACK)
    {
      bRet = false;
      CCDEBUG("Error during transfer of Block: " + CcString::fromNumber(oHeaderData.size()));
    }
    else if (uiBlockNr != m_uiBlockNr)
    {
      bRet = false;
      CCDEBUG("Wrong Block acked: " + CcString::fromNumber(uiBlockNr) + "!=" + CcString::fromNumber(m_uiBlockNr));
    }
  }
  else
  {
    bRet = false;
    CCDEBUG("Block not acked: " + CcString::fromNumber(oHeaderData.size()));
  }
  m_uiBlockNr++;
  return bRet;
}
