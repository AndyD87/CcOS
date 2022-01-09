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
 * @brief     Implementation of Class CcIniFile
 */

#include "CcIniFile.h"
#include "CcFile.h"
#include "CcStringUtil.h"
#include "CcStringList.h"
#include "CcGlobalStrings.h"
#include "CcByteArray.h"

CcStatus CcIniFile::readFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  CcStatus oStatus = oFile.open(EOpenFlags::Read);
  if(oStatus)
  {
    oStatus = readStream(oFile);
  }
  return oStatus;
}

CcStatus CcIniFile::readStream(IIo& oStream)
{
  CcStatus oStatus(true);
  m_oSections.clear();
  m_oSections.append(CIniLine(), CSection());
  CcString sData = oStream.readAll();
  CcStringList oLines = sData.splitLines(true);
  for(CcString& sLine : oLines)
  {
    size_t uiNext = 0;
    // Read Pre Key
    uiNext = CcStringUtil::findNextNotWhiteSpace(sLine);
    if(uiNext < sLine.size())
    {
      if(sLine[uiNext] == CcGlobalStrings::Brackets::SquareLeft[0])
      {
        addSection(sLine[uiNext]);
      }
    }
  }
  return oStatus;
}

CcStatus CcIniFile::addSection(const CcString& sLine)
{
  CcStatus oStatus(false);
  CIniLine oLine;
  size_t uiPos = 0;
  size_t uiNext = CcStringUtil::findChar(sLine, CcGlobalStrings::Brackets::SquareLeft[0], sLine.length());
  oLine.sPreKey = sLine.substr(uiPos, uiNext - uiPos);
  if(uiNext < sLine.length())
  {
    uiPos = uiNext + 1;
    uiNext = CcStringUtil::findChar(sLine, CcGlobalStrings::Brackets::SquareRight[0], uiPos);
    if(uiNext < sLine.length())
    {
      oLine.sKey = sLine.substr(uiPos, uiNext - uiPos);
      uiPos = uiNext + 1;
      oLine.sPostKey = sLine.substr(uiPos);
      m_oSections.append(oLine, CSection());
      oStatus = true;
    }
  }
  return oStatus;
}

CcStatus CcIniFile::addLine(const CcString& sLine)
{
  CcStatus oStatus(false);
  CIniLine oLine;
  size_t uiPos = 0;
  size_t uiNext = CcStringUtil::findNextNotWhiteSpace(sLine);
  if(uiNext < sLine.length())
  {
    oLine.sPreKey = sLine.substr(uiPos, uiNext - uiPos);
    uiPos = uiNext;
    uiNext = CcStringUtil::findNextWhiteSpace(sLine);
    if(uiNext < sLine.length())
    {
      oLine.sKey = sLine.substr(uiPos, uiNext - uiPos);
      uiPos = uiNext;
      uiNext = CcStringUtil::findChar(sLine, CcGlobalStrings::Seperators::Equal[0]);
      if(uiNext < sLine.length())
      {
        oLine.sPostKey = sLine.substr(uiPos, uiNext - uiPos);
        uiPos = uiNext+1;
        uiNext = CcStringUtil::findNextNotWhiteSpace(sLine);
        if(uiNext < sLine.length())
        {
          uiPos = uiNext;
          oLine.sPreValue = sLine.substr(uiPos, uiNext - uiPos);
        }
      }

    }
  }
  return oStatus;
}

