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
    Pixmap,
    Any
  };

private:
  EType m_eType;
  ECoding m_eCoding;
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
  return eType == EImageType::Ppm;
}

EImageType CImagePpm::checkFormat(const CcByteArray& oToCheck)
{
  EImageType eType = EImageType::Unknown;
  if (oToCheck.size() > 1 &&
    oToCheck[0] == 'P' &&
    oToCheck[1] > '0' &&
    oToCheck[1] < '8'
    )
  {
    eType = EImageType::Ppm;
  }
  return eType;
}

CcImageRaw CImagePpm::convertToRaw(const CcByteArray& oInput)
{
  CCUNUSED(oInput);
  return CcImageRaw();
}

CcByteArray CImagePpm::convertFromRaw(const CcImageRaw& oInput)
{
  CCUNUSED(oInput);
  return CcByteArray();
}
