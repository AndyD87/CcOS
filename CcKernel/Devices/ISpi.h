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
 * @page      Communication
 * @subpage   ISpi
 * 
 * @page      ISpi
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2017
 * @par       Language: C++11
 * @brief     Class ISpi
 */
#ifndef H_ISpi_H_
#define H_ISpi_H_

#include "CcBase.h"
#include "IDevice.h"
#include "IIo.h"
#include "CcBase.h"
#include "CcEventHandler.h"

/**
 * @brief ISpi bus device
 * @todo requires an implementation!
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

  virtual size_t writeRead(void* pBuffer, size_t uSize) = 0;

  EMode getMode()
    { return m_eMode;}
  void registerOnTransferComplete(const CcEvent& pEvent)
    { m_oEventHandler.append(pEvent); }

  //! @brief No need to implement this methods from IoDevice
  //! @{
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
  //! @}
protected:
  EMode m_eMode = EMode::Undefined;  //! Current operation mode
  CcEventHandler m_oEventHandler;
};

#endif //_ISpi_H_
