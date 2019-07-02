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
 * @subpage   CcPtpProtocol
 *
 * @page      CcPtpProtocol
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcPtpProtocol
 */
#ifndef _CcPtProctocol_H_
#define _CcPtProctocol_H_

#include <Network/INetworkProtocol.h>
#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"

class CcKernelSHARED CcPtpProtocol : public INetworkProtocol
{
public: // Types
#pragma pack(push, 1)
  /**
   * @brief typedef for Ptp Header
   */
  typedef struct
  {
    uint8   uiTransportMessage; //!< Specified by IEEE 1588
    uint8   uiVersionPtp;       //!< Specified by IEEE 1588
    uint16  uiLength;           //!< Specified by IEEE 1588
    uint8   uiDomain;           //!< Specified by IEEE 1588
    uint8   uiReserved1;        //!< Specified by IEEE 1588
    uint16  uiFlags;            //!< Specified by IEEE 1588
    uint64  uilCorrectionField; //!< Specified by IEEE 1588
    uint32  uiReserved2;        //!< Specified by IEEE 1588
    uint64  uilClockIdentity;   //!< Specified by IEEE 1588
    uint16  uiSourceId;         //!< Specified by IEEE 1588
    uint16  uiSequenceId;       //!< Specified by IEEE 1588
    uint8   uiControlField;     //!< Specified by IEEE 1588
    uint8   uiInterval;         //!< Specified by IEEE 1588
  } CHeader;
#pragma pack(pop)

public:
  CcPtpProtocol(INetworkProtocol* pParentProtocol);
  virtual ~CcPtpProtocol();

  bool init();
  virtual uint16 getProtocolType() const override;
  virtual bool transmit(CcNetworkPacketRef pPacket) override;
  virtual bool receive(CcNetworkPacketRef pPacket) override;

private:
  CcPtpProtocol(const CcPtpProtocol& oToCopy) = delete;
  CcPtpProtocol(CcPtpProtocol&& oToMove) = delete;
};

#endif //_CcPtProctocol_H_
