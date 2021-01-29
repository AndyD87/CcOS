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
 * @brief      Class CcString
 */
#pragma once

#include "CcBase.h"
#include "CcTypes.h"

//! Forward Declarations
class CcStringList;
class CcByteArray;
class CcWString;
class CcIp;

/**
 * @brief Enumartion for Sensitivity
 */
enum class ESensitivity
{
  CaseSensitiv = 0,  //!< String has to used case-sensitiv
  CaseInsensitiv     //!< String has to used case-insensitiv
};

/**
 * @brief String-class for basic string handling
 *
 *  The class is based on std::string and adds some usefull functions
 */
class CcKernelSHARED CcString
{
public:
  /**
   * @brief Initialzie CcString with an external buffer.
   *
   * @param pBuff:      String buffer to use
   * @param uiLength:   Lenght of string in pBuff
   * @param uiReserved: Number of reserved bytes in pBuff
   */
  inline CcString(char* pBuff, size_t uiLength, size_t uiReserved) :
    m_pBuffer(pBuff), m_uiLength(uiLength), m_uiReserved(uiReserved)
  {}

  /**
   * @brief Create a empty string-class.
   */
  CcString();

  /**
   * @brief Create a string and copy content from another
   * @param sToCopy: Another string to copy content from
   */
  CcString(const CcString& sToCopy) :
    CcString()
  { operator=(sToCopy); }

  /**
   * @brief Create a string and move content from another
   * @param sToMove: Another string to move content from
   */
  CcString(CcString&& sToMove) NOEXCEPT :
    CcString()
  { operator=(CCMOVE(sToMove)); }

  /**
   * @brief Create a string with a predefined size and pattern.
   *        For strings wich target size is known,
   *        it saves performance if String sizes are predefined.
   * @param uiLength: Size of buffer to initially
   * @param cDefaultChar: Charakter to set for whole buffer.
   */
  CcString(size_t uiLength, const char cDefaultChar);

  /**
   * @brief Create a String-class with initialized const char array
   * @param cString: pointer to char array containing a null terminated string
   */
  CcString(const char* cString);

  /**
   * @brief Create a String-class with an initialized string of variable length
   * @param cString:  pointer to char array to be inserted
   * @param uiLength: Size of char-string to be stored in class
   */
  CcString(const char* cString, size_t uiLength);

  /**
   * @brief Create a String-class with initialized const wchar array
   *        Conversion to CcString format will automatically done.
   * @param pwcString: pointer to wchar array containing a null terminated string
   */
  CcString(const wchar_t* pwcString);

  /**
   * @brief Create a String-class with initialized const wchar array
   *        Conversion to CcString format will automatically done.
   * @param pwcString: pointer to wchar array to be inserted
   * @param uiLength:  Length of wchar-string to be stored in class. Not the size!
   */
  CcString(const wchar_t* pwcString, size_t uiLength);

  /**
   * @brief Create a String-class with an initialized string of variable length
   * @param cString:  pointer to char array to be inserted
   */
  CcString(const char cString);

  /**
   * @brief Import a ByteArray as String.
   * @param baString: ByteArray to import as String
   */
  CcString(const CcByteArray& baString);

  /**
   * @brief Import a ByteArray as String.
   * @param baString: ByteArray to import as String
   */
  CcString(CcByteArray&& baString)
    { operator=(CCMOVE(baString)); }

  /**
   * @brief Clean up and free all requested Memory
   */
  ~CcString();

  /**
   * @brief Append an addtional buffer at the end of String.
   * @param uiLength: Size of buffer to initially
   */
  void reserve(size_t uiLength);

  /**
   * @brief Append an addtional buffer at the end of String.
   * @param uiLength: Size of buffer to initially
   * @param cDefaultChar: Charakter to set for whole buffer.
   */
  void reserve(size_t uiLength, const char cDefaultChar);

  /**
   * @brief Set string in a sprintf formated way.
   */
  CcString& format(const char* sFormat, ...);

  /**
   * @brief Remove characters from string.
   * @param uiPos: Position of firs character to remove in string.
   * @param uiLength: Number o characters to be removed. Default 1.
   */
  CcString& remove(size_t uiPos, size_t uiLength = 1);

  /**
   * @brief Get a needle of String stack
   * @param pos: start position
   * @param len: length of string, or 0 until end of String
   * @return Needle, or "" if failed
   */
  CcString substr(size_t pos, size_t len = SIZE_MAX) const;

