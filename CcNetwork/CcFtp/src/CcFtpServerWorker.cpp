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
 * @brief     Implementation of Class CcFtpServerWorker
 *           Protocol: http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
 */
#include "CcKernel.h"
#include "CcThreadObject.h"
#include "CcFtpServerWorker.h"
#include "CcFtpTypes.h"
#include "CcFtpServer.h"
#include "CcFileInfoList.h"
#include "stdio.h"

CcFtpServerWorker::CcFtpServerWorker(CcSocket *socket, CcFtpServer *incomeServer) :
m_Socket(socket),
m_DataPortInc(12378),
m_Server(incomeServer),
m_UserState(userNone)
{
  m_WD = CcKernel::getFileSystemManager().getWorkingDir();
}

CcFtpServerWorker::~CcFtpServerWorker(void){
  if (m_Socket != 0)
  {
    CCMONITORDELETE(m_Socket); delete m_Socket;
  }
}

void CcFtpServerWorker::run(){
  char recBuf[1024];
  size_t recSize = 0;
  CcString recStr;
  if (m_Socket != 0)
  {
    recSize = m_Socket->write((char*)"220 FTP-Server ready\r\n", 22);
    if (recSize != 0 && recSize != SIZE_MAX)
    {
      recSize = m_Socket->read(recBuf, 1024);
      while (recSize != SIZE_MAX && recSize != 0)
      {
        recStr.clear();
        recStr.append(recBuf, recSize);
        parseCommand(recStr.trim());
        recSize = m_Socket->read(recBuf, 1024);
      }
    }
  }
}

