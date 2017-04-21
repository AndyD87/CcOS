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
 * @subpage   CcByteArray
 *
 * @page      CcByteArray
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcByteArray
 **/
#ifndef CcByteArray_H_
#define CcByteArray_H_

#include "CcKernelBase.h"
#include "CcVector.h"

#ifdef WIN32
template CcKernelSHARED class CcVector<char>;
#endif

class CcString;
class CcCrc32;

/**
 * @brief Dynamic Storage for char's
 */
class CcKernelSHARED CcByteArray : public CcVector<char>
{
public:
  /**
   * @brief Constructor wich can append a 0 terminated char array
   * @param toAppend: pointer to constant/0 terminiated char array
   */
  CcByteArray(void);
  CcByteArray(const char* toAppend);
  CcByteArray(const char* toAppend, size_t uiLen);
  CcByteArray(const CcByteArray& oToAppend);
  CcByteArray(CcByteArray&& oToMove);
  CcByteArray(const CcString& sToAppend);
  CcByteArray(size_t uiReserverd);

  /**
   * @brief Destructor
   */
  virtual ~CcByteArray();

  /**
   * @brief check if item is allready added to List
   * @return true if list contains item, otherwise false
   */
  size_t find(const CcString& toFind) const;

  /**
   * @brief Write a part of stored chars to buffer
   * @param buf:     buffter to write chars to
   * @param bufSize: maximum buffer Size
   * @param from:    set startpoint for charray in storage
   * @return count of written chars to buffer
   */
  size_t getCharArray(char *buf, size_t bufSize, size_t from = 0);

  bool setHexString(const CcString& sHexString);
  CcString getHexString()const;

  CcString getOctalString();

  CcCrc32 getCrc32();

  void swapEndian();

  CcByteArray& operator=(const CcByteArray& oToCopy);
  CcByteArray& operator=(CcByteArray&& oToCopy);
};

#endif /* CcByteArray_H_ */
