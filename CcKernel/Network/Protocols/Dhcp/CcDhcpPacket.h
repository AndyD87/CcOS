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
 * @brief     Class CcDhcpPacket
 **/
#pragma once

#include "CcBase.h"
#include "CcTypes.h"
#include "CcList.h"
#include "CcByteArray.h"

/**
 * @brief Dhcp packet type enumeration
 */
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

/**
 * @brief Known dhcp option and data storage
 */
class CcDhcpOption
{
public:
  /**
   * @brief Compare option type and data if they are same
   * @param oToCompare: Other option to compare with
   * @return True if they are even.
   */
  bool operator==(const CcDhcpOption& oToCompare) const
  {
    return oToCompare.eOption == eOption && oToCompare.oOptionData == oOptionData;
  }

public:
  EDhcpOption eOption = EDhcpOption::Start; //!< Option as enumeration
  CcByteArray oOptionData;                  //!< Addtional data for option
};

//! @brief List of options
typedef CcList<CcDhcpOption> CcDhcpOptionList;

#ifdef _MSC_VER
template class CcKernelSHARED CcList<CcDhcpOption>;
#endif

class CcDhcpPacketData;
class CcIp;
class CcMacAddress;

/**
 * @brief Control openssl library
 */
class CcKernelSHARED CcDhcpPacket
{
public:
  /**
   * @brief Create an empty dhcp packet
   */
  CcDhcpPacket();
  /**
   * @brief Initialize DHCP packet with existing buffer
   * @param pPacket:      Buffer assigned for DHCP packet
   * @param uiPacketSize: Max size of @ref pPacket to write to.
   */
  CcDhcpPacket(void* pPacket, size_t uiPacketSize);

  /**
   * @brief Move content from other packet to this
   * @param oToMove: Object to move data from
   */
  CcDhcpPacket(CcDhcpPacket&& oToMove)
    { operator=(CCMOVE(oToMove)); }

  /**
   * @brief Copy content from other packet to this
   * @param oToCopy: Object to copy data from
   */
  CcDhcpPacket(const CcDhcpPacket& oToCopy)
    { operator=(oToCopy); }

  /**
   * @brief Destructor
   */
  ~CcDhcpPacket();

  /**
   * @brief Move content from other packet to this
   * @param oToMove: Object to move data from
   * @return Handle to this packet
   */
  CcDhcpPacket& operator=(CcDhcpPacket&& oToMove);

  /**
   * @brief Copy content from other packet to this
   * @param oToCopy: Object to copy data from
   * @return Handle to this packet
   */
  CcDhcpPacket& operator=(const CcDhcpPacket& oToCopy);

  /**
   * @brief Get packet content
   * @return
   */
  CcDhcpPacketData* getPacket() const;

  /**
   * @brief Get size of current packet with all options
   * @return
   */
  size_t getPacketSize() const;

  /**
   * @brief Get current type of packet
   * @return Type of packet as enum
   */
  EDhcpPacketType getType() const;

  /**
   * @brief Extract option list from buffer.
   * @return Option list extracted.
   */
  CcDhcpOptionList getOptionList() const;

  /**
   * @brief Check if specific option is available in option list.
   * @param oOptionList:  List to search in
   * @param eType:        Type of option to search for
   * @return True if option was found.
   */
  static bool hasOption(CcDhcpOptionList& oOptionList, EDhcpOption eType);

  /**
   * @brief Search specific option available in option list and return it.
   *        The availablity should be checked with @ref hasOption before
   * @param oOptionList:  List to search in
   * @param eType:        Type of option to search for
   * @return Handle to found option
   */
  static CcDhcpOption& findOption(CcDhcpOptionList& oOptionList, EDhcpOption eType);

  /**
   * @brief Get next option in buffer from a specific location.
   * @param[in,out] uiBegin: Postion to start next option search.
   *                         It will be update with position of next option.
   * @return Type of option at @ref uiBegin
   */
  EDhcpOption getNextOptionPos(size_t& uiBegin) const;

  /**
   * @brief Mark packet as reply
   */
  void setReply();

  /**
   * @brief Mark packet as cookie
   */
  void setCookie();

  //! @param oIp: Set target ip in packet
  void setIp(const CcIp& oIp);
  //! @param oIp: Set server ip in packet
  void setServerIp(const CcIp& oIp);
  //! @param oIp: Set target physical address in packet
  void setMac(const CcMacAddress& oMac);
  //! @param oIp: Set transaction id in packet
  void setTransactionId(uint32 uiTransactionId);
  //! @param oIp: Set boot file for pxe boot
  void setBootfile(const CcString& sBootfile);
  //! @param oIp: Set message type as enum in packet
  void setMesageType(EDhcpPacketType uiTransactionId);

  /**
   * @brief Add raw option to packet
   * @param uiOptionId: Option id
   * @param uiLength:   Length of pucData
   * @param pucData:    Data to append
   */
  void addOptionRaw(EDhcpOption uiOptionId, uchar uiLength, uchar* pucData);

  /**
   * @brief Add IP Option with id
   * @param uiOptionId: Option to set
   * @param oIp:        IP to append to option
   */
  void addOptionIp(EDhcpOption uiOptionId, const CcIp& oIp);

  /**
   * @brief Add uint32 Option with id
   * @param uiOptionId: Option to set
   * @param ui32:       Uint32 to append
   */
  void addOptionUint32(EDhcpOption uiOptionId, uint32 ui32);

  /**
   * @brief Add string Option with id
   * @param uiOptionId: Option to set
   * @param sString:    String to append
   */
  void addOptionString(EDhcpOption uiOptionId, const CcString& sString);

  /**
   * @brief Check if packet is valid
   * @return True if verified
   */
  bool isValid();
  //! @return True if packet is a request packet
  bool isRequest();
  //! @return True if packet is a response packet
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
