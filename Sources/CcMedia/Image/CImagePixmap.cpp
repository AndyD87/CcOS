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
 * @brief     Implementation of Class CImagePixmap
 */
#include "CImagePixmap.h"
#include "CcStatic.h"

using namespace NImage;

CImagePixmap::CImagePixmap()
{
}

CImagePixmap::~CImagePixmap()
{
}

bool CImagePixmap::checkType(EImageType eType)
{
  return eType == EImageType::Pixmap;
}

EImageType CImagePixmap::checkFormat(const CcByteArray& oToCheck)
{
  EImageType eType = EImageType::Unknown;
  if (oToCheck.size() > 1 &&
      oToCheck[0] == '/' &&
      oToCheck[1] == '*')
  {
    eType = EImageType::Pixmap;
  }
  return eType;
}

CcImageRaw CImagePixmap::convertToRaw(const CcByteArray& oInput)
{
  CcImageRaw oImage;
  if (checkFormat(oInput) == EImageType::Pixmap)
  {

  }
  return oImage;
}

CcByteArray CImagePixmap::convertFromRaw(const CcImageRaw& oInput)
{
  CcString sImageSource  = "/**\n";
  sImageSource += " * @brief Generated Pixmap from CcOS\n";
  sImageSource += " */\n";
  sImageSource += "\n";
  sImageSource += "#pragma pack(push, 1)\n";
  sImageSource += "/**\n";
  sImageSource += " * @brief Structure for describing an filled rectangular.\n";
  sImageSource += " *        Containing data structure of row and columns is depending on it's width.\n";
  sImageSource += " *        For size of a row, getArrayWidth can be used to get it width of row in bytes.\n";
  sImageSource += " */\n";
  sImageSource += "class CPixmap\n";
  sImageSource += "{\n";
  sImageSource += "public:\n";
  sImageSource += "  uint16_t Width = " + CcString::fromNumber(oInput.getWidth()) + ";\n";
  sImageSource += "  uint16_t Height = " + CcString::fromNumber(oInput.getHeight()) + ";\n";
  uint32 uiArrayWidth = oInput.getWidth();
  if (uiArrayWidth % 8)
    uiArrayWidth += 8;
  uiArrayWidth /= 8;
  sImageSource += "  unsigned char pData[" + CcString::fromNumber(uiArrayWidth * oInput.getHeight()) + "] = {\n";

  for (uint32 uiCurrentY = 0; uiCurrentY < oInput.getHeight(); uiCurrentY += 1)
  {
    sImageSource += "    ";
    for (uint32 uiCurrentX = 0; uiCurrentX < oInput.getWidth(); uiCurrentX += 8)
    {
      uint8 uiCurrentByte = 0;
      for (uint32 uiCurrentXInner = 0; uiCurrentXInner < 8 && uiCurrentX + uiCurrentXInner < oInput.getWidth(); uiCurrentXInner++)
      {
        CcImageRaw::CPixel oPixel = oInput.getPixel(uiCurrentX + uiCurrentXInner, uiCurrentY);
        uint16 uiPixel = oPixel.RGBA.R + oPixel.RGBA.G + oPixel.RGBA.B;
        uiPixel = uiPixel / 3;
        if (uiPixel >= 0xff / 2)
        {
          uiCurrentByte |= 1 << uiCurrentXInner;
        }
      }
      sImageSource += "0x" + CcString::fromNumber(uiCurrentByte, 16);
      if(uiCurrentY + 1 < oInput.getHeight() || uiCurrentX + 8 < oInput.getWidth())
        sImageSource += ",";
    }
    sImageSource += "\n";
  }

  sImageSource += "  };\n";
  sImageSource += "\n";
  sImageSource += "  inline bool getPixel(uint8_t uiX, uint8_t uiY) const\n";
  sImageSource += "  {\n";
  sImageSource += "    uint8_t uiXDiv = uiX>>3;\n";
  sImageSource += "    return (0x1 & (pData[(uiY * getArrayWidth()) + uiXDiv] >> (uiX & 0x7)));\n";
  sImageSource += "  }\n";
  sImageSource += "\n";
  sImageSource += "  /**\n";
  sImageSource += "   * @brief Get width of row in bytes\n";
  sImageSource += "   * @return number of real width in bytes.\n";
  sImageSource += "   */\n";
  sImageSource += "  inline unsigned char getArrayWidth() const\n";
  sImageSource += "  {\n";
  sImageSource += "    unsigned char uiValue=0;\n";
  sImageSource += "    uiValue = Width / 8;\n";
  sImageSource += "    if((Width % 8) != 0)\n";
  sImageSource += "    {\n";
  sImageSource += "      uiValue++;\n";
  sImageSource += "    }\n";
  sImageSource += "    return uiValue;\n";
  sImageSource += "  }\n";
  sImageSource += "};\n";
  sImageSource += "#pragma pack(pop)\n";
  return sImageSource;
}
