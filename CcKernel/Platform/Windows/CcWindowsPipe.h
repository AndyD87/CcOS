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
 * @brief     Class CcWindowsPipe
 */
#pragma once

#include "CcBase.h"
#include "CcWindowsGlobals.h"
#include "IIo.h"
#include "CcByteArray.h"
#include "CcMutex.h"

/**
 * @brief Button for GUI Applications
 */
class CcWindowsPipe : public IIo, private CcMutex
{
public:
  /**
   * @brief Constructor
   */
  CcWindowsPipe();

  /**
   * @brief Destructor
   */
  virtual ~CcWindowsPipe();
  
  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param pBuffer: Buffer to load data to.
   * @param uSize:   Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  virtual size_t read(void* pBuffer, size_t uSize) override;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param pBuffer: Buffer to load data from.
   * @param uSize:   Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  virtual size_t write(const void* pBuffer, size_t uSize) override;

  /**
   * @brief Open Device in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return Status of operation
   */
  virtual CcStatus open(EOpenFlags) override;

  /**
   * @brief Close the connection to device.
   * @return true if Connection was successfully closed.
   */
  virtual CcStatus close() override;

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting Device.
   *        Look at device definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  virtual CcStatus cancel() override;

  /**
   * @brief Read data from windows cache to m_oReadBuffer
   */
  void readCache();

public:
  CcByteArray m_oReadBuffer;//!< Tempary buffer for readings from peer
  HANDLE      m_HandleIn;   //!< Input stream of peer
  HANDLE      m_HandleOut;  //!< Output stream of peer
  HANDLE      m_hWrite;     //!< Write stream to push data to peer
  HANDLE      m_hRead;      //!< Read steram to pull data from peer
};
