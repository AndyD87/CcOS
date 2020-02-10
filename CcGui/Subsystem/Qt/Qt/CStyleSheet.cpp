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
 * @brief     Implemtation of class CStyleSheet
 */
#include "CStyleSheet.h"
#include "CcGlobalStrings.h"
#include "CcColor.h"

namespace NQt
{

CStyleSheet::CStyleSheet()
{
}

CStyleSheet::~CStyleSheet()
{
}

CcString CStyleSheet::getStyleSheet()
{
  CcString sStyle;
  bool bFirst = true;
  for(CcStringPair& oPair : *this)
  {
    if(bFirst)
      bFirst = false;
    else
      sStyle << CcGlobalStrings::Seperators::Semicolon;
    sStyle << oPair.getKey() << CcGlobalStrings::Seperators::Colon << oPair.getValue();
  }
  return sStyle;
}

CcString CStyleSheet::getStyleSheet(const CcString& sBaseType)
{
  CcString sStyle = sBaseType + CcGlobalStrings::Brackets::CurlyLeft;
  sStyle << getStyleSheet() << CcGlobalStrings::Brackets::CurlyRight;
  return sStyle;
}

void CStyleSheet::setBackgroundColor(const CcColor& oColor)
{
  if(containsKey("background-color"))
  {
    operator[]("background-color") = oColor.getCssString();
  }
  else
  {
    add("background-color", oColor.getCssString());
  }
}

void CStyleSheet::setForegroundColor(const CcColor& oColor)
{
  if(containsKey("color"))
  {
    operator[]("color") = oColor.getCssString();
  }
  else
  {
    add("color", oColor.getCssString());
  }
}

void CStyleSheet::setBorderColor(const CcColor& oColor)
{
  if(containsKey("border-color"))
  {
    operator[]("border-color") = oColor.getCssString();
  }
  else
  {
    add("border-color", oColor.getCssString());
  }
}

void CStyleSheet::setBorderSize(uint16 uiSize)
{
  if(containsKey("border-size"))
  {
    operator[]("border-size") = CcString::fromNumber(uiSize);
  }
  else
  {
    add("border-size", CcString::fromNumber(uiSize));
  }
}

}
