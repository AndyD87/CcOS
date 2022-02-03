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

// Disable known bug warning in VS 2015 for init static variables
#if _MSC_VER == 1900
  #pragma warning(push) 
  #pragma warning(disable:4592)
#endif
CcIniFile::CSection CcIniFile::CSection::s_oInvalidSection;
#if _MSC_VER == 1900
  #pragma warning(pop) 
#endif


bool CcIniFile::CSection::keyExists(const CcString& sName) const
{
  bool bExists = false;
  for(const CLine& oLine : *this)
  {
    if(oLine.sKey == sName)
    {
      bExists = true;
      break;
    }
  }
  return bExists;
}

const CcString& CcIniFile::CSection::getValue(const CcString& sName) const
{
  for(const CLine& oLine : *this)
  {
    if(oLine.sKey == sName)
    {
      return oLine.sValue;
    }
  }
  return CcGlobalStrings::Empty;
}

void CcIniFile::CSection::setValue(const CcString& sName, const CcString& sValue)
{
  bool bCreateValue = true;
  for(CLine& oLine : *this)
  {
    if(oLine.sKey == sName)
    {
      oLine.sValue = sValue;
      bCreateValue = false;
    }
  }
  if(bCreateValue)
  {
    CLine oLine;
    oLine.sKey = sName;
    oLine.sValue = sValue;
    append(oLine);
  }
}

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

CcStatus CcIniFile::writeFile(const CcString& sPath)
{
  CcFile oFile(sPath);
  CcStatus oStatus = oFile.open(EOpenFlags::Write);
  if(oStatus)
  {
    oStatus = writeStream(oFile);
    oFile.close();
  }
  return oStatus;
}

CcStatus CcIniFile::readStream(IIo& oStream)
{
  m_eError = true;
  m_oSections.clear();
  appendSection(CSection());
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
        addSection(sLine);
      }
      else
      {
        addLine(sLine);
      }
    }
    else
    {
      addLine(sLine);
    }
  }
  return m_eError;
}

CcStatus CcIniFile::writeStream(IIo& oStream, const CcString& sLineEnding)
{
  for (CSection& oSection : m_oSections)
  {
    if (oSection.getSectionLine().sKey.length())
    {
      oStream.writeString(oSection.getSectionLine().sPreKey);
      oStream.writeString(CcGlobalStrings::Brackets::SquareLeft);
      oStream.writeString(oSection.getSectionLine().sKey);
      oStream.writeString(CcGlobalStrings::Brackets::SquareRight);
      oStream.writeString(oSection.getSectionLine().sPostKey);

      if (oSection.getSectionLine().cCommentSign != 0)
      {
        oStream.writeString(oSection.getSectionLine().cCommentSign);
        oStream.writeString(oSection.getSectionLine().sComment);
      }
      oStream.writeString(sLineEnding);
    }
    for (CLine& oLine : oSection)
    {
      oStream.writeString(oLine.sPreKey);
      oStream.writeString(oLine.sKey);
      oStream.writeString(oLine.sPostKey);
      if (oLine.sValue.length())
      {
        oStream.writeString(CcGlobalStrings::Seperators::Equal);
        oStream.writeString(oLine.sPreValue);
        switch (oLine.cValueSign)
        {
          case '"':
            oStream.writeString(CcGlobalStrings::Seperators::Quote);
            oStream.writeString(oLine.sValue.getReplace("\"", "\\\""));
            oStream.writeString(CcGlobalStrings::Seperators::Quote);
            break;
          case '\'':
            oStream.writeString(CcGlobalStrings::Seperators::SingleQuote);
            oStream.writeString(oLine.sValue.getReplace("'", "\\'"));
            oStream.writeString(CcGlobalStrings::Seperators::SingleQuote);
            break;
          default:
            oStream.writeString(oLine.sValue);

        }
        oStream.writeString(oLine.sPostValue);
      }

      if (oLine.cCommentSign != 0)
      {
        oStream.writeString(oLine.cCommentSign);
        oStream.writeString(oLine.sComment);
      }
      if(&oLine != &oSection.last())
        oStream.writeString(sLineEnding);
    }
    if (&oSection != &m_oSections.last())
      oStream.writeString(sLineEnding);
  }
  return true;
}

