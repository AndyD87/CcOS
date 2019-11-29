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
 * @page      Types
 * @subpage   CcStringStream
 *
 * @page      CcStringStream
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcStringStream
 */
#ifndef H_CcStringStream_H_
#define H_CcStringStream_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"
#include "IIo.h"

/**
 * @brief Class for using a string as stream in- and output
 */
class CcKernelSHARED CcStringStream : public IIo
{
public:
  CcStringStream(CcString& rString) : rString(rString)
    {}

  virtual size_t write(const void* pData, size_t uiSize) override;
  virtual size_t read(void *pData, size_t uiSize) override;
  virtual CcStatus open(EOpenFlags) override;
  virtual CcStatus close() override;
  virtual CcStatus cancel() override;
private:
  CcString& rString;
};

#endif // H_CcStringStream_H_
