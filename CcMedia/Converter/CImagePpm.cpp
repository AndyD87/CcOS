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
 * @brief     Implementation of Class CImagePpm
 */
#include "CImagePpm.h"
#include "CcStringUtil.h"

using namespace NImage;

CImagePpm CImagePpm::s_oConverter;

class CImagePpm::CPrivate
{
public:
  enum class ECoding
  {
    Binary,
    Ascii,
  };

  enum class EType
  {
    Bitmap,
    Graymap,
    Pixmap
  };

  size_t parseHeader(const CcByteArray& oData)
  {
    size_t uiCurrentPos = 0;
    if (oData.size() > 1 &&
        oData[0] == 'P' &&
        oData[1] > '0' &&
        oData[1] < '7'
      )
    {
      switch (oData[1])
      {
        case '1':
          m_eImageType = EImageType::Pbm;
          break;
        case '2':
          m_eImageType = EImageType::Pgm;
          break;
        case '3':
          m_eImageType = EImageType::Ppm;
          break;
        case '4':
          m_eImageType = EImageType::Pbm;
          break;
        case '5':
          m_eImageType = EImageType::Pgm;
          break;
        case '6':
          m_eImageType = EImageType::Ppm;
          break;
      }
      setEncoding(oData[1]);
      uiCurrentPos = 2;
      if (getNextAscii(oData, uiCurrentPos))
      {
        CcString sData = getString(oData, uiCurrentPos);
        m_uiX = sData.toUint32();
      }
      if (uiCurrentPos < oData.size() &&
        getNextAscii(oData, uiCurrentPos))
      {
        CcString sData = getString(oData, uiCurrentPos);
        m_uiY = sData.toUint32();
      }
      if((m_eImageType == EImageType::Ppm || m_eImageType == EImageType::Pgm) &&
        uiCurrentPos < oData.size() &&
        getNextAscii(oData, uiCurrentPos))
      {
        CcString sData = getString(oData, uiCurrentPos);
        m_uiGrayscale = sData.toUint16();
        if (m_uiGrayscale > 0xff)
        {
          m_b16bit = true;
        }
      }
    }
    else
    {
      uiCurrentPos = SIZE_MAX;
    }
    return uiCurrentPos;
  }

  static bool getNextAscii(const CcByteArray& oData, size_t& uiCurrentPos)
  {
    bool bSuccess = true;
    do
    {
      if (CcStringUtil::isWhiteSpace(oData[uiCurrentPos]))
      {
        uiCurrentPos++;
      }
      else if ('#' == oData[uiCurrentPos])
      {
        do
        {
          uiCurrentPos++;
        } while ('\n' != oData[uiCurrentPos] && oData.size() > uiCurrentPos);
      }
      else
      {
        break;
      }
    } while (oData.size() > uiCurrentPos);
    if (uiCurrentPos >= oData.size())
    {
      uiCurrentPos = SIZE_MAX;
      bSuccess = false;
    }
    return bSuccess;
  }

  static CcString getString(const CcByteArray& oData, size_t& uiCurrentPos)
  {
    CcString sOut;
    while ( oData.size() > uiCurrentPos &&
            !CcStringUtil::isWhiteSpace(oData[uiCurrentPos]) &&
            oData[uiCurrentPos] != '#')
    {
      sOut.append(oData[uiCurrentPos]);
      uiCurrentPos++;
    }
    if (uiCurrentPos >= oData.size())
      uiCurrentPos = SIZE_MAX;
    return sOut;
  }

  uint8 getNextAsciiValue(const CcByteArray& oData, size_t& uiCurrentPos, bool& bOk)
  {
    uint16 uiValue = 0;
    if (uiCurrentPos < oData.size() &&
        getNextAscii(oData, uiCurrentPos))
    {
      CcString sData = getString(oData, uiCurrentPos);
      uiValue = sData.toUint16();
      bOk = sData.size() > 0;
    }
    if (bOk)
    {
      if (m_b16bit)
      {
        uiValue = uiValue / 0x100;
      }
    }
    return static_cast<uint8>(uiValue);
  }

  uint8 getNextBinaryValue(const CcByteArray& oData, size_t& uiCurrentPos, bool& bOk)
  {
    uint16 uiValue = 0;
    if(oData.size() > uiCurrentPos)
    {
      if (m_b16bit)
      {
        uint16* pValue = CCVOIDPTRCAST(uint16*, oData.getArray() + uiCurrentPos);
        uiValue = *pValue;
        uiCurrentPos += 2;
      }
      else
      {
        uiValue = oData[uiCurrentPos];
        uiCurrentPos++;
      }
      bOk = true;
    }
    else
    {
      bOk = false;
    }
    if (uiCurrentPos >= oData.size())
    {
      if (m_b16bit)
      {
        uiValue = uiValue / 0x100;
      }
      uiCurrentPos = SIZE_MAX;
    }
    return static_cast<uint8>(uiValue);
  }

  void setEncoding(char cCode)
  {
    switch (cCode)
    {
      case '1':
        m_eType = EType::Bitmap;
        m_eCoding = ECoding::Ascii;
        break;
      case '2':
        m_eType = EType::Graymap;
        m_eCoding = ECoding::Ascii;
        break;
      case '3':
        m_eType = EType::Pixmap;
        m_eCoding = ECoding::Ascii;
        break;
      case '4':
        m_eType = EType::Bitmap;
        m_eCoding = ECoding::Binary;
        break;
      case '5':
        m_eType = EType::Graymap;
        m_eCoding = ECoding::Binary;
        break;
      case '6':
        m_eType = EType::Pixmap;
        m_eCoding = ECoding::Binary;
        break;
    }
  }

