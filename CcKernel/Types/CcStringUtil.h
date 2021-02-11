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
 * @brief     Class CcStringUtil
 */
#ifndef H_CcStringUtil_H_
#define H_CcStringUtil_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcWString.h"

//! Forward Declarations
class CcByteArray;
class CcString;
class CcDateTime;

/**
 * @brief Static Class for String functions
 */
class CcKernelSHARED  CcStringUtil
{
public:
  /**
   * @brief Get length of \\0 terminated char array
   * @param pcString: Char array to search for
   * @param uiMaxLen: Max length of char array to search for
   * @return Position of found \\0 or uiMaxLen if not found or uiMaxLen is reached.
   */
  static size_t strlen(const char* pcString, size_t uiMaxLen = SIZE_MAX);

  /**
   * @brief Compare to char arrays up to a specific length or first found \\0 terminator.
   * @param pcString1:  First array for search
   * @param pcString2:  Second array for search
   * @param uiLen:      Maximum number of signs to compare
   * @return If 0 both strings are same, if < 0 first is lower than second or vice versa
   */
  static int strcmp(const char* pcString1, const char* pcString2, size_t uiLen = SIZE_MAX);

  /**
   * @brief Search position of a character in string.
   * @param pcString: \\0 terminated string
   * @param cToFind:  Char to searc in @p pcString
   * @return Pointer to first match, nullptr if not found
   */
  static char* strchr(char* pcString, char cToFind);

  /**
   * @brief Find a character in character string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of pcString
   * @param cToFind:  Target Character to find
   * @return Position of found character or uiLength if not found
   */
  static size_t findChar(const char* pcString, size_t uiLength, char cToFind);

  /**
   * @brief Find a character in character string, but escape found character if escaped paramter is set before.
   * @param pcString: String to search in
   * @param uiLength: Maximum length of pcString
   * @param cToFind:  Target Character to find
   * @param cEscape:  Character to ignore next character if it is cToFind
   * @return Position of found character or SIZE_MAX if not found
   */
  static size_t findChar(const char* pcString, size_t uiLength, char cToFind, char cEscape);

  /**
   * @brief Find specific character from a list of character in string.
   * @param pcString:     String to search in
   * @param uiLength:     Maximum length of string to search
   * @param pcToFind:     Array of character to find
   * @param uiToFindSize: Number of character in @p pcToFind to search for
   * @param[out] cFound:  First found character as output or not changed if not found
   * @return Position of first found charachter or SIZE_MAX if no character was found.
   */
  static size_t findCharOf(const char* pcString, size_t uiLength, const char* pcToFind, size_t uiToFindSize, char& cFound);

  /**
   * @brief Find position of last maching character in string.
   * @param pcString: \\0 terminated string
   * @param cToFind:  Charachter to search for
   * @return Position of last maching character or length of string if not found
   */
  static size_t findLastChar(const char* pcString, char cToFind)
  { return findLastChar(pcString, SIZE_MAX, cToFind);}

  /**
   * @brief Find position of last maching character in string up to specific length or \\0 character reached.
   * @param pcString: string to search in
   * @param uiLength: Maximum length of character to search
   * @param cToFind:  Charachter to search for
   * @return Position of last maching character or uiLength or \\0 what ever comes first if not found
   */
  static size_t findLastChar(const char* pcString, size_t uiLength, char cToFind);

  /**
   * @brief Find next whitespace in string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of string
   * @return Position of next whitespace or uiLength if not found
   */
  static size_t findNextWhiteSpace(const char* pcString, size_t uiLength);

  /**
   * @brief Find next not whitespace in string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of string
   * @return Position of next not whitespace or uiLength if not found
   */
  static size_t findNextNotWhiteSpace(const char* pcString, size_t uiLength);

  /**
   * @brief Check if character is a whitespace
   * @param toTest: Character to test
   * @return True if whitespace
   */
  static bool isWhiteSpace(const char toTest);

  /**
   * @brief Get length of \\0 terminated char array
   * @param pcString: Char array to search for
   * @param uiMaxLen: Max length of char array to search for
   * @return Position of found \\0 or uiMaxLen if not found or uiMaxLen is reached.
   */
  static size_t strlen(const wchar_t* pcString, size_t uiMaxLen = SIZE_MAX);

  /**
   * @brief Compare to char arrays up to a specific length or first found \\0 terminator.
   * @param pcString1:  First array for search
   * @param pcString2:  Second array for search
   * @param uiLen:      Maximum number of signs to compare
   * @return If 0 both strings are same, if < 0 first is lower than second or vice versa
   */
  static int strcmp(const wchar_t* pcString1, const wchar_t* pcString2, size_t uiLen = SIZE_MAX);

  /**
   * @brief Search position of a character in string.
   * @param pcString: \\0 terminated string
   * @param cToFind:  Char to searc in @p pcString
   * @return Pointer to first match, nullptr if not found
   */
  static wchar_t* strchr(wchar_t* pcString, wchar_t cToFind);
  /**
   * @brief Find a character in character string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of pcString
   * @param cToFind:  Target Character to find
   * @return Position of found character or uiLength if not found
   */
  static size_t findChar(const wchar_t* pcString, size_t uiLength, wchar_t cToFind);

