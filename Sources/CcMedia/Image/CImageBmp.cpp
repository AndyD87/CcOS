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
 * @brief     Implementation of Class CImageBmp
 *            Additional info for BMP Formats:
 *                  https://gibberlings3.github.io/iesdp/file_formats/ie_formats/bmp.htm
 *                  https://de.wikipedia.org/wiki/Windows_Bitmap
 */
#include "CImageBmp.h"
#include "CcStatic.h"

using namespace NImage;

//! Bit for rgb value
#define CImageBmp_BI_RGB          0
//! Bit for bitfield image
#define CImageBmp_BI_BITFIELDS    3

#pragma pack(push, 1)
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

class CImageBmp::CColorTable
{
public:
  uint8 uiR;
  uint8 uiG;
  uint8 uiB;
  uint8 uiReserved;

  inline uint8 getR() const
  {
    return uiR;
  }

  inline uint8 getG() const
  {
    return uiG;
  }

  inline uint8 getB() const
  {
    return uiB;
  }
};

class CImageBmp::CRgb24
{
public:
  uint8 uiB;
  uint8 uiG;
  uint8 uiR;

  inline uint8 getR() const
  {
    return uiR;
  }

  inline uint8 getG() const
  {
    return uiG;
  }

  inline uint8 getB() const
  {
    return uiB;
  }
};

class CImageBmp::CRgb32
{
public:
  uint8 uiB;
  uint8 uiG;
  uint8 uiR;
  uint8 uiReserved;

  inline uint8 getR() const
  {
    return uiR;
  }

  inline uint8 getG() const
  {
    return uiG;
  }

