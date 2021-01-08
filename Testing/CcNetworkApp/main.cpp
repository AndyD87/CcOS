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
 * @brief    Entry point for Application
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "NetworkApp.h"
#include "CcArguments.h"
#include "Network/Stack/CcNetworkStack.h"
#include "CcHttpServer.h"
#include "CcConsole.h"

int main(int iArgc, char **ppArgv)
{
  int iError = 0;
  CCUNUSED(iArgc);
  CCUNUSED(ppArgv);
  CcKernel::initCLI();

  CcArguments oArguments(iArgc, ppArgv);
  if (oArguments.size() > 1)
  {
    if (oArguments[1].compareInsensitve("stack"))
    {
      CcNetworkStack oStack;
      oStack.init();
      ISocket* pSocket = oStack.getSocket(ESocketType::TCP);
      CcSocket oSocket(pSocket);
      CcHttpServer oHttpServer(80);
      CcConsole::writeLine("Start Server");
      iError = oHttpServer.exec().getErrorInt();
      if (iError == 0)
      {
        CcConsole::writeLine("Echo Server stopped successfully");
      }
      else
      {
        CcConsole::writeLine("Echo Server stopped with error");
      }
    }
    else
    {
      NetworkApp oNetworkApp;
      iError = oNetworkApp.exec().getErrorInt();
    }
  }
  else
  {
    NetworkApp oNetworkApp;
    iError = oNetworkApp.exec().getErrorInt();
  }
  return iError;
}