void CcFtpServerWorker::parseCommand(const CcString& sCommandLine)
{
  CcString param;
  CcString Command;
  // Search for Arguments
  size_t posParam = sCommandLine.find(" ");
  if (posParam != SIZE_MAX)
  {
    // Strip Arguments from Command
    param = sCommandLine.substr(posParam+1).trim();
    Command = sCommandLine.substr(0, posParam);
  }
  else
  {
    Command = sCommandLine.trim();
  }
  CCVERBOSE("FTP: Incoming Command " + Command);
  eFtpCommands eCmd = FTP_UNKNOWN;
  // Find Command/eNum Match
  for (size_t i=0; i < sFtpCommandListSize && eCmd == FTP_UNKNOWN; i++)
  {
    if (Command.compare(sFtpCommandList[i].strCommand, ESensitivity::CaseInsensitiv))
    {
      eCmd = sFtpCommandList[i].eCommand;
    }
  }
  switch (eCmd)
  {
    case FTP_UNKNOWN:
      CCWARNING("FTP_UNKNOWN - First Command was: " + Command);
      m_Socket->write((char*)FTP_501, sizeof(FTP_501));
      break;
    case FTP_OPTS:
      CCVERBOSE("FTP_OPTS");
      m_Socket->write((char*)"200\r\n", 5);
      break;
    case FTP_USER:
      CCVERBOSE("FTP_USER");
      doUser(param);
      break;
    case FTP_PASS:
      CCVERBOSE("FTP_PASS");
      doPass(param);
      break;
    case FTP_AUTH:
      CCVERBOSE("FTP_AUTH");
      m_Socket->write((char*)"500 Syntax error\r\n", 18);
      break;
    case FTP_SYST:
      CCVERBOSE("FTP_SYST");
      m_Socket->write((char*)"215 UNIX Type: L8\r\n", 19);
      break;
    case FTP_FEAT:
    {
      CCVERBOSE("FTP_FEAT");
      CcString sToClient;
      sToClient << "211 \r\n" << "SIZE\r\n" << "211 END\r\n";
      m_Socket->write(sToClient.getCharString(), sToClient.length());
    }
      break;
    case FTP_PWD:
    {
      CCVERBOSE("FTP_PWD");
      CcString pwd("257 \"");
      pwd.append(m_WD);
      pwd.append("\" \r\n");
      m_Socket->write(pwd.getCharString(), pwd.length());
    }
      break;
    case FTP_TYPE:
      CCVERBOSE("FTP_TYPE");
      if (param == "I")
      {
        m_TransferType = false;
        m_Socket->write((char*)"200 Type set to I\r\n", 19);
      }
      else if (param == "A")
      {
        m_TransferType = true;
        m_Socket->write((char*)"200 Type set to A\r\n", 19);
      }
      break;
    case FTP_PASV:
    {
      CCVERBOSE("FTP_PASV");
      m_Active = false;
      if ((m_DataSocket = CcKernel::getSocket(ESocketType::TCP)) != nullptr)
      {
        while(true != m_DataSocket->bind( m_DataPortInc))
        {
          if (m_DataPortInc > 20000) m_DataPortInc = 12378;
          else m_DataPortInc++;
        }
        m_DataSocket->listen();
        CcString sRet((char*)"227 Entering Passive Mode (127,0,0,1,");
        uint8 tempPort = ((m_DataPortInc) & 0xff00) >> 8;
        sRet.appendNumber(tempPort);
        tempPort = (m_DataPortInc) & 0xff;
        sRet << "," << CcString::fromNumber(tempPort);
        sRet.append(").\r\n");
        m_Socket->write(sRet.getCharString(), sRet.length());
        if (m_DataPortInc > 20000) m_DataPortInc = 12378;
        else m_DataPortInc++;
      }
      break;
    }
    case FTP_LIST:
    {
      CCVERBOSE("FTP_LIST");
      m_Socket->write((char*)"150 Opening ASCII mode data connection for /bin/ls \r\n", 53);
      acceptDataConnection();
      CcFile dir(m_WD);
      CcStringList slFiles;
      if (dir.isDir())
      {
        if (param == "-a")
          slFiles = dir.getFileList().getFormatedList(SHOW_EXTENDED | SHOW_HIDDEN);
        else if (param == "-l")
          slFiles = dir.getFileList().getFormatedList(SHOW_EXTENDED);
        else
          slFiles = dir.getFileList().getFormatedList(SHOW_EXTENDED);
        for (CcString& sFile : slFiles)
        {
          m_DataSocket->write(sFile.getCharString(), sFile.length());
        }
        CCMONITORDELETE(m_DataSocket); delete m_DataSocket;
        m_Socket->write((char*)FTP_226, sizeof(FTP_226));
      }
      else
      {
        m_Socket->write((char*)FTP_500, sizeof(FTP_500));
      }
      break;
    }
    case FTP_NLST:
    {
      CCVERBOSE("FTP_NLST");
      m_Socket->write((char*)"150 Opening ASCII mode data connection for /bin/ls \r\n", 53);
      acceptDataConnection();
      CcFile dir(m_WD);
      if (dir.isDir())
      {
        CcStringList slFiles(dir.getFileList().getFormatedList(0));
        for (CcString& sFile : slFiles)
        {
          m_DataSocket->write(sFile.getCharString(), sFile.length());
          m_DataSocket->write("\r\n", 2);
        }
        CCMONITORDELETE(m_DataSocket); delete m_DataSocket;
        m_Socket->write((char*)"226 Transfer complete. \r\n", 24);
      }
      else{
        m_Socket->write((char*)FTP_500, sizeof(FTP_500));
      }
      break;
    }
    case FTP_CWD:
    {
      CCVERBOSE("FTP_CWD");
      CcString sTemp(m_WD); 
      sTemp.appendPath(param);
      CcFile dir(sTemp);
      if (dir.isDir())
      {
        m_WD = sTemp;
        m_Socket->write("250 CWD command successful.\r\n", 29);
      }
      else{
        CcString resp( "550 " + sTemp + " No such file or directory.\r\n");
        m_Socket->write(resp.getCharString(), resp.length());
      }
      break;
    }
    case FTP_CDUP:
    {
      CCVERBOSE("FTP_CDUP");
      CcString sTemp;
      size_t pos = m_WD.findLast("/");
      // For supporting Windows DriveLetters set Working dir to ""
      if (pos == SIZE_MAX)
        sTemp = "";
      else
        sTemp = m_WD.substr(0, pos);
      CcFilePointer dir = CcKernel::getFileSystemManager().getFile(sTemp);
      if (dir->isDir())
      {
        m_WD = sTemp;
        m_Socket->write("250 CDUP command successful.\r\n", 29);
      }
      else{
        CcString resp("550 " + sTemp + " No such file or directory.\r\n");
        m_Socket->write(resp.getCharString(), resp.length());
      }
      break;
    }
    case FTP_RETR:
    {
      CCVERBOSE("FTP_RETR");
      bool bDone = false;
      CcString sRet;
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      CcFile file(sTemp);
      if (file.isFile() && file.open(EOpenFlags::Read))
      {
        m_Socket->write("150 Opening ASCII mode data connection for /bin/ls \r\n", 53);
        if(acceptDataConnection())
        {
          size_t read, readLeft;
          char buf[1024];
          while (!bDone)
          {
            read = file.read(buf, 1024);
            if (read != SIZE_MAX && read != 0)
            {
              readLeft = m_DataSocket->write(buf, read);
              while ((read != SIZE_MAX) && (readLeft != read))
                readLeft += m_DataSocket->write(buf, read - readLeft);
              if (readLeft == SIZE_MAX)
              {
                bDone = true;
                sRet = "426 Error occured on connection file \r\n";
              }
            }
            else if (read == SIZE_MAX)
            {
              bDone = true;
              sRet = "551 Error reading file from disk \r\n";
            }
            else{
              bDone = true;
              sRet = "226 Transfer complete. \r\n";
            }
          }
          file.close();
          CCMONITORDELETE(m_DataSocket); delete m_DataSocket;
        }
      }
      else{
        sRet = "551 File not exists \r\n";
      }
      m_Socket->write(sRet.getCharString(), sRet.length());
      break;
    }
    case FTP_RNFR:
    {
      CCVERBOSE("FTP_RNFR");
      m_Temp = m_WD; m_Temp.appendPath(param);
      m_Socket->write(FTP_200, sizeof(FTP_200));
      break;
    }
    case FTP_RNTO:
    {
      CCVERBOSE("FTP_RNTO");
      CcString sTemp;
      param.normalizePath();
      if (param.startWith("/"))
        sTemp = param;
      else if (param.startWith("./"))
        sTemp = m_WD + "/" + sTemp.substr(2);
      else if (param.at(1) == ':') //WindowsDrive
        sTemp = param;
      else
        sTemp = m_WD + "/" + param;
      CcFile file(m_Temp);
      if (file.isDir())
      {
        if (file.move(sTemp))
          m_Socket->write(FTP_200, sizeof(FTP_200));
        else
          m_Socket->write(FTP_500, sizeof(FTP_500));
      }
      else if (file.isFile())
      {
        if (file.move(sTemp))
          m_Socket->write(FTP_200, sizeof(FTP_200));
        else
          m_Socket->write(FTP_500, sizeof(FTP_500));
      }
      break;
    }
    case FTP_STOR:
    {
      CCVERBOSE("FTP_STOR");
      bool bDone = false;
      CcString sRet;
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      CcFile file(sTemp);
      if (file.open( EOpenFlags::Overwrite))
      {
        m_Socket->write("150 Opening ASCII mode data connection for /bin/ls \r\n", 53);
        if(acceptDataConnection())
        {
          size_t read, readLeft;
          char buf[1024];
          while (!bDone)
          {
            read = m_DataSocket->read(buf, 1024);
            if (read != SIZE_MAX && read != 0)
            {
              readLeft = file.write(buf, read);
              while ((read != SIZE_MAX) && (readLeft != read))
                readLeft += file.write(buf, read - readLeft);
              if (readLeft == SIZE_MAX)
              {
                bDone = true;
                sRet = "426 Error occured on connection file \r\n";
              }
            }
            else if (read == SIZE_MAX)
            {
              bDone = true;
              sRet = "551 Error reading fiel from socket \r\n";
            }
            else{
              bDone = true;
              sRet = "226 Transfer complete. \r\n";
            }
          }
          file.close();
          m_DataSocket->close();
          CCMONITORDELETE(m_DataSocket); delete m_DataSocket;
        }
      }
      else{
        sRet = FTP_550;
      }
      m_Socket->write(sRet.getCharString(), sRet.length());
      break;
    }
    case FTP_MKD:
    {
      CCVERBOSE("FTP_MKD");
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      if (CcKernel::getFileSystemManager().mkdir(sTemp))
      {
        sTemp = "257 \"" + sTemp + "\" created\r\n";
        m_Socket->write(sTemp.getCharString(), sTemp.length());
      }
      else
        m_Socket->write(FTP_550, sizeof(FTP_550));
      break;
    }
    case FTP_DELE:
    {
      CCVERBOSE("FTP_DELE");
      CcString sTemp = m_WD; sTemp.appendPath(param);
      if (CcKernel::getFileSystemManager().remove(sTemp))
      {
        sTemp = "257 \"" + sTemp + "\" created\r\n";
        m_Socket->write(FTP_250, sizeof(FTP_250));
      }
      else
        m_Socket->write(FTP_550, sizeof(FTP_550));
      break;
    }
    case FTP_RMD:
    {
      CCVERBOSE("FTP_RMD");
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      if (CcKernel::getFileSystemManager().remove(sTemp))
      {
        sTemp = "257 \"" + sTemp + "\" created\r\n";
        m_Socket->write(FTP_250, sizeof(FTP_250));
      }
      else
        m_Socket->write(FTP_550, sizeof(FTP_550));
      break;
    }
    case FTP_PORT:
    {
      CCVERBOSE("FTP_PORT");
      m_Active = true;
      bool bOk;
      m_PasvAddress.setIpPort(param, bOk);
      if (bOk)
      {
        m_Socket->write(FTP_501, sizeof(FTP_501));
      }
      else
      {
        m_Socket->write("200 PORT command successfull\r\n", 30);
      }
      break;
    }
    case FTP_SIZE:
    {
      CCVERBOSE("FTP_SIZE");
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      CcFile file(sTemp);
      if (file.isFile())
      {
        size_t size = file.size();
        sTemp = "213 " + CcString::fromNumber(size) + "\r\n";
      }
      else
        sTemp = FTP_550;
      m_Socket->write(sTemp.getCharString(), sTemp.length());
      break;
    }
    case FTP_MDTM:
    {
      CCVERBOSE("FTP_MDTM");
      CcString sTemp(m_WD);
      sTemp.appendPath(param);
      CcFile file(sTemp);
      if (file.isFile())
      {
        sTemp = "213 " + file.getModified().getString("yyyyMMddhhmmss") + "\r\n";
      }
      else
        sTemp = FTP_550;
      m_Socket->write(sTemp.getCharString(), sTemp.length());
      break;
    }
    default:
      CCERROR("Unexpected Message: " + Command);
      break;
  }
}

