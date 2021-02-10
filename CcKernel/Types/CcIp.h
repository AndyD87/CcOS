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
 * @brief     Class CcIp
 */
#ifndef H_CcIp_H_
#define H_CcIp_H_

#include "CcBase.h"
#include "CcBase.h"
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
  /**
   * @brief Ip with empty string
   */
  CcIp();
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcIp)

  /**
   * @brief Initialize ip from string
   * @param pString: String to parse
   */
  CcIp(const char* pString)
  { setIp(pString); }

  /**
   * @brief Initialize ip from string
   * @param sIpString: String to parse
   */
  CcIp(const CcString& sIpString);

  /**
   * @brief Initialize ip direct with ipv4 values
   * @param uiIp3: 4th and most significant value
   * @param uiIp2: 3rd ip value
   * @param uiIp1: 2nd ip value
   * @param uiIp0: 1st and least significant
   */
  CcIp(uint8 uiIp3, uint8 uiIp2, uint8 uiIp1, uint8 uiIp0);

  /**
   * @brief Import ip from byte array.
   * @param pIpV4:  Ip as array
   * @param bSwap:  True for swap/reverse order
   */
  CcIp(const uint8* pIpV4, bool bSwap = false);
  ~CcIp();

  /**
   * @brief Copy ip address from another value
   * @param oToCopy: Ip to copy from
   * @return Handle to this
   */
  CcIp& operator=(const CcIp& oToCopy);

  /**
   * @brief Compare if ip address is same as another
   * @param oToCompare: Ip to compare
   * @return True if they are same
   */
  bool operator==(const CcIp& oToCompare) const;

  /**
   * @brief Compare if ip address is not same as another
   * @param oToCompare: Ip to compare
   * @return True if they are not same
   */
  bool operator!=(const CcIp& oToCompare) const
  { return !operator==(oToCompare);}
  
  /**
   * @brief Check if this ip address is lower than other
   * @param oToCompare: Ip to compare
   * @return True if ip is lower
   */
  bool operator<(const CcIp& oToCompare) const;

  /**
   * @brief Check if this ip address is greater than other
   * @param oToCompare: Ip to compare
   * @return True if ip is greater
   */
  bool operator>(const CcIp& oToCompare) const;

  /**
   * @brief Add offset to this ip and create new
   * @param uiToAdd: Distance to add
   * @return New generated ip
   */
  CcIp operator+(uint32 uiToAdd) const
  { CcIp oIp(*this); oIp.add(uiToAdd); return oIp; }

  /**
   * @brief Add distance to this ip
   * @param uiToAdd: Distance to add
   * @return Handle to this
   */
  CcIp& operator+=(uint32 uiToAdd)
  { return add(uiToAdd); }

  /**
   * @brief increase ip by one
   * @return Handle to this;
   */
  CcIp& operator++()
  { return add(1); }
  //! @copydoc CcIp::operator++()
  CcIp& operator++(int)
  { return add(1); }

  /**
   * @brief Set IP to current connection info
   * @param sIpString: It has to be in format like 127.0.0.1 as localhost
   * @return True if ip was parse successfully
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

  /**
   * @brief Import ip from byte array.
   * @param pIpv4:  Ip as array
   * @param bSwap:  True for swap/reverse order
   * @return True if set successfully
   */
  bool setIpV4(const uint8* pIpv4, bool bSwap = false);

  /**
   * @brief Add distance to this ip
   * @param iToAdd: distance to add
   * @return Handle to this
   */
  CcIp& add(uint32 iToAdd);

  //! @return True if ip is 0.0.0.0
  bool isNullIp() const;
  //! @copydoc isNullIp()
  bool isNull() const
  { return isNullIp(); }
  //! @return True if ip is 255.255.255.255
  bool isMulticastIp() const;
  //! @return Ip as array
  const uint8* getIpV4() const
  { return Data.m_pArrayV4; }
  //! @return 4th as most significant value
  const uint8& getIpV4_3() const
  { return Data.m_pArrayV4[3]; }
  //! @return 3rd value of IpV4
  const uint8& getIpV4_2() const
  { return Data.m_pArrayV4[2]; }
  //! @return 2nd value of IpV4
  const uint8& getIpV4_1() const
  { return Data.m_pArrayV4[1]; }
  //! @return 1st and least significant value of IpV4
  const uint8& getIpV4_0() const
  { return Data.m_pArrayV4[0]; }
  //! @return Get editable 4th as most significant value
  uint8& getIpV4_3()
  { return Data.m_pArrayV4[3]; }
  //! @return Get editable 3rd value of IpV4
  uint8& getIpV4_2()
  { return Data.m_pArrayV4[2]; }
  //! @return Get editable 2nd value of IpV4
  uint8& getIpV4_1()
  { return Data.m_pArrayV4[1]; }
  //! @return Get editable 1st and least significant value of IpV4
  uint8& getIpV4_0()
  { return Data.m_pArrayV4[0]; }

  /**
   * @brief Get ip as uint32 value
   * @param bReverse: Invert ip order if true
   * @return Ip as uint32 value
   */
  uint32 getUint32(bool bReverse) const;

  /**
   * @brief Get ip as string value like 192.168.0.1
   * @return Ip as string
   */
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

#endif // H_CcIp_H_
