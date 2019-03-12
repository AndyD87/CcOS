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
#ifndef _ISpi_H_
#define _ISpi_H_

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
 * @brief ISpi bus device
 * @todo requires an implementation!
 */
class CcKernelSHARED ISpi
{
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
   * @brief Abstract init method has to be overloaded from defining spi device
   * @return true if initialization succeeded
   */
  virtual bool init() = 0;

protected:
  ESpiBusType m_eSpiBusType = ESpiBusType::Undefined; //! Type of configuraion of this bus
  uint8  m_Frequency        = 0;                      //! Frequency of current bus
};

#endif //_ISpi_H_
