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
 * @brief     Class CcMacAddress
 */
#pragma once

#include "CcBase.h"
#include "CcGlobalStrings.h"

/**
 * @brief Mac address storage class with option to generate and read strings.
 */
class CcKernelSHARED CcMacAddress
{
public:
  CcMacAddress() = default;
  ~CcMacAddress() = default;
  CCDEFINE_COPY_CONSTRUCTOR_TO_OPERATOR(CcMacAddress)

  /**
   * @brief Create mac address object from existing string.
   * @param sMacString: String to get mac address from
   */
  CcMacAddress(const CcString& sMacString);

  /**
   * @brief Initialize mac address with a uint8 value for every 6 parts of the address.
   *        Format of input example is: uiMac5:uiMac4:uiMac3:uiMac2:uiMac1:uiMac0
   * @param uiMac5: 6th value to set for mac address
   * @param uiMac4: 5th value to set for mac address
   * @param uiMac3: 4th value to set for mac address
   * @param uiMac2: 3rd value to set for mac address
   * @param uiMac1: 2nd value to set for mac address
   * @param uiMac0: 1st value to set for mac address
   */
  CcMacAddress(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0);

  /**
   * @brief Generate mac address from existing array.
   * @param pMac:     Mac address with least significant byte first
   * @param bRevert:  Revert direction so first is most significant
   */
  CcMacAddress(uint8* pMac, bool bRevert = false);

  /**
   * @brief Copy value of an other mac address to this.
   * @param oToCopy: Object to copy from
   * @return Handle to this
   */
  CcMacAddress& operator=(const CcMacAddress& oToCopy) NOEXCEPT;

  /**
   * @brief Compare this mac address with an other if they are same.
   * @param oToCompare: Mac address to compare with
   * @return True if they are same
   */
  bool operator==(const CcMacAddress& oToCompare) const;

  /**
   * @brief Set Mac to current connection info
   * @param sMacString: It has to be in format like 127.0.0.1 as localhost
   * @param[out] pbOk: if not null, pbOk will be set to true if conversion was correct.
   * return void
   */
  bool setMac(const CcString& sMacString);

  /**
   * @brief Set ech single mac address byte with this method.
   *        Format of input example is: uiMac5:uiMac4:uiMac3:uiMac2:uiMac1:uiMac0
   * @param uiMac5: 6th value to set for mac address
   * @param uiMac4: 5th value to set for mac address
   * @param uiMac3: 4th value to set for mac address
   * @param uiMac2: 3rd value to set for mac address
   * @param uiMac1: 2nd value to set for mac address
   * @param uiMac0: 1st value to set for mac address
   * @return True if successfully set
   */
  bool setMac(uint8 uiMac5, uint8 uiMac4, uint8 uiMac3, uint8 uiMac2, uint8 uiMac1, uint8 uiMac0);

  /**
   * @brief Generate mac address from existing array.
   * @param pMac:     Mac address with least significant byte first
   * @param bRevert:  Revert direction so first is most significant
   */
  bool setMac(const uint8* pMac, bool bRevert = false);

  //! @brief Set broadcast address with ff:ff:ff:ff:ff:ff
  void setBroadcast();

  //! @return Get 6th value of mac address
  uint8 getMacV4_5() const
  { return m_pBuffer[5]; }
  //! @return Get 5th value of mac address
  uint8 getMacV4_4() const
  { return m_pBuffer[4]; }
  //! @return Get 4th value of mac address
  uint8 getMacV4_3() const
  { return m_pBuffer[3]; }
  //! @return Get 3rd value of mac address
  uint8 getMacV4_2() const
  { return m_pBuffer[2]; }
  //! @return Get 2nd value of mac address
  uint8 getMacV4_1() const
  { return m_pBuffer[1]; }
  //! @return Get 1st value of mac address
  uint8 getMacV4_0() const
  { return m_pBuffer[0]; }

  //! @return Get mac address as array with leas significant first.
  const uint8* getMac() const
  { return m_pBuffer; }

  //! @return True if mac address value is default (00:00:00:00:00:00)
  bool isNull() const;
  //! @return True if mac address value is broadcast (ff:ff:ff:ff:ff:ff)
  bool isBroadcast() const;

  /**
   * @brief Convert stored mac address to string.
   * @param sSeparator: Target seperator us use between values
   * @param bUpper:     True for using upper Hex value, false for lower.
   * @return Converted string.
   */
  CcString getString(const CcString& sSeparator = CcGlobalStrings::Seperators::MacAddress, bool bUpper = false ) const;

private:
  uint8 m_pBuffer[6] = {0}; //!< Mac address as 6 byte array
};
