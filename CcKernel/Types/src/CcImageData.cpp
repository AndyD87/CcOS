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
 * @brief     Implementation of Class CcImageData
 */
#include "Types/CcImageData.h"
#include "CcFile.h"

CcImageData::CcImageData(void)
{
}

CcImageData::CcImageData(const CcByteArray &oBuffer, EImageType eType)
{
  m_Buffer = oBuffer;
  m_Type = eType;
}

CcImageData::~CcImageData( void )
{
}

const CcByteArray& CcImageData::getBuffer()
{
  return m_Buffer;
}

EImageType CcImageData::getType(void)
{
  return m_Type;
}