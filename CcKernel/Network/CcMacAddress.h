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
 * @page      Types
 * @subpage   CcMacAddress
 *
 * @page      CcMacAddress
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMacAddress
 */
#ifndef CcMacAddress_H
#define CcMacAddress_H

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcGlobalStrings.h"

class CcKernelSHARED CcMacAddress
{
public:
  CcMacAddress();
  CcMacAddress(const CcMacAddress& oToCopy)
    { operator=(oToCopy);}
  CcMacAddress(CcMacAddress&& oToMove)
    { operator=(std::move(oToMove));}
  CcMacAddress(const CcString& sMacString);
  CcMacAddress(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0);
  CcMacAddress(uint8* pMac);
  ~CcMacAddress();
  
  CcMacAddress& operator=(CcMacAddress&& oToMove);
  CcMacAddress& operator=(const CcMacAddress& oToCopy);
  bool operator==(const CcMacAddress& oToCompare) const;
  bool operator!=(const CcMacAddress& oToCompare) const
    { return !operator==(oToCompare);}

  /**
   * @brief Set Mac to current connection info
   * @param sMacString: It has to be in format like 127.0.0.1 as localhost
   * @param[out] pbOk: if not null, pbOk will be set to true if conversion was correct.
   * return void
   */
  bool setMac(const CcString& sMacString);

  /**
   * @brief Set Mac to current connection info
   *        Example call would be setMac(127, 0, 0, 1) for localhost
   * @param uiMac3: Highest Mac value
   * @param uiMac2: Higher middle Mac value
   * @param uiMac1: Lower middle Mac value
   * @param uiMac0: Lowest Mac value
   * return void
   */
  bool setMac(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0);

  bool setMac(const uint8* pMac, bool bRevert = false);
  
  uint8 getMacV4_5() const
    { return m_pBuffer[5]; }
  uint8 getMacV4_4() const
    { return m_pBuffer[4]; }
  uint8 getMacV4_3() const
    { return m_pBuffer[3]; }
  uint8 getMacV4_2() const
    { return m_pBuffer[2]; }
  uint8 getMacV4_1() const
    { return m_pBuffer[1]; }
  uint8 getMacV4_0() const
    { return m_pBuffer[0]; }

  CcString getString(const CcString& sSeparator = CcGlobalStrings::Seperators::MacAddress, bool bUpper = false ) const;

private:
  uint8 m_pBuffer[6];
};

#endif // CcMacAddress_H
