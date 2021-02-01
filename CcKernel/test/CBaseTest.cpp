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
 * @brief     Implemtation of class CBaseTest
 */
#include "CBaseTest.h"

CBaseTest::CBaseTest() :
  CcTest("CBaseTest")
{
  appendTestMethod("Test if alignment is working", &CBaseTest::testAlignment);
  appendTestMethod("Test CLASSOFP makro", &CBaseTest::testTypeMacro);
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
    uint32 uiData;
  } STestBool;
  struct
  {
    char cAlign;
    uint32 uiData;
  } STestChar;
  CCUNUSED(STestBool);
  CCUNUSED(STestChar);
  size_t uiValue1 = sizeof(STestBool);
  size_t uiValue2 = sizeof(STestChar);

  #pragma pack(8)
  struct
  {
    bool bAlign;
    uint32 uiData;
  } STestBool8;
  struct
  {
    char cAlign;
    uint32 uiData;
  } STestChar8;
  CCUNUSED(STestBool8);
  CCUNUSED(STestChar8);
  #pragma pack(pop)

  if(uiValue1 == 1+4 &&
     uiValue2 == 1+4)
  {
    int iTemp1 = sizeof(STestBool8);
    int iTemp2 = sizeof(STestChar8);
    if( iTemp1 <= 8+8 && //! use void* because on 32bit system pragma 8 is 4
        iTemp2 <= 8+8)   //! use void* because on 32bit system pragma 8 is 4
    {
      #pragma pack(push,4)
      struct
      {
        bool bAlign;
        uint32 uiData;
      } STestBool4;
      struct
      {
        char cAlign;
        uint32 uiData;
      } STestChar4;
      CCUNUSED(STestBool4);
      CCUNUSED(STestChar4);
      #pragma pack(pop)
      iTemp1 = sizeof(STestBool4);
      iTemp2 = sizeof(STestChar4);
      if(iTemp1 == 4+4 &&
         iTemp2 == 4+4)
      {
        bRet = true;
      }
      else
      {
        CcTestFramework::writeDebug("Alignment 4 Test failed");
      }
    }
    else
    {
      CcTestFramework::writeDebug("Alignment 8 Test failed");
    }
  }
  else
  {
    CcTestFramework::writeDebug("Alignment 1 Test failed");
  }
  return bRet;
}

bool CBaseTest::testTypeMacro()
{
  bool bRet = false;
  int16 i=100;
  CcString* pString;
  CcString sConverted = CLASSOFP(pString)::fromNumber(i);
  if(sConverted == "100")
  {
    bRet = true;
  }
  return bRet;
}
