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
 * @brief     Class CcEepromSimulation
 */

#ifndef H_CcEepromSimulation_H_
#define H_CcEepromSimulation_H_

#include "CcBase.h"
#include "CcBase.h"
#include "Devices/IEeprom.h"
#include "CcByteArray.h"
#include "CcBinaryStream.h"

/**
 * @brief Control the Input and Outputports on device
 */
class CcKernelSHARED CcEepromSimulation : public IEeprom
{
public:

  CcEepromSimulation(size_t uiSize);

  /**
   * @brief Destructor
   */
  virtual ~CcEepromSimulation() = default;

  virtual size_t size() const override;
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus setPosition(size_t uiPosition) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;

private:
  CcByteArray     m_oData;
  CcBinaryStream  m_oStream;
};

#endif // H_CcEepromSimulation_H_
