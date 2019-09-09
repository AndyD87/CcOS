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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStdFile
 */

#ifndef H_CcStdFile_H_
#define H_CcStdFile_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "IIo.h"
#include <stdio.h>

class CcKernelSHARED CcStdFile : public IIo {
public:
  CcStdFile(FILE* stdFile);
  virtual ~CcStdFile() = default;


  virtual size_t size();
  virtual size_t read(void* pBuffer, size_t uSize) override;
  virtual size_t write(const void* pBuffer, size_t uSize) override;
  virtual CcStatus open(EOpenFlags flags) override;
  virtual CcStatus close() override;

private:
  FILE* m_File;
};

#endif /* H_CcStdFile_H_ */
