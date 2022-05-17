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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class INetworkProtocol
 */
#pragma once

#include "Network/Stack/CcNetworkPacket.h"
#include "CcNetworkPacket.h"
#include "CcBase.h"
#include "CcBase.h"
#include "CcVector.h"
#include "CcStatic.h"

class CcNetworkStack;

/**
 * @brief Basic interface each network protocol has to support.
 */
class CcKernelSHARED INetworkProtocol : protected CcVector<INetworkProtocol*>
{
public:
  /**
   * @brief Init network protocol with parent.
   * @param pParentProtocol: pParent to register at if null.
   */
  INetworkProtocol(INetworkProtocol* pParentProtocol) :
    m_pParentProtocol(pParentProtocol)
    {}
  virtual ~INetworkProtocol() = default;

  /**
   * @brief Get protocol number which is commonly availbale on each protocol header
   * @return Number of this header to identify in parent header.
   */
  virtual uint16 getProtocolType() const = 0;

  /**
   * @brief Passthrough packet from Application to Physical space.
   *        Additional data can be app- and or prepended.
   * @param pPacket: Packet to forward and edit.
   * @return True if no error occured.
   */
  virtual bool transmit(CcNetworkPacketRef pPacket) = 0;

  /**
   * @brief Passthrough packet from Physical to Application space.
   *        Additional data can be app- and or prepended.
   * @param pPacket: Packet to forward and edit.
   * @return True if no error occured.
   */
  virtual bool receive(CcNetworkPacketRef pPacket) = 0;

  /**
   * @brief Find protocol in childs by protocol number
   * @param uiProtocolType: Protocol number to search for.
   * @return Found protocol if found or nullptr if not.
   */
  INetworkProtocol* findProtocol(uint16 uiProtocolType);
  
private: // Methods
  INetworkProtocol(const INetworkProtocol& oToCopy) = delete;
  INetworkProtocol(INetworkProtocol&& oToMove) = delete;
  
protected:
  /**
   * @brief Get network stack where this chain is assigned to.
   * @return Parent networkstack, or nullptr if no networkstack available.
   */
  CcNetworkStack* getNetworkStack();

protected: // Member
  INetworkProtocol* m_pParentProtocol; //!< Pointer to parent protocol
};
