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
 * @subpage   CcWString
 *
 * @page      CcWString
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcWString
 *            ## Comparing to CcString
 *            Other than CcString, CcWString will store all data in wchar_t buffer array.
 *            For common ascii chars, it will cost the double of space.
 **/
#ifndef H_CcWString_H_
#define H_CcWString_H_

#include "CcBase.h"
#include "CcKernelBase.h"

// forward declarations
class CcString;

/**
 * @brief Unicode String class
 *        This class is only available to keep external methods, wich requires Unicode, are supported.
 *        You should not use this class for interacting with Framework.
 */
class CcKernelSHARED CcWString {
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

  CcWString& operator=(CcWString&& oToMove);
  CcWString& operator=(const CcWString& oToCopy);
  bool operator==(const CcWString& oToCompare) const;
  bool operator!=(const CcWString& oToCompare) const;
  
  void clear();
  void clearAndReserve(size_t uiLength);
  void resize(size_t uiLength);

  CcWString& append(const CcWString& sString);
  CcWString& append(wchar_t wcSingle);
  CcWString& append(const wchar_t* wcString);
  CcWString& append(const wchar_t* wcString, size_t uiLength);
  
  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int8 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int16 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(int32 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
   * @return true if conversion was successful, otherwise false
   */
  CcWString& appendNumber(uint64 number, uint8 uiBase = 10);

  /**
   * @brief Append a signed Number to String
   * @param number: value to add
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
  inline CcWString appendNumber(long number)
  {
    return appendNumber(static_cast<int32>(number));
  }
  inline CcWString appendNumber(unsigned long number)
  {
    return appendNumber(static_cast<uint32>(number));
  }
#endif

  CcWString& set(const CcWString& sString);
  CcWString& set(wchar_t wcSingle);
  CcWString& set(const wchar_t* wcString);
  CcWString& set(const wchar_t* wcString, size_t uiLength);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(uint8 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(int8 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(uint16 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(int16 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(uint32 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(int32 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
  * @return true if conversion was successful, otherwise false
  */
  CcWString& setNumber(uint64 number, uint8 uiBase = 10);

  /**
  * @brief Set a signed Number to String
  * @param number: value to add
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
  inline CcWString setNumber(long number)
  {
    return setNumber(static_cast<int32>(number));
  }
  inline CcWString setNumber(unsigned long number)
  {
    return setNumber(static_cast<uint32>(number));
  }
#endif


  CcWString& fromString(const char* wcString, size_t uiLength);
  CcWString& fromString(const CcString& oString);
  size_t length() const
    { return m_uiLength; }

  /**
   * @brief Get Length of String
   * @return String-lenth
   */
  size_t size() const
    { return m_uiLength * sizeof(wchar_t); }

  
  CcString getString() const;
  inline wchar_t* getWcharString()
    { return m_pBuffer; }
  inline const wchar_t* getWcharString() const
    { return m_pBuffer; }
  inline const wchar_t* getLPCWSTR() const
    { return m_pBuffer; }
  inline wchar_t* getLPWSTR()
    { return m_pBuffer; }

private:
  void reserve(size_t uiLength);
  void deleteBuffer();

private:
  wchar_t* m_pBuffer    = nullptr;
  size_t   m_uiLength   = 0;
  size_t   m_uiReserved = 0;
};

#endif // H_CcWString_H_
