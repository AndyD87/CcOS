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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcIniFile
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcList.h"
#include "CcGlobalStrings.h"

class IIo;

/**
 * @brief Ini file format reader and writer
 */
class CcKernelSHARED  CcIniFile
{
public:
  class CcKernelSHARED CLine
  {
  public:
    bool operator==(const CLine& oToCompare) const
    { return oToCompare.sKey == sKey && oToCompare.sValue == oToCompare.sValue; }

    CcString sKey;
    CcString sValue;
    CcString sPreKey;
    CcString sPostKey;
    CcString sPreValue;
    CcString sPostValue;
    char     cValueSign = 0;
    char     cCommentSign = 0;
    CcString sComment;
  };

  class CcKernelSHARED CSection : public CcList<CLine>
  {
  public:
    CSection() = default;
    CSection(CLine oSectionData) : 
      m_oSectionData(oSectionData)
    {}

    const CcString& getName() const
    { return m_oSectionData.sKey; }
    const CLine& getSectionLine() const
    { return m_oSectionData; }
    static CSection& getInvalidSection()
    { return s_oInvalidSection; }

    bool isValid()const
    { return this != &s_oInvalidSection; }

    bool keyExists(const CcString& sName) const;
    const CcString& getValue(const CcString& sName) const;
    void setValue(const CcString& sName, const CcString& sValue);

  private:
    CLine m_oSectionData;
    
    static CSection s_oInvalidSection;
  };

  typedef CcList<CSection> CSectionList;
  #ifdef _MSC_VER
    class CcKernelSHARED CSectionList;
  #endif

  /**
   * @brief Constructor
   */
  CcIniFile() = default;

  /**
   * @brief Destructor
   */
  ~CcIniFile() = default;

  CcStatus readFile(const CcString& sPath);
  CcStatus writeFile(const CcString& sPath);

  CcStatus readStream(IIo& oStream);
  CcStatus writeStream(IIo& oStream, const CcString& sLineEnding = CcGlobalStrings::EolOs);

  CSection& operator[](size_t uiPos)
  { return m_oSections[uiPos]; }
  const CSection& operator[](size_t uiPos) const
  { return m_oSections[uiPos]; }

  CSection& operator[](const CcString& sSectionName);
  const CSection& operator[](const CcString& sSectionName) const;

  CSection& createSection(const CcString& sSectionName);

private:
  CcStatus addSection(const CcString& sLine);
  CcStatus addLine(const CcString& sLine);

private:
  CcStatus    m_eError;
  CSectionList m_oSections;
};
