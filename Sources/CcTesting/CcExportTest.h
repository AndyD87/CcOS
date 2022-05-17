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
  * @copyright Andreas Dirmeier (C) 2021
  * @author    Andreas Dirmeier
  * @par       Web:      https://coolcow.de/projects/CcOS
  * @par       Language: C++11
  * @brief     Test exporting listable templates to require only as basic subset of operators.
  */
#pragma once

#include "CcTesting.h"
#include "CcList.h"
#include "CcVector.h"

// Test std export if it is required
#ifdef TEST_STD
  #include <list>
  #include <vector>
  #define TestVector std::vector
  #define TestList std::list
#else 
  #include "CcList.h"
  #include "CcVector.h"
  //! Define CcVector for testing vector export
  #define TestVector CcVector
//! Define CcList for testing list export
  #define TestList CcList
#endif

/**
 * @brief Item with basic operators for test and a
 *        pointer variable to force other operator.
 */
class CcTestingSHARED CcExportTestItem
{
public:
  void* pTest; //!< Pointer to force additional operators if used.

  /**
   * @brief Compare operator for testing
   * @param oToCompare: CcExportTestItem to compare to
   * @return true if both pTest are same
   */
  bool operator==(const CcExportTestItem& oToCompare) const
  { return oToCompare.pTest == pTest; }
  /**
   * @brief Less operator for testing
   * @param oToCompare: CcExportTestItem to check if less to
   * @return true if this pTest is less then from oToCompare
   */
  bool operator<(const CcExportTestItem& oToCompare) const
  { return oToCompare.pTest < pTest; }
};

#ifdef _MSC_VER
template class CcTestingSHARED CcList<CcExportTestItem>;
template class CcTestingSHARED CcVector<CcExportTestItem>;
#endif

/**
 * @brief Export list of basic CcExportTestItem
 */
class CcTestingSHARED CcExportTestList : public TestList<CcExportTestItem>
{

};

/**
 * @brief Export vector of basic CcExportTestItem
 */
class CcTestingSHARED CcExportTestVector : public TestVector<CcExportTestItem>
{

};
