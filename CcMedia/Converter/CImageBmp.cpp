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
 * @brief     Implementation of Class CImageBmp
 */
#include "CImageBmp.h"
#include "CcStatic.h"

using namespace NImage;

#define CImageBmp_BI_RGB          0
#define CImageBmp_BI_BITFIELDS    3

#pragma pack(push, 1)
// https://de.wikipedia.org/wiki/Windows_Bitmap
class CImageBmp::CBmpInfo
{
public:
  uint32 uiSize;
  int32  iWidth;
  int32  iHeight;
  uint16 uiPlanes;
  uint16 uiBitCount;
  uint32 uiCompression;
  uint32 uiSizeImage;
  int32  iXPelsPerMeter;
  int32  iYPelsPerMeter;
  uint32 uiClrUsed;
  uint32 uiClrImportant;
};

class CImageBmp::CBmpHeader
{
public:
  uint16 uiType;
  uint32 uiSize;
  uint32 uiReserved;
  uint32 uiOffBits;
  CBmpInfo oInfo;
};

class CImageBmp::CRgb24
{
public:
  uint8 uiB;
  uint8 uiG;
  uint8 uiR;
};

class CImageBmp::CRgb32
{
public:
  uint8 uiB;
  uint8 uiG;
  uint8 uiR;
  uint8 uiReserved;
};

class CImageBmp::CRgb16
{
public:
  uint8 pBuf[2];

  uint8 getR() const
  {
    return (pBuf[1] & 0x7c) >> 2;
  }

  uint8 getG() const
  {
    return ((pBuf[1] & 0x03) << 3) | ((pBuf[0] & 0xe0) >> 5);
  }

  uint8 getB() const
  {
    return pBuf[0] & 0x1f;
  }
};
#pragma pack(pop)

CImageBmp::CImageBmp()
{
}

CImageBmp::~CImageBmp()
{
}

bool CImageBmp::checkType(EImageType eType)
{
  return eType == EImageType::Bmp;
}

EImageType CImageBmp::checkFormat(const CcByteArray& oToCheck)
{
  EImageType eType = EImageType::Unknown;
  if (oToCheck.size() > 1 &&
      oToCheck[0] == 'B' &&
      oToCheck[1] == 'M')
  {
    eType = EImageType::Bmp;
  }
  return eType;
}

CcImageRaw CImageBmp::convertToRaw(const CcByteArray& oInput)
{
  CcImageRaw oImage;
  if (checkFormat(oInput) == EImageType::Bmp)
  {
    if (oInput.size() > 14)
    {
      const CBmpHeader* pHeader = CCVOIDPTRCONSTCAST(CBmpHeader*, oInput.getArray());
      if (oInput.size() >= pHeader->uiSize &&
          oInput.size() >= pHeader->uiOffBits)
      {
        if (pHeader->oInfo.uiCompression == CImageBmp_BI_RGB)
        {
          if      (pHeader->oInfo.uiBitCount == 24)
          {
            const void* pImageData = static_cast<const void*>(oInput.getArray() + pHeader->uiOffBits);
            oImage.resize(static_cast<uint32>(pHeader->oInfo.iWidth),
              static_cast<uint32>(pHeader->oInfo.iHeight));
            copyImageData24Bit(oImage, pImageData);
          }
          else if (pHeader->oInfo.uiBitCount == 16)
          {
            const void* pImageData = static_cast<const void*>(oInput.getArray() + pHeader->uiOffBits);
            oImage.resize(static_cast<uint32>(pHeader->oInfo.iWidth),
              static_cast<uint32>(pHeader->oInfo.iHeight));
            copyImageData24Bit(oImage, pImageData);
          }
          else if (pHeader->oInfo.uiBitCount == 32)
          {
            const void* pImageData = static_cast<const void*>(oInput.getArray() + pHeader->uiOffBits);
            oImage.resize(static_cast<uint32>(pHeader->oInfo.iWidth),
              static_cast<uint32>(pHeader->oInfo.iHeight));
            copyImageData32Bit(oImage, pImageData);
          }
        }
        else if (pHeader->oInfo.uiCompression == CImageBmp_BI_BITFIELDS)
        {
        }
      }
    }
  }
  return oImage;
}

CcByteArray CImageBmp::convertFromRaw(const CcImageRaw& oInput)
{
  CCUNUSED(oInput);
  return CcByteArray();
}

void CImageBmp::copyImageData32Bit(CcImageRaw& oRaw, const void* pImageData)
{
  uint64 uiBufferSize = oRaw.getPixelCount();
  const CRgb32* pItems = CCVOIDPTRCONSTCAST(CRgb32*, pImageData);
  for (uint64 i = 0; i < uiBufferSize; i++)
  {
    oRaw.getBuffer()[i].RGBA.R = pItems[i].uiR;
    oRaw.getBuffer()[i].RGBA.G = pItems[i].uiG;
    oRaw.getBuffer()[i].RGBA.B = pItems[i].uiB;
    oRaw.getBuffer()[i].RGBA.A = 0xff;
  }
}

void CImageBmp::copyImageData24Bit(CcImageRaw& oRaw, const void* pImageData)
{
  uint64 uiBufferSize = oRaw.getPixelCount();
  const CRgb24* pItems = CCVOIDPTRCONSTCAST(CRgb24*, pImageData);
  for (uint64 i = 0; i < uiBufferSize; i++)
  {
    oRaw.getBuffer()[i].RGBA.R = pItems[i].uiR;
    oRaw.getBuffer()[i].RGBA.G = pItems[i].uiG;
    oRaw.getBuffer()[i].RGBA.B = pItems[i].uiB;
    oRaw.getBuffer()[i].RGBA.A = 0xff;
  }
}

void CImageBmp::copyImageData16Bit(CcImageRaw& oRaw, const void* pImageData)
{
  uint64 uiBufferSize = oRaw.getPixelCount();
  const CRgb16* pItems = CCVOIDPTRCONSTCAST(CRgb16*, pImageData);
  for (uint64 i = 0; i < uiBufferSize; i++)
  {
    oRaw.getBuffer()[i].RGBA.R = pItems[i].getR();
    oRaw.getBuffer()[i].RGBA.G = pItems[i].getG();
    oRaw.getBuffer()[i].RGBA.B = pItems[i].getB();
    oRaw.getBuffer()[i].RGBA.A = 0xff;
  }
}
