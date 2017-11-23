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
 * @subpage   CcStringUtil
 *
 * @page      CcStringUtil
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcStringUtil
 */
#ifndef CcStringUtil_H_
#define CcStringUtil_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcString.h"

/// Forward Declarations
class CcByteArray;

/**
 * @brief Static Class for String functions
 */
class CcKernelSHARED  CcStringUtil {
public:
  static size_t strlen(const char* pcString, size_t uiMaxLen = SIZE_MAX);
  static char* strchr(char* pcString, char cToFind);
  static bool isWhiteSpace(const char toTest);
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
  static CcString getOctalStringFromByte(char uiByte);
  static CcString encodeBase64(const CcByteArray& toEncode);
  static CcByteArray decodeBase64(const CcString& toDecode);
  static CcString encodeBase58(const CcByteArray& toEncode);
  static CcByteArray decodeBase58(const CcString& toDecode);
  static CcString getFilenameFromPath(const CcString& sPath);
  static uint64 toUint64(const char* pcString, size_t uiLen, bool* pbOk = nullptr);
  static uint32 toUint32(const char* pcString, size_t uiLen, bool* pbOk = nullptr);

private:
  /**
   * @brief Constructor
   */
  CcStringUtil(void){}

  static CcString encodeBaseX(const CcByteArray& toEncode, const char* pcAlphabet, uint8 uiBaseSize);
  static CcByteArray decodeBaseX(const CcString& toDecode, const char* pcAlphabet, uint8 uiBaseSize);

  static char getBase64EncodedStringChar(char cIn);
  static char getBase64DecodedStringChar(char cIn);
};

#endif /* CcStringUtil_H_ */