  inline uint8 getB() const
  {
    return uiB;
  }
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

class CImageBmp::CRgb8
{
public:
  uint8 bIndex;
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
      m_pHeader = CCVOIDPTRCONSTCAST(CBmpHeader*, oInput.getArray());
      if (m_pHeader->oInfo.uiClrUsed)
      {
        m_pColorTable = CCVOIDPTRCONSTCAST(CColorTable*, oInput.getArray() + sizeof(CBmpHeader));
      }
      if (oInput.size() >= m_pHeader->uiSize &&
          oInput.size() >= m_pHeader->uiOffBits)
      {
        if (m_pHeader->oInfo.uiCompression == CImageBmp_BI_RGB)
        {
          const void* pImageData = static_cast<const void*>(oInput.getArray() + m_pHeader->uiOffBits);
          oImage.resize(static_cast<uint32>(m_pHeader->oInfo.iWidth), static_cast<uint32>(m_pHeader->oInfo.iHeight));
          if (m_pHeader->oInfo.uiBitCount == 1)
          {
            copyImageData1Bit(oImage, pImageData, m_pHeader->oInfo.uiClrUsed);
          }
          else if      (m_pHeader->oInfo.uiBitCount == 24)
          {
            copyImageData24Bit(oImage, pImageData);
          }
          else if (m_pHeader->oInfo.uiBitCount == 16)
          {
            copyImageData16Bit(oImage, pImageData);
          }
          else if (m_pHeader->oInfo.uiBitCount == 8)
          {
            copyImageData8Bit(oImage, pImageData);
          }
          else if (m_pHeader->oInfo.uiBitCount == 32)
          {
            copyImageData32Bit(oImage, pImageData);
          }
        }
        else if (m_pHeader->oInfo.uiCompression == CImageBmp_BI_BITFIELDS)
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
  const CRgb32* pItems = CCVOIDPTRCONSTCAST(CRgb32*, pImageData);
  uint64 uiOffsetBmp = 0;
  for (uint32 y = 0; y < oRaw.getHeight(); y++)
  {
    for (uint32 x = 0; x < oRaw.getWidth(); x++)
    {
      uint64 uiOffset = 0;
      if (m_pHeader && m_pHeader->oInfo.iHeight > 0)
        uiOffset = ((oRaw.getHeight() - y - 1)*oRaw.getWidth()) + x;
      else
        uiOffset = uiOffsetBmp;
      oRaw.getBuffer()[uiOffset].RGBA.R = pItems[uiOffsetBmp].getR();
      oRaw.getBuffer()[uiOffset].RGBA.G = pItems[uiOffsetBmp].getG();
      oRaw.getBuffer()[uiOffset].RGBA.B = pItems[uiOffsetBmp].getB();
      oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      uiOffsetBmp++;
    }
  }
}

void CImageBmp::copyImageData24Bit(CcImageRaw& oRaw, const void* pImageData)
{
  const CRgb24* pItems = CCVOIDPTRCONSTCAST(CRgb24*, pImageData);
  uint64 uiOffsetBmp = 0;
  for (uint32 y = 0; y < oRaw.getHeight(); y++)
  {
    for (uint32 x = 0; x < oRaw.getWidth(); x++)
    {
      uint64 uiOffset = 0;
      if (m_pHeader && m_pHeader->oInfo.iHeight > 0)
        uiOffset = ((oRaw.getHeight() - y - 1)*oRaw.getWidth()) + x;
      else
        uiOffset = uiOffsetBmp;
      oRaw.getBuffer()[uiOffset].RGBA.R = pItems[uiOffsetBmp].getR();
      oRaw.getBuffer()[uiOffset].RGBA.G = pItems[uiOffsetBmp].getG();
      oRaw.getBuffer()[uiOffset].RGBA.B = pItems[uiOffsetBmp].getB();
      oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      uiOffsetBmp++;
    }
  }
}

void CImageBmp::copyImageData16Bit(CcImageRaw& oRaw, const void* pImageData)
{
  const CRgb16* pItems = CCVOIDPTRCONSTCAST(CRgb16*, pImageData);
  uint64 uiOffsetBmp = 0;
  for (uint32 y = 0; y < oRaw.getHeight(); y++)
  {
    for (uint32 x = 0; x < oRaw.getWidth(); x++)
    {
      uint64 uiOffset = 0;
      if (m_pHeader && m_pHeader->oInfo.iHeight > 0)
        uiOffset = ((oRaw.getHeight() - y - 1)*oRaw.getWidth()) + x;
      else
        uiOffset = uiOffsetBmp;
      oRaw.getBuffer()[uiOffset].RGBA.R = pItems[uiOffsetBmp].getR();
      oRaw.getBuffer()[uiOffset].RGBA.G = pItems[uiOffsetBmp].getG();
      oRaw.getBuffer()[uiOffset].RGBA.B = pItems[uiOffsetBmp].getB();
      oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      uiOffsetBmp++;
    }
  }
}

void CImageBmp::copyImageData8Bit(CcImageRaw& oRaw, const void* pImageData)
{
  const CRgb8* pItems = CCVOIDPTRCONSTCAST(CRgb8*, pImageData);
  uint64 uiOffsetBmp = 0;
  for (uint32 y = 0; y < oRaw.getHeight(); y++)
  {
    for (uint32 x = 0; x < oRaw.getWidth(); x++)
    {
      uint64 uiOffset = 0;
      if (m_pHeader && m_pHeader->oInfo.iHeight > 0)
        uiOffset = ((oRaw.getHeight() - y - 1)*oRaw.getWidth()) + x;
      else
        uiOffset = uiOffsetBmp;
      uint8 uiIndex = pItems[uiOffsetBmp].bIndex;
      oRaw.getBuffer()[uiOffset].RGBA.R = m_pColorTable[uiIndex].getG();
      oRaw.getBuffer()[uiOffset].RGBA.G = m_pColorTable[uiIndex].getG();
      oRaw.getBuffer()[uiOffset].RGBA.B = m_pColorTable[uiIndex].getB();
      oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      uiOffsetBmp++;
    }
    if (oRaw.getWidth() & 0x3)
    {
      uiOffsetBmp += 4 - (oRaw.getWidth() & 0x3);
    }
  }
}

void CImageBmp::copyImageData1Bit(CcImageRaw& oRaw, const void* pImageData, uint32 uiColorTables)
{
  CCUNUSED(uiColorTables);
  const uint8* pItems = CCVOIDPTRCONSTCAST(uint8*, pImageData);
  uint64 uiWidthBoundary = oRaw.getWidth();
  if(oRaw.getWidth() % 32)
  {
    uiWidthBoundary += (32 - ((oRaw.getWidth()) % 32));
  }
  for (uint32 y = 0; y < oRaw.getHeight(); y++)
  {
    for (uint32 x = 0; x < oRaw.getWidth(); x++)
    {
      uint64 uiOffset = 0;
      if(m_pHeader && m_pHeader->oInfo.iHeight > 0)
        uiOffset = ((oRaw.getHeight() - y - 1)*oRaw.getWidth()) + x;
      else
        uiOffset = (y*oRaw.getWidth()) + x;
      uint64 uiPixelOffset = (y*uiWidthBoundary) + x;
      uint32 uiPixelByteOffset  = static_cast<uint32>(uiPixelOffset /8);
      uint32 uiPixelBitOffset   = static_cast<uint32>(uiPixelOffset % 8);
      uint32 uiPixel = pItems[uiPixelByteOffset] & (1 << (7-uiPixelBitOffset));
      if (uiPixel)
      {
        oRaw.getBuffer()[uiOffset].RGBA.R = 0xff;
        oRaw.getBuffer()[uiOffset].RGBA.G = 0xff;
        oRaw.getBuffer()[uiOffset].RGBA.B = 0xff;
        oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      }
      else
      {
        oRaw.getBuffer()[uiOffset].RGBA.R = 0x00;
        oRaw.getBuffer()[uiOffset].RGBA.G = 0x00;
        oRaw.getBuffer()[uiOffset].RGBA.B = 0x00;
        oRaw.getBuffer()[uiOffset].RGBA.A = 0xff;
      }
    }
    // boundary to 32bit
  }
}
