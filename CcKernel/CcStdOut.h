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
 * @page      CcKernel
 * @subpage   CcStdOut
 *
 * @page      CcStdOut
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStdOut
 */

#ifndef _CcStdOut_H_
#define _CcStdOut_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"

/**
 * @brief IoDevice representing the std output.
 *        This makes it possible to use std out like streams within CcOS.
 */
class CcKernelSHARED CcStdOut : public CcIODevice 
{
public:
  /**
   * @brief Constructor
   */
  CcStdOut() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcStdOut() = default;

  /**
   * @brief Reading from std out is not possible. It will return with SIZE_MAX
   * @param pBuffer: unused
   * @param uSize: unused
   * @return always SIZE_MAX
   */
  size_t read(void* pBuffer, size_t uSize) override;

  /**
   * @brief Write a number of Bytes to std out
   * @param pBuffer: Buffer to write out
   * @param uSize: number of bytes in pBuffer to write.
   * @return Number of Bytes written to std out.
   */
  size_t write(const void* pBuffer, size_t uSize) override;

  /**
   * @brief Open does nothing on std out. It will allways be true.
   *        std out will always be opend as Write.
   * @param eOpenFlags: unused
   * @return alway successfuly state.
   */
  CcStatus open(EOpenFlags eOpenFlags) override;

  /**
   * @brief std out could not be closed, it is always here.
   * @return alway successfuly state.
   */
  CcStatus close() override;
  /**
   * @brief Cancel all currently running Operations
   * @return true if all was canceled successfully
   * @todo Implementation if required
   */
  CcStatus cancel()override {return false;}

  /**
   * @brief Disable output buffering.
   */
  static void disableBuffer();
};

#endif /* _CcStdOut_H_ */
