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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcXmlFile
 */
#include "CcXml/CcXmlFile.h"
#include "CcFile.h"
#include "CcByteArray.h"

CcXmlFile::CcXmlFile( void )
{
}

CcXmlFile::CcXmlFile(const CcString& sFilePath) :
  m_sFilePath(sFilePath)
{
}

CcXmlFile::CcXmlFile( const CcXmlFile& oToCopy )
{
  operator=(oToCopy);
}

CcXmlFile::CcXmlFile( CcXmlFile&& oToMove )
{
  operator=(std::move(oToMove));
}

CcXmlFile::~CcXmlFile( void )
{
}

CcXmlFile& CcXmlFile::operator=(const CcXmlFile& oToCopy)
{
  CCUNUSED(oToCopy);
  return *this;
}

CcXmlFile& CcXmlFile::operator=(CcXmlFile&& oToMove)
{
  if(this != &oToMove)
  {
    m_sFilePath = std::move(m_sFilePath);
    m_oDocument = std::move(m_oDocument);
  }
  return *this;
}

bool CcXmlFile::operator==(const CcXmlFile& oToCompare) const
{
  bool bRet = false;
  CCUNUSED(oToCompare);
  return bRet;
}

bool CcXmlFile::operator!=(const CcXmlFile& oToCompare) const
{
  return !operator==(oToCompare);
}

bool CcXmlFile::writeData()
{
  bool bRet = false;
  CcFile oFile(m_sFilePath);
  if (oFile.open(EOpenFlags::Write))
  {
    bRet = true;
    bRet = oFile.writeString(m_oDocument.getXmlDocument());
    oFile.close();
  }
  return bRet;
}

bool CcXmlFile::readData()
{
  bool bRet = false;
  CcFile oFile(m_sFilePath);
  if (oFile.open(EOpenFlags::Read | EOpenFlags::ShareRead))
  {
    CcString sFileContent = oFile.readAll();
    bRet = m_oDocument.parseDocument(sFileContent);
    oFile.close();
  }
  return bRet;
}