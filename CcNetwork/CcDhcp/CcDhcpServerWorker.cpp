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
 * @brief     Implemtation of class CcDhcpServerWorker
 */

#include "CcDhcpServerWorker.h"
#include "CcString.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcDhcpServerConfig.h"
#include "CcDhcpServerData.h"
#include "CcDhcpPacket.h"
#include "CcDhcpPacketData.h"
#include "Network/CcCommonIps.h"
#include "Network/CcCommonPorts.h"
#include "CcKernel.h"

class CcDhcpServerWorker::CPrivate
{
public:
  CcDhcpPacket oPacketSend;
};

CcDhcpServerWorker::CcDhcpServerWorker(const CcDhcpServerConfig& oConfig, CcDhcpServerData& oData, CcDhcpPacket&& oPacket):
  m_oConfig(oConfig),
  m_oData(oData),
  m_oPacket(std::move(oPacket))
{
}

CcDhcpServerWorker::~CcDhcpServerWorker()
{
  CCDELETE(m_pPrivate);
}

void CcDhcpServerWorker::run()
{
  m_pPrivate = new CPrivate();
  CCMONITORNEW(m_pPrivate);
  switch (m_oPacket.getType())
  {
    case EDhcpPacketType::Discover:
      processIpV4Discover(false);
      break;
    case EDhcpPacketType::Request:
      processIpV4Discover(true);
      break;
    default:
      CCDEBUG("No valid packet type, Do nothing!");
      break;
  }
  CCDELETE(m_pPrivate);
}

void CcDhcpServerWorker::send()
{
  CcSocket oTransfer(ESocketType::UDP);
  CcSocketAddressInfo oPeerInfo;
  oPeerInfo.init(ESocketType::UDP);
  oPeerInfo.setPort(CcCommonPorts::DHCP_CLI);
  oPeerInfo.setIp(CcCommonIps::Broadcast);
  oTransfer.setPeerInfo(oPeerInfo);
  if (oTransfer.open())
  {
    oTransfer.setOption(ESocketOption::Broadcast);
    oTransfer.write(m_pPrivate->oPacketSend.getPacket(), m_pPrivate->oPacketSend.getPacketSize());
    oTransfer.close();
  }
}

void CcDhcpServerWorker::processIpV4Discover(bool bIsRequest)
{
  bool bSuccess = true;
  CcString     sVendorClass;
  int          iArchitecture = -1;
  CcString     sHostName;
  const CcDhcpPacketData& oPacket = *m_oPacket.getPacket();
  CcDhcpLeaseItem oNewLease;
  oNewLease.mac().setMac(oPacket.chaddr, true);
  oNewLease.transactionId() = oPacket.xid;
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
      oNewLease.ip() = m_oData.getIpV4LeaseList().getNextFree(m_oConfig.getIpBegin(), m_oConfig.getIpEnd());
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
  oNewLease.leaseTime() = m_oConfig.getLeaseTime();
  oNewLease.renewTime() = m_oConfig.getRenewTime();
  oNewLease.rebindTime() = m_oConfig.getRebindTime();
  m_pPrivate->oPacketSend.setReply();
  m_pPrivate->oPacketSend.setCookie();
  m_pPrivate->oPacketSend.setIp(oNewLease.ip());
  m_pPrivate->oPacketSend.setServerIp(m_oConfig.getNextServer());
  m_pPrivate->oPacketSend.setMac(oNewLease.mac());
  m_pPrivate->oPacketSend.setTransactionId(oNewLease.transactionId());
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpIpLeaseTime, m_oConfig.getLeaseTime());
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpRenewalTime, m_oConfig.getRenewTime());
  m_pPrivate->oPacketSend.addOptionUint32(EDhcpOption::DhcpRebindingTime, m_oConfig.getRebindTime());
  CCDEBUG(oNewLease.mac().getString());
  size_t uiOptionPosIn = 0;
  EDhcpOption eOption;
  do
  {
    eOption = m_oPacket.getNextOptionPos(uiOptionPosIn);
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
        break;
    }
  } while (eOption != EDhcpOption::End && uiOptionPosIn != SIZE_MAX);
  if (iArchitecture >= 0)
  {
    m_pPrivate->oPacketSend.setBootfile(m_oConfig.getBootfile(static_cast<EDhcpVendorClassId>(iArchitecture)));
  }
  else if (sVendorClass.length() > 0)
  {
    m_pPrivate->oPacketSend.setBootfile(m_oConfig.getBootfile(sVendorClass));
  }
  else
  {
    m_pPrivate->oPacketSend.setBootfile(m_oConfig.getBootfile());
  }
  if (m_oConfig.getNextServer().isNullIp() == false)
  {
    m_pPrivate->oPacketSend.addOptionString(EDhcpOption::DhcpTftpServerName, m_oConfig.getNextServer().getString());
  }
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

void CcDhcpServerWorker::setupRequestOption(size_t uiPos)
{
  const CcDhcpPacketData& oPacket = *m_oPacket.getPacket();
  size_t uiLen = oPacket.options[uiPos];
  uiPos++;
  for (size_t uiI = 0; uiI < uiLen; uiI++)
  {
    EDhcpOption eOption = static_cast<EDhcpOption>(oPacket.options[uiPos + uiI]);
    switch (eOption)
    {
      case EDhcpOption::Subnet:
        if (m_oConfig.getSubnet().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oConfig.getSubnet());
        }
        break;
      case EDhcpOption::Router:
        if (m_oConfig.getGateway().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oConfig.getGateway());
        }
        break;
      case EDhcpOption::DomainNameServer:
        if (m_oConfig.getDns1().isNullIp() == false)
        {
          m_pPrivate->oPacketSend.addOptionIp(EDhcpOption::Subnet, m_oConfig.getDns1());
        }
        break;
      default:
        break;
    }
  }
}
