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
 * @brief     Class CcIcmpProtocol
 */
#pragma once

#include "Network/Stack/INetworkProtocol.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcGlobalStrings.h"
#include "CcIp.h"

/**
 * @brief Icmp protocoll for sending ip messages over ethernet.
 */
class CcKernelSHARED CcIcmpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief Known Icmp messages
   */
  enum class EType : uint8
  {
    Echo = 0,
    EchoRequest = 8
  };
  /**
   * @brief typedef for ip header
   */
  class CHeader
  {
  public:
    uint8 eType;        //!< ip version
    uint8 uiCode;       //!< TOS
    uint16 uiChecksum;  //!< packet len

    //! @return Get known type of this received message
    EType getType()
    { return static_cast<EType>(eType); }
    //! @param Set type of packet for sending.
    void setType(EType eNewType)
    { eType = static_cast<uint8>(eNewType);}
  };
#pragma pack(pop)
public:
  /**
   * @brief Initializes this protocol handler with parent network stack.
   * @param pParentProtocol: Parent protocol
   */
  CcIcmpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcIcmpProtocol() override;

  //! @brief Get static protocol type for IPV4
  virtual uint16 getProtocolType() const override;

  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

private:
  CcIcmpProtocol(const CcIcmpProtocol& oToCopy) = delete;
  CcIcmpProtocol(CcIcmpProtocol&& oToMove) = delete;
};
