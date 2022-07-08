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
 * @brief     Class CcWString
 *            ## Comparing to CcString
 *            Other than CcString, CcWString will store all data in wchar_t buffer array.
 *            For common ascii chars, it will cost the double of space.
 **/
#pragma once

#include "CcBase.h"
#include "CcString.h"

/**
 * @brief Unicode String class
 *        This class is only available to keep external methods, wich requires Unicode, are supported.
 *        You should not use this class for interacting with Framework.
 */
class CcKernelSHARED CcWString
{
public:
  /**
   * @brief Constructor
   */
  CcWString();

  /**
   * @brief CopyConstructor
   */
  CcWString( const CcWString& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcWString( CcWString&& oToMove );

  /**
   * @brief Constructor
   */
  CcWString(const CcString& sString);
  
  /**
   * @brief Constructor
   */
  CcWString(const char* pcString, size_t uiLength );
  
  /**
   * @brief Constructor
   */
  CcWString(const wchar_t* wcString);
  
  /**
   * @brief Constructor
   */
  CcWString(const wchar_t* wcString, size_t uiLength );
  
  /**
   * @brief Constructor
   */
  CcWString(size_t uiLength, wchar_t wcInitValue = 0 );

  /**
   * @brief Destructor
   */
  ~CcWString();

  /**
   * @brief Move data from ohter WString to this
   * @param oToMove: WString to move data from
   * @return Handle to this
   */
  CcWString& operator=(CcWString&& oToMove);

  /**
   * @brief Copy data from ohter WString to this
   * @param oToCopy: WString to copy data from
   * @return Handle to this
   */
  CcWString& operator=(const CcWString& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcWString& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator!=(const CcWString& oToCompare) const;

  /**
   * @brief Get character on specific offset
   * @param uiPos: Offset to get char from
   * @return Return queried char
   */
  wchar_t& operator[](size_t uiPos)
  { return m_pBuffer[uiPos]; }

  /**
   * @brief Get const character on specific offset
   * @param uiPos: Offset to get char from
   * @return Return queried char
   */
  wchar_t operator[](size_t uiPos) const
  { return m_pBuffer[uiPos]; }
  
  /**
   * @brief Clear string and reset to 0
   */
  void clear();

  /**
   * @brief Remove characters from string.
   * @param uiPos: Position of firs character to remove in string.
   * @param uiLength: Number o characters to be removed. Default 1.
   */
  CcWString& remove(size_t uiPos, size_t uiLength = 1);

  /**
   * @brief Append a CcString
   * @param sString: String to append to existing String
   */
  CcWString& append(const CcWString& sString);

  /**
   * @brief Append a single Character
   * @param wcSingle: null terminated char array;
   */
  CcWString& append(wchar_t wcSingle);

  /**
   * @brief Append a array of character which is \0 terminated
   * @param wcString: null terminated char array
   */
  CcWString& append(const wchar_t* wcString);

  /**
   * @brief Append a array of character with specific size
   * @param wcString: char array to add
   * @param uiLength:   Number of characters at pToAppend to add
   */
  CcWString& append(const wchar_t* wcString, size_t uiLength);
  
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int64 number, uint8 uiBase = 10);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(float number);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(double number);

#ifdef WINDOWS
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  inline CcWString appendNumber(int number)
  {
    return appendNumber(static_cast<int32>(number));
  }

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  inline CcWString appendNumber(uint number)
  {
    return appendNumber(static_cast<uint32>(number));
  }
#endif

  /**
   * @brief Set a CcString
   * @param sString: String to set to existing String
   * @return Handle to this
   */
  CcWString& set(const CcWString& sString);

  /**
   * @brief Set a sincle Character
   * @param wcSingle: null terminated char array;
   * @return Handle to this
   */
  CcWString& set(wchar_t wcSingle);

  /**
   * @brief Set a char String
   * @param wcString: null terminated char array;
   * @return Handle to this
   */
  CcWString& set(const wchar_t* wcString);

  /**
   * @brief Set a sincle Character
   * @param wcString: null terminated char array;
   * @param uiLength: Number of chars from toSet to set;
   * @return Handle to this
   */
  CcWString& set(const wchar_t* wcString, size_t uiLength);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @param uiBase: Target base of number of supported values 10 for decimal or 16 for hexadecimal
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(int64 number, uint8 uiBase = 10);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(float number);

  /**
   * @brief Set a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& setNumber(double number);

#ifdef WINDOWS
  inline CcWString setNumber(int number)
  {
    return setNumber(static_cast<int32>(number));
  }
  inline CcWString setNumber(uint number)
  {
    return setNumber(static_cast<uint32>(number));
  }
#endif

  /**
   * @brief Insert string at specific location
   * @param pos: Target location of first character
   * @param pcToInsert: Source array of characters
   * @param uiLength: Number of characters from array to insert
   * @return Handle to this
   */
  CcWString& insert(size_t pos, const wchar_t* pcToInsert, size_t uiLength);

  /**
   * @brief Insert string at specific location
   * @param pos: Target location of first character
   * @param toInsert: String to insert
   * @return Handle to this
   */
  CcWString& insert(size_t pos, const CcWString& toInsert);

  /**
   * @brief Check if String starts with a specific value.
   *        The start of comparision can be adjusted to a specific position.
   *        It will be compared until end.
   * @param sToCompare:   Search this string at specific position.
   * @param uiOffset:     Position of first character where search should start
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String starts with sToCompare, otherwise false
   */
  bool isStringAtOffset(const CcWString& sToCompare, size_t uiOffset, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Check if String starts with a specific value
   * @param sToCompare:   Search this string at the beginning of this String
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String starts with sToCompare, otherwise false
   */
  bool startsWith(const CcWString& sToCompare, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Check if String ends with a specific value
   * @param sToCompare:   Search this string at the end of this String
   * @param eSensitivity: Sensitivity is default CaseSensitive, but it can be overwritten here
   * @return true if String ends with sToCompare, otherwise false
   */
  bool endsWith(const CcWString& sToCompare, ESensitivity eSensitivity = ESensitivity::CaseSensitiv) const;

  /**
   * @brief Erase a part of this String
   * @param pos: Start-Position to delete from
   *          default: 0
   * @param len:  amount of elements to delete from String
   *          default: SIZE_MAX (end of string)
   * @return reference to this String
   */
  CcWString &erase(size_t pos = 0, size_t len = SIZE_MAX);

  /**
   * @brief Find the position of a occurrencing String
   * @param sToFind: String to search for
   * @param offset: Position where search has to be started at.
   * @return position of First occurrence or SIZE_MAX if not found
   */
  size_t find(const CcWString& sToFind, size_t offset = 0) const;

  /**
   * @brief Find the position of a occurrencing String
   * @param pcString: String to search for
   * @param uiLength: Number of bytes to read from @p pcString
   * @param uiOffset: Position where search has to be started at.
   * @return position of First occurrence or SIZE_MAX if not found
   */
  size_t find(const wchar_t* pcString, size_t uiLength, size_t uiOffset) const;

  /**
   * @brief Get a needle of String stack
   * @param pos: start position
   * @param len: length of string, or 0 until end of String
   * @return Needle, or "" if failed
   */
  CcWString substr(size_t pos, size_t len = SIZE_MAX) const;

  /**
   * @brief Convert current stored string to complete lowercase
   * @return reference to this
   */
  CcWString& toLower();

  /**
   * @brief Convert current stored string to complete lowercase
   * @return reference to this
   */
  CcWString getLower() const;

  /**
   * @brief Replace every needle with other value;
   * @param needle:   String to find in Haystack
   * @param replace:  String replaces the needle;
   * @return Needle, or "" if failed
   */
  CcWString& replace(const CcWString& needle, const CcWString& replace);

  /**
   * @brief Import string from UTF8
   * @param wcString: String as char array
   * @param uiLength: Number character to import
   * @return Handle to this
   */
  CcWString& fromString(const char* wcString, size_t uiLength);

  /**
   * @brief Import string from UTF8
   * @param oString: CcString to import
   * @return Handle to this
   */
  CcWString& fromString(const CcString& oString);

  /**
   * @brief Write string to uint16 array
   * @param pBuffer: Target output buffer
   * @param uiSize:  Size of target output buffer
   * @return Handle to this
   */
  CcWString& toUtf16(uint16* pBuffer, size_t uiSize);

  /**
   * @brief Length of this WString in characters
   * @return Number of characters
   */
  size_t length() const
  { return m_uiLength; }

  /**
   * @brief Get number of Bytes for this string
   * @return Number of bytes used
   */
  size_t size() const
  { return m_uiLength * sizeof(wchar_t); }

  /**
   * @brief Get char at position
   * @param pos: Position of target
   * @return char at pos
   */
  inline wchar_t& at(size_t pos) const
  { return m_pBuffer[pos]; }
  
  /**
   * @brief Export this string to UTF8 formated CcString
   * @return Converted string
   */
  CcString getString() const;

  //! @return Get pointer to internal buffer
  inline wchar_t* getWcharString()
  { return m_pBuffer; }
  //! @return Get pointer to internal const buffer
  inline const wchar_t* getWcharString() const
  { return m_pBuffer; }
  //! @return Get pointer to internal const buffer
  inline const wchar_t* getLPCWSTR() const
  { return m_pBuffer; }
  //! @return Get pointer to internal buffer
  inline wchar_t* getLPWSTR()
  { return m_pBuffer; }

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
  void reserve(size_t uiLength, const wchar_t cDefaultChar);


private:
  void allocateBuffer(size_t uiLength);
  void deleteBuffer();

private:
  wchar_t* m_pBuffer    = nullptr;
  size_t   m_uiLength   = 0;
  size_t   m_uiReserved = 0;
  static const size_t c_uiDefaultMultiplier;
};
