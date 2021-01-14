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
 * @brief     Class CcSpiSimulation
 */

#ifndef H_CcSpiSimulation_H_
#define H_CcSpiSimulation_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Devices/ISpi.h"
#include "CcByteArray.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcSpiSimulation : public ISpi
{
public:
  /**
   * @brief Destructor
   */
  virtual ~CcSpiSimulation() = default;

  virtual CcStatus setMode(EMode eMode) override;
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus setFrequency(uint32 uiFrequency) override;
  virtual uint32 getFrequency() override;
  virtual size_t writeRead(void* pBuffer, size_t uSize) override;

private:
  EMode       m_eMode;
  uint32      m_uiFrequency;
  CcByteArray m_oDataIn;
  CcByteArray m_oDataOut;
};

#endif // H_CcSpiSimulation_H_
