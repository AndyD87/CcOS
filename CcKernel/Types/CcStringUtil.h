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
 * @page      CcStringUtil
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
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
  static bool isWhiteSpace(const char toTest);
  static bool getBoolFromStirng(const CcString& sToParse, bool* pbOk = NULL);
  static CcString getOctalStringFromByte(char uiByte);
  static CcString encodeBase64(const CcByteArray& toEncode);
  static CcByteArray decodeBase64(const CcString& toDecode);
  static CcString encodeBase58(const CcByteArray& toEncode);
  static CcByteArray decodeBase58(const CcString& toDecode);
  static CcString getFilenameFromPath(const CcString& sPath);
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
