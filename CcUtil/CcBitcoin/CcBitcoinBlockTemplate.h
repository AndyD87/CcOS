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
 * @brief     Class CcBitcoinBlockTemplate
 */
#pragma once

#include "CcBitcoin.h"
#include "CcBase.h"
#include "CcByteArray.h"
#include "CcString.h"
#include "CcArray.h"

#pragma pack(push, 1)

#ifndef SHA256_HASH_WIDTH
#define SHA256_HASH_WIDTH 32
#endif

class CcBitcoinSHARED CBitcoinHash
{
  size_t getSize() { return SHA256_HASH_WIDTH; }
public:
  uint8 pHash[SHA256_HASH_WIDTH];
};

/**
 * @brief Example Class implementation
 */
class CcBitcoinSHARED CcBitcoinBlockTemplate
{
public:
  uint32  uiBlockHeight       = 0;
  uint8   pPreviousBlock[SHA256_HASH_WIDTH];
  uint64  uiCoinbaseValue     = 0;
  bool    bIsSegwitEnabled    = true;

  CcByteArray oCoinbaseData;
  CcByteArray oCoinbaseExtraData;

  CcArray<CBitcoinHash> oTransactions;

private:
  bool createCoinbase();
};

#pragma pack(pop)
