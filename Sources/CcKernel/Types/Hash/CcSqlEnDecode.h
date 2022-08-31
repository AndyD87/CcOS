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
 * @brief     Class CcSqlEnDecode
 */
#pragma once

#include "CcBase.h"
#include "CcHash.h"
#include "CcByteArray.h"

class CcByteArray;
class CcString;

#define CcSqlEnDecode_CryptoBufferSize 256

/**
 * @brief Create Crc32 Hashes with this class.
 *        It is addtionally possible to continue an older Crc32 Session.
 */
class CcKernelSHARED CcSqlEnDecode : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcSqlEnDecode();
  virtual ~CcSqlEnDecode() = default;

  virtual const CcByteArray& getValue() override
  { return m_oValue; }
  virtual CcSqlEnDecode& generate(const void* pData, size_t uiSize) override;
  virtual CcSqlEnDecode& append(const void* pData, size_t uiSize) override;
  virtual CcSqlEnDecode& finalize(const void* pData, size_t uiSize) override;
  virtual IHash& setKey(const void* pcData, size_t uiLen) override;
  virtual const CcByteArray& decode(const void* pcData, size_t uiLen) override;
  
  virtual size_t getBlockSize() override
  { return CcSqlEnDecode_CryptoBufferSize; }
  virtual size_t getHashSize() override
  { return m_oValue.size(); }

private: // Methods
  struct CRandom
  {
    uint32 seed1,seed2,max_value;
    double max_value_dbl;
  } RandWork;

  void initRandom();
  double Random();

private:

  uchar        m_oEncodeBuffer[CcSqlEnDecode_CryptoBufferSize]  = {0}; //!< Temporary buffer for encoding
  uchar        m_oDecodeBuffer[CcSqlEnDecode_CryptoBufferSize]  = {0}; //!< Temporary buffer for decoding
  uint32       m_uiShift                                        = {0}; //!< Current location in coding buffer
  uint32       m_uiKey[2]                                       = {0}; //!< Key Size is always 2

  CcByteArray  m_oValue;    //!< Encoded or decoded output
};
