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
 * @subpage   CcStdErr
 *
 * @page      CcStdErr
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcStdErr
 */

#ifndef CcStdErr_H_
#define CcStdErr_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcFile.h"

class CcKernelSHARED CcStdErr : public CcFile {
public:
  CcStdErr( void );
  virtual ~CcStdErr();


  virtual size_t size(void) override;
  virtual size_t read(void* buffer, size_t size) override;
  virtual size_t write(const void* buffer, size_t size) override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close() override;
  virtual CcStatus setFilePointer(size_t pos);


  size_t write(CcByteArray &charArray, size_t offset = 0, size_t len = SIZE_MAX);
  size_t read(CcByteArray &charArray, size_t offset = 0, size_t len = SIZE_MAX);
};

#endif /* CcStdErr_H_ */
