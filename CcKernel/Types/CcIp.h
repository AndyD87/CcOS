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
 * @subpage   CcIp
 *
 * @page      CcIp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIp
 */
#ifndef _CcIp_H_
#define _CcIp_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcTypes.h"
#include "CcString.h"

/**
 * @brief Class for handling IpV4 and IpV6 Addresses.
 *        Good for conversions from string to binary.
 * @todo  Currently, just IpV4 is working.
 */
class CcKernelSHARED CcIp
{
public:
  CcIp();
  CcIp(const CcIp& oToCopy)
    { operator=(oToCopy);}
  CcIp(CcIp&& oToMove)
    { operator=(std::move(oToMove));}
  CcIp(const char* pString)
    { setIp(pString); }
  CcIp(const CcString& sIpString);
  CcIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0);
  CcIp(const uint8* pIpV4, bool bSwap = false);
  ~CcIp();

  CcIp& operator=(CcIp&& oToMove);
  CcIp& operator=(const CcIp& oToCopy);
  bool operator==(const CcIp& oToCompare) const;
  bool operator!=(const CcIp& oToCompare) const
    { return !operator==(oToCompare);}
  
  bool operator<(const CcIp& oToCompare) const;
  bool operator>(const CcIp& oToCompare) const;
  CcIp operator+(uint32 uiToAdd) const
    { CcIp oIp(*this); oIp.add(uiToAdd); return oIp; }
  CcIp& operator+=(uint32 uiToAdd)
    { return add(uiToAdd); }
  CcIp& operator++()
    { return add(1); }
  CcIp& operator++(int)
    { return add(1); }

  /**
   * @brief Set IP to current connection info
   * @param sIpString: It has to be in format like 127.0.0.1 as localhost
   * @param[out] pbOk: if not null, pbOk will be set to true if conversion was correct.
   * return void
   */
  bool setIp(const CcString& sIpString);

  /**
   * @brief Set IP to current connection info
   *        Example call would be setIp(127, 0, 0, 1) for localhost
   * @param uiIp3: Highest Ip value
   * @param uiIp2: Higher middle Ip value
   * @param uiIp1: Lower middle Ip value
   * @param uiIp0: Lowest Ip value
   * return void
   */
  bool setIpV4(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0);
  bool setIpV4(const uint8* pIpv4, bool bSwap = false);

  CcIp& add(uint32 iToAdd);

  bool isNullIp() const;
  bool isNull() const
    { return isNullIp(); }
  bool isMulticastIp() const;
  const uint8* getIpV4() const
    { return Data.m_pArrayV4; }
  const uint8& getIpV4_3() const
    { return Data.m_pArrayV4[3]; }
  const uint8& getIpV4_2() const
    { return Data.m_pArrayV4[2]; }
  const uint8& getIpV4_1() const
    { return Data.m_pArrayV4[1]; }
  const uint8& getIpV4_0() const
    { return Data.m_pArrayV4[0]; }
  uint8& getIpV4_3()
    { return Data.m_pArrayV4[3]; }
  uint8& getIpV4_2()
    { return Data.m_pArrayV4[2]; }
  uint8& getIpV4_1()
    { return Data.m_pArrayV4[1]; }
  uint8& getIpV4_0()
    { return Data.m_pArrayV4[0]; }

  uint32 getUint32(bool bReverse) const;
  CcString getString() const;

private:
  EIpType m_eIpType = EIpType::Unknown;
  union CcKernelSHARED
  {
    uint16  m_pArrayV6[8];
    uint64  m_pArrayV6Uint64[2];
    uint8   m_pArrayV4[4];
    uint32  m_pArrayV4Uint32;
  } Data;
};

#endif //_CcIp_H_
