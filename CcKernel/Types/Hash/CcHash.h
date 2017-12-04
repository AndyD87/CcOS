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
 * @page      Types
 * @subpage   Hash
 *
 * @page      Types
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Types for making exchanges more efficient or simple.
 */
#ifndef CCTYPES_H_
#define CCTYPES_H_

#include "CcBase.h"
#include "CcKernelBase.h"

/**
 * @brief Ip-Structure vor IPv4
 */
typedef struct{
  uint8 ip4;
  uint8 ip3;
  uint8 ip2;
  uint8 ip1;
} ipv4_t;

/**
 * @brief Ip-Structure vor IPv6
 */
typedef struct{
  uint16 ip8;
  uint16 ip7;
  uint16 ip6;
  uint16 ip5;
  uint16 ip4;
  uint16 ip3;
  uint16 ip2;
  uint16 ip1;
} ipv6_t;

/**
 * @brief Ip-Type enum to identify wich ip version is used
 */
typedef enum{
  IPv4,
  IPv6
} eIpType;

/**
 * @brief Ip-Structure for IPv6 or IPv4, identification wil be
 *        done by IpType;
 */
typedef struct{
  eIpType type;
  union{
    ipv4_t ipv4;
    ipv6_t ipv6;
  };
} ip_t;

/// forward declaration
class CcString;

class CcKernelSHARED  CcLanguage
{
public:
  /**
   * @brief Enumerate Languages for a better Exchanging instead of Strings
   */
  typedef enum
  {
    Unknown=0, //!< Unknown Language
    English  , //!< English
    German   , //!< German
    Chinese  , //!< Chinese
    Espanol  , //!< Espanol
    French   , //!< French
    Italian  , //!< Italian
    Japanese , //!< Japanese
    Portugese, //!< Portugese
    Russian,   //!< Russian
    Korean,    //!< Korean
  } LanguageType;

  /**
   * @brief Structure for Mapping enums and strings for differnt Languages
   */
  typedef struct
  {
    LanguageType Lang;      //!< Enum of Language
    const char *LangShort2C; //!< Language in Shortform with 2 Chars
    const char *LangShort3C; //!< Language in Shortform with 3 Chars
    const char *LangLong;    //!< Language in long form, in English
  } s_LanguageMapItem;

  /**
   * @brief Get language-enum from string
   * @param sLanguage: Language as string
   * @return LanguageType as enum or Unknown if not found.
   */
  static LanguageType getLanguage(const CcString& sLang);
  static CcString getLanguageShort2C(LanguageType type);
  static size_t LanguageMapSize;          //!< Number of Entries in @ref LanguageMap
};
#endif /* CCTYPES_H_ */
