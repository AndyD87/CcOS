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
 * @par       Web: http://adirmeier.de/CcOS
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

uint16 CcTftpServerWorker::s_uiTransferId = 0;

CcTftpServerWorker::CcTftpServerWorker(CcByteArray* inData, CcSocket *oSocket, CcTftpServer *oIncomeServer) :
m_Socket(oSocket),
m_Server(oIncomeServer),
m_InData(inData)
{
}

CcTftpServerWorker::~CcTftpServerWorker(void)
{
  if (m_Socket != nullptr)
  {
    CCMONITORDELETE(m_Socket); delete m_Socket;
  }
}

void CcTftpServerWorker::run()
{
  ETftpServerCommands uiOpCode = static_cast<ETftpServerCommands>((*m_InData)[0] << 8 | (*m_InData)[1]);
  m_InData->remove(0, 2);
  switch (uiOpCode)
  {
    case ETftpServerCommands::RRQ:
      CCVERBOSE("TftpServer Read Request");
      runFileUpload();
      break;
    case ETftpServerCommands::WRQ:
      CCVERBOSE("TftpServer Write Request");
      runFileUpload();
      break;
    case ETftpServerCommands::DATA:
      CCVERBOSE("TftpServer Data");
      parseRequest(*m_InData);
      break;
    case ETftpServerCommands::ACK:
      CCVERBOSE("TftpServer Acknowledged");
      parseRequest(*m_InData);
      break;
    case ETftpServerCommands::ERROR:
      CCVERBOSE("TftpServer Error");
      parseRequest(*m_InData);
      break;
    case ETftpServerCommands::OACK:
      CCVERBOSE("TftpServer Option acknowledged");
      parseRequest(*m_InData);
      break;
    default:
      CCERROR("Wront TFTP-Command received");
  }
  CCMONITORDELETE(m_InData); delete m_InData;
}

void CcTftpServerWorker::parseRequest(const CcString& sRequest)
{
  CcByteArray oSendOACK;
  CcStringList oCommandList(sRequest.split('\0'));
  if (oCommandList.size() > 1)
  {
    m_sFileName = m_Server->getRootDir();
    oSendOACK.append('\0');
    oSendOACK.append((char) ETftpServerCommands::OACK);
    m_sFileName.appendPath(oCommandList[0].trim());
    if (oCommandList[1].compare("octet", ESensitivity::CaseInsensitiv))
    {
      m_eTransferType = ETftpServerTransferType::octet;
    }
    for (size_t i = 0; i < oCommandList.size(); i++)
    {
      if (oCommandList[i].compare("blksize", ESensitivity::CaseInsensitiv))
      {
        oSendOACK.append(oCommandList[i].getByteArray());
        oSendOACK.append('\0');
        i++;
        if (i < oCommandList.size())
        {
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_uiBlockSize = oCommandList[i].toUint16();
        }
      }

      if (oCommandList[i].compare("tsize", ESensitivity::CaseInsensitiv))
      {
        oSendOACK.append(oCommandList[i].getByteArray());
        oSendOACK.append('\0');
        i++;
        if (i < oCommandList.size())
        {
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_uiTSize = oCommandList[i].toUint16();
        }
      }

      if (oCommandList[i].compare("timeout", ESensitivity::CaseInsensitiv))
      {
        oSendOACK.append(oCommandList[i].getByteArray());
        oSendOACK.append('\0');
        i++;
        if (i < oCommandList.size())
        {
          oSendOACK.append(oCommandList[i].getByteArray());
          oSendOACK.append('\0');
          m_uiTimeout = oCommandList[i].toUint16();
        }
      }
    }
  }
  if (oSendOACK.size() > 0)
  {
    m_Socket->writeArray(oSendOACK);
  }
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
  parseRequest(*m_InData);
  m_uiTransferId = getNewTransferId();
  CcFile oFile(m_sFileName);
  if (oFile.open(EOpenFlags::Read))
  {
    CcByteArray oData(m_uiBlockSize);
    oFile.readArray(oData);
    while (oData.size() != 0)
    {
      if (!sendBlock(oData))
      {
        sendError(ETftpServerErrors::Termination);
        break;
      }
      oData.resize(m_uiBlockSize);
      oFile.readArray(oData);
    }
    if (oData.size() != 0)
    {
      sendError(ETftpServerErrors::IllegalOperation);
    }
    oFile.close();
  }
  else
  {
    sendError(ETftpServerErrors::FileNotFound);
  }
  m_Socket->close();
  //cmake_install.cmake
}


void CcTftpServerWorker::sendError(ETftpServerErrors eErrorCode)
{
  CcByteArray oSendData;
  switch (eErrorCode)
  {
    case ETftpServerErrors::FileNotFound:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::AccessViolation:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::DiskFull:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::IllegalOperation:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
    case ETftpServerErrors::UnknownTransfer:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    case ETftpServerErrors::FileAlreadyExists:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
    case ETftpServerErrors::NoSuchUser:
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
      oSendData.append('\0');
      break;
    default: // ETftpServerErrors::Termination and rest.
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerCommands::ERROR));
      oSendData.append('\0');
      oSendData.append(static_cast<char>(ETftpServerErrors::FileNotFound));
      oSendData.append(STftpServerErrorMessages::FileNotFound.getCharString(), STftpServerErrorMessages::FileNotFound.length());
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
  oHeaderData.append(static_cast<char>(m_uiBlockNr >> 8));
  oHeaderData.append(static_cast<char>(m_uiBlockNr & 0x00ff));
  oHeaderData.append(oData);
  m_Socket->writeArray(oHeaderData);
  oHeaderData.resize(1024); // @todo Magic number
  m_Socket->readArray(oHeaderData);
  if (oHeaderData.size() > 3)
  {
    ETftpServerCommands uiOpCode = static_cast<ETftpServerCommands>(oHeaderData[0] << 8 | oHeaderData[1]);
    uint16 uiBlockNr = static_cast<uint16>(oHeaderData[2] << 8 | oHeaderData[3]);
    if (uiOpCode == ETftpServerCommands::ACK &&
        uiBlockNr == m_uiBlockNr)
    {
      bRet = true;
    }
  }
  m_uiBlockNr++;
  return bRet;
}