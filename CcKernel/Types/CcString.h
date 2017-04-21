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
 * @page      CcString CcString
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief      Class CcString
 */
#ifndef CCSTRING_H_
#define CCSTRING_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcTypes.h"
#include <string>

/// Forward Declarations
class CcStringList;
class CcByteArray;
class CcUCString;

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
public: //methods
  /**
   * @brief Create a empty string-class.
   */
  CcString();

  /**
   * @brief Create a empty string-class.
   */
  CcString(const CcString& oToCopy);
  
  /**
   * @brief Create a empty string-class.
   */
  CcString(CcString&& oToMove)
  {
    operator=(std::move(oToMove));
  }

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

  CcString(wchar_t* wstr);

  CcString(wchar_t* wstr, size_t uiLength);

  /**
   * @brief Create a String-class with an initialized string of variable length
   * @param cString:  pointer to char array to be inserted
   * @param uiLength: Size of char-string to be stored in class
   */
  CcString(const char cString);

  /**
   * @brief Import a ByteArray as String.
   * @param baString: ByteArray to import as String
   * @param uiLength: Size of char-string to be stored in class
   */
  CcString(const CcByteArray& baString);

  /**
   * @brief Clean up and free all requested Memory
   */
  ~CcString();

  CcString& format(const char* sFormat, ...);

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
   * @param needle: String to find in Haystack
   * @param replac: String replaces the needle;
   * @return Needle, or "" if failed
   */
  CcString replace(const CcString& needle, const CcString& replace) const;

  /**
   * @brief Get String between two strings
   * @param preStr: String to find
   * @param posStr: first String to find after position of preStr
   * @param offset: offset for begin of searching, default 0
   * @param[out] pos: position of found string if required
   * @return Needle, or "" if failed
   */
  CcString getStringBetween(const CcString& preStr, const CcString& postStr, size_t offset = 0, size_t *pos = NULL) const;

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
   * @brief Check if String starts with a specific value
   * @param sToCompare: Search this string at the beginning of this String
   * @return true if String starts with sToCompare, otherwise false
   */
  bool startWith(const CcString& sToCompare, size_t offset = 0) const;

  /**
  * @brief Check if String ends with a specific value
  * @param sToCompare: Search this string at the end of this String
  * @return true if String ends with sToCompare, otherwise false
  */
  bool endWith(const CcString& sToCompare, size_t offset = 0) const;
  
  /**
   * @brief Convert string into a unsigned int 32bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  uint64 toUint64(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a unsigned int 32bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  uint32 toUint32(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a unsigned int 16bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  uint16 toUint16(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a unsigned int 8bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  uint8 toUint8(bool* bOk = NULL)const;
  
  /**
   * @brief Convert string into a int 64bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  int64 toInt64(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a int 32bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  int32 toInt32(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a int 16bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  int16 toInt16(bool* bOk = NULL) const;

  /**
   * @brief Convert string into a int 8bit
   * @param bOk: is set to true if conversion was successfully, otherwise false
   * @return returns the converted value, or 0 if conversion fails
   */
  int8 toInt8(bool* bOk = NULL) const;

  float toFloat(bool* bOk = NULL) const;
  double toDouble(bool* bOk = NULL) const;

  /**
   * @brief Convert current stored string to complete uppercase
   * @return reference to this
   */
  CcString& toUpper(void);

  /**
   * @brief Convert current stored string to complete lowercase
   * @return reference to this
   */
  CcString& toLower(void);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int8 number, uint8 uiBase = 10);
  
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int16 number, uint8 uiBase = 10);
  
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int32 number, uint8 uiBase = 10);
  
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int64 number, uint8 uiBase = 10);

#ifdef WIN32
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(uint number, uint8 uiBase = 10);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(int number, uint8 uiBase = 10);
#endif

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(double number);

  /**
   * @brief Compare a String with content if they are the same
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcString& appendNumber(float number);

  /**
   * @brief Append a CcString
   * @param toAppend: String to append to existing String
   */
  CcString& append(const CcString& toAppend);

  /**
   * @brief Append a char String
   * @param toAppend: null terminated char array;
   */
  CcString& append(const char* toAppend);

  /**
   * @brief Append a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& append(const char toAppend);

  /**
   * @brief Append a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& append(const char *toAppend, size_t length);

  CcString& append(const wchar_t* str);

  CcString& append(const wchar_t* str, size_t length);

  /**
   * @brief Append a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& append(const CcByteArray &toAppend, size_t pos = 0, size_t length = SIZE_MAX);

  /**
   * @brief prepend a CcString
   * @param toprepend: String to prepend to existing String
   */
  CcString& prepend(const CcString& toAppend);

  /**
   * @brief prepend a char String
   * @param toAppend: null terminated char array;
   */
  CcString& prepend(const char* toAppend);

  /**
   * @brief prepend a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& prepend(const char toAppend);

  /**
   * @brief prepend a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& prepend(const char* toAppend, size_t length);

  /**
   * @brief prepend a sincle Character
   * @param toAppend: null terminated char array;
   */
  CcString& prepend(const CcByteArray& toAppend, size_t pos = 0, size_t length = SIZE_MAX);

  /**
   * @brief prepend a std String
   * @param toAppend: null terminated char array;
   */
  CcString& appendIPv4(const ipv4_t& ipAddr);
  
  /**
   * @brief Set new Strint.
   * @param sToSet: String that has to replace current string.
   */
  CcString& set(const CcString& sToSet);

  CcString& setOsPath(const CcString& sPathToSet);

  /**
   * @brief Append a std String
   * @param toAppend: null terminated char array;
   */
  CcString& insert(size_t pos, const CcString& toInsert);

  /**
   * @brief Get Length of String
   * @return String-lenth
   */
  size_t length( void ) const;

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
  char& at(size_t pos) const;
  
  /**
   * @brief Get char at position
   * @param pos: Position of target
   * @return char at pos
   */
  inline char& last() const
    { return at(length()-1);}

  /**
   * @brief Empty String
   */
  void clear( void );

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
   * @brief Get a Standard char string of content
   * @return standard c char array with content of String
   */
  const char* getCharString(void) const;
  
  /**
   * @brief Get a Standard char string of content
   * @return standard c char array with content of String
   */
  char* getCharString(void);

  /**
   * @brief Get an CcByteArray from String
   * @return CcByteArray with content of String
   */
  CcByteArray getByteArray(void) const;

  CcString getOsPath(void) const;

  /**
   * @brief Split String by a delimiter. Delimiter will be excluded from String in List.
   * @param delimiter: String to search for and split at.
   * @return List of Strings
   */
  CcStringList split(const CcString& delimiter) const;
  
  /**
   * @brief Split String by a delimiter. Delimiter will be excluded from String in List.
   * @param delimiter: String to search for and split at.
   * @return List of Strings
   */
  CcStringList splitEvery(size_t uiNumber) const;

  /**
  * @brief Split String by a delimiter. Delimiter will be excluded from String in List.
  * @param delimiter: String to search for and split at.
  * @return List of Strings
  */
  CcStringList splitLines() const;
  
  CcString& fromLatin1(const char* cString, size_t uiLength);
  inline CcString& fromLatin1(const CcString& sString)
    { return CcString::fromLatin1(sString.getCharString(), sString.length()); }
  CcString getLatin1() const;
  CcString& fromUnicode(const wchar_t* cString, size_t uiLength);
  CcString& fromUnicode(const CcUCString& sString);
  CcUCString getUnicode() const;

  /**
   * @brief Generate a String from Number
   * @param number: number to convert to string
   * @return Generated String
   * @{
   */
  static CcString fromNumber(uint8 number, uint8 uiBase = 10);
  static CcString fromNumber(uint16 number, uint8 uiBase = 10);
  static CcString fromNumber(uint32 number, uint8 uiBase = 10);
  static CcString fromNumber(uint64 number, uint8 uiBase = 10);
  static CcString fromNumber(int8 number, uint8 uiBase = 10);
  static CcString fromNumber(int16 number, uint8 uiBase = 10);
  static CcString fromNumber(int32 number, uint8 uiBase = 10);
  static CcString fromNumber(int64 number, uint8 uiBase = 10);
#ifdef WIN32
  static CcString fromNumber(int number, uint8 uiBase = 10);
  static CcString fromNumber(uint number, uint8 uiBase = 10);
#endif
  static CcString fromNumber(float number);
  static CcString fromNumber(double number);
  ///@}

  /**
   * @brief Remove multiple slashes of Path
   * @return reference to this String
   */
  CcString &normalizePath(void);

  /**
   * @brief Extract a Filename of a Path
   *        Filname will be recognized as String from last
   *        occurence of / until end of String
   * @return Filname as String
   */
  CcString extractFilename(void) const;

  /**
   * @brief Extract a Path from String
   *        Path will be discoverd through last occurency of '/'.
   *        String is ending with /
   * @return Filname as String
   */
  CcString extractPath(void);

  /**
   * @brief Append a Path to current String
   * @param toAppend: Path to Append
   * @return reference to this String
   */
  CcString& appendPath(const CcString& toAppend);

  CcString& fillBegin(const CcString& sFillString, size_t uiCount);
  CcString& fillEnd(const CcString& sFillString, size_t uiCount);
  CcString& fillEndUpToLength(const CcString& sFillString, size_t uiCount);
  CcString& fillBeginUpToLength(const CcString& sFillString, size_t uiCount);

  CcString trimL(void) const;
  CcString trimR(void) const;
  inline CcString trim(void) const
    { return trimL().trimR(); }

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
   * @return true if they are the same, otherwis false
   */
  inline bool operator==(const CcString& sToCompare) const
    { return compare(sToCompare); }

  /**
   * @brief Compare two items
   * @param sToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  inline bool operator!=(const CcString& sToCompare) const
    { return !compare(sToCompare); }
  bool operator<(const CcString& toCompare);
  bool operator>(const CcString& toCompare);

  inline CcString& operator=(const CcString& assign)
    { return set(assign); }

  CcString& operator=(CcString&& oToMove);
#ifdef WIN32
public:

  inline const char*    getLPCSTR(void) const
  { return getCharString(); }
  inline char *         getLPSTR(void)
  { return getCharString(); }
#endif

private:
  /**
   * @brief check if global settings for strings is set to utf8
   */
  static bool s_bIsUtf8;

  /**
   * @brief set global locale to utf9
   *        It is getting called on initiation, and is setting @ref s_bIsUtf8 to true
   */
  static bool utf8(const char* locale);

private:
  std::string *m_String = nullptr;
};

#endif /* CCSTRING_H_ */
