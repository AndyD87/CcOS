/**
* @copyright  Andreas Dirmeier (C) 2017
*
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
 * @page      CcSPI
 * @author    Andreas Dirmeier
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcSPI
 */
#ifndef CCSPI_H
#define CCSPI_H

#include "CcBase.h"
#include "CcKernelBase.h"

#define CCSPI_MASTER  0x01
#define CCSPI_SLAVE   0x02

class CcKernelSHARED CcSPI
{
public:
  CcSPI();
  virtual ~CcSPI();

  virtual bool init() = 0;
  virtual bool readWriteBuffer() = 0;

  bool writeBuffer();
  bool readBuffer();
private:
  uint8  m_SPIType;
  uint8  m_Frequency;
};

#endif // CCSPI_H
