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
 * @brief     Implementation of class CcUrl
 */
#include "CcUrl.h"
#include "CcStringUtil.h"
#include "CcGlobalStrings.h"

CcUrl& CcUrl::operator=(CcUrl&& oToMove)
{
  if (this != &oToMove)
  {
    m_Hostname = CCMOVE(oToMove.m_Hostname);
    m_uiPort   = oToMove.m_uiPort;
    m_Username = CCMOVE(oToMove.m_Username);
    m_Password = CCMOVE(oToMove.m_Password);
    m_Protocol = CCMOVE(oToMove.m_Protocol);
    m_Path     = CCMOVE(oToMove.m_Path    );
    m_IsUrl = oToMove.m_IsUrl;
  }
  return *this;
}

CcUrl& CcUrl::operator=(const CcUrl& oToCopy)
{
  if (this != &oToCopy)
  {
    m_Hostname = oToCopy.m_Hostname;
    m_uiPort   = oToCopy.m_uiPort;
    m_Username = oToCopy.m_Username;
    m_Password = oToCopy.m_Password;
    m_Protocol = oToCopy.m_Protocol;
    m_Path     = oToCopy.m_Path    ;
    m_IsUrl    = oToCopy.m_IsUrl;
  }
  return *this;
}

CcUrl& CcUrl::operator=(const CcString& sUrlLink)
{
  setUrl(sUrlLink);
  return *this;
}

bool CcUrl::operator==(const CcUrl& oToCompare) const
{
  if( m_Hostname == oToCompare.m_Hostname &&
      m_uiPort == oToCompare.m_uiPort &&
      m_Username == oToCompare.m_Username &&
      m_Password == oToCompare.m_Password &&
      m_Protocol == oToCompare.m_Protocol &&
      m_Path == oToCompare.m_Path &&
      m_IsUrl == oToCompare.m_IsUrl)
  {
    return true;
  }
  return false;
}

bool CcUrl::setUrl(const CcString& url)
{
  m_IsUrl = false;
  CcString userPart;
  CcString hostPart;
  size_t pos = url.find("://");
  if (pos < url.length())
  {
    m_IsUrl = true;
    m_Protocol = url.substr(0, pos);
    hostPart = url.getStringBetween("://", "/");
    pos += hostPart.length() + 3;//strlen("://");
    m_Path = url.substr(pos);
    if (m_Path.length() == 0)
    {
      m_Path = "/";
    }
    pos = hostPart.find("@");
    if (pos < hostPart.length())
    {
      userPart = hostPart.substr(0, pos);
      hostPart = hostPart.substr(pos + 1);
    }
    pos = hostPart.find(CcGlobalStrings::Seperators::Colon);
    if (pos < hostPart.length())
    {
      m_Hostname = hostPart.substr(0, pos);
      m_uiPort = hostPart.substr(pos + 1).toUint16();
    }
    else
    {
      m_Hostname = hostPart;
    }
    pos = userPart.find(CcGlobalStrings::Seperators::Colon);
    if (pos < userPart.length())
    {
      m_Username = userPart.substr(0, pos);
      m_Password = userPart.substr(pos + 1);
    }
    else{
      m_Username = userPart;
    }
  }
  else
  {
    m_Path = url;
  }
  m_IsUrl = true;
  return m_IsUrl;
}

CcString CcUrl::getUrl() const
{
  CcString sRet;
  if (m_Protocol.length() > 0)
  {
    sRet.append(m_Protocol).append("://");
  }
  if (m_Username.length() > 0)
  {
    sRet.append(m_Username);
    if (m_Password.length() > 0)
    {
      sRet.append(CcGlobalStrings::Seperators::Colon).append(m_Password);
    }
    sRet.append("@");
  }
  sRet.append(m_Hostname);
  if (m_uiPort > 0)
  {
    sRet.append(CcGlobalStrings::Seperators::Colon).append(getPortString());
  }
  sRet.append(m_Path);
  return sRet;
}


bool CcUrl::isUrl() const
{
  return m_IsUrl;
}

const CcString& CcUrl::getHostname() const
{
  return m_Hostname;
}

CcString CcUrl::getPortString() const
{
  return CcString::fromNumber(m_uiPort);
}

const CcString& CcUrl::getUsername() const
{
  return m_Username;
}

const CcString& CcUrl::getPassword() const
{
  return m_Password;
}

const CcString& CcUrl::getProtocol() const
{
  return m_Protocol;
}

const CcString& CcUrl::getPath() const
{
  return m_Path;
}

CcUrl& CcUrl::setHostname(const CcString& Hostname)
{
  m_Hostname = Hostname;
  return *this;
}

CcUrl& CcUrl::setPort(const CcString& sPort)
{
  m_uiPort = sPort.toUint16();
  return *this;
}

CcUrl& CcUrl::setUsername(const CcString& Username)
{
  m_Username = Username;
  return *this;
}

CcUrl& CcUrl::setPassword(const CcString& Password)
{
  m_Password = Password;
  return *this;
}

CcUrl& CcUrl::setProtocol(const CcString& Protocol)
{
  m_Protocol = Protocol;
  return *this;
}

CcUrl& CcUrl::setPath(const CcString& Path)
{
  if (Path.length() == 0 || Path[0] == '/')
  {
    m_Path = Path;
  }
  else{
    m_Path = m_Path.extractPath() + Path;
  }
  return *this;
}

void CcUrl::getPortByProtocol()
{
  
}
