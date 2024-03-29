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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CGenericMallocTest
 **/
#pragma once

#include "CcBase.h"
#include "CcTest.h"

/**
 * @brief Class implementation
 */
class CGenericMallocTest : public CcTest<CGenericMallocTest>
{
public:
  /**
   * @brief Constructor
   */
  CGenericMallocTest();

  /**
   * @brief Destructor
   */
  virtual ~CGenericMallocTest();
private:
  /**
   * @brief For checking algorithm, we have to lock kernel space
   * @return true if scceeded
   */
  bool testKernelLock();
  /**
   * @brief Check if allocating some bytes is possible
   * @return true if scceeded
   */
  bool testBasic();
  /**
   * @brief Malloc of more size than availbale should result in nullptr
   * @return true if scceeded
   */
  bool testOversize();
  /**
   * @brief Malloc, free and malloc again of same size should result in same
   *        address, due to first fit algorithm.
   * @return true if scceeded
   */
  bool testSameAddress();

  /**
   * @brief Allocate all memory with little chunks and free it.
   *        Check if available() and used() is working.
   * @return true if scceeded
   */
  bool testLittleAllocations();

  /**
   * @brief Allocate all memory with little chunks and free every second.
   *        Next bigger memory should fail with nullptr
   * @return true if scceeded
   */
  bool testLittleAllocationInvalidate();
private:
  void* m_pKernelSpace = nullptr;
};
