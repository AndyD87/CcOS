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
#include "Converter/IImageConverter.h"
#include "Converter/CImageBmp.h"
#include "Converter/CImagePpm.h"

size_t                               CcImage::m_iInit(0);
CcMutex                              CcImage::m_oConverterListLock;
CcVector<NImage::IImageConverter*>   CcImage::m_pConverterList;
CcImage                              CcImage::m_oImage;

CcImage::CcImage()
{
  if (m_iInit == 0)
  {
    m_oConverterListLock.lock();
    CCNEWTYPE(pBmp, NImage::CImageBmp);
    m_pConverterList.append(pBmp);
    CCNEWTYPE(pPpm, NImage::CImagePpm);
    m_pConverterList.append(pPpm);
    m_oConverterListLock.unlock();
  }
  m_iInit++;
}

CcImage::~CcImage()
{
  m_iInit--;
  if (m_iInit == 0)
  {
    m_oConverterListLock.lock();
    for (NImage::IImageConverter* pConverter : m_pConverterList)
    {
      CCDELETE(pConverter);
    }
    m_pConverterList.clear();
    m_oConverterListLock.unlock();
  }
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

bool CcImage::setBuffer(const CcByteArray& oToCopy, EImageType eType)
{
  if (eType == EImageType::Unknown)
    eType = findType(oToCopy);
  CcImageData::setBuffer(oToCopy, eType);
  return eType != EImageType::Unknown;
}

CcImageRaw CcImage::getRaw()
{
  CcImageRaw oImage;
  NImage::IImageConverter* pConverter = getConverter(m_eType);
  if (pConverter)
    oImage = pConverter->convertToRaw(m_oBuffer);
  return oImage;
}

EImageType CcImage::findType(const CcByteArray& oData)
{
  m_oConverterListLock.lock();
  EImageType eType(EImageType::Unknown);
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

NImage::IImageConverter* CcImage::getConverter(EImageType eType)
{
  NImage::IImageConverter* pConverter = nullptr;
  m_oConverterListLock.lock();
  for (NImage::IImageConverter* pConverterTemp : m_pConverterList)
  {
    if (pConverterTemp->checkType(eType))
    {
      pConverter = pConverterTemp;
      break;
    }
  }
  m_oConverterListLock.unlock();
  return pConverter;
}

void CcImage::registerConverter(NImage::IImageConverter* pConverter)
{
  m_oConverterListLock.lock();
  if(!m_pConverterList.contains(pConverter))
    m_pConverterList.append(pConverter);
  m_oConverterListLock.unlock();
}

void CcImage::deregisterConverter(NImage::IImageConverter* pConverter)
{
  m_oConverterListLock.lock();
  m_pConverterList.removeItem(pConverter);
  m_oConverterListLock.unlock();
}
