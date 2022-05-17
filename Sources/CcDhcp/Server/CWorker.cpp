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
 * @brief     Implemtation of class CWorker
 */
#include "Server/CWorker.h"
#include "CcString.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "Server/CConfig.h"
#include "Server/CData.h"
#include "Network/Protocols/Dhcp/CcDhcpPacket.h"
#include "Network/Protocols/Dhcp/CcDhcpPacketData.h"
#include "Network/CcCommonIps.h"
#include "Network/CcCommonPorts.h"
#include "CcKernel.h"
#include "CcDhcpServer.h"

namespace NDhcp
{

namespace NServer
{

class CWorker::CPrivate
{
public:
  CcDhcpPacket oPacketSend;
};

CWorker::CWorker(CcDhcpServer& oDhcpServer, CData& oData):
  IWorker("CWorker"),
  m_oServer(oDhcpServer),
  m_oData(oData)
{
  CCNEW(m_pPrivate, CPrivate);
}

CWorker::~CWorker()
{
  CCDELETE(m_pPrivate);
}

void CWorker::run()
{
  switch (getPacket()->getType())
  {
    case EDhcpPacketType::Discover:
      processIpV4Discover(false);
      break;
    case EDhcpPacketType::Request:
      processIpV4Discover(true);
      break;
    default:
      // Unhandled/Not required
      break;
  }
}

void CWorker::send()
{
  m_oServer.write(m_pPrivate->oPacketSend.getPacket(), m_pPrivate->oPacketSend.getPacketSize());
}

void CWorker::processIpV4Discover(bool bIsRequest)
{
  bool bSuccess = true;
  CcString     sVendorClass;
  int          iArchitecture = -1;
  CcString     sHostName;
  const CcDhcpPacketData& oPacket = *getPacket()->getPacket();
  CcDhcpLeaseItem oNewLease;
  oNewLease.mac().setMac(oPacket.chaddr, true);
  size_t uiPos;
  if (bIsRequest == false)
  {
    if ((uiPos = m_oData.getIpV4LeaseList().findMacAddress(oNewLease.mac())) != SIZE_MAX)
    {
      oNewLease.ip() = m_oData.getIpV4LeaseList()[uiPos].getIp();
      m_oData.getIpV4LeaseList().remove(uiPos);
    }
    else if ((uiPos = m_oData.getIpV4PendingLeaseList().findMacAddress(oNewLease.mac())) != SIZE_MAX)
    {
      oNewLease.ip() = m_oData.getIpV4PendingLeaseList()[uiPos].getIp();
      m_oData.getIpV4PendingLeaseList().remove(uiPos);
    }
    else
    {
      oNewLease.ip() = m_oData.getIpV4LeaseList().getNextFree(m_oServer.getConfig().getIpBegin(), m_oServer.getConfig().getIpEnd());
    }
    m_pPrivate->oPacketSend.setMesageType(EDhcpPacketType::Offer);
  }
  else
  {
    if ((uiPos = m_oData.getIpV4PendingLeaseList().findMacAddress(oNewLease.mac())) != SIZE_MAX)
    {
      oNewLease.ip() = m_oData.getIpV4PendingLeaseList()[uiPos].getIp();
      m_oData.getIpV4PendingLeaseList().remove(uiPos);
      m_pPrivate->oPacketSend.setMesageType(EDhcpPacketType::Ack);
    }
    else
    {
      bSuccess = false;
      m_pPrivate->oPacketSend.setMesageType(EDhcpPacketType::Nak);
    }
  }
  oNewLease.timestamp() = CcKernel::getDateTime();
  oNewLease.leaseTime() = m_oServer.getConfig().getLeaseTime();
  oNewLease.renewTime() = m_oServer.getConfig().getRenewTime();
  oNewLease.rebindTime() = m_oServer.getConfig().getRebindTime();
  m_pPrivate->oPacketSend.setReply();
  m_pPrivate->oPacketSend.setCookie();
  m_pPrivate->oPacketSend.setIp(oNewLease.ip());
  m_pPrivate->oPacketSend.setServerIp(m_oServer.getConfig().getNextServer());
  m_pPrivate->oPacketSend.setMac(oNewLease.mac());
  m_pPrivate->oPacketSend.setTransactionId(oPacket.xid);
  CCDEBUG(oNewLease.mac().getString());
  size_t uiOptionPosIn = 0;
  EDhcpOption eOption;
  do
  {
    eOption = getPacket()->getNextOptionPos(uiOptionPosIn);
    switch (eOption)
    {
      case EDhcpOption::DhcpParameterRequestList:
      {
        setupRequestOption(uiOptionPosIn+1);
        break;
      }
      case EDhcpOption::DhcpVendorClassId:
      {
        size_t uiLength = oPacket.options[uiOptionPosIn + 1];
        sVendorClass.set(reinterpret_cast<const char*>(oPacket.options + uiOptionPosIn + 2), uiLength);
        break;
      }
      case EDhcpOption::HostName:
      {
        size_t uiLength = oPacket.options[uiOptionPosIn + 1];
        oNewLease.name().set(reinterpret_cast<const char*>(oPacket.options + uiOptionPosIn +2), uiLength);
        break;
      }
      case EDhcpOption::DhcpClientId:
      {
        size_t uiLength = oPacket.options[uiOptionPosIn + 1];
        oNewLease.clientId().add(reinterpret_cast<const char*>(oPacket.options + uiOptionPosIn + 2), uiLength);
        break;
      }
      case EDhcpOption::ClientSystemArchitecture:
      {
        iArchitecture  = (static_cast<int>(oPacket.options[uiOptionPosIn + 2]) << 8) & 0xff00;
        iArchitecture |= (static_cast<int>(oPacket.options[uiOptionPosIn + 3])     ) & 0xff;
        break;
      }
      default:
        // Unhandled/Not required
        break;
    }
  } while (eOption != EDhcpOption::End && uiOptionPosIn != SIZE_MAX);
  if (iArchitecture >= 0)
  {
    m_pPrivate->oPacketSend.setBootfile(m_oServer.getConfig().getBootfile(static_cast<EDhcpVendorClassId>(iArchitecture)));
  }
  else if (sVendorClass.length() > 0)
  {
    m_pPrivate->oPacketSend.setBootfile(m_oServer.getConfig().getBootfile(sVendorClass));
  }
  else
  {
    m_pPrivate->oPacketSend.setBootfile(m_oServer.getConfig().getBootfile());
  }
  if (m_oServer.getConfig().getNextServer().isNullIp() == false)
  {
    m_pPrivate->oPacketSend.addOptionString(EDhcpOption::DhcpTftpServerName, m_oServer.getConfig().getNextServer().getString());
  }
  // Required for DHCP Server Address
  m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::DhcpServerId, m_oServer.getConfig().getNextServer());
  // Gateway Router
  m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Router, m_oServer.getConfig().getNextServer());