CcIniFile::CSection& CcIniFile::operator[](const CcString& sSectionName)
{
  for (CSection& oSection : m_oSections)
  {
    if (oSection.getName() == sSectionName)
    {
      return oSection;
    }
  }
  return CSection::getInvalidSection();
}

const CcIniFile::CSection& CcIniFile::operator[](const CcString& sSectionName) const
{
  for (CSection& oSection : m_oSections)
  {
    if (oSection.getName() == sSectionName)
    {
      return oSection;
    }
  }
  return CSection::getInvalidSection();
}

CcStatus CcIniFile::addSection(const CcString& sLine)
{
  CLine oLine;
  size_t uiPos = 0;
  size_t uiNext = CcStringUtil::findChar(sLine, CcGlobalStrings::Brackets::SquareLeft[0]);
  oLine.sPreKey = sLine.substr(uiPos, uiNext - uiPos);
  if(uiNext < sLine.length())
  {
    uiPos = uiNext + 1;
    uiNext = CcStringUtil::findChar(sLine, CcGlobalStrings::Brackets::SquareRight[0], uiPos);
    if(uiNext < sLine.length())
    {
      oLine.sKey = sLine.substr(uiPos, uiNext - uiPos);
      uiPos = uiNext + 1;

      // Check for comment if not at the end
      if (uiPos < sLine.length())
      {
        // Read until comment or end of string
        uiNext = CcStringUtil::findChar(sLine, ';', uiPos);
        uiNext = CCMIN(uiNext, CcStringUtil::findChar(sLine, '#', uiPos));
        if (uiNext < sLine.length() && uiNext > uiPos)
        {
          oLine.sPostKey = sLine.substr(uiPos, uiNext - uiPos);
          uiPos = uiNext;

          // Read key value from current position until comment section or end of line is reached.
          switch (sLine[uiPos])
          {
            case ';':
              CCFALLTHROUGH;
            case '#':
              oLine.cCommentSign = sLine[uiPos];
              oLine.sComment = sLine.substr(uiPos + 1);
              uiPos = sLine.length();
              break;
          }
        }
        else
        {
          oLine.sPostKey = sLine.substr(uiPos);
          uiPos = sLine.length();
        }

      }

      appendSection(oLine);
    }
    else
    {
      // Else sign for section not found
      m_eError = false;
    }
  }
  else
  {
    // Starting sign for section not found
    m_eError = false;
  }
  return m_eError;
}

