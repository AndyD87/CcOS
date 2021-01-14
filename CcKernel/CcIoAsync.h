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
 * @brief     Class CcIoAsync
 */

#ifndef H_CcIoAsync_H_
#define H_CcIoAsync_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcObject.h"
#include "IIo.h"

#define CcIoAsync_CB_READDONE   0
#define CcIoAsync_CB_WRITEDONE  1


/**
 * @brief Abstract Class for inheriting to every IODevice
 */
class CcKernelSHARED CcIoAsync : private CcObject
{
public:
  /**
   * @brief Constructor
   */
  CcIoAsync(IIo *device);

  /**
   * @brief Destructor
   */
  virtual ~CcIoAsync();
  virtual bool read(char* pBuffer, size_t uSize);
  virtual bool write(const char* pBuffer, size_t uSize);

  virtual bool onReadDone(size_t size);
  virtual bool onWriteDone(size_t size);

private:
  IIo *m_Device;
};

#endif // H_CcIoAsync_H_
