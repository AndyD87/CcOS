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

#ifdef TEST_STD
  #include <list>
  #include <vector>
  #define TestVector std::vector
  #define TestList std::list
#else 
  #include "CcList.h"
  #include "CcVector.h"
  #define TestVector CcVector
  #define TestList CcList
#endif

class CcTestingSHARED CItem
{
public:
  void* pTest;

  bool operator==(const CItem& oToCompare) const
  { return oToCompare.pTest == pTest; }
  bool operator<(const CItem& oToCompare) const
  { return oToCompare.pTest < pTest; }
};

#ifdef _MSC_VER
template class CcTestingSHARED CcList<CItem>;
template class CcTestingSHARED CcVector<CItem>;
#endif

class CcTestingSHARED CcExportTestList : public TestList<CItem>
{

};

class CcTestingSHARED CcExportTestVector : public TestVector<CItem>
{

};
