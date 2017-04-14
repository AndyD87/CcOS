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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcImage
 */
#include "CcImage.h"
#include "CcFile.h"

CcImage::CcImage(void)
{
}

CcImage::CcImage(const CcImage &copy)
{
  this->m_Buffer = copy.m_Buffer;
  this->m_Type = this->m_Type;
}

CcImage::~CcImage( void )
{
}

CcByteArray CcImage::getImageBuffer()
{
  return m_Buffer;
}

bool CcImage::loadFile(const CcString& sPathToFile)
{
  bool bSuccess = false;
  CcFile cFile(sPathToFile);
  if (cFile.open(EOpenFlags::Read))
  {
    m_Buffer = cFile.readAll();
    cFile.close();
    if (m_Buffer.size() > 0)
      bSuccess = true;
  }
  return bSuccess;
}

void CcImage::fillBuffer(const CcByteArray &oToCopy, EImageType Type)
{
  m_Buffer = oToCopy;
  m_Type = Type;
}

void CcImage::fillBuffer(const char *buffer, size_t bufSize, EImageType Type)
{
  m_Buffer.clear();
  m_Buffer.append(buffer, bufSize);
  m_Type = Type;
}

bool CcImage::convert(EImageType Type, void* Settings)
{
  bool bSuccess = false;
  CCUNUSED(Type);
  CCUNUSED(Settings);
  return bSuccess;
}

EImageType CcImage::getType(void)
{
  return m_Type;
}