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
 * @subpage   CcCrc32
 *
 * @page      CcCrc32
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcCrc32
 */
#ifndef H_CcCrc32_H_
#define H_CcCrc32_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcHash.h"
#include "CcByteArray.h"

class CcByteArray;
class CcString;

/**
 * @brief Create Crc32 Hashes with this class.
 *        It is addtionally possible to continue an older Crc32 Session.
 */
class CcKernelSHARED CcCrc32 : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcCrc32();
  virtual ~CcCrc32() = default;
  
  virtual const CcByteArray& getValue() const override
   { return m_oCrcValue; }
  virtual CcByteArray& getValue() override
   { return m_oCrcValue; }
  uint32 getValueUint32() const
    { return ~castUint32(); }
  void setValueUint32(uint32 uiValue)
    { castUint32() = ~uiValue; }

  virtual CcCrc32& generate(const void* pData, size_t uiSize) override;
  virtual CcCrc32& append(const void* pData, size_t uiSize) override;
  virtual CcCrc32& finalize(const void* pData, size_t uiSize) override;
  
  inline CcCrc32& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}
  inline CcCrc32& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}
  inline CcCrc32& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}
  inline CcCrc32& finalize()
    { return finalize(nullptr, 0);}
  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcCrc32& oToCompare) const
    { return castUint32() == oToCompare.castUint32(); }

  /**
   * @brief Compare with uint32 value
   * @param uiCrcValue: 32bit Value wich will be compared with containing value
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(uint32 uiCrcValue) const
    { return castUint32() == ~uiCrcValue; }

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator!=(const CcCrc32& oToCompare) const
    { return castUint32() != oToCompare.castUint32(); }

  /**
   * @brief Compare with uint32 value
   * @param uiCrcValue: 32bit Value wich will be compared with containing value
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(uint32 uiCrcValue) const
    { return castUint32() != ~uiCrcValue; }
  inline CcCrc32& operator=(uint32 uiCrcValue)
    { castUint32() = ~uiCrcValue; return *this;}
private:
  static void computeLookup();
  inline const uint32& castUint32() const
    { return *static_cast<const uint32*>(static_cast<const void*>(&m_oCrcValue[0]));}
  inline uint32& castUint32()
    { return *static_cast<uint32*>(static_cast<void*>(&m_oCrcValue[0]));}
private:
  CcByteArray   m_oCrcValue;    //!< Red value
  static uint32 s_puiLookUp[256];
  static bool   s_bLookupDone;
};

#endif // H_CcCrc32_H_
