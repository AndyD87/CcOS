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
 * @subpage   CcHttpResponse
 *
 * @page      CcHttpResponse
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpResponse
 */
#ifndef CcHttpResponse_H_
#define CcHttpResponse_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcByteArray.h"

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpResponse
{
public:
  /**
   * @brief Constructor
   */
  CcHttpResponse(const CcString& Parse);

  /**
   * @brief Create a Response Header with possbaly filled data
   * @param init: If true is set or empty, Header will be filled with default information
   *              otherwise if false, no values are set and Header-Info is Blank
   */
  CcHttpResponse(bool init = true);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpResponse(void);

  CcString getHeader();

  void parse(const CcString& Parse);

private:
  void parseLine(const CcString& Parse);
public:
  struct sHeaderData{
    CcString AcceptRanges;
    CcString Allow;
    CcString CacheControl;
    CcString Connection;
    CcString ContentEncoding;
    CcString ContentLength;
    CcString ContentLocation;
    CcString ContentMd5;
    CcString ContentRange;
    CcString ContentType;
    CcString Date;
    CcString HTTP;
    CcString LastModified;
    CcString Location;
    CcString Refresh;
    CcString Server;
    CcString SetCookie;
    CcString TransferEncoding;
  } m_Header;
  struct sContentData
  {
    CcByteArray Content;
  } m_Data;
};

#endif /* CcHttpResponse_H_ */
