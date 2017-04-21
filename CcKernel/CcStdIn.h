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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcStdIn
 */

#ifndef CcStdIn_H_
#define CcStdIn_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcIODevice.h"


class CcKernelSHARED CcStdIn : public CcIODevice 
{
public:
  CcStdIn( void );
  virtual ~CcStdIn();

  /**
   * @brief Read an amount of Data from Std-Input
   * @param buffer: buffer to store Data to.
   * @param size: Maximum size of buffer to write
   * @return Number of written byten.
   */
  virtual size_t read(char* buffer, size_t size) override;

  /**
   * @brief The write function has no effect on StdIn.
   * @param buffer: not used
   * @param size: not used:
   * @return Always 0
   */
  virtual size_t write(const char* buffer, size_t size) override;

  /**
   * @brief Connect to std in
   * @param flags: open mode is allways set to read.
   * @return return true if succeeded
   */
  virtual bool open(EOpenFlags flags);

  virtual bool cancel() override
    {return true;}

  /**
   * @brief Close connection to std in.
   * @return always true.
   */
  virtual bool close();
};

#endif /* CcStdIn_H_ */
