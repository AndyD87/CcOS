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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CBaseTest
 */
#include "CBaseTest.h"

CBaseTest::CBaseTest() :
  CcTest("CBaseTest")
{
  appendTestMethod("Test if alignment is working", &CBaseTest::testAlignment);
}

CBaseTest::~CBaseTest()
{
}

bool CBaseTest::testAlignment()
{
  bool bRet = false;
  #pragma pack(push)
  #pragma pack(1)
  struct
  {
    bool bAlign;
    uint32_t uiData;
  } STestBool;
  struct
  {
    char cAlign;
    uint32_t uiData;
  } STestChar;
  size_t uiValue1 = sizeof(STestBool);
  size_t uiValue2 = sizeof(STestChar);
  #pragma pack(2)
  struct
  {
    bool bAlign;
    uint32_t uiData;
  } STestBool2;
  struct
  {
    char cAlign;
    uint32_t uiData;
  } STestChar2;
  #pragma pack(pop)
  if(uiValue1 == 1+4 &&
     uiValue2 == 1+4)
  {
    if(sizeof(STestBool2) == 2+4 &&
       sizeof(STestChar2) == 2+4)
    {
      #pragma pack(push,4)
      struct
      {
        bool bAlign;
        uint32_t uiData;
      } STestBool4;
      struct
      {
        char cAlign;
        uint32_t uiData;
      } STestChar4;
      #pragma pack(pop)
      size_t uiValue = sizeof(STestChar4);
      if(uiValue            == 4+4 &&
         sizeof(STestBool4) == 4+4)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}