  /**
   * @brief Find a character in character string, but escape found character if escaped paramter is set before.
   * @param pcString: String to search in
   * @param uiLength: Maximum length of pcString
   * @param cToFind:  Target Character to find
   * @param cEscape:  Character to ignore next character if it is cToFind
   * @return Position of found character or uiLength if not found
   */
  static size_t findChar(const wchar_t* pcString, size_t uiLength, wchar_t cToFind, wchar_t cEscape);

  /**
   * @brief Find specific character from a list of character in string.
   * @param pcString:     String to search in
   * @param uiLength:     Maximum length of string to search
   * @param pcToFind:     Array of character to find
   * @param uiToFindSize: Number of character in @p pcToFind to search for
   * @param[out] cFound:  First found character as output or not changed if not found
   * @return Position of first found charachter or SIZE_MAX if no character was found.
   */
  static size_t findCharOf(const wchar_t* pcString, size_t uiLength, const wchar_t* pcToFind, size_t uiToFindSize, wchar_t& cFound);

  /**
   * @brief Find next whitespace in string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of string
   * @return Position of next whitespace or uiLength if not found
   */
  static size_t findNextWhiteSpace(const wchar_t* pcString, size_t uiLength);

  /**
   * @brief Find next not whitespace in string
   * @param pcString: String to search in
   * @param uiLength: Maximum length of string
   * @return Position of next not whitespace or uiLength if not found
   */
  static size_t findNextNotWhiteSpace(const wchar_t* pcString, size_t uiLength);

  /**
   * @brief Check if character is a whitespace
   * @param toTest: Character to test
   * @return True if whitespace
   */
  static bool isWhiteSpace(const wchar_t toTest);
  /**
   * @brief Check if containing String contains the following values for true:
   *          - TRUE
   *          - ON
   *          - >0
   *        Values for false are:
   *          - TRUE
   *          - OFF
   *          - 0
   *          - 0
   *        All comparision will be done case insensitive.
   * @param sToParse: String to parse for boolean
   * @param[out] pbOk:  optional, default is null
   *                    If not null, the value will be set to true if string was recognized
   *                    as a boolean value.
   * @return If bool string is parsed successfully, the correct value will be set as return value,
   *         otherwise false will returend as default value;
   */
  static bool getBoolFromStirng(const CcString& sToParse, bool* pbOk = nullptr);
  static bool getBoolFromStirng(const CcWString& sToParse, bool* pbOk = nullptr);
  static CcString getOctalStringFromByte(char uiByte);
  static CcString encodeBase64(const CcByteArray& toEncode);
  static CcByteArray decodeBase64(const CcString& toDecode);
  static CcString encodeBase58(const CcByteArray& toEncode);
  static CcByteArray decodeBase58(const CcString& toDecode);
  static CcString getExtensionFromPath(const CcString& sPath);
  static CcString getFilenameFromPath(const CcString& sPath);
  static CcString getDirectoryFromPath(const CcString& sPath);
  static CcString getHumanReadableSizePerSeconds(uint64 uiSize, const CcDateTime& oTime);
  static CcString getHumanReadableSize(uint64 uiSize, uint8 uiPrecision = 2);

  static uint64 toUint64(const char* pcString, size_t uiLen, bool* pbOk = nullptr, uint8 uiBase = 10);
  static uint32 toUint32(const char* pcString, size_t uiLen, bool* pbOk = nullptr, uint8 uiBase = 10);
  static uint64 toUint64(const wchar_t* pcString, size_t uiLen, bool* pbOk = nullptr, uint8 uiBase = 10);
  static uint32 toUint32(const wchar_t* pcString, size_t uiLen, bool* pbOk = nullptr, uint8 uiBase = 10);
  static CcString fromUint64(uint64 uiValue, uint8 uiBase = 10);
  static CcString fromUint32(uint32 uiValue, uint8 uiBase = 10);
  static CcString fromInt64(int64 iValue, uint8 uiBase = 10);
  static CcString fromInt32(int32 iValue, uint8 uiBase = 10);
  static CcString fromFloat(float fValue, uint8 uiBase = 10, bool bDisableExponent = false);
  static CcString fromDouble(double fValue, uint8 uiBase = 10, bool bDisableExponent = false);
  static CcString fromUtf16(const uint16* pUtf16, size_t uiLen);

  static bool cmpWithLower(const CcString& sToCompare, const CcString& sToLowerCompare);
  static CcString findArgument(const CcString& sHeader, size_t& uiPosition);

private:
  /**
   * @brief Constructor
   */
  CcStringUtil(){}

  static CcString encodeBaseX(const CcByteArray& toEncode, const char* pcAlphabet, uint8 uiBaseSize);
  static CcByteArray decodeBaseX(const CcString& toDecode, const char* pcAlphabet, uint8 uiBaseSize);

  static char getBase64EncodedStringChar(char cIn);
  static char getBase64EncodedUrlStringChar(char cIn);
  static char getBase64DecodedStringChar(char cIn);
  static char getBase64DecodedUrlStringChar(char cIn);
};

#endif // H_CcStringUtil_H_
