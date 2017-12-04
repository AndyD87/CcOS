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
 * @page      Hash
 * @subpage   CcMd5
 *
 * @page      CcMd5
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcMd5
 */
#ifndef CcMd5_H_
#define CcMd5_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcByteArray.h"

class CcString;

class CcKernelSHARED CcMd5 
{
public:
  /**
   * @brief Constructor
   */
  CcMd5();
  ~CcMd5()
    {}

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcMd5& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcMd5& oToCompare) const;
  CcMd5& operator=(const CcByteArray& oByteArray);
  CcMd5& operator=(const CcString& sHexString);

  const CcByteArray& getValue() const
   { return m_oResult; }
  CcByteArray& value()
   { return m_oResult; }

  CcString getHexString() const;
  void setHexString(const CcString& sHexString);

  inline void generate(const CcByteArray& oByteArray)
   { initValues(); append(oByteArray); finalize(); }
  void append(const CcByteArray& oByteArray);
  void append(const char *pData, size_t uiSize);
  void finalize();

private:
  void initValues();
  void update(const void *data, size_t size);
  const void* body(const void *data, size_t size);
private:
  uint32 a;
  uint32 b;
  uint32 c;
  uint32 d;
  uint32 lo;
  uint32 hi;
  uint8  buffer[64];
  uint32 block[16];
  CcByteArray m_oResult;
};

#endif /* CcMd5_H_ */
