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
#include "CcTftpGlobals.h"
#include "CcFile.h"

class CcTftpServerWorkerPrivate
{
public:
  CcFile oFile;
  static uint16 s_uiTransferId;
  uint16        m_uiTransferId;
  size_t        m_uiBlockNr = 1;
  size_t        m_uiBlockEndNr = 0;
  size_t        m_uiBlockSize = 512;
  size_t        m_uiWindowSize = 1;
  size_t        m_uiTimeout = 0;
  size_t        m_uiTSize = 0;
  size_t        m_uiRetransmissions = 0;
  CcString      m_sFileName;
  bool          m_bOverSize = false;
  ETftpServerTransferType m_eTransferType;
};

uint16 CcTftpServerWorkerPrivate::s_uiTransferId = 0;

CcTftpServerWorker::CcTftpServerWorker(CcByteArray* inData, CcSocket *oSocket, CcTftpServerConfigHandle hServerConfig) :
  m_pSocket(oSocket),
  m_InData(inData),
  m_hServerConfig(hServerConfig)
{
  m_pPrivate = new CcTftpServerWorkerPrivate();
  CCMONITORNEW(m_pPrivate);
}

CcTftpServerWorker::~CcTftpServerWorker(void)
{
  if (m_pSocket != nullptr)
  {
    CCMONITORDELETE(m_pSocket); 
    delete m_pSocket;
  }
  if (m_InData != nullptr)
  {
    CCMONITORDELETE(m_InData);
    delete m_InData;
  }
  if (m_pPrivate != nullptr)
  {
    CCMONITORDELETE(m_pPrivate);
    delete m_pPrivate;
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
        if (m_hServerConfig->isReadEnabled())
        {
          runFileUpload();
        }
        else
        {
          CCDEBUG("  Read blocked, read is disabled");
          sendError(ETftpServerErrors::AccessViolation);
        }
        break;
      case ETftpServerCommands::WRQ:
        CCDEBUG("TftpServer Write Request");
        if (m_hServerConfig->isWriteEnabled())
        {
          runFileDownload();
        }
        else
        {
          CCDEBUG("  Write blocked, write is disabled");
          sendError(ETftpServerErrors::AccessViolation);
        }
        break;
      case ETftpServerCommands::DATA:
        CCDEBUG("TftpServer Data");
      case ETftpServerCommands::ACK:
        CCDEBUG("TftpServer Acknowledged");
      case ETftpServerCommands::ERROR:
        CCDEBUG("TftpServer Error");
      case ETftpServerCommands::OACK:
        CCDEBUG("TftpServer Option acknowledged");
      default:
        CCERROR("Wrong TFTP-Command received");
        sendError(ETftpServerErrors::IllegalOperation);
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
    m_pPrivate->m_sFileName = m_hServerConfig->getRootDir();
    oSendOACK.append('\0');
    oSendOACK.append((char) ETftpServerCommands::OACK);
    CcString sTempPath;
    sTempPath.setOsPath(oCommandList[0].trim());
    if (sTempPath.startsWith("/"))
      sTempPath = sTempPath.substr(1);
    m_pPrivate->m_sFileName.appendPath(sTempPath);
    CCDEBUG("File for upload: " + m_pPrivate->m_sFileName);
    if (oCommandList[1].compare("octet", ESensitivity::CaseInsensitiv))
    {
      m_pPrivate->m_eTransferType = ETftpServerTransferType::octet;
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
          m_pPrivate->m_uiBlockSize = oCommandList[i].toUint16();
        }
      }

      else if (oCommandList[i].compare("tsize", ESensitivity::CaseInsensitiv))
      {
        if (i < oCommandList.size())
        {
          m_pPrivate->oFile = CcFile(m_pPrivate->m_sFileName);
          if (m_pPrivate->oFile.exists())
          {
            uiOptionsToAck++;
            m_pPrivate->m_uiTSize = m_pPrivate->oFile.size();
            CcString sFileSize = CcString::fromNumber(m_pPrivate->m_uiTSize);
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
          m_pPrivate->m_uiTimeout = oCommandList[i].toUint16();
        }
      }

      else if (oCommandList[i].compare("windowsize", ESensitivity::CaseInsensitiv))
      {
        if (i + 1 < oCommandList.size())
        {
          uiOptionsToAck++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          i++;
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_pPrivate->m_uiWindowSize = oCommandList[i].toUint16();
        }
      }
    }
  }
  if (uiOptionsToAck > 0)
  {
    bRet = false;
    m_pSocket->writeArray(oSendOACK);
    CcByteArray oArray(1024);
    m_pSocket->readArray(oArray, true);
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
  if (bRet == true)
  {
    m_pPrivate->m_uiBlockEndNr = m_pPrivate->m_uiTSize / m_pPrivate->m_uiBlockSize;
    if (m_pPrivate->m_uiTSize % m_pPrivate->m_uiBlockSize != 0)
    {
      m_pPrivate->m_uiBlockEndNr++;
    }
    if(m_pPrivate->m_uiBlockEndNr > UINT16_MAX)
    {
      m_pPrivate->m_bOverSize = true;
    }
  }
  return bRet;
}

uint16 CcTftpServerWorker::getNewTransferId()
{
  return (CcTftpServerWorkerPrivate::s_uiTransferId++);
}

void CcTftpServerWorker::runFileDownload()
{
  parseRequest(*m_InData);
  m_pPrivate->m_uiTransferId = getNewTransferId();
}