bool CcFtpServerWorker::acceptDataConnection(void)
{
  CcSocket *temp;
  if (m_Active != true)
  {
    temp = m_DataSocket->accept();
    CCMONITORDELETE(m_DataSocket); delete m_DataSocket;
    m_DataSocket = temp;
  }
  else
  {
    m_DataSocket = CcKernel::getSocket(ESocketType::TCP);
    m_DataSocket->connect(m_PasvAddress);
  }
  if (m_DataSocket != nullptr)
  {
    return true;
  }
  return false;
}

void CcFtpServerWorker::doUser(const CcString& Username)
{
  m_User = m_Server->getUserList().findUser(Username);
  if (m_User != nullptr)
  {
    m_UserState = userNameSet;
    if (m_User->login(CcString("")))
    {
      m_UserState = userLogedIn;
      CcByteArray msg("230 User login ok\r\n");
      m_Socket->write(msg.getArray(), msg.size());
    }
    else
    {
      m_UserState = userNameSet;
      CcByteArray msg("331 Password required\r\n");
      m_Socket->write(msg.getArray(), msg.size());
    }
  }
  else
  {
    m_UserState = userNone;
    CcByteArray msg("530 User not accepted\r\n");
    m_Socket->write(msg.getArray(), msg.size());
  }
}

void CcFtpServerWorker::doPass(const CcString& Password)
{
  if (m_UserState == userNameSet)
  {
    if (m_User->login(Password))
    {
      m_UserState = userLogedIn;
      m_WD = m_User->getHomeDir();
      CcByteArray msg("230 User successfully logged in\r\n");
      m_Socket->write(msg.getArray(), msg.size());
    }
    else{
      m_UserState = userNone;
      CcByteArray msg("530 Password not accepted\r\n");
      m_Socket->write(msg.getArray(), msg.size());
    }
  }
  else
  {
    m_UserState = userNone;
    CcByteArray msg("530 Password not accepted\r\n");
    m_Socket->write(msg.getArray(), msg.size());
  }
}
