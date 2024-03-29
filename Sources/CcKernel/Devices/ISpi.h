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
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2017
 * @par       Language: C++11
 * @brief     Class ISpi
 */
#pragma once

#include "CcBase.h"
#include "IDevice.h"
#include "IIo.h"
#include "CcEventHandler.h"

/**
 * @brief ISpi bus device
 */
class CcKernelSHARED ISpi : public IDevice, public IIo
{
public: // Types
  /**
   * @brief Bus configuration type
   */
  enum class EMode
  {
    Undefined = 0,
    Master,
    Slave
  };

public:
  /**
   * @brief Constructor for default Spi Device
   */
  ISpi();
  /**
   * @brief Destructor for SPI device
   */
  virtual ~ISpi();

  /**
   * @brief Set target operation mode like Slave or Master
   * @param eMode: Target Mode as enum
   * @return Status of Operation. True if mode is available and successfully set.
   */
  virtual CcStatus setMode(EMode eMode) = 0;

  /**
   * @brief Get Speed of Bus in Hz
   * @return Speedvalue in Hz
   */
  virtual CcStatus setFrequency(uint32 uiFrequency) = 0;

  /**
   * @brief Get Speed of Bus in Hz
   * @return Speedvalue in Hz
   */
  virtual uint32 getFrequency() = 0;

  /**
   * @brief Read and write to same buffer on write command
   * @param pBuffer:  Target buffer for read from for output and write to on input
   * @param uSize:    Size of Buffer to write and maximum to read.
   * @return Size of buffer read.
   */
  virtual size_t writeRead(void* pBuffer, size_t uSize)
  { return writeRead(pBuffer, pBuffer, uSize); }

  /**
   * @brief Read and write to same buffer on write command
   * @param pWriteBuffer:  Target buffer to read from for output
   * @param pReadBuffer:   Target buffer to write to on input
   * @param uSize:    Size of Buffer to write and maximum to read.
   * @return Size of buffer read.
   */
  virtual size_t writeRead(void* pWriteBuffer, void* pReadBuffer, size_t uSize) = 0;

  //! @return Get current mode of operation
  EMode getMode()
  { return m_eMode;}

  //! @param pEvent: Register event for transfer done
  void registerOnTransferComplete(const CcEvent& pEvent)
  { m_oEventHandler.append(pEvent); }

  //! @brief No need to implement this methods from IoDevice
  //! @{
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  //! @}
protected:
  EMode m_eMode = EMode::Undefined;  //!< Current operation mode, to set from inheriting class
  CcEventHandler m_oEventHandler;    //!< Handler for transfer complete events.
};