  EImageType m_eImageType = EImageType::Unknown;
  EType m_eType;
  ECoding m_eCoding;
  uint32 m_uiX = 0;
  uint32 m_uiY = 0;
  uint16 m_uiGrayscale = 0;
  bool m_b16bit = false;
};

CImagePpm::CImagePpm()
{
  registerConverter();
}

CImagePpm::~CImagePpm()
{
  unregisterConverter();
}

bool CImagePpm::checkType(EImageType eType)
{
  return eType == EImageType::Pbm || eType == EImageType::Pgm || eType == EImageType::Ppm;
}

EImageType CImagePpm::checkFormat(const CcByteArray& oToCheck)
{
  EImageType eType = EImageType::Unknown;
  if (oToCheck.size() > 1 &&
    oToCheck[0] == 'P' &&
    oToCheck[1] > '0' &&
    oToCheck[1] < '7'
    )
  {
    switch (oToCheck[1])
    {
      case '1':
        eType = EImageType::Pbm;
        break;
      case '2':
        eType = EImageType::Pgm;
        break;
      case '3':
        eType = EImageType::Ppm;
        break;
      case '4':
        eType = EImageType::Pbm;
        break;
      case '5':
        eType = EImageType::Pgm;
        break;
      case '6':
        eType = EImageType::Ppm;
        break;
    }
  }
  return eType;
}

CcImageRaw CImagePpm::convertToRaw(const CcByteArray& oInput)
{
  CcImageRaw oOutput;
  CPrivate oRun;
   size_t uiOffset = oRun.parseHeader(oInput);
  if (uiOffset < oInput.size())
  {
    uint32 uiX = 0;
    uint32 uiY = 0;
    bool bOk = true;
    oOutput.resize(oRun.m_uiX, oRun.m_uiY);
    if (oRun.m_eCoding == CPrivate::ECoding::Ascii)
    {
      switch (oRun.m_eType)
      {
        case CPrivate::EType::Bitmap:
          do
          {
            uint8 uiValue = oRun.getNextAsciiValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              uint8 cColor;
              if (uiValue != 0) cColor = 255;
              else cColor = 0;
              oOutput.setPixel(uiX, uiY, cColor, cColor, cColor, 0);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) 
                  break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
        case CPrivate::EType::Graymap:
          do
          {
            uint8 uiValue = oRun.getNextAsciiValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              char cColor = static_cast<uint8>(uiValue);
              oOutput.setPixel(uiX, uiY, cColor, cColor, cColor, 0);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
        case CPrivate::EType::Pixmap:
          do
          {
            uint8 uiValueR = 0;
            uint8 uiValueG = 0;
            uint8 uiValueB = 0;
            if (bOk) uiValueR = oRun.getNextAsciiValue(oInput, uiOffset, bOk);
            if (bOk) uiValueG = oRun.getNextAsciiValue(oInput, uiOffset, bOk);
            if (bOk) uiValueB = oRun.getNextAsciiValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              uint8 uiValueA = static_cast<uint8>((uiValueR + uiValueG + uiValueB)/(255*3));
              oOutput.setPixel(uiX, uiY, static_cast<uint8>(uiValueR), static_cast<uint8>(uiValueG), static_cast<uint8>(uiValueB), uiValueA);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
      }
    }
    else if(oRun.getNextAscii(oInput, uiOffset))
    {
      switch (oRun.m_eType)
      {
        case CPrivate::EType::Bitmap:
          do
          {
            uint8 uiValue = oRun.getNextBinaryValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              uint8 cColor;
              if (uiValue != 0) cColor = 255;
              else cColor = 0;
              oOutput.setPixel(uiX, uiY, cColor, cColor, cColor, 0);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
        case CPrivate::EType::Graymap:
          do
          {
            uint8 uiValue = oRun.getNextBinaryValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              uint8 cColor = static_cast<uint8>(uiValue);
              oOutput.setPixel(uiX, uiY, cColor, cColor, cColor, 0);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
        case CPrivate::EType::Pixmap:
          do
          {
            uint8 uiValueR = 0;
            uint8 uiValueG = 0;
            uint8 uiValueB = 0;
            if (bOk) uiValueR = oRun.getNextBinaryValue(oInput, uiOffset, bOk);
            if (bOk) uiValueG = oRun.getNextBinaryValue(oInput, uiOffset, bOk);
            if (bOk) uiValueB = oRun.getNextBinaryValue(oInput, uiOffset, bOk);
            if (bOk)
            {
              uint8 uiValueA = static_cast<uint8>((uiValueR + uiValueG + uiValueB) / (255 * 3));
              oOutput.setPixel(uiX, uiY, static_cast<uint8>(uiValueR), static_cast<uint8>(uiValueG), static_cast<uint8>(uiValueB), uiValueA);
              uiX++;
              if (uiX >= oRun.m_uiX)
              {
                uiX = 0;
                uiY++;
                if (uiY >= oRun.m_uiY) break;
              }
            }
          } while (uiOffset < oInput.size() && bOk);
          break;
      }
    }
    if (uiX != 0 || uiY < oOutput.getY())
    {
      oOutput.clear();
    }
  }
  return oOutput;
}

CcByteArray CImagePpm::convertFromRaw(const CcImageRaw& oInput)
{
  CCUNUSED(oInput);
  return CcByteArray();
}
