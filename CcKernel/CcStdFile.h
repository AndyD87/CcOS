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
 */
/**
 * @page      CcKernel
 * @subpage   CcStdFile
 *
 * @page      CcStdFile
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcStdFile
 */

#ifndef CcStdFile_H_
#define CcStdFile_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "CcIODevice.h"
#include <stdio.h>

class CcKernelSHARED CcStdFile : public CcIODevice {
public:
  CcStdFile(FILE* stdFile);
  virtual ~CcStdFile();


  virtual size_t size(void);
  virtual size_t read(char* buffer, size_t size) override;
  virtual size_t write(const char* buffer, size_t size) override;
  virtual bool open(EOpenFlags flags) override;
  virtual bool close() override;
  virtual bool setFilePointer(size_t pos);

private:
  FILE* m_File;
};

#endif /* CcStdFile_H_ */
