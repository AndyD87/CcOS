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
 * @subpage   CcHttpProvider
 *
 * @page      CcHttpProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpProvider
 */
#ifndef _CcHttpProvider_H_
#define _CcHttpProvider_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcRegEx.h"
#include "Network/CcSocket.h"
#include "CcHttpResponse.h"
#include "CcHttpRequest.h"

/**
 * @brief Basic Webpage-Provider for registering Webpages on Server
 */
class CcHttpSHARED CcHttpProvider
{
public:
  /**
   * @brief Constructor
   */
  CcHttpProvider( const CcString& sPath = "");

  /**
   * @brief Destructor
   */
  virtual ~CcHttpProvider();

  virtual CcHttpResponse execGet(CcHttpRequest &Data);
  virtual CcHttpResponse execPost(CcHttpRequest &Data);
  virtual CcHttpResponse execHead(CcHttpRequest &Data);

  CcHttpResponse &getHeader();
  void setOutput(CcSocket *Socket);
  void setPath(const CcString& sPath);
  CcString getPath();
  bool pregMatch(const CcString& toCompare);
  bool write(CcByteArray &data);
  bool writeHeader();

private:
  CcString  m_sPath;
  CcSocket *m_Output;
  CcHttpResponse m_Header;
};

#endif /* _CcHttpProvider_H_ */
