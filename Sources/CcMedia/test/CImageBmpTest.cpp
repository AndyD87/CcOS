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
 * @brief     Implemtation of class CImageBmpTest
 */
#include "CImageBmpTest.h"
#include "CcString.h"
#include "CcImage.h"
#include "CcImageRaw.h"

#include "Resources/TestBild-1bit.bmp.h"
#include "Resources/TestBild-4bit.bmp.h"
#include "Resources/TestBild-16bit.bmp.h"
#include "Resources/TestBild-24bit.bmp.h"

CImageBmpTest::CImageBmpTest() :
  CcTest("CImageBmpTest")
{
  appendTestMethod("Test basics", &CImageBmpTest::testBasic);
  appendTestMethod("Test converting from 1bit bmp file", &CImageBmpTest::testConvertFrom1Bit);
  appendTestMethod("Test converting from 4bit bmp file", &CImageBmpTest::testConvertFrom4Bit);
  appendTestMethod("Test converting from 16bit bmp file", &CImageBmpTest::testConvertFrom16Bit);
  appendTestMethod("Test converting from 24bit bmp file", &CImageBmpTest::testConvertFrom24Bit);
}

CImageBmpTest::~CImageBmpTest()
{
}

bool CImageBmpTest::testBasic()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CcByteArray(TestBild_16Bit_Bmp, TestBild_16Bit_Bmp_Size));
  bRet = oImage.getType() == EImageType::Bmp;
  return bRet;
}

bool CImageBmpTest::testConvertFrom1Bit()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CcByteArray(TestBild_1Bit_Bmp, TestBild_1Bit_Bmp_Size));
  if (oImage.getType() == EImageType::Bmp)
  {
    bRet = true;
  }
  return bRet;
}

bool CImageBmpTest::testConvertFrom4Bit()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CcByteArray(TestBild_4Bit_Bmp, TestBild_4Bit_Bmp_Size));
  bRet = oImage.getType() == EImageType::Bmp;
  return bRet;
}

bool CImageBmpTest::testConvertFrom16Bit()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CcByteArray(TestBild_16Bit_Bmp, TestBild_16Bit_Bmp_Size));
  if (oImage.getType() == EImageType::Bmp)
  {
    CcImageRaw oRawImage = oImage.getRaw();
    if (oRawImage.getWidth())
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CImageBmpTest::testConvertFrom24Bit()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CcByteArray(TestBild_24Bit_Bmp, TestBild_24Bit_Bmp_Size));
  if (oImage.getType() == EImageType::Bmp)
  {
    CcImageRaw oRawImage = oImage.getRaw();
    if (oRawImage.getWidth())
    {
      bRet = true;
    }
  }
  return bRet;
}
