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
 * @brief     Class CcOtp
 */
#pragma once

#include "CcBase.h"
#include "CcHash.h"
#include "CcByteArray.h"
#include "CcHmac.h"

class CcByteArray;
class CcString;

/**
 * @brief Create Crc32 Hashes with this class.
 *        It is addtionally possible to continue an older Crc32 Session.
 */
class CcKernelSHARED CcOtp : public IHash
{
public:
  /**
   * @brief Constructor
   */
  CcOtp();
  virtual ~CcOtp() = default;

  virtual size_t getBlockSize() override
  { return 64; }
  virtual size_t getHashSize() override
  { return 20; }

  virtual const CcByteArray& getValue() override
  { return m_oResult; }
  virtual IHash& setKey(const void* pcData, size_t uiLen)
  { m_oHash.setKey(pcData, uiLen); return *this;}

  virtual CcOtp& generate(const void* pData, size_t uiSize) override;
  virtual CcOtp& append(const void* pData, size_t uiSize) override;
  virtual CcOtp& finalize(const void* pData, size_t uiSize) override;
  inline CcOtp& generate(const CcByteArray& oByteArray)
  { return generate(oByteArray.getArray(), oByteArray.size());}
  inline CcOtp& append(const CcByteArray& oByteArray)
  { return append(oByteArray.getArray(), oByteArray.size());}
  inline CcOtp& finalize(const CcByteArray& oByteArray)
  { return finalize(oByteArray.getArray(), oByteArray.size());}
  inline CcOtp& finalize()
  { return finalize(nullptr, 0);}

  CcOtp& finalizeCurrentTime();
  CcOtp& finalizeCounter(uint64 uiCounter);

  CcString getDigits(size_t uiSize);

private:
  CcByteArray   m_oResult;    //!< Red value
  CcHmac        m_oHash;
};
