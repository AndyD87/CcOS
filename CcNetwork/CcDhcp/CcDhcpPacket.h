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
 * @page      CcDhcp
 * @subpage   CcDhcpPacket
 *
 * @page      CcDhcpPacket
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcDhcpPacket
 **/
#ifndef _CcDhcpPacket_H_
#define _CcDhcpPacket_H_

#include "CcBase.h"
#include "CcDhcp.h"
#include "CcTypes.h"
#include "CcList.h"
#include "CcByteArray.h"

enum class EDhcpPacketType
{
  Unknown = 0,
  Discover,
  Offer,
  Request,
  Decline,
  Ack,
  Nak,
  Release,
  Inform
};

/**
 * @brief Option Ids in Option field within DHCP Packet
 *        All informations are from https://tools.ietf.org/html/rfc2132
 *        and http://www.networksorcery.com/enp/protocol/bootp/options.htm
 */
enum class EDhcpOption : uchar
{
  Start = 0,
  Subnet,
  Time,
  Router,
  TimeServer,
  NameServer,
  DomainNameServer,
  LogServer,
  CookeServer,
  LPRServer,
  ImpressServer,
  ResourceLocationServer,
  HostName,
  BootFileSize,
  MeritDumpFile,
  DomainName,
  SwapServer,
  RootPath,
  ExtensionsPath,
  IpForwardingOnOff,
  IpNonLocalSourceRoutingOnOff,
  IpPolicyFilter,
  IpMaxDatagram,
  IpDefaultIpTimeToLive,
  IpPathMtoAgingTimeout,
  IpPathMtuPlateauTable,
  IfcIfcMtuOption,
  IfcAllSubnetAreLocal,
  IfcBroadcastAddress,
  IfcPerformMarkDiscovery,
  IfcMarkSupplier,
  IfcPerformRouterDiscovery,
  IfcRouterSolicitationAddress,
  IfcStaticRoute,
  LinkTrailerEncaps,
  LinkArpCacheTimeout,
  LinkEthernetEncaps,
  TcpDefaultTtl,
  TcpKeepAliveInterval,
  TcpKeepGarbage,
  NetInfoService,
  NetInfoServer,
  NetTimeProtocolServers,
  VendorSpecificInfo,
  NetBiosOverTcpIpNameServer,
  NetBiosOverTcpIpDatagramDistServer,
  NetBiosOverTcpIpNodeType,
  NetBiosoverTcpIpScope,
  XWindowSystemFontServer,
  XWindowSystemDisplayManager,
  DhcpRequestedIp,
  DhcpIpLeaseTime,
  DhcpOptionOverload,
  DhcpMessageType,
  DhcpServerId,
  DhcpParameterRequestList,
  DhcpMessage,
  DhcpMessageMaxSize,
  DhcpRenewalTime,
  DhcpRebindingTime,
  DhcpVendorClassId,
  DhcpClientId,
  NetInfoServiceDomain = 64,
  NetInfoServiceServers,
  DhcpTftpServerName,
  DhcpBootfileName,
  MobileIpHomeAgent = 68,
  SmtpServers,
  Pop3SServers,
  NntpServers,
  WwwServers,
  DefaultFingerServers,
  DefaultIrcServers,
  StreetTalkServers,
  StreetTalkDirectoryAssistance,
  UserClassInformation,
  SLPDirectoryAgent,
  SLPServiceScope,
  RapidCommit,
  Fqdn,
  RelayAgentInformation,
  InternetStoarageNameService,
  NdsServers = 85,
  NdsTreeName,
  NdsContext,
  BcmcsControllerDomainNameList,
  BcmcsControllerIpV4List,
  Athentication,
  ClientLastTransactionTime,
  AssociatedIp,
  ClientSystemArchitecture,
  ClientInterfaceIdentifier,
  Ldap,

  ClientMachineIdentifier=97,
  OpenGroupUserAuth,
  GeoconfCivic,
  Ieee1003_1TZString,
  End = 255,
};

class CcDhcpOption
{
public:
  bool operator==(const CcDhcpOption& oToCompare) const
  {
    return oToCompare.eOption == eOption && oToCompare.oOptionData == oOptionData;
  }
public:
  EDhcpOption eOption = EDhcpOption::Start;
  CcByteArray oOptionData;
};

typedef CcList<CcDhcpOption> CcDhcpOptionList;
#ifdef WIN32
template class CcDhcpSHARED CcList<CcDhcpOption>;
#endif


class CcDhcpPacketData;
class CcIp;
class CcMacAddress;

/**
 * @brief Control openssl library
 */
class CcDhcpSHARED CcDhcpPacket 
{
public:
  /**
   * @brief Constructor
   */
  CcDhcpPacket();

  CcDhcpPacket(void* pPacket, size_t uiPacketSize);
  
  CcDhcpPacket(CcDhcpPacket&& oToMove)
    { operator=(std::move(oToMove)); }

  CcDhcpPacket(const CcDhcpPacket& oToCopy)
    { operator=(oToCopy); }
  
  /**
   * @brief Destructor
   */
  ~CcDhcpPacket();

  CcDhcpPacket& operator=(CcDhcpPacket&& oToMove);
  CcDhcpPacket& operator=(const CcDhcpPacket& oToCopy);

  CcDhcpPacketData* getPacket();
  size_t getPacketSize();

  CcDhcpPacketData* packet()
    { return m_pPacket; }
  size_t packetSize()
    { return m_uiPacketSize; }

  EDhcpPacketType getType() const;
  CcDhcpOptionList getOptionList() const;
  static bool hasOption(CcDhcpOptionList& oOptionList, EDhcpOption eType);
  static CcDhcpOption& findOption(CcDhcpOptionList& oOptionList, EDhcpOption eType);
  EDhcpOption getNextOptionPos(size_t& uiBegin) const;

  void setReply();
  void setCookie();
  void setIp(const CcIp& oIp);
  void setServerIp(const CcIp& oIp);
  void setMac(const CcMacAddress& oMac);
  void setTransactionId(uint32 uiTransactionId);
  void setBootfile(const CcString& sBootfile);

  void setMesageType(EDhcpPacketType uiTransactionId);
  void addOptionRaw(EDhcpOption uiOptionId, uchar uiLength, uchar* pucData);
  void addOptionIp(EDhcpOption uiOptionId, const CcIp& oIp);
  void addOptionUint32(EDhcpOption uiOptionId, uint32 oIp);
  void addOptionString(EDhcpOption uiOptionId, const CcString& oIp);

  bool isValid();
  bool isRequest();
  bool isReply();

private: // Methods
  void createBuffer();
  void deleteBuffer();

private:
  CcDhcpPacketData *m_pPacket =  nullptr;
  size_t m_uiPacketSize = 0;
  bool m_bPacketOwner = false;
  size_t m_uiOptionCount = 0;
};
#endif /* _CcDhcpPacket_H_ */