CcStatus CcIniFile::addLine(const CcString& sLine)
{
  CLine oLine;
  size_t uiPos = 0;
  size_t uiNext = CcStringUtil::findNextNotWhiteSpace(sLine);
  if(uiNext < sLine.length())
  {
    oLine.sPreKey = sLine.substr(uiPos, uiNext - uiPos);
    uiPos = uiNext;
    // Check for comment
    switch (sLine[uiPos])
    {
      case ';':
        CCFALLTHROUGH;
      case '#':
        oLine.cCommentSign = sLine[uiPos];
        oLine.sComment = sLine.substr(uiPos + 1);
        uiPos = sLine.length();
        break;
      default:
        // Find Key section
        uiNext = CcStringUtil::findNextWhiteSpace(sLine, uiPos);
        uiNext = CCMIN(uiNext, CcStringUtil::findChar(sLine, '=', uiPos));
        if (uiNext < sLine.length())
        {
          oLine.sKey = sLine.substr(uiPos, uiNext - uiPos);
          uiPos = uiNext;
          if (sLine[uiPos] != '=')
          {
            uiNext = CcStringUtil::findChar(sLine, '=', uiPos);
            if (uiNext < sLine.length())
            {
              oLine.sPostKey = sLine.substr(uiPos, uiNext - uiPos);
              uiPos = uiNext + 1;
            }
          }
          else
          {
            uiPos++;
          }

          // Find Value section
          uiNext = CcStringUtil::findNextNotWhiteSpace(sLine, uiPos);

          if (uiNext < sLine.length())
          {
            oLine.sPreValue = sLine.substr(uiPos, uiNext - uiPos);
            uiPos = uiNext;

            // Read key value from current position until comment section or end of line is reached.
            switch (sLine[uiPos])
            {
              case ';':
                CCFALLTHROUGH;
              case '#':
                oLine.cCommentSign = sLine[uiPos];
                oLine.sComment = sLine.substr(uiPos + 1);
                uiPos = sLine.length();
                break;
              case '"':
                uiPos++;
                oLine.cValueSign = '"';
                uiNext = CcStringUtil::findCharEscaped(sLine, '"', '\\', uiPos);
                if (uiNext < sLine.length())
                {
                  oLine.sValue += sLine.substr(uiPos, uiNext - uiPos);
                  uiPos = uiNext + 1;
                }
                else
                {
                  m_eError = false;
                  uiPos = sLine.length();
                }
                oLine.sValue.replace("\\\"", "\"");
                break;
              case '\'':
                uiPos++;
                oLine.cValueSign = '\'';
                uiNext = CcStringUtil::findCharEscaped(sLine, '\'', '\\', uiPos);
                if (uiNext < sLine.length())
                {
                  oLine.sValue += sLine.substr(uiPos, uiNext - uiPos);
                  uiPos = uiNext + 1;
                }
                else
                {
                  m_eError = false;
                  uiPos = sLine.length();
                }
                oLine.sValue.replace("\\'", "'");
                break;
              default:
              {
                CcString sValue;
                // Read until comment or end of string
                uiNext = CcStringUtil::findChar(sLine, ';', uiPos);
                uiNext = CCMIN(uiNext, CcStringUtil::findChar(sLine, '#', uiPos));
                if (uiNext < sLine.length())
                {
                  sValue += sLine.substr(uiPos, uiNext - uiPos);
                  uiPos = uiNext;
                }
                else
                {
                  sValue += sLine.substr(uiPos);
                  uiPos = sLine.length();
                }

                // Trim last spaces
                oLine.sValue = sValue.getTrimR();
                oLine.sPostValue = sValue.substr(oLine.sValue.length());
                break;
              }
            }

            // Check for comment if not at the end
            if (uiPos < sLine.length())
            {
              // Read until comment or end of string
              uiNext = CcStringUtil::findChar(sLine, ';', uiPos);
              uiNext = CCMIN(uiNext, CcStringUtil::findChar(sLine, '#', uiPos));
              if (uiNext < sLine.length() && uiNext > uiPos)
              {
                oLine.sPostValue = sLine.substr(uiPos, uiNext - uiPos);
                uiPos = uiNext;
              }

              // Read key value from current position until comment section or end of line is reached.
              switch (sLine[uiPos])
              {
                case ';':
                  CCFALLTHROUGH;
                case '#':
                  oLine.cCommentSign = sLine[uiPos];
                  oLine.sComment = sLine.substr(uiPos + 1);
                  uiPos = sLine.length();
                  break;
              }
            }
          }
        }
    }
  }

  if (m_eError)
  {
    m_oSections.last().append(oLine);
  }
  return m_eError;
}

CcIniFile::CSection& CcIniFile::createSection(const CcString& sSectionName)
{
  if(operator[](sSectionName).isValid())
  {
    return operator[](sSectionName);
  }
  else
  {
    CLine oSectionLine;
    oSectionLine.sKey = sSectionName;
    appendSection(oSectionLine);
    return m_oSections.last();
  }
}

CcIniFile::CSection& CcIniFile::getSection(const CcString& sSectionName)
{
  for (CSection& oSection : m_oSections)
  {
    if (oSection.getName() == sSectionName)
    {
      return oSection;
    }
  }
  return CSection::getInvalidSection();
}

void CcIniFile::appendSection(CSection&& oSection)
{
  if(getSection(oSection.getName()).isValid())
  {
    m_eError = EStatus::AlreadyExisting;
  }
  else
  {
    m_oSections.append(oSection);
  }
}
