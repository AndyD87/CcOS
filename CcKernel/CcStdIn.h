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
 * @subpage   CcStdIn
 *
 * @page      CcStdIn
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStdIn
 */

#ifndef H_CcStdIn_H_
#define H_CcStdIn_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IIo.h"
#ifdef WINDOWS
  #include "CcString.h"
#endif // WINDOWS

/**
 * @brief IoDevice representing the std input.
 *        This makes it possible to use StdIn like streams within CcOS.
 */
class CcKernelSHARED CcStdIn : public IIo 
{
public:

  /**
   * @brief Constructor
   */
  CcStdIn() = default;

  /**
   * @brief Destructor
   */
  virtual ~CcStdIn() = default;

  /**
   * @brief Read an amount of Data from Std-Input
   * @param buffer: buffer to store Data to.
   * @param size: Maximum size of buffer to write
   * @return Number of written byten.
   */
  virtual size_t read(void* pBuffer, size_t uSize) override;

  /**
   * @brief The write function has no effect on StdIn.
   * @param buffer: not used
   * @param size: not used:
   * @return Always 0
   */
  virtual size_t write(const void* pBuffer, size_t uSize) override;


  /**
   * @brief Read an amount of Data from Std-Input but hide output to User.
   * @param buffer: buffer to store Data to.
   * @param size: Maximum size of buffer to write
   * @return Number of written byten.
   */
  size_t readHidden(void* pBuffer, size_t uSize);

  /**
   * @brief Connect to std in
   * @param flags: open mode is allways set to read.
   * @return return true if succeeded
   */
  virtual CcStatus open(EOpenFlags flags) override;

  /**
   * @brief Nothing to cancel on std in
   */
  virtual CcStatus cancel() override
    {return true;}

  /**
   * @brief Close connection to std in.
   * @return always true.
   */
  virtual CcStatus close() override;

  /**
   * @brief Disable input buffering.
   */
  static void disableBuffer();

private:
#ifdef WINDOWS
  CcString m_sTemporaryBackup; //!< Temporary String for oversized unicode strings.
#endif
};

#endif /* H_CcStdIn_H_ */
