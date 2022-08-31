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
 * @brief     Class CcLinuxPipe
 */
#pragma once

#include "CcBase.h"
#include "IIo.h"

/**
* @brief Button for GUI Applications
*/
class CcLinuxPipe : public IIo
{
public:
  /**
   * @brief Constructor
   */
  CcLinuxPipe();

  /**
   * @brief Destructor
   */
  virtual ~CcLinuxPipe();

  size_t read(void* pBuffer, size_t uSize) override;
  size_t write(const void* pBuffer, size_t uSize) override;
  CcStatus open(EOpenFlags) override;
  CcStatus close() override;
  CcStatus cancel() override;

  /**
   * @brief Close pipe from index and set -1
   * @param iPipeDirection: Pipe direction index
   * @param iPipenumber:    Pipe number from direction
   */
  void closePipe(int iPipeDirection, int iPipenumber);

  /**
   * @brief Close child pipes
   */
  void init();

  /**
   * @brief Close child pipes
   */
  void setupChild();

  /**
   * @brief Close parent pipes
   */
  void setupParent();

  /**
   * @brief Close child pipes
   */
  void closeChild();

  /**
   * @brief Close parent pipes
   */
  void closeParent();
private:
  int m_iPipes[3][2] = {{-1, -1},{-1,-1},{-1,-1}};
};
