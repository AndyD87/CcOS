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
 * @subpage   CcSPI
 * 
 * @page      CcSPI
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2017
 * @par       Language: C++11
 * @brief     Class CcSPI
 */
#ifndef CCSPI_H
#define CCSPI_H

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Bus configuration type
 */
enum class ESpiBusType
{
  Undefined = 0,
  Master,
  Slave
};

/**
 * @brief CcSPI bus device
 * @todo requires an implementation!
 */
class CcKernelSHARED CcSPI
{
public:
  /**
   * @brief Constructor for default Spi Device
   */
  CcSPI();
  /**
   * @brief Destructor for SPI device
   */
  virtual ~CcSPI();

  /**
   * @brief Abstract init method has to be overloaded from defining spi device
   * @return true if initialization succeeded
   */
  virtual bool init() = 0;

private:
  ESpiBusType m_eSpiBusType; //! Type of configuraion of this bus
  uint8  m_Frequency;        //! Frequency of current bus
};

#endif // CCSPI_H
