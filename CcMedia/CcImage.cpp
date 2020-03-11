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
 * @brief     Implementation of Class CcImage
 */
#include "CcImage.h"
#include "CcFile.h"
#include "Private/IImageConverter.h"

CcMutex                              CcImage::m_oConverterListLock;
CcVector<NImage::IImageConverter*>   CcImage::m_pConverterList;

CcImage::CcImage()
{
}

CcImage::~CcImage()
{
}

bool CcImage::loadFile(const CcString& sPathToFile)
{
  bool bSuccess = false;
  CcFile cFile(sPathToFile);
  if (cFile.open(EOpenFlags::Read))
  {
    m_oBuffer = cFile.readAll();
    cFile.close();
    if (m_oBuffer.size() > 0)
      bSuccess = true;
  }
  return bSuccess;
}

bool CcImage::convert(EImageType Type, void* Settings)
{
  bool bSuccess = false;
  CCUNUSED(Type);
  CCUNUSED(Settings);
  return bSuccess;
}

EImageType CcImage::findType(const CcByteArray& oData)
{
  m_oConverterListLock.lock();
  EImageType eType;
  for(NImage::IImageConverter* pConverter : m_pConverterList)
  {
    eType = pConverter->checkFormat(oData);
    if(eType != EImageType::Unknown)
    {
      break;
    }
  }
  m_oConverterListLock.unlock();
  return eType;
}

void CcImage::registerConverter(NImage::IImageConverter* pConverter)
{
  m_oConverterListLock.lock();
  if(!m_pConverterList.contains(pConverter))
    m_pConverterList.append(pConverter);
  m_oConverterListLock.unlock();
}

void CcImage::unregisterConverter(NImage::IImageConverter* pConverter)
{
  m_oConverterListLock.lock();
  m_pConverterList.removeItem(pConverter);
  m_oConverterListLock.unlock();
}
