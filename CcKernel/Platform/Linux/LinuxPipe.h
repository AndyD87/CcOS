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
 * @page      Windows
 * @subpage   LinuxPipe
 *
 * @page      LinuxPipe
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class LinuxPipe
 */
#ifndef LinuxPipe_H_
#define LinuxPipe_H_

#include "CcBase.h"
#include "CcIODevice.h"

/**
* @brief Button for GUI Applications
*/
class LinuxPipe : public CcIODevice
{
public:
  /**
   * @brief Constructor
   */
  LinuxPipe();

  /**
   * @brief Destructor
   */
  virtual ~LinuxPipe(void);

  /**
   * @brief Read an amount of Data from inheriting Device.
   * @param buffer: Buffer to load data to.
   * @param size: Maximum Size of buffer to write.
   * @return Number of Bytes read from device.
   */
  size_t read(char* buffer, size_t size) override;

  /**
   * @brief Write an amount of Data to inheriting Device.
   * @param buffer: Buffer to load data from.
   * @param size: Maximum size of buffer to read.
   * @return Number of Bytes written to device.
   */
  size_t write(const char* buffer, size_t size) override;

  /**
   * @brief Open Device in a specific mode.
   *        For more informations lock at: @ref EOpenFlags
   * @return true if Device was opened successfully.
   */
  bool open(EOpenFlags) override;

  /**
   * @brief Close the connection to device.
   * @return true if Connection was successfully closed.
   */
  bool close() override;

  /**
   * @brief Cancel Current Operation.
   *        It can optionally be Implemented from inheriting Device.
   *        Look at device definintion it it supports canceling.
   * @return true if Opperation was aborted successfully.
   */
  bool cancel() override;

  int* getPipes()
    { return m_iPipes; }

public:
  int m_iPipes[2] = {-1, -1};
};

#endif /* LinuxPipe_H_ */
