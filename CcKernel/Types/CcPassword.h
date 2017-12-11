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
 * @subpage   CcPassword
 *
 * @page      CcPassword
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcPassword
 **/
#ifndef CcPassword_H_
#define CcPassword_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

enum class EPasswordType
{
  ClearText,
  SHA256,
  SHA512, // @todo not yet suported 
  MD5,    // @todo not yet suported 
};

/**
 * @brief Class impelmentation
 */
class CcKernelSHARED CcPassword {
public:
  /**
   * @brief Constructor
   */
  CcPassword( void );
  

  /**
   * @brief CopyConstructor
   */
  CcPassword( const CcPassword& oToCopy );

  /**
   * @brief MoveConstructor
   */
  CcPassword( CcPassword&& oToMove );

  /**
   * @brief Constructor
   */
  CcPassword(const CcString& sPassword);

  /**
   * @brief Constructor
   */
  CcPassword(const CcString& sPassword, EPasswordType eType);

  /**
   * @brief Destructor
   */
  ~CcPassword( void );

  void setPassword(const CcString& sPassword, EPasswordType eType = EPasswordType::ClearText);
  bool setType(EPasswordType eType);
  bool setType(const CcString& sType);


  inline EPasswordType getType() const
    { return m_eType; }
  inline const CcString& getString() const
    { return m_sPassword; }

  static EPasswordType getTypeByString(const CcString& sType, bool& bConvOk);
  static EPasswordType getTypeByString(const CcString& sType)
    { bool bOk; return getTypeByString(sType, bOk);}
  static CcString getTypeAsString(EPasswordType eType);

  CcPassword& operator=(CcPassword&& oToMove);
  CcPassword& operator=(const CcPassword& oToCopy);

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwis false
   */
  bool operator==(const CcPassword& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwis false
   */
  bool operator!=(const CcPassword& oToCompare) const
    { return !operator==(oToCompare);}

private:
  EPasswordType m_eType = EPasswordType::ClearText;
  CcString m_sPassword;
};

#endif /* CcPassword_H_ */
