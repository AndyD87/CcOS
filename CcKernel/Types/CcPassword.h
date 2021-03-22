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
 * @brief     Class CcPassword
 **/
#ifndef H_CcPassword_H_
#define H_CcPassword_H_

#include "CcBase.h"
#include "CcBase.h"
#include "CcString.h"
#include "Hash/CcHash.h"

/**
 * @brief Password class wich is holding a password as String.
 *        This class can transport the defined password hash type too.
 *
 *        For safety reason, the Password String will be locked in ram and
 *        zeroed if deleted.
 */
class CcKernelSHARED CcPassword
{
public:
  /**
   * @brief Constructor
   */
  CcPassword() = default;
  

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
   *        Initializes Password with a string and Unknown Hash Type
   * @param sPassword: Password as string
   */
  CcPassword(const CcString& sPassword);

  /**
   * @brief Constructor
   *        Initializes Password with a string and Unknown Hash Type
   * @param sPassword: Password as string
   * @param eType: Type of hash algorithm
   */
  CcPassword(const CcString& sPassword, EHashType eType);

  /**
   * @brief Destructor
   */
  ~CcPassword();

  /**
   * @brief Move password content of an CcPassword Object to this.
   * @param oToMove: Old password object to move from
   * @return Handle of this class
   */
  CcPassword& operator=(CcPassword&& oToMove);

  /**
   * @brief Copy password content of an CcPassword Object to this.
   * @param oToCopy: Source password object to copy from
   * @return Handle of this class
   */
  CcPassword& operator=(const CcPassword& oToCopy);

  /**
   * @brief Compare two Passwords.
   * @param oToCompare: Item to compare to
   * @return true if they are the same, otherwise false
   */
  bool operator==(const CcPassword& oToCompare) const;

  /**
   * @brief Compare two items
   * @param oToCompare: Item to compare to
   * @return true if they are not same, otherwise false
   */
  bool operator!=(const CcPassword& oToCompare) const
    { return !operator==(oToCompare);}


  /**
   * @brief Store new password and safely clean old one
   * @param sPassword: New Password to set as string
   * @param eType:     Type of hash algorithm of sPassword, or EHashType::String as default
   */
  void setPassword(const CcString& sPassword, EHashType eType = EHashType::String);

  /**
   * @brief Set Type of password hash algorithm.
   * @param eType: type of hash as enum
   * @return true if eType has a valid value
   */
  bool setType(EHashType eType);

  /**
   * @brief Set Type of password hash algorithm.
   *        Type will be processed as string and translated in to EHashType
   *
   *        Strings will be transfered with @ref getTypeByString
   *        Setting string to "Unknown" will return false;
   * @param sType: Name of target hash type
   * @return true if name was correctly parsed
   */
  bool setType(const CcString& sType);

  /**
   * @brief Get type of current stored type.
   * @return EHashType
   */
  inline EHashType getType() const
    { return m_eType; }

  /**
   * @brief Get currently stored password as string.
   * @return Password as string
   */
  inline const CcString& getString() const
    { return m_sPassword; }

  /**
   * @brief Translate a string into an EHashType.
   *        Valid Names can be found in CcGlobalStrings::Types::Hash
   * @param sType: Name of target hash type
   * @param[out] bConvOk: Output will be set to true if conversion succeeded, otherwise false.
   * @return Converted Hashtype or Unknown if not recognized.
   */
  static EHashType getTypeByString(const CcString& sType, bool& bConvOk);

  /**
   * @brief Translate a string into an EHashType.
   *        Valid Names can be found in CcGlobalStrings::Types::Hash
   * @param sType: Name of target hash type
   * @return Converted Hashtype or Unknown if not recognized.
   */
  inline static EHashType getTypeByString(const CcString& sType)
    { bool bOk; return getTypeByString(sType, bOk);}

  /**
   * @brief Get a name of a Hashtype as string.
   * @param eType: Target hash to translate to string.
   * @return HashType as string, or empty if eType was not valid.
   */
  static CcString getTypeAsString(EHashType eType);

  /**
   * @brief Generate a password, based on eType as hash algorithm.
   *        Binary hasdata will be stored as hex-string.
   *
   *        EHashType::Unknown will simply store the password.
   * @param sPassword:  Password to get hashed.
   * @param eType:      Type of algorithm to use for hash sPassword string.
   * @return EStatus::Success if password was hashed successfully
   *         EStatus::NotSupported if hash algorithm was not found
   *
   */
  CcStatus generatePassword(const CcString& sPassword, EHashType eType = EHashType::Unknown);
private:
  EHashType m_eType   = EHashType::Unknown; //!< Type of stored password.
  CcString m_sPassword;                     //!< Current stored password.
};

#endif // H_CcPassword_H_
