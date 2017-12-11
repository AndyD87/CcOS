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
 * @subpage   CcSha256
 *
 * @page      CcSha256
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSha256
 */
#ifndef CCSHA256_H_
#define CCSHA256_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcByteArray.h"
#include "CcHash.h"

class CcString;

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcSha256 : public CcHash
{
public:
  /**
   * @brief Constructor
   */
  CcSha256( void );
  
  /**
   * @brief Constructor
   */
  CcSha256(const CcByteArray& oInputData );

  /**
   * @brief Destructor
   */
  ~CcSha256( void );
  
  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcSha256& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcSha256& oToCompare) const;
  CcSha256& operator=(const CcByteArray& oByteArray);
  CcSha256& operator=(const CcString& sHexString);

  const CcByteArray& getValue() const
   { return m_oResult; }
  CcByteArray& value()
   { return m_oResult; }

  CcSha256& generate(const char* pcData, size_t uiLen);
  CcSha256& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}
  CcSha256& append(const char* pcData, size_t uiLen);
  CcSha256& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}
  CcSha256& finalize(const char* pcData, size_t uiLen);
  CcSha256& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}

  void setMidstate(const CcByteArray& oMidstate, size_t uiLength);
  const uint32* getMidstate() const
    {return m_aState;}

private:
  static uint32 doRor32(uint32 word, unsigned int shift);
  void doTransform(const uchar* input);
  static inline uint32 s0(uint32 x)
    { return doRor32(x, 7) ^ doRor32(x, 18) ^ (x >> 3); }
  static inline uint32 s1(uint32 x) 
    { return doRor32(x, 17) ^ doRor32(x, 19) ^ (x >> 10); }

private:
  static const uint32 c_aInitState[8];
  static const uint32 c_aK[64];
  CcByteArray m_oResult;
  uint32 m_aState[8];
  size_t m_uiLength = 0;
};

#endif /* CCSHA256_H_ */
