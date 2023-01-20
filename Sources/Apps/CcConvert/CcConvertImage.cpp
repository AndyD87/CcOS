/*
 * This file is part of CcConvertImage.
 *
 * CcConvertImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConvertImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConvertImage.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcConvertImage
 */
#include "CcConvertImage.h"
#include "CcKernel.h"
#include "CcFile.h"

#ifdef WINDOWS
  #include "Shell/CcWinRawConsole.h"
#endif

CcConvertImage::CcConvertImage()
{
}

CcConvertImage::~CcConvertImage()
{
}

bool CcConvertImage::loadImage(const CcString& sImage)
{
  bool bSuccess = false;
  bSuccess = loadFile(sImage);
  return bSuccess;
}

bool CcConvertImage::convertToPixmap(const CcString& sImage)
{
  bool bSuccess = false;
  CcByteArray oData = CcImage::convert(EImageType::Pixmap, nullptr);
  return bSuccess;
}