void CcTftpServerWorker::runFileUpload()
{
  if (parseRequest(*m_InData))
  { 
    m_pPrivate->m_uiTransferId = getNewTransferId();
    m_pPrivate->oFile = CcFile(m_pPrivate->m_sFileName);
    CCDEBUG("TftpServer: FileUpload: " + m_pPrivate->m_sFileName);
    if (m_pPrivate->oFile.open(EOpenFlags::Read | EOpenFlags::ShareRead))
    {
      while (sendNextWindow());
      m_pPrivate->oFile.close();
    }
    else
    {
      sendError(ETftpServerErrors::FileNotFound);
    }
    m_pSocket->close();
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
      oSendData.append(CcTftpGlobals::ErrorMsgs::FileNotFound.getCharString(), CcTftpGlobals::ErrorMsgs::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::AccessViolation:
      CCERROR("TftpServer: AccessViolation");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::AccessViolation));
      oSendData.append(CcTftpGlobals::ErrorMsgs::AccessViolation.getCharString(), CcTftpGlobals::ErrorMsgs::AccessViolation.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::DiskFull:
      CCERROR("TftpServer: DiskFull");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::DiskFull));
      oSendData.append(CcTftpGlobals::ErrorMsgs::DiskFull.getCharString(), CcTftpGlobals::ErrorMsgs::DiskFull.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::IllegalOperation:
      CCERROR("TftpServer: IllegalOperation");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::IllegalOperation));
      oSendData.append(CcTftpGlobals::ErrorMsgs::IllegalOperation.getCharString(), CcTftpGlobals::ErrorMsgs::IllegalOperation.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::UnknownTransfer:
      CCERROR("TftpServer: UnknownTransfer");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::UnknownTransfer));
      oSendData.append(CcTftpGlobals::ErrorMsgs::UnknownTransfer.getCharString(), CcTftpGlobals::ErrorMsgs::UnknownTransfer.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::FileAlreadyExists:
      CCERROR("TftpServer: FileAlreadyExists");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileAlreadyExists));
      oSendData.append(CcTftpGlobals::ErrorMsgs::FileAlreadyExists.getCharString(), CcTftpGlobals::ErrorMsgs::FileAlreadyExists.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::NoSuchUser:
      CCERROR("TftpServer: NoSuchUser");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::NoSuchUser));
      oSendData.append(CcTftpGlobals::ErrorMsgs::NoSuchUser.getCharString(), CcTftpGlobals::ErrorMsgs::NoSuchUser.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::Termination:
      CCERROR("TftpServer: NoSuchUser");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::Termination));
      oSendData.append(CcTftpGlobals::ErrorMsgs::Termination.getCharString(), CcTftpGlobals::ErrorMsgs::Termination.length());
      oSendData.append('\0');
      break;
    default: // ETftpServerErrors::Termination and rest.
      CCERROR("TftpServer: Unknown");
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::Unknown));
      oSendData.append(CcTftpGlobals::ErrorMsgs::Unknown.getCharString(), CcTftpGlobals::ErrorMsgs::Unknown.length());
      oSendData.append('\0');
      break;
  }
  m_pSocket->writeArray(oSendData);
}

bool CcTftpServerWorker::sendNextWindow()
{
  bool bRet = false;
  if (m_pPrivate->m_uiRetransmissions < m_hServerConfig->getMaxRetransMissions())
  {
    uint64 uiFilePointer = (m_pPrivate->m_uiBlockNr - 1) * m_pPrivate->m_uiBlockSize;
    if (m_pPrivate->oFile.setFilePointer(uiFilePointer))
    {
      CcByteArray oData(m_pPrivate->m_uiBlockSize);
      m_pPrivate->oFile.readArray(oData, true);
      if (oData.size() != 0)
      {
        bRet = sendBlock(oData);
        if (oData.size() != m_pPrivate->m_uiBlockSize)
          bRet = false;
      }
    }
  }
  return bRet;
}

bool CcTftpServerWorker::sendBlock(const CcByteArray& oData)
{
  bool bRet = false;
  CcByteArray oHeaderData;
  oHeaderData.append('\0');
  oHeaderData.append(static_cast<char>(ETftpServerCommands::DATA));
  oHeaderData.append(static_cast<char>((static_cast<uint16>(m_pPrivate->m_uiBlockNr) & 0xff00) >> 8));
  oHeaderData.append(static_cast<char>( static_cast<uint16>(m_pPrivate->m_uiBlockNr) & 0x00ff));
  oHeaderData.append(oData);
  //CCDEBUG("Write Data with size: " + CcString::fromNumber(oHeaderData.size()));
  if (m_pSocket->writeArray(oHeaderData))
  {
    bRet = true;
    bool bTransferDone = false;
    if (static_cast<uint16>(m_pPrivate->m_uiBlockNr) % m_pPrivate->m_uiWindowSize == 0 ||
      oData.size() != m_pPrivate->m_uiBlockSize)
    {
      m_pSocket->readArray(oHeaderData, true);
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
        else if (uiBlockNr != static_cast<uint16>(m_pPrivate->m_uiBlockNr))
        {
          bTransferDone = false;
          CCDEBUG("Wrong Block acked: " + CcString::fromNumber(uiBlockNr) + "!=" + CcString::fromNumber(static_cast<uint16>(m_pPrivate->m_uiBlockNr)));
          m_pPrivate->m_uiBlockNr = (m_pPrivate->m_uiBlockNr & (~UINT16_MAX)) + uiBlockNr + 1;
          m_pPrivate->m_uiRetransmissions++;
        }
        else
        {
          m_pPrivate->m_uiRetransmissions = 0;
          bTransferDone = true;
        }
      }
      else
      {
        bTransferDone = false;
        CCDEBUG("Block not acked: " + CcString::fromNumber(oHeaderData.size()));
      }
    }
    else
    {
      bTransferDone = true;
    }
    if (bTransferDone)
    {
      m_pPrivate->m_uiBlockNr++;
    }
  }
  return bRet;
}
