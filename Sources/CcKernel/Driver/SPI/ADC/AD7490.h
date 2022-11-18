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
 * @copyright Andreas Dirmeier (C) 2021
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class AD7490
 */

#pragma once
#include "CcBase.h"
#include "Devices/ISpi.h"

/**
 * @brief Driver for AD7490 Chip
 */
class AD7490
{
public:
  /**
   * @brief Create ADC with target spi interface
   * @para pSpiDevice: Target interface for communication
   */
  AD7490(ISpi* pSpiDevice);

  /**
   * @brief Remove ADC
   */~AD7490();

  /**
   * @brief Initialize ADC with it's init sequence and basic settings
   * @return True if init succeeded
   */
  bool Init();

  /**
   * @brief Read value from device.
   *        ISelector will automaticaly select the device from Hal.
   * @param uiAddress: Adress returend from ADC response masked with 0x7
   *                   to verify requested address.
   *                   The 4th bit is ignored so it's not finaly the whole address.
   * @return Target value from ADC masked with 0x0fff
   */
  uint16_t readValue(uint8_t& uiAddress);

  /**
   * @brief Set next address to receive value from
   * @param uiAddress: Next address to request for value
   */
  void setAddress(uint16_t uiAddress);

  /**
   * @brief Set range of ADC if Reference is doubled for conversion.
   * @param bDouble: True if Reference voltage should be doulbed for next conversion.
   */
  void setRange(bool bDouble);

  /**
   * @brief Set coding of output value. It can be the natural value or a 2 complement of it.
   * @param bComplement: True if complement sould be created on next conversion
   */
  void setCoding(bool bComplement);

  /**
   * @brief Set weak or tristate output after last bit was transfered.
   * @param bWeak: True if output should be weak, or false if output should be tristate
   */
  void setWeakOutput(bool bWeak);

  /**
   * @brief Set shadow access of next conversion.
   *        This would allow to setup the enabled Input pins for a sequenced access.
   *        Shadow access is not supported yet in this object, so the value should be false
   * @param bShadow: True if shadow access should be enabled.
   */
  void setShadow(bool bShadow);

private:
  ISpi* m_pSpi;
  uint16_t m_uiTransferValue = 0;
  uint16_t m_uiResultValue   = 0;
};
