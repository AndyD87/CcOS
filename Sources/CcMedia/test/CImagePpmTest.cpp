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
 * @brief     Implemtation of class CImagePpmTest
 */
#include "CImagePpmTest.h"
#include "CcString.h"
#include "CcImage.h"
#include "CcImageRaw.h"

//! Test Picture from wikedia
//! Test image for ppm p1
const char CImagePpmTest_pTestImageP1[] =
"P1          #Bitmap                                        \n\
# Beispiel fuer das Bild des Buchstabens \"J\"              \n\
6 10        #Breite des Bildes, Leerstelle, Hoehe des Bildes\n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
1 0 0 0 1 0                                                 \n\
0 1 1 1 0 0                                                 \n\
0 0 0 0 0 0                                                 \n\
0 0 0 0 0 0 ";

//! Test image for ppm p2
const char CImagePpmTest_pTestImageP2[] =
"P2          #Bitmap                                        \n\
# Beispiel fuer das Bild des Buchstabens \"J\"              \n\
6 10 255    #Breite des Bildes, Leerstelle, Hoehe des Bildes\n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
0 0 0 0 1 0                                                 \n\
1 0 0 0 1 0                                                 \n\
0 1 1 1 0 0                                                 \n\
0 0 0 0 0 0                                                 \n\
0 0 0 0 0 0 ";

//! Test image for ppm p3
const char CImagePpmTest_pTestImageP3[] =
"P3          #Bitmap                                        \n\
# Beispiel fuer das Bild des Buchstabens \"J\"              \n\
6 10 255    #Breite des Bildes, Leerstelle, Hoehe des Bildes\n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
0 0 0 0 1 0 0 0 0 0 1 0 0 0 0 0 1 0                         \n\
1 0 0 0 1 0 1 0 0 0 1 0 1 0 0 0 1 0                         \n\
0 1 1 1 0 0 0 1 1 1 0 0 0 1 1 1 0 0                         \n\
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0                         \n\
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1";

//! Test Picture from wikedia for p5
const char CImagePpmTest_pTestImageP5[] =
"P5          #Bitmap                                        \n\
# Beispiel fuer das Bild des Buchstabens \"J\"              \n\
6 10 255    #Breite des Bildes, Leerstelle, Hoehe des Bildes\n\
000010\
000010\
000010\
000010\
000010\
000010\
100010\
011100\
000000\
000000";

//! Test Picture from wikedia for p6
const char CImagePpmTest_pTestImageP6[] =
"P6          #Bitmap                                        \n\
# Beispiel fuer das Bild des Buchstabens \"J\"              \n\
6 10 255    #Breite des Bildes, Leerstelle, Hoehe des Bildes\n\
000010000010000010\
000010000010000010\
000010000010000010\
000010000010000010\
000010000010000010\
000010000010000010\
100010100010100010\
011100011100011100\
000000000000000000\
000000000000000101";

CImagePpmTest::CImagePpmTest() :
  CcTest("CImagePpmTest")
{
  appendTestMethod("Test basics", &CImagePpmTest::testBasic);
  appendTestMethod("Test convert P1", &CImagePpmTest::testConvertP1);
  appendTestMethod("Test convert P2", &CImagePpmTest::testConvertP2);
  appendTestMethod("Test convert P3", &CImagePpmTest::testConvertP3);
  appendTestMethod("Test convert P5", &CImagePpmTest::testConvertP5);
  appendTestMethod("Test convert P6", &CImagePpmTest::testConvertP6);
  appendTestMethod("Test convert P6 and fail", &CImagePpmTest::testConvertP6GrayFail);
}

CImagePpmTest::~CImagePpmTest()
{
}

bool CImagePpmTest::testBasic()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CImagePpmTest_pTestImageP1);
  bRet = oImage.getType() == EImageType::Pbm;
  return bRet;
}

bool CImagePpmTest::testConvertP1()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CImagePpmTest_pTestImageP1);
  if (oImage.getType() == EImageType::Pbm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
        oRaw.getHeight() == 10)
    {
      if (oRaw.getPixel(4, 6).RGBA.R == 0xff &&
          oRaw.getPixel(4, 6).RGBA.G == 0xff &&
          oRaw.getPixel(4, 6).RGBA.B == 0xff)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CImagePpmTest::testConvertP2()
{
  bool bRet = false;
  CcImage oImage;
  CcByteArray oBuffer(CImagePpmTest_pTestImageP2);
  oImage.setBuffer(oBuffer);
  if (oImage.getType() == EImageType::Pgm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
        oRaw.getHeight() == 10)
    {
      if (oRaw.getPixel(4, 6).RGBA.R == 1 &&
          oRaw.getPixel(4, 6).RGBA.G == 1 &&
          oRaw.getPixel(4, 6).RGBA.B == 1)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CImagePpmTest::testConvertP3()
{
  bool bRet = false;
  CcImage oImage;
  CcByteArray oBuffer(CImagePpmTest_pTestImageP3);
  oImage.setBuffer(oBuffer);
  if (oImage.getType() == EImageType::Ppm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
      oRaw.getHeight() == 10)
    {
      if (oRaw.getPixel(4, 6).RGBA.R == 1 &&
          oRaw.getPixel(4, 6).RGBA.G == 0 &&
          oRaw.getPixel(4, 6).RGBA.B == 0)
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CImagePpmTest::testConvertP5()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CImagePpmTest_pTestImageP5);
  if (oImage.getType() == EImageType::Pgm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
      oRaw.getHeight() == 10)
    {
      if (oRaw.getPixel(4, 6).RGBA.R == '1' &&
          oRaw.getPixel(4, 6).RGBA.G == '1' &&
          oRaw.getPixel(4, 6).RGBA.B == '1')
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CImagePpmTest::testConvertP6()
{
  bool bRet = false;
  CcImage oImage;
  oImage.setBuffer(CImagePpmTest_pTestImageP6);
  if (oImage.getType() == EImageType::Ppm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
      oRaw.getHeight() == 10)
    {
      CcImageRaw::CPixel oPixel = oRaw.getPixel(4, 6);
      if (oPixel.RGBA.R == '1' &&
          oPixel.RGBA.G == '0' &&
          oPixel.RGBA.B == '0')
      {
        bRet = true;
      }
    }
  }
  return bRet;
}

bool CImagePpmTest::testConvertP6GrayFail()
{
  bool bRet = false;
  CcImage oImage;
  CcString sTest(CImagePpmTest_pTestImageP6);
  sTest.replace("255", "256");
  oImage.setBuffer(sTest);
  if (oImage.getType() == EImageType::Ppm)
  {
    CcImageRaw oRaw = oImage.getRaw();
    if (oRaw.getWidth() == 6 &&
      oRaw.getHeight() == 10)
    {
    }
    else
    {
      bRet = true;
    }
  }
  return bRet;
}