  /**
   * @brief Replace every needle with other value;
   * @param oNeedle:  String to find in Haystack
   * @param oReplace: String replaces the needle;
   * @return Needle, or "" if failed
   */
  CcString& replace(const CcString& oNeedle, const CcString& oReplace);

  /**
   * @brief Replace every needle with other value;
   * @param oNeedle: String to find in Haystack
   * @param oReplace: String replaces the needle;
   * @return Needle, or "" if failed
   */
  CcString getReplace(const CcString& oNeedle, const CcString& oReplace) const
    { return CcString(*this).replace(oNeedle, oReplace); }

  /**
   * @brief Get String between two strings
   * @param sPreStr:      String to find
   * @param sPosStr:      First String to find after position of preStr
   * @param uiOffset:     Offset for begin of searching, default 0
   * @param[out] puiPos:  Position of found string if required
   * @return Get found string or empty if not found.
   */
  CcString getStringBetween(const CcString& sPreStr, const CcString& sPosStr, size_t uiOffset = 0, size_t* puiPos = nullptr) const;

  /**
   * @brief Replace every needle with other value;
   * @param needle: String to find in Haystack
   * @param replac: String replaces the needle;
   * @return Needle, or "" if failed
   */
  CcString getLastLine() const;

  /**
   * @brief Compare a String with content if they are the same
   * @param sToCompare: String to compare to
   * @param bSensitivity: true if comparing with ignored sensitivity
   * @return true if same, otherwise false
   */
  bool compare(const CcString& sToCompare, ESensitivity bSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Compare a String with content if they are the same
   * @param sToCompare: String to compare to
   * @return true if same, otherwise false
   */
  bool compare(const char* sToCompare) const;

  /**
   * @brief Compare a String with other String, by ignoring Sensitivity, if they are the same
   * @param sToCompare: String to compare to
   * @return true if same, otherwise false
   */
  inline bool compareInsensitve(const CcString& sToCompare) const
    { return compare(sToCompare, ESensitivity::CaseInsensitiv); }

  /**
   * @brief Find the position of the last occurrence of a String
   * @param sToFind: String to search for
   * @return position of Last occurrence or SIZE_MAX if not found
   */
  size_t findLast(const CcString& sToFind) const;

  /**
   * @brief Find the position of a occurrencing String
   * @param sToFind: String to search for
   * @param offset: Position where search has to be started at.
   * @return position of First occurrence or SIZE_MAX if not found
   */
  size_t find(const CcString& sToFind, size_t offset=0) const;

  /**
   * @brief Find the position of a occurrencing String
   * @param pcString: String to search for
   * @param uiLength: Size of pcString
   * @param uiOffset: Position where search has to be started at.
   * @return position of First occurrence or SIZE_MAX if not found
   */
  size_t find(const char* pcString, size_t uiLength, size_t uiOffset) const;

  /**
   * @brief Find the position of a occurrencing String
   * @param sToFind: String to search for
   * @param offset: Position where search has to be started at.
   * @return position of First occurrence or SIZE_MAX if not found
   */
  bool contains(const CcString& sToFind, size_t offset=0) const
    { return (find(sToFind, offset) != SIZE_MAX); }

  /**
   * @brief Check if String starts with a specific value.
   *        The start of comparision can be adjusted to a specific position.
   *        It will be compared until end.
   * @param sToCompare:   Search this string at specific position.
   * @param uiOffset:     Position of first character where search should start
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String starts with sToCompare, otherwise false
   */
  bool isStringAtOffset(const CcString& sToCompare, size_t uiOffset, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Check if String starts with a specific value
   * @param sToCompare:   Search this string at the beginning of this String
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String starts with sToCompare, otherwise false
   */
  bool startsWith(const CcString& sToCompare, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Check if String ends with a specific value
   * @param sToCompare:   Search this string at the end of this String
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String ends with sToCompare, otherwise false
   */
  bool endsWith(const CcString& sToCompare, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Convert string into a unsigned int 32bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  uint64 toUint64(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a unsigned int 32bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  uint32 toUint32(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a unsigned int 16bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  uint16 toUint16(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a unsigned int 8bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  uint8 toUint8(bool* pbOk = nullptr, uint8 uiBase = 10)const;

  /**
   * @brief Convert string into a int 64bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  int64 toInt64(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a int 32bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  int32 toInt32(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a int 16bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  int16 toInt16(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert string into a int 8bit
   * @param pbOk: is set to true if conversion was successfully, otherwise false
   * @param uiBase: Set base of string of supported values 10 for decimal or 16 for hexadecimal
   * @return returns the converted value, or 0 if conversion fails
   */
  int8 toInt8(bool* pbOk = nullptr, uint8 uiBase = 10) const;

  /**
   * @brief Convert current stored string value to float value.
   * @param pbOk: Set a boolean pointer here to get error in conversion.
   * @return Float value or 0.0 if failed
   */
  float toFloat(bool* pbOk = nullptr) const;

  /**
   * @brief Convert current stored string value to float value with double precision.
   * @param pbOk: Set a boolean pointer here to get error in conversion.
   * @return Double value or 0.0 if failed
   */
  double toDouble(bool* pbOk = nullptr) const;

  /**
   * @brief Check if String contains the following values for true:
   *          - TRUE
   *          - ON
   *          - >0
   *        Values for false are:
   *          - TRUE
   *          - OFF
   *          - 0
   *        All comparision will be done case insensitive.
   * @param[out] pbOk:  optional, default is null
   *                    If not null, the value will be set to true if string was recognized
   *                    as a boolean value.
   * @return If bool string is parsed successfully, the correct value will be set as return value,
   *         otherwise false will returend as default value;
   */
  bool toBool(bool* pbOk = nullptr) const;

  /**
   * @brief Convert current stored string to complete uppercase
   * @return reference to this
   */
  CcString& toUpper();

  /**
   * @brief Convert current stored string to complete uppercase
   * @return reference to this
   */
  CcString getUpper() const;

  /**
   * @brief Convert current stored string to complete lowercase
   * @return reference to this
   */
  CcString& toLower();

  /**
   * @brief Convert current stored string to complete lowercase
   * @return reference to this
   */
  CcString getLower() const;

  /**
   * @brief Append a CcString
   * @param pToAppend: String to append to existing String
   */
  CcString& append(const CcString& pToAppend);

  /**
   * @brief Append a single Character
   * @param pToAppend: null terminated char array;
   */
  CcString& append(const char pToAppend);

  /**
   * @brief Append a array of character which is \0 terminated
   * @param pToAppend: null terminated char array
   */
  CcString& append(const char* pToAppend);

  /**
   * @brief Append a array of character with specific size
   * @param pToAppend: char array to add
   * @param length:   Number of characters at pToAppend to add
   */
  CcString& append(const char *pToAppend, size_t length);

  /**
   * @brief Append a single Character
   * @param pToAppend: null terminated char array;
   */
  CcString& appendWchar(const wchar_t pToAppend);

  /**
   * @brief Append a array of character which is \0 terminated
   * @param pToAppend:  Char array to add
   */
  CcString& appendWchar(const wchar_t* pToAppend);

  /**
   * @brief Append a array of character with specific size
   * @param pToAppend:  Char array to add
   * @param uiLength:   Number of characters at pToAppend to add
   */
  CcString& appendWchar(const wchar_t* pToAppend, size_t uiLength);

  /**
   * @brief Append a char array to string extracted from a ByteArray
   * @param pToAppend: null terminated char array;
   */
  CcString& append(const CcByteArray &pToAppend, size_t pos = 0, size_t length = SIZE_MAX);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int64 number, uint8 uiBase = 10);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(float number, uint8 uiPrecision = 10, bool bDisableExponent = false);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(double number, uint8 uiPrecision = 10, bool bDisableExponent = false);

  /**
   * @brief Append a signed number of size_t to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendSize(size_t number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendInt(int number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendUint(uint number, uint8 uiBase = 10);

#ifdef WINDOWS
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  inline CcString appendNumber(int number, uint8 uiBase = 10)
  {
    return appendInt(number, uiBase);
  }

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  inline CcString appendNumber(uint number, uint8 uiBase = 10)
  {
    return appendUint(number, uiBase);
  }
#endif

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @return handle to this string
   */
  CcString& setNumber(int64 number, uint8 uiBase = 10);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return handle to this string
   */
  CcString& setNumber(float number, uint8 uiPrecision, bool bDisableExponent);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return handle to this string
   */
  CcString& setNumber(double number, uint8 uiPrecision, bool bDisableExponent);

  /**
   * @brief Write a size_t value to string
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setSize(size_t number, uint8 uiBase = 10);

  /**
   * @brief Write a int value to string
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setInt(int number, uint8 uiBase = 10);

  /**
   * @brief Write a uint value to string
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  CcString& setUint(uint number, uint8 uiBase = 10);

#ifdef WINDOWS
  /**
   * @brief Write a int value to string
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  inline CcString setNumber(int number, uint8 uiBase = 10)
  {
    return setNumber(static_cast<int32>(number), uiBase);
  }

  /**
   * @brief Write a uint value to string
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return handle to this string
   */
  inline CcString setNumber(uint number, uint8 uiBase = 10)
  {
    return setNumber(static_cast<uint32>(number), uiBase);
  }
#endif
  /**
   * @brief Set a CcString
   * @param toSet: String to set to existing String
   * @return Handle to this
   */
  CcString& set(const CcString& toSet);

  /**
   * @brief Set a sincle Character
   * @param toSet: null terminated char array;
   * @return Handle to this
   */
  CcString& set(const char toSet);

  /**
   * @brief Set a char String
   * @param toSet: null terminated char array;
   * @return Handle to this
   */
  CcString& set(const char* toSet);

  /**
   * @brief Set a sincle Character
   * @param toSet: null terminated char array;
   * @return Handle to this
   */
  CcString& set(const char *toSet, size_t length);

  /**
   * @brief Set single character as string
   * @param toSet: null terminated char array;
   * @return Handle to this
   */
  CcString& setWchar(const wchar_t toSet);

  /**
   * @brief Set character array as string which is \0 terminated.
   * @param toSet: null terminated char array
   * @return Handle to this
   */
  CcString& setWchar(const wchar_t* str);

  CcString& setWchar(const wchar_t* str, size_t length);

  /**
   * @brief Set a sincle Character
   * @param toSet: null terminated char array;
   */
  CcString& set(const CcByteArray &toSet, size_t pos = 0, size_t length = SIZE_MAX);

  /**
   * @brief prepend a CcString
   * @param toprepend: String to prepend to existing String
   * @return Handle to this
   */
  CcString& prepend(const CcString& pToAppend);

  /**
   * @brief prepend a char String
   * @param pToAppend: null terminated char array;
   * @return Handle to this
   */
  CcString& prepend(const char* pToAppend);

  /**
   * @brief prepend a sincle Character
   * @param pToAppend: null terminated char array;
   * @return Handle to this
   */
  CcString& prepend(const char pToAppend);

  /**
   * @brief prepend a sincle Character
   * @param pToAppend: null terminated char array;
   * @return Handle to this
   */
  CcString& prepend(const char* pToAppend, size_t length);

  /**
   * @brief Prepend a array of characters to string, extracted from ByteArray.
   * @param pToAppend: ByteArray to get characters from
   * @param pos: First characater in ByteArray to add;
   * @param length: Number of characters to add.
   * @return Handle to this
   */
  CcString& prepend(const CcByteArray& pToAppend, size_t pos = 0, size_t length = SIZE_MAX);

  /**
   * @brief prepend a std String
   * @param pToAppend: null terminated char array;
   * @return Handle to this
   */
  CcString& appendIp(const CcIp& ipAddr);

  CcString& setOsPath(const CcString& sPathToSet);

  /**
   * @brief Insert string at specific location
   * @param pos: Target location of first character
   * @param pcToInsert: Source array of characters
   * @param uiLength: Number of characters from array to insert
   * @return Handle to this
   */
  CcString& insert(size_t pos, const char* pcToInsert, size_t uiLength);

  /**
   * @brief Insert string at specific location
   * @param pos: Target location of first character
   * @param toInsert: String to insert
   * @return Handle to this
   */
  CcString& insert(size_t pos, const CcString& toInsert);

  /**
   * @brief Get Next position of an not Whitespace Character in String
   * @param offset: Start-Position for searching
   * @return Position of next not Whitespace char, or SIZE_MAX if not found
   */
  size_t posNextNotWhitespace(size_t offset = 0) const;

  /**
   * @brief Get Next position of the next Whitespace Character in String
   * @param offset: Start-Position for searching
   * @return Position of next Whitespace char, or SIZE_MAX if not found
   */
  size_t posNextWhitespace(size_t offset = 0) const;

  /**
   * @brief Get char at position
   * @param pos: Position of target
   * @return char at pos
   */
  inline char& at(size_t pos) const
    { return m_pBuffer[pos]; }

  //! @return Get Length of String
  size_t length() const
  { return m_uiLength; }

  //! @return Get size of reserved buffer for string.
  size_t getBufferSize() const
  { return m_uiReserved;}

  //! @return Get Length of String
  size_t size() const
  { return m_uiLength; }

  //! @return Get pointer to internal const buffer
  inline const char* getCharString() const
  { return m_pBuffer; }

  //! @return Get pointer to internal buffer
  inline char* getCharString()
  { return m_pBuffer; }

  //! @return Get last character of this string. You have to check size of string before.
  inline char& last() const
    { return at(length()-1);}

  /**
   * @brief Lock memory so it will be forced to keep in RAM and not swaped out.
   */
  void mlock();

  /**
   * @brief Unlock memory so it will become back the ability to be swaped out.
   */
  void munlock();

  /**
   * @brief Empty String
   */
  void clear();

  /**
   * @brief Overwirte current Buffer 0 and empty string.
   */
  void clearSave();

  /**
   * @brief Erase a part of this String
   * @param pos: Start-Position to delete from
   *          default: 0
   * @param len:  amount of elements to delete from String
   *          default: SIZE_MAX (end of string)
   * @return reference to this String
   */
  CcString &erase(size_t pos = 0, size_t len = SIZE_MAX);

  /**
   * @brief Get an CcByteArray from String
   * @return CcByteArray with content of String
   */
  CcByteArray getByteArray() const;

  /**
   * @brief Get OS specific path. On Windows / will be replaced with \
   * @return Path converted to match OS characteristic
   */
  CcString getOsPath() const;

  /**
   * @brief Split String by a delimiter. Delimiter will be excluded from String in List.
   * @param delimiter:  String to search for and split at.
   * @param bKeepEmpty: On false, empty strings will not be added to list.
   * @return List of Strings
   */
  CcStringList split(const CcString& delimiter, bool bKeepEmpty = true) const;

  /**
   * @brief Split string in specific sizes.
   * @param uiNumber: Number of characters to split string.
   * @return List of Strings
   */
  CcStringList splitEvery(size_t uiNumber) const;

  /**
   * @brief Split string on every \n found in string.
   * @param bKeepEmptyLines: On false, empty strings will not be added to list.
   * @return List of Strings
   */
  CcStringList splitLines(bool bKeepEmptyLines = true) const;

  CcString& fromLatin1(const char* cString, size_t uiLength);
  inline CcString& fromLatin1(const CcString& sString)
    { return CcString::fromLatin1(sString.getCharString(), sString.length()); }
  CcString getLatin1() const;
  CcString& fromUnicode(const wchar_t* cString, size_t uiLength);
  CcString& fromUnicode(const CcWString& sString);
  CcString& fromUtf16(const uint16* cString, size_t uiLength);
  CcWString getWString() const;

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromNumber(int64 number, uint8 uiBase = 10);

  /**
   * @brief Generate a string of float.
   * @param number:           value to generate
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return handle to this string
   */
  static CcString fromNumber(float number, uint8 uiPrecision = 10, bool bDisableExponent = false);

  /**
   * @brief Generate a string of float of double precision.
   * @param number:           value to generate
   * @param uiPrecision:      Target precision of float write to string
   * @param bDisableExponent: Disable exponent usage, use full value based on precision.
   * @return handle to this string
   */
  static CcString fromNumber(double number, uint8 uiPrecision = 10, bool bDisableExponent = false);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromSize(size_t number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromInt(int number, uint8 uiBase = 10);

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  static CcString fromUint(uint number, uint8 uiBase = 10);
#ifdef WINDOWS
  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  inline static CcString fromNumber(int number, uint8 uiBase = 10)
  { return fromNumber(static_cast<int32>(number),uiBase);}

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return Generated String
   */
  inline static CcString fromNumber(uint number, uint8 uiBase = 10)
  { return fromNumber(static_cast<uint32>(number),uiBase);}
#endif

  /**
   * @brief Remove all // ../ and ./ from Path
   * @return reference to this String
   */
  CcString &normalizePath();

  /**
   * @brief Extract a Filename of a Path
   *        Filname will be recognized as String from last
   *        occurence of / until end of String
   * @return Filname as String
   */
  CcString extractFilename() const;

  /**
   * @brief Extract a Path from String
   *        Path will be discoverd through last occurency of '/'.
   *        String is ending with /
   * @return Filname as String
   */
  CcString extractPath();

  /**
   * @brief Append a Path to current String
   * @param pToAppend: Path to Append
   * @return reference to this String
   */
  CcString& appendPath(const CcString& pToAppend);

  CcString& fillBegin(const CcString& sFillString, size_t uiCount);
  CcString& fillEnd(const CcString& sFillString, size_t uiCount);
  CcString& fillEndUpToLength(const CcString& sFillString, size_t uiCount);
  CcString& fillBeginUpToLength(const CcString& sFillString, size_t uiCount);

  CcString& trimL();
  CcString& trimR();
  inline CcString& trim()
    { return trimL().trimR(); }

  void transfer(char* pData, size_t uiCount);
  void extract(char*& pData, size_t& uiCount, size_t& uiReserved);

  CcString getTrimL() const
    { return CcString(*this).trimL(); }
  CcString getTrimR() const
    { return CcString(*this).trimR(); }
  CcString getTrim() const
    { return CcString(*this).trimR().trimL(); }

  inline const char& operator[](size_t pos) const
    { return at(pos); }
  inline char& operator[](size_t pos)
    { return at(pos); }
  inline CcString &operator+=(const CcString& toAdd)
    { return append(toAdd); }
  inline CcString &operator+=(const char toAdd)
    { return append(toAdd); }
  inline CcString &operator+=(const char* toAdd)
    { return append(toAdd); }

  friend CcString operator+(const char* sLeft, const CcString& toAdd)
    { CcString sRet(sLeft); return sRet.append(toAdd); }
  CcString operator+(const CcString& toAdd) const
    { return CcString(*this).append(toAdd); }
  CcString operator+(const char* toAdd) const
    { return CcString(*this).append(toAdd); }
  CcString operator+(char* toAdd) const
    { return CcString(*this).append(toAdd); }
  CcString operator+(const char toAdd) const
    { return CcString(*this).append(toAdd); }

  inline CcString& operator<<(const CcString& toAdd)
    { return append(toAdd); }
  inline CcString& operator<<(const char *toAdd)
    { return append(toAdd); }
  inline CcString& operator=(char *toAdd)
    { return set(toAdd); }
  inline CcString& operator=(const char *assign)
    { return set(assign); }
  inline CcString& operator=(const CcByteArray& assign)
    { return set(assign); }
  inline CcString& operator=(const char cAssign)
    { return set(cAssign); }

  /**
   * @brief Compare two items
   * @param sToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const CcString& sToCompare) const
    { return compare(sToCompare); }

  /**
   * @brief Compare two items
   * @param sToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  inline bool operator==(const char* sToCompare) const
    { return compare(sToCompare); }

  /**
   * @brief Compare two items
   * @param sToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  inline bool operator!=(const CcString& sToCompare) const
    { return !compare(sToCompare); }
  bool operator<(const CcString& toCompare);
  bool operator>(const CcString& toCompare);

  CcString& operator=(CcString&& oToMove) NOEXCEPT;
  CcString& operator=(const CcString& sToCopy);
  CcString& operator=(CcByteArray&& oToMove) NOEXCEPT;
#ifdef WINDOWS
public:

  inline const char*    getLPCSTR() const
  { return getCharString(); }
  inline char *         getLPSTR()
  { return getCharString(); }
#endif

private: // methods
  void allocateBuffer(size_t uiLength);
  void deleteBuffer();

private: // member
  char* m_pBuffer       = nullptr; //
  size_t   m_uiLength   = 0;       //
  size_t   m_uiReserved = 0;       //
  static const size_t c_uiDefaultMultiplier;
};

#define CcConstString_H(NAME) \
  const CcString NAME
#define CcConstString_C(NAME,STRING) \
  const CcString NAME(const_cast<char*>(STRING))

#define CcConstStringClass_H(NAME) \
  static const CcString NAME
#define CcConstStringClass_C(NAME,STRING,CLASS) \
  const CcString CLASS::NAME(const_cast<char*>(STRING),sizeof(STRING)-1,0)

inline bool operator==(const char* pcL, const CcString& sR)
{
  return sR.compare(pcL);
}

inline bool operator!=(const char* pcL, const CcString& sR)
{
  return !sR.compare(pcL);
}
