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
 * @brief     Implemtation of class CArgumentTest
 */
#include "CArgumentTest.h"
#include "CcArguments.h"

CArgumentTest::CArgumentTest() :
  CcTest("CArgumentTest")
{
  appendTestMethod("Test basic callup", &CArgumentTest::testBasic);
}

CArgumentTest::~CArgumentTest()
{
}

bool CArgumentTest::testBasic()
{
  bool bRet = false;
  CcArguments oArguments;
  oArguments.setVariablesList(
    {
      {"-url",        CcVariant::EType::String},
      {"-request",    CcVariant::EType::String, "get"},
      {"-stream",     CcVariant::EType::String},
      {"-resolution", CcVariant::EType::Uint32},
      {"-output",     CcVariant::EType::String}
    }
  );
  oArguments.parse(" -url http://coolcow.de -request get -output \"Test File.html\"");
  if (oArguments.contains("-url") &&
      oArguments.contains("-request") &&
      oArguments.contains("-output") &&
      oArguments.getVariablesList().size() == 0)
  {
    bRet = true;
  }
  return bRet;
}
