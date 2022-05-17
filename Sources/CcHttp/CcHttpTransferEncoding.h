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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpTransferEncoding
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"

class CcString;

/**
 * @brief Transfer encoding informations
 */
class CcHttpSHARED CcHttpTransferEncoding
{
public:
  CcHttpTransferEncoding() = default;
  ~CcHttpTransferEncoding() = default;

  /**
   * @brief Create with existing flags
   * @param uiFlag: Flags to import
   */
  CcHttpTransferEncoding(uint32 uiFlag) : m_uiFlags(uiFlag)
  {}

  /**
   * @brief Parse line of header for import flags
   * @param sData: Line to parse
   */
  void parseLine(const CcString& sData);

  /**
   * @brief Parse value, without Key, for importing flags
   * @param sData: Value to parse
   */
  void parseValue(const CcString& sData);

  //! @return Generated line from flags
  CcString getLine();
  //! @return Generated value from flags
  CcString getValue();
  //! @return Get current flags
  uint32 getFlags() const
  { return m_uiFlags; }
  //! @return True if at least one flag is set
  bool hasFlags() const
  { return m_uiFlags > 0; }
  //! @return True if chunked transfer is set
  bool isChunked() const
  { return IS_FLAG_SET( m_uiFlags, Chunked); }
  //! @return True if compressed transfer is set
  bool isCompressed() const
  { return IS_FLAG_SET( m_uiFlags, Compress); }
  //! @return True if deflate transfer is set
  bool isDeflate() const
  { return IS_FLAG_SET( m_uiFlags, Deflate); }
  //! @return True if gzip transfer is set
  bool isGzip() const
  { return IS_FLAG_SET( m_uiFlags, Gzip); }
  //! @return True if identity transfer is set
  bool isIdentity() const
  { return IS_FLAG_SET( m_uiFlags, Identity); }

  //! @param uiFlag: At this flag to currently stored flags
  void addFlag(uint32 uiFlag);
  //! @param uiFlag: Replace current flags wit this
  void setFlag(uint32 uiFlag);

public:
  static const uint32 Normal;   //!< Flag for normal transfer
  static const uint32 Chunked;  //!< Flag for chunked transfer
  static const uint32 Compress; //!< Flag for compressed transfer
  static const uint32 Deflate;  //!< Flag for deflated transfer
  static const uint32 Gzip;     //!< Flag for gzip transfer
  static const uint32 Identity; //!< Flag for identity transfer
private:
  uint32 m_uiFlags = Normal;
};
