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
 * @subpage   CcUCString
 *
 * @page      CcUCString
 * @copyright Andreas Dirmeier (C) 2016
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcUCString
 **/
#ifndef CcUCString_H_
#define CcUCString_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

/**
 * @brief Unicode String class
 *        This class is only available to keep external methods, wich requires Unicode, are supported.
 *        You should not use this class for interacting with Framework.
 */
class CcKernelSHARED CcUCString {
public:
  /**
   * @brief Constructor
   */
  CcUCString( void );

  /**
   * @brief CopyConstructor
   */
  CcUCString( const CcUCString& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcUCString( CcUCString&& oToMove );

  /**
   * @brief Constructor
   */
  CcUCString(const CcString& sString);
  
  /**
   * @brief Constructor
   */
  CcUCString(const wchar_t* wcString, size_t uiLength );
  
  /**
   * @brief Constructor
   */
  CcUCString(size_t uiLength, wchar_t wcInitValue = 0 );

  /**
   * @brief Destructor
   */
  virtual ~CcUCString( void );

  CcUCString& operator=(const CcUCString& oToCopy);
  CcUCString& operator=(CcUCString&& oToMove);
  bool operator==(const CcUCString& oToCompare) const;
  bool operator!=(const CcUCString& oToCompare) const;
  
  void clear();
  void clearAndReserve(size_t uiLength);
  void resize(size_t uiLength);

  CcUCString& append(wchar_t wcSingle);
  CcUCString& append(const wchar_t* wcString, size_t uiLength);

  CcUCString& fromString(const CcString& oString);
  size_t length() const
    { return m_uiLength; }
  
  CcString getString() const;
  inline wchar_t* getCharString(void)
    { return m_pBuffer; }
  inline const wchar_t* getCharString(void) const
    { return m_pBuffer; }
  inline const wchar_t* getLPCWSTR(void) const
    { return m_pBuffer; }
  inline wchar_t* getLPWSTR(void)
    { return m_pBuffer; }

private:
  void reserve(size_t uiLength);
  void reserveByLength(size_t uiLength);

private:
  wchar_t* m_pBuffer    = nullptr;
  size_t   m_uiLength   = 0;
  size_t   m_uiReserved = 0;
};

#endif /* CcUCString_H_ */
