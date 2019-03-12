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
#ifndef _CCSHA256_H_
#define _CCSHA256_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcByteArray.h"
#include "IHash.h"

class CcString;

/**
 * @brief Example Class impelmentation
 */
class CcKernelSHARED CcSha256 : public IHashAbstract
{
public:
  /**
   * @brief Constructor
   */
  CcSha256();
  
  /**
   * @brief Constructor
   */
  CcSha256(const CcByteArray& oInputData );

  /**
   * @brief Destructor
   */
  virtual ~CcSha256();
  
  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcSha256& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcSha256& oToCompare) const;

  /**
   * @brief Directly generate hash from ByteArray
   * @param oByteArray: ByteArray to generate hash from
   * @return this
   */
  CcSha256& operator=(const CcByteArray& oByteArray);

  /**
   * @brief Directly generate hash from String
   * @param sString: String to generate hash from
   * @return this
   */
  CcSha256& operator=(const CcString& sString);

  //! @copydoc
  virtual const CcByteArray& getValue() const override
   { return m_oResult; }
  //! @copydoc
  virtual CcByteArray& getValue() override
   { return m_oResult; }

  //! @copydoc
  virtual CcSha256& generate(const void* pcData, size_t uiLen) override;
  //! @copydoc
  virtual CcSha256& append(const void* pcData, size_t uiLen) override;
  //! @copydoc
  virtual CcSha256& finalize(const void* pcData, size_t uiLen) override;
  
  inline CcSha256& generate(const CcByteArray& oByteArray)
    { return generate(oByteArray.getArray(), oByteArray.size());}
  inline CcSha256& append(const CcByteArray& oByteArray)
    { return append(oByteArray.getArray(), oByteArray.size());}
  inline CcSha256& finalize(const CcByteArray& oByteArray)
    { return finalize(oByteArray.getArray(), oByteArray.size());}
  inline CcSha256& finalize()
    { return finalize(nullptr, 0);}

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

#endif /* _CCSHA256_H_ */
