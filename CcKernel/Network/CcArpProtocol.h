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
 * @page      Network
 * @subpage   CcArpProtocol
 *
 * @page      CcArpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcArpProtocol
 */
#ifndef _CcArpProtocol_H_
#define _CcArpProtocol_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "Network/INetworkProtocol.h"
#include "CcEventHandleMap.h"

class CcKernelSHARED CcArpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for TCP Header
   */
  typedef struct
  {
    uint16  uiMacType;       //!<
    uint16  uiProtocolType;  //!<
    uint8   uiMacSize;       //!<
    uint8   uiProtocolSize;  //!<
    uint16  uiOperation;     //!<
    uint8   puiSourceMac[6]; //!<
    uint8   puiSourceIp[4];  //!<
    uint8   puiDestinationMac[6]; //!<
    uint8   puiDestinationIp[4];  //!<
  } SHeader;
#pragma pack(pop)

public:
  CcArpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcArpProtocol();

  bool initDefaults();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcBufferList& oBuffer) override;
  virtual bool receive(CcBufferList& oBuffer) override;

private: // Methods
  CcArpProtocol(const CcArpProtocol& oToCopy) = delete;
  CcArpProtocol(CcArpProtocol&& oToMove) = delete;
};

#endif //_CcArpProtocol_H_
