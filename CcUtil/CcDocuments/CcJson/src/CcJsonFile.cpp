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
 * @brief     Implemtation of class CcJsonFile
 */
#include "CcJson/CcJsonFile.h"
#include "CcFile.h"
#include "CcByteArray.h"

CcJsonFile::CcJsonFile(void )
{
}

CcJsonFile::CcJsonFile(const CcString& sPathToFile)
{
  setFile(sPathToFile);
  if (CcFile::exists(sPathToFile))
  {
    parseFile(sPathToFile);
  }
}

CcJsonFile::~CcJsonFile(void )
{
}

void CcJsonFile::setFile(const CcString& sPathToFile)
{
  m_sFilePath = sPathToFile;
}

bool CcJsonFile::parseFile(const CcString& sPathToFile)
{
  setFile(sPathToFile);
  return parseFile();
}

bool CcJsonFile::parseFile()
{
  bool bRet = false;
  if (CcFile::exists(m_sFilePath))
  {
    CcFile oFile(m_sFilePath);
    if (oFile.open(EOpenFlags::Read))
    {
      CcString sDocument = oFile.readAll();
      if (sDocument.length() > 0)
      {
        bRet = m_oDocument.parseDocument(sDocument);
      }
      else
      {
        CCDEBUG("File was empty, unable to parse");
      }
      oFile.close();
    }
    else
    {
      CCDEBUG("File could not be read to parse");
    }
  }
  else
  {
    CCDEBUG("File not existing to parse");
  }
  return bRet;
}

bool CcJsonFile::write(bool bCompact)
{
  bool bRet = false;
  CcFile oFile(m_sFilePath);
  if (oFile.open(EOpenFlags::Write))
  {
    bRet = oFile.writeString(m_oDocument.getDocument(bCompact));
    oFile.close();
  }
  return bRet;
}