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
 * @brief     Implemtation of class CcDhcpPacket
 */
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"
#include "Network/Protocols/Dhcp/CcDhcpPacketData.h"
#include "CcString.h"
#include "CcIp.h"
#include "Network/CcMacAddress.h"
#include "Network/CcSocketAddressInfo.h"
#include "CcStatic.h"

CcDhcpPacket::CcDhcpPacket()
{
  createBuffer();
}

CcDhcpPacket::CcDhcpPacket(void* pPacket, size_t uiPacketSize) :
  m_pPacket(static_cast<CcDhcpPacketData*>(pPacket)),
  m_uiPacketSize(uiPacketSize),
  m_bPacketOwner(true)
{
}

CcDhcpPacket::~CcDhcpPacket()
{
  deleteBuffer();
}

CcDhcpPacket& CcDhcpPacket::operator=(CcDhcpPacket&& oToMove)
{
  if (this != &oToMove)
  {
    deleteBuffer();
    m_pPacket = oToMove.m_pPacket;
    m_uiPacketSize = oToMove.m_uiPacketSize;
    m_bPacketOwner = oToMove.m_bPacketOwner;
    oToMove.m_pPacket = nullptr;
    oToMove.m_uiPacketSize = 0;
    oToMove.m_bPacketOwner = false;
  }
  return *this;
}

CcDhcpPacket& CcDhcpPacket::operator=(const CcDhcpPacket& oToCopy)
{
  deleteBuffer();
  if (oToCopy.m_uiPacketSize > 0 &&
      oToCopy.m_pPacket != nullptr)
  {
    createBuffer();
    CcStatic::memcpy(m_pPacket, oToCopy.m_pPacket, oToCopy.m_uiPacketSize);
    m_bPacketOwner = true;
  }
  return *this;
}

CcDhcpPacketData* CcDhcpPacket::getPacket()
{
  return m_pPacket;
}

size_t CcDhcpPacket::getPacketSize()
{
  if (m_bPacketOwner)
  {
    return (m_uiPacketSize - 312) + m_uiOptionCount;
  }
  else
  {
    return m_uiPacketSize;
  }
}

EDhcpPacketType CcDhcpPacket::getType() const
{
  EDhcpPacketType eType = EDhcpPacketType::Unknown;
  // look at https://tools.ietf.org/html/rfc2132
  size_t uiSearch = 0;
  do
  {
    EDhcpOption eCurrentOption = static_cast<EDhcpOption>(m_pPacket->options[uiSearch]);
    if (eCurrentOption != EDhcpOption::DhcpMessageType)
    {
      getNextOptionPos(uiSearch);
    }
    else
    {
      if (m_pPacket->options[uiSearch + 1] > 0 &&
          m_pPacket->options[uiSearch + 1] < 9)
      {
        eType = static_cast<EDhcpPacketType>(m_pPacket->options[uiSearch + 2]);
        break;
      }
      else
      {
        CCDEBUG("Unknown DhcpMessageType");
        break;
      }
    }
  } while (uiSearch != SIZE_MAX);
  return eType;
}

CcDhcpOptionList CcDhcpPacket::getOptionList() const
{
  CcDhcpOptionList oList;
  size_t uiSearch = 0;
  do
  {
    EDhcpOption eCurrentOption = static_cast<EDhcpOption>(m_pPacket->options[uiSearch]);
    if (eCurrentOption != EDhcpOption::Start)
    {

    }
    else if (eCurrentOption != EDhcpOption::End)
    {
      break;
    }
    else
    {
      CcDhcpOption oOption;
      oOption.eOption = eCurrentOption;
      size_t uiLen = m_pPacket->options[uiSearch + 1];
      if (uiLen > 0)
      {
        oOption.oOptionData.append((char*) (m_pPacket->options) + uiSearch + 2, uiLen);
      }
      oList.append(oOption);
      getNextOptionPos(uiSearch);
    }
  } while (uiSearch != SIZE_MAX);
  return oList;
}

bool CcDhcpPacket::hasOption(CcDhcpOptionList& oOptionList, EDhcpOption eType)
{
  for (CcDhcpOption& oOption : oOptionList)
  {
    if (oOption.eOption == eType)
      return true;
  }
  return false;
}

CcDhcpOption& CcDhcpPacket::findOption(CcDhcpOptionList& oOptionList, EDhcpOption eType)
{
  for (CcDhcpOption& oOption : oOptionList)
  {
    if (oOption.eOption == eType)
      return oOption;
  }
  return CcStatic::getNullRef<CcDhcpOption>();
}

EDhcpOption CcDhcpPacket::getNextOptionPos(size_t& uiStart) const
{
  EDhcpOption eCurrentOption = static_cast<EDhcpOption>(m_pPacket->options[uiStart]);
  if (eCurrentOption == EDhcpOption::Start)
  {
    uiStart++;
  }
  else if (eCurrentOption == EDhcpOption::End)
  {
    uiStart = SIZE_MAX;
  }
  else
  {
    uiStart++;
    size_t uiLength = m_pPacket->options[uiStart] + 1;
    uiStart += uiLength;
  }
  return static_cast<EDhcpOption>(m_pPacket->options[uiStart]);
}

void CcDhcpPacket::setReply()
{
  m_pPacket->op = 2;
  m_pPacket->htype = 1; //set ethernet
}

void CcDhcpPacket::setCookie()
{
  m_pPacket->uiMagicCookie = 0x63538263;
}

void CcDhcpPacket::setIp(const CcIp& oIp)
{
  m_pPacket->yiaddr = oIp.getUint32(true);
}

void CcDhcpPacket::setServerIp(const CcIp& oIp)
{
  m_pPacket->siaddr = oIp.getUint32(true);
}

void CcDhcpPacket::setMac(const CcMacAddress& oMac)
{
  m_pPacket->hlen = 6; //set ethernet
  m_pPacket->chaddr[5] = oMac.getMacV4_0();
  m_pPacket->chaddr[4] = oMac.getMacV4_1();
  m_pPacket->chaddr[3] = oMac.getMacV4_2();
  m_pPacket->chaddr[2] = oMac.getMacV4_3();
  m_pPacket->chaddr[1] = oMac.getMacV4_4();
  m_pPacket->chaddr[0] = oMac.getMacV4_5();
}

void CcDhcpPacket::setTransactionId(uint32 uiTransactionId)
{
  m_pPacket->xid = uiTransactionId;
}

void CcDhcpPacket::setBootfile(const CcString& sBootfile)
{
  if (sBootfile.length() > 0)
  {
    CcStatic::memcpy(m_pPacket->file, sBootfile.getCharString(), sBootfile.length() + 1);
    addOptionString(EDhcpOption::DhcpBootfileName, sBootfile);
  }
}

void CcDhcpPacket::setMesageType(EDhcpPacketType ePacketType)
{
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(EDhcpOption::DhcpMessageType);
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = 1;
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(ePacketType);
  m_uiOptionCount++;
}

void CcDhcpPacket::addOptionRaw(EDhcpOption uiOptionId, uchar uiLength, uchar* pucData)
{
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(uiOptionId);
  m_uiOptionCount++;
  if (uiLength > 0)
  {
    m_pPacket->options[m_uiOptionCount] = uiLength;
    m_uiOptionCount++;
    for (uchar uiSize = 0; uiSize < uiLength; uiSize++)
    {
      m_pPacket->options[m_uiOptionCount] = pucData[uiSize];
      m_uiOptionCount++;
    }
  }
}

void CcDhcpPacket::addOptionIp(EDhcpOption uiOptionId, const CcIp& oIp)
{
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(uiOptionId);
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = 4;
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = oIp.getIpV4_3();
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = oIp.getIpV4_2();
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = oIp.getIpV4_1();
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = oIp.getIpV4_0();
  m_uiOptionCount++;
}

void CcDhcpPacket::addOptionUint32(EDhcpOption uiOptionId, uint32 uiData)
{
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(uiOptionId);
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = 4;
  m_uiOptionCount++;
  void* pTempPointer = static_cast<void*>(m_pPacket->options + m_uiOptionCount);
  *static_cast<uint32*>(pTempPointer) = CcSocketAddressInfo::htonl(uiData);
  m_uiOptionCount += 4;
}


void CcDhcpPacket::addOptionString(EDhcpOption uiOptionId, const CcString& sString)
{
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(uiOptionId);
  m_uiOptionCount++;
  m_pPacket->options[m_uiOptionCount] = static_cast<uchar>(sString.length()+1);
  m_uiOptionCount++;
  CcStatic::memcpy(m_pPacket->options + m_uiOptionCount, sString.getCharString(), sString.length() + 1);
  m_uiOptionCount += sString.length() + 1;
}

bool CcDhcpPacket::isValid()
{
  if (m_pPacket != nullptr &&
      (isRequest() || isReply()))
  {
    return true;
  }
  return false;
}

bool CcDhcpPacket::isRequest()
{
  return m_pPacket->op == 1;
}

bool CcDhcpPacket::isReply()
{
  return m_pPacket->op == 2;
}

void CcDhcpPacket::createBuffer()
{
  deleteBuffer();
  m_uiPacketSize = sizeof(CcDhcpPacketData);
  CCNEW(m_pPacket, CcDhcpPacketData);
  CcStatic::memset(m_pPacket, 0, m_uiPacketSize);
  m_bPacketOwner = true;
}

void CcDhcpPacket::deleteBuffer()
{
  if (m_bPacketOwner)
  {
    CCDELETE(m_pPacket);
  }
}
