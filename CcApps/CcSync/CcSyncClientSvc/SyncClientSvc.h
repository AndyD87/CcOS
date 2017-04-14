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
 * @page      SyncClientSvc
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class SyncClientSvc
 *
 *  Example GUI-Application with Menue Structure and Application Loader
 */
#ifndef SyncClientSvc_H_
#define SyncClientSvc_H_

#include "CcBase.h"
#include "CcApp.h"
#include "CcSyncClient.h"
#include "CcArguments.h"

class SyncClientSvc: public CcApp {
public:
  SyncClientSvc(const CcArguments& oArguments);
  virtual ~SyncClientSvc();

  void run() override;

private:
  CcArguments m_oArguments;
  CcSyncClient* m_poSyncClient = NULL;
};

#endif /* SyncClientSvc_H_ */
