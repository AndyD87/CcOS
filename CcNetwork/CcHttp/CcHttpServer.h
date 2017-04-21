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
 * @subpage   CcHttpServer
 *
 * @page      CcHttpServer
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpServer
 */
#ifndef CcHttpServer_H_
#define CcHttpServer_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcHandle.h"
#include "Network/CcSocket.h"
#include "CcByteArray.h"
#include "CcStringList.h"
#include "CcUrl.h"
#include "CcHttpRequest.h"
#include "CcHttpResponse.h"
#include "CcApp.h"
#include "CcHttpServerWorker.h"
#include "CcHttpProvider.h"
#include "CcHttpDefaultProvider.h"

#ifdef WIN32
template class CcHttpSHARED CcList<CcHttpProvider*>;
template class CcHttpSHARED CcHandle<CcHttpProvider>;
template class CcHttpSHARED CcList<CcHandle<CcHttpProvider>>;
#endif

/**
 * @brief Button for GUI Applications
 */
class CcHttpSHARED CcHttpServer : public CcApp
{
public:
  /**
   * @brief Constructor
   */
  CcHttpServer( uint16 Port = 27580 );
  CcHttpServer(const CcStringList &Arg);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpServer( void );

  static CcApp* main(const CcStringList &Arg);

  void run(void) override;

  void setWorkingDir(const CcString& Wd);
  CcString getWorkingDir(void);

  void registerProvider(CcHandle<CcHttpProvider> &toAdd);
  void deregisterProvider(CcHandle<CcHttpProvider> &toRemove);
  const CcHandle<CcHttpProvider> findProvider(const CcString& Path);
  const CcList<CcHandle<CcHttpProvider>>& getReceiverList(void);

private:
  void init();
  void parseConfigHeader();
private:
  uint16    m_Port;
  CcString  m_WD;
  CcSocket *m_Socket;
  CcList<CcHandle<CcHttpProvider>> m_ProviderList;
  CcHandle<CcHttpProvider> m_DefaultProvider;
  CcString m_sConfigFilePath;
};

#endif /* CcHttpServer_H_ */
