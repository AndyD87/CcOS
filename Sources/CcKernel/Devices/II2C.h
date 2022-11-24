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
 * @brief     Class II2C
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "IIo.h"

class CcKernelSHARED II2CClient : public IIo
{
public:
  /**
   * @brief Constructor
   */
  II2CClient(uint16 uiAddress) :
    m_uiAddress(uiAddress)
  {}

  /**
   * @brief Destructor
   */
  virtual ~II2CClient();

  virtual CcStatus open(EOpenFlags)
  { return true; }
  virtual CcStatus close()
  { return true; }
  virtual CcStatus cancel()
  { return true; }


  virtual size_t readByte(uint8& uiData)
  { return read(&uiData, sizeof(uiData)); }
  virtual size_t writeByte(uint8 uiData)
  { return write(&uiData, sizeof(uiData)); }

  virtual size_t readRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize);
  virtual size_t writeRegister8(uint8 uiRegister, void* pBuffer, size_t uiSize);

protected:
  uint16 m_uiAddress;
};

/**
 * @brief Communication Device for I2C
 */
class CcKernelSHARED II2C : public IDevice
{
public:
  /**
   * @brief Constructor
   */
  II2C();

  /**
   * @brief Destructor
   */
  virtual ~II2C();

  virtual II2CClient* createInterface(uint16 uiAddress) = 0;
  virtual void removeInterface(II2CClient* pInterface) = 0;
};

