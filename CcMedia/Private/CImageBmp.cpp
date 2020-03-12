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

CImageBmp CImageBmp::s_oConverter;

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

  CBmpHeader swapped() const
  {
    CBmpHeader oNewHeader;
    oNewHeader.uiType = CcStatic::swapUint16(uiType);
    oNewHeader.uiSize = CcStatic::swapUint32(uiSize);
    oNewHeader.uiReserved = CcStatic::swapUint32(uiReserved);
    oNewHeader.uiOffBits = CcStatic::swapUint32(uiOffBits);

    oNewHeader.oInfo.uiSize =         CcStatic::swapUint32(oInfo.uiSize);
    oNewHeader.oInfo.iWidth =         CcStatic::swapInt32(oInfo.iWidth);
    oNewHeader.oInfo.iHeight =        CcStatic::swapInt32(oInfo.iHeight);
    oNewHeader.oInfo.uiPlanes =       CcStatic::swapUint16(oInfo.uiPlanes);
    oNewHeader.oInfo.uiBitCount =     CcStatic::swapUint16(oInfo.uiBitCount);
    oNewHeader.oInfo.uiCompression =  CcStatic::swapUint32(oInfo.uiCompression);
    oNewHeader.oInfo.uiSizeImage =    CcStatic::swapUint32(oInfo.uiSizeImage);
    oNewHeader.oInfo.iXPelsPerMeter = CcStatic::swapInt32(oInfo.iXPelsPerMeter);
    oNewHeader.oInfo.iYPelsPerMeter = CcStatic::swapInt32(oInfo.iYPelsPerMeter);
    oNewHeader.oInfo.uiClrUsed =      CcStatic::swapUint32(oInfo.uiClrUsed);
    oNewHeader.oInfo.uiClrImportant = CcStatic::swapUint32(oInfo.uiClrImportant);

    uint16* pSwap = CCVOIDPTRCAST(uint16*, &oNewHeader);
    for (int i = 0; i < sizeof(oNewHeader) / 2; i++)
    {
      pSwap[i] = CcStatic::swapUint16(pSwap[i]);
    }

    return oNewHeader;
  }
};

CImageBmp::CImageBmp()
{
  registerConverter();
}

CImageBmp::~CImageBmp()
{
  unregisterConverter();
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
      CBmpHeader oHeader = pHeader->swapped();
      if (oInput.size() > oHeader.uiSize &&
          oInput.size() > oHeader.uiOffBits)
      {
        const void* pImageData = static_cast<const void*>(oInput.getArray() + oHeader.uiOffBits);
        CCUNUSED(pImageData);
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
