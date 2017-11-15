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
 * @subpage   CrCrc32
 *
 * @page      CcCrc32
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcCrc32
 */
#ifndef CcCrc32_H_
#define CcCrc32_H_

#include "CcBase.h"
#include "CcKernelBase.h"

class CcByteArray;
class CcString;

class CcKernelSHARED CcCrc32 
{
public:
  /**
   * @brief Constructor
   */
  CcCrc32();
  ~CcCrc32()
    {}

  uint32 getValue() const
    { return ~m_uiCrcValue; }
  void setValue(uint32 uiValue)
    { m_uiCrcValue = ~uiValue; }

  inline CcCrc32& generate(const CcByteArray& oByteArray)
    { return append(oByteArray); }
  CcCrc32& append(const CcByteArray& oByteArray);
  CcCrc32& append(const char *pData, size_t uiSize);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcCrc32& oToCompare) const
    { return m_uiCrcValue == oToCompare.m_uiCrcValue; }

  /**
   * @brief Compare with uint32 value
   * @param uiCrcValue: 32bit Value wich will be compared with containing value
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(uint32 uiCrcValue) const
    { return m_uiCrcValue == ~uiCrcValue; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  inline bool operator!=(const CcCrc32& oToCompare) const
    { return m_uiCrcValue!= oToCompare.m_uiCrcValue; }

  /**
   * @brief Compare with uint32 value
   * @param uiCrcValue: 32bit Value wich will be compared with containing value
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(uint32 uiCrcValue) const
    { return m_uiCrcValue != ~uiCrcValue; }
  inline CcCrc32& operator=(uint32 uiCrcValue)
    {m_uiCrcValue = ~uiCrcValue; return *this;}
private:
  static void computeLookup();
private:
  uint32 m_uiCrcValue = ~((uint32)0);    //!< Red value
  static uint32 s_puiLookUp[256];
  static bool   s_bLookupDone;
};

#endif /* CcCrc32_H_ */
