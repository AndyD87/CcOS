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
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcWString
 *            ## Comparing to CcString
 *            Other than CcString, CcWString will store all data in wchar_t buffer array.
 *            For common ascii chars, it will cost the double of space.
 **/
#ifndef CcWString_H_
#define CcWString_H_

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
  CcWString( void );

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
  CcWString(const wchar_t* wcString, size_t uiLength );
  
  /**
   * @brief Constructor
   */
  CcWString(size_t uiLength, wchar_t wcInitValue = 0 );

  /**
   * @brief Destructor
   */
  ~CcWString( void );

  CcWString& operator=(const CcWString& oToCopy);
  CcWString& operator=(CcWString&& oToMove);
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

#ifdef WIN32
  inline CcWString appendNumber(long number)
  {
    return appendNumber(static_cast<int32>(number));
  }
  inline CcWString appendNumber(unsigned long number)
  {
    return appendNumber(static_cast<uint32>(number));
  }
#endif


  CcWString& fromString(const char* wcString, size_t uiLength);
  CcWString& fromString(const CcString& oString);
  size_t length() const
    { return m_uiLength; }
  
  CcString getString() const;
  inline wchar_t* getWcharString(void)
    { return m_pBuffer; }
  inline const wchar_t* getWcharString(void) const
    { return m_pBuffer; }
  inline const wchar_t* getLPCWSTR(void) const
    { return m_pBuffer; }
  inline wchar_t* getLPWSTR(void)
    { return m_pBuffer; }

private:
  void reserve(size_t uiLength);
  void deleteBuffer();

private:
  wchar_t* m_pBuffer    = nullptr;
  size_t   m_uiLength   = 0;
  size_t   m_uiReserved = 0;
};

#endif /* CcWString_H_ */
