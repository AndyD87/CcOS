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
 * @brief     Types for making exchanges more efficient or simple.
 */

#include "CcBase.h"
#include "CcTypes.h"
#include "CcString.h"

/// Mapping Languages with Enums and English Strings
CcLanguage::s_LanguageMapItem LanguageMap[] =
{
//  enum      2char 3char   long
  { CcLanguage::Unknown  , "UN", "UNK", "Unknown"   },
  { CcLanguage::English  , "EN", "ENG", "English"   },
  { CcLanguage::German   , "DE", "GER", "German"    },
  { CcLanguage::Chinese  , "CH", "CHN", "Simplified Chinese"   },
  { CcLanguage::Espanol  , "ES", "ESP", "Spanish"   },
  { CcLanguage::French   , "FR", "FRE", "French"    },
  { CcLanguage::Italian  , "IT", "ITA", "Italian"   },
  { CcLanguage::Japanese , "JP", "JAP", "Japanese"  },
  { CcLanguage::Portugese, "PT", "POR", "Portuguese"},
  { CcLanguage::Russian  , "RU", "RUS", "Russian"   },
  { CcLanguage::Korean   , "KO", "KOR", "Korean"    },
};


CcLanguage::LanguageType CcLanguage::getLanguage(const CcString& sLang)
{
  LanguageType eRet = Unknown;
  if (sLang.length() == 2)
  {
    for (uint16 i = 0; i < LanguageMapSize; i++)
    {
      if (sLang == LanguageMap[i].LangShort2C)
      {
        eRet = LanguageMap[i].Lang;
      }
    }
  }
  else if (sLang.length() == 3)
  {
    for (uint16 i = 0; i < LanguageMapSize; i++)
    {
      if (sLang == LanguageMap[i].LangShort3C)
      {
        eRet = LanguageMap[i].Lang;
      }
    }
  }
  else
  {
    for (uint16 i = 0; i < LanguageMapSize; i++)
    {
      if (sLang == LanguageMap[i].LangLong)
      {
        eRet = LanguageMap[i].Lang;
      }
    }
  }
  return eRet;
}

CcString CcLanguage::getLanguageShort2C(LanguageType eType)
{
  return LanguageMap[eType].LangShort2C;
}

size_t CcLanguage::LanguageMapSize = sizeof(LanguageMap) / sizeof(s_LanguageMapItem);
