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
 * @subpage   CcEthernetFrame
 *
 * @page      CcEthernetFrame
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcEthernetFrame
 */
#ifndef _CcEthernetFrame_H_
#define _CcEthernetFrame_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"
#include "CcMacAddress.h"

class CcKernelSHARED CcEthernetFrame
{
public: // types
  typedef struct
  {
    uint8 pPreampble[7];
    uint8 uiStartOfFrame;
    CcMacAddress uiDestination;
    CcMacAddress uiSource;
    uint16 uiType;
    uint8 pPayload[1];
  } SEthernetFrame8023Raw;
  typedef struct
  {
    uint8 pPreampble[7];
    uint8 uiStartOfFrame;
    CcMacAddress uiDestination;
    CcMacAddress uiSource;
    uint16 uiLength;
    uint8 pPayload[1];
  } SEthernetFrame2;
  typedef union
  {
    SEthernetFrame8023Raw oEthernetFrame2;
    SEthernetFrame8023Raw oEthernetFrame8023Raw;
  } SEthernetFrame;
public:
  CcEthernetFrame();
  CcEthernetFrame(const CcEthernetFrame& oToCopy)
    { operator=(oToCopy);}
  CcEthernetFrame(CcEthernetFrame&& oToMove)
    { operator=(std::move(oToMove));}
  ~CcEthernetFrame();
  
  CcEthernetFrame& operator=(CcEthernetFrame&& oToMove);
  CcEthernetFrame& operator=(const CcEthernetFrame& oToCopy);
  bool operator==(const CcEthernetFrame& oToCompare) const;
  bool operator!=(const CcEthernetFrame& oToCompare) const
    { return !operator==(oToCompare);}

  void* getData() const;
  size_t getSize()
    { return m_uiSize; }

private: // Methods
  void clear();
private:
  SEthernetFrame* m_pFrame = nullptr;
  size_t          m_uiSize = 0;
  bool            m_bOwner = false;
};

#endif //_CcEthernetFrame_H_
