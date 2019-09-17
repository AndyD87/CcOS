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
 * @page      CcHttp
 * @subpage   CcHttpTransferEncoding
 *
 * @page      CcHttpTransferEncoding
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpTransferEncoding
 */
#ifndef H_CcHttpTransferEncoding_H_
#define H_CcHttpTransferEncoding_H_

#include "CcBase.h"
#include "CcHttp.h"

class CcString;

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpTransferEncoding
{
public:
  CcHttpTransferEncoding() = default;

  CcHttpTransferEncoding(uint32 uiFlag) : m_uiFlags(uiFlag)
    {}

  /**
   * @brief Destructor
   */
  ~CcHttpTransferEncoding() = default;

  bool isChunked() const
    { return IS_FLAG_SET( m_uiFlags, Chunked); }
  bool isCompressed() const
    { return IS_FLAG_SET( m_uiFlags, Compress); }
  bool isDeflate() const
    { return IS_FLAG_SET( m_uiFlags, Deflate); }
  bool isGzip() const
    { return IS_FLAG_SET( m_uiFlags, Gzip); }
  bool isIdentity() const
    { return IS_FLAG_SET( m_uiFlags, Identity); }

  void setFlag(uint32 uiFlag);
  void addFlag(uint32 uiFlag);
  bool hasFlags()
    { return m_uiFlags > 0; }

  void parseLine(const CcString& sData);
  CcString getLine();

public:
  static const uint32 Normal;
  static const uint32 Chunked;
  static const uint32 Compress;
  static const uint32 Deflate;
  static const uint32 Gzip;
  static const uint32 Identity;
private:
  uint32 m_uiFlags = 0;
};

#endif // H_CcHttpTransferEncoding_H_
