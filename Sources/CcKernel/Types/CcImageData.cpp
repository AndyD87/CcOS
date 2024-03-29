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
 * @brief     Implementation of Class CcImageData
 */
#include "CcImageData.h"
#include "CcGlobalStrings.h"
#ifdef FULL_OS_AVAILABLE
  #include "CcFile.h"
  #include "CcKernel.h"
#endif

CcImageData::CcImageData(const CcByteArray &oBuffer, EImageType eType)
{
  m_oBuffer = oBuffer;
  m_eType = eType;
}

void CcImageData::setBuffer(const CcByteArray& oToCopy, EImageType eType)
{
  m_oBuffer = oToCopy;
  m_eType = eType;
}

const CcString& CcImageData::getFileExtension(EImageType eType)
{
  switch (eType)
  {
    case EImageType::Unknown:
      CCFALLTHROUGH;
    case EImageType::Raw:
      return CcGlobalStrings::Extensions::Bin;
    case EImageType::Jpeg:
      return CcGlobalStrings::Extensions::Jpg;
    case EImageType::Png:
      return CcGlobalStrings::Extensions::Png;
    case EImageType::Bmp:
      return CcGlobalStrings::Extensions::Bmp;
    case EImageType::Gif:
      return CcGlobalStrings::Extensions::Gif;
    case EImageType::Pbm:
      return CcGlobalStrings::Extensions::Pbm;
    case EImageType::Pgm:
      return CcGlobalStrings::Extensions::Pgm;
    case EImageType::Ppm:
      return CcGlobalStrings::Extensions::Ppm;
    default:
      return CcGlobalStrings::Empty;
  }
}

void CcImageData::setTimestampNow()
{
  #ifdef FULL_OS_AVAILABLE
    m_oTimestamp = CcKernel::getDateTime();
  #else
    m_oTimestamp = CcDateTime(0);
  #endif
}

CcStatus CcImageData::saveToFile(const CcString& sPathToFile)
{
  CcStatus oStatus;
  #ifdef FULL_OS_AVAILABLE
    CcFile oFile(sPathToFile);
    if (oFile.open(EOpenFlags::Write))
    {
      oFile.writeArray(m_oBuffer);
      oFile.close();
    }
  #else
    CCUNUSED(sPathToFile);
  #endif
  return oStatus;
}