  // Lease times
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpRenewalTime, m_oServer.getConfig().getRenewTime());
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpRebindingTime, m_oServer.getConfig().getRebindTime());
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpIpLeaseTime, m_oServer.getConfig().getLeaseTime());
  m_pPrivate->oPacketSend.addOptionRaw(EDhcpOption::End, 0, nullptr);
  if (bIsRequest == false)
  {
    m_oData.getIpV4PendingLeaseList().add(oNewLease);
  }
  else
  {
    if (bSuccess)
    {
      m_oData.getIpV4LeaseList().add(oNewLease);
    }
  }
  send();
}

void CWorker::setupRequestOption(size_t uiPos)
{
  const CcDhcpPacketData& oPacket = *getPacket()->getPacket();
  size_t uiLen = oPacket.options[uiPos];
  uiPos++;
  for (size_t uiI = 0; uiI < uiLen; uiI++)
  {
    EDhcpOption eOption = static_cast<EDhcpOption>(oPacket.options[uiPos + uiI]);
    switch (eOption)
    {
      case EDhcpOption::Subnet:
        if (m_oServer.getConfig().getSubnet().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oServer.getConfig().getSubnet());
        }
        break;
      case EDhcpOption::Router:
        if (m_oServer.getConfig().getGateway().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oServer.getConfig().getGateway());
        }
        break;
      case EDhcpOption::DomainNameServer:
        if (m_oServer.getConfig().getDns1().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oServer.getConfig().getDns1());
        }
        break;
      default:
        // Unhandled/Not required
        break;
    }
  }
}

}

}
