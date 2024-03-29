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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcSha1
 */
#pragma once

#include "CcBase.h"
#include "CcByteArray.h"
#include "CcHash.h"

class CcString;

/**
 * @brief Example Class implementation
 */
class CcKernelSHARED CcSha1 : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcSha1();

  /**
   * @brief Constructor
   */
  CcSha1(const CcByteArray& oInputData );

  /**
   * @brief Destructor
   */
  virtual ~CcSha1();

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcSha1& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcSha1& oToCompare) const;

  /**
   * @brief Directly generate hash from ByteArray
   * @param oByteArray: ByteArray to generate hash from
   * @return this
   */
  CcSha1& operator=(const CcByteArray& oByteArray);

  /**
   * @brief Directly generate hash from String
   * @param sString: String to generate hash from
   * @return this
   */
  CcSha1& operator=(const CcString& sString);

  virtual const CcByteArray& getValue() override
  { return m_oResult; }
  virtual CcSha1& generate(const void* pcData, size_t uiLen) override;
  virtual CcSha1& append(const void* pcData, size_t uiLen) override;
  virtual CcSha1& finalize(const void* pcData, size_t uiLen) override;
  
  virtual size_t getBlockSize() override
  { return 64; }
  virtual size_t getHashSize() override
  { return 20; }

  /**
   * @brief Set midstate of hashing
   * @param oMidstate:  Midstate value
   * @param uiLength:   Size of midstate
   */
  void setMidstate(const CcByteArray& oMidstate, size_t uiLength);
  /**
   * @brief Get calculated midstate befor finishing
   * @return Midstate as array
   */
  const uint32* getMidstate() const
  {return m_aState;}

private:
  void initValues();
  void doTransform(const uchar* input);

private:
  static const uint32 c_aInitState[5];
  static const uint32 c_aK[4];
  CcByteArray m_oContextData;
  CcByteArray m_oResult;
  uint32 m_aState[5];
  size_t m_uiLength = 0;
};
