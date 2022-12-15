/*
 * This file is part of CcScpiCli.
 *
 * CcScpiCli is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcScpiCli is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcScpiCli.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcScpiCli
 */
#include "CcScpiCli.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcScpiCli.h"
#include "CcScpiClient.h"
#include "CcConsole.h"
#include "CcOSVersion.h"

#ifdef WINDOWS
  #include "Shell/CcWinRawConsole.h"
#endif

CcScpiCli::CcScpiCli() :
  CcApp("CcScpiCli", { CCOS_VERSION_COMMA }),
  m_oArguments(
    {
      {"-address",     CcVariant::EType::String, "localhost", "Target device IP Address or host name, default is localhost"},
      {"-port",        CcVariant::EType::Uint16, "5025",      "Target device port, default is 5025"},
      {"-send",        CcVariant::EType::Switch, "",          "Request is send only"},
      {"-query",       CcVariant::EType::Switch, "",          "Request is send and request"},
      {"-timeout",     CcVariant::EType::Uint64, "2000",      "Timeout to wait until query response is received in milliseconds"},      {"-h",           CcVariant::EType::Switch, "",          "Print help message"}
    }
  )
{
}

CcScpiCli::~CcScpiCli()
{
}

void CcScpiCli::run()
{
  CcStatus oStatus;
  if (m_oArguments.getValue("-h").getBool())
  {
    writeHelp();
  }
  else if (m_oArguments.getUnparsed().size())
  {
    CcScpiClient oClient;
    oClient.setTimeout(m_oArguments.getValue("-timeout").getUint64());
    oClient.setServer(m_oArguments.getValue("-address").getString());
    oClient.setPort(m_oArguments.getValue("-port").getString());
    if (oClient.connect())
    {
      for (size_t uiArg = 0; uiArg < m_oArguments.getUnparsed().size(); uiArg++)
      {
        if (m_oArguments.getValue("-send").getBool())
        {
          if (uiArg >= m_oArguments.getUnparsed().size())
          {
            CcConsole::writeLine("[ERR] Query requires an addtional parameter");
            setExitCode(EStatus::CommandInvalidParameter);
          }
          else
          {
            oStatus = oClient.send(m_oArguments.getUnparsed()[uiArg]);
            if (!oStatus)
            {
              CcConsole::writeLine("[ERR] Failed to send data to device");
              setExitCode(EStatus::NetworkError);
            }
            else
            {
              CcConsole::writeLine("Command successfully sent");
            }
          }
        }
        else if (m_oArguments.getValue("-query").getBool())
        {
          if (uiArg >= m_oArguments.getUnparsed().size())
          {
            CcConsole::writeLine("[ERR] query requires an addtional parameter");
            setExitCode(EStatus::CommandInvalidParameter);
          }
          else
          {
            CcString sResult;
            oStatus = oClient.query(m_oArguments.getUnparsed()[uiArg], sResult);
            if (!oStatus)
            {
              CcConsole::writeLine("[ERR] Failed to send data to device");
              setExitCode(EStatus::NetworkError);
            }
            else
            {
              CcConsole::writeLine(sResult);
            }
          }

        }
        else
        {
          CcConsole::writeLine("[ERR] Unknown request type");
          setExitCode(EStatus::CommandInvalidParameter);
        }
      }
    }
    else
    {
      CcConsole::writeLine("[ERR] Failed to connect to device");
      setExitCode(EStatus::NetworkError);
    }
  }
  else
  {
    CcConsole::writeLine("No command set, please refer to manual:");
    oStatus = EStatus::CommandRequiredParameter;
    writeHelp();
  }
  if (getExitCode() && !oStatus)
  {
    setExitCode(oStatus);
  }
  CCVERBOSE("CcScpiCli stopped, code " + CcString::fromNumber(getExitCode().getErrorUint()));
}

CcStatus CcScpiCli::parseArguments(int iArgs, char** pArgs)
{
  return m_oArguments.parse(iArgs, pArgs);
}

void CcScpiCli::writeHelp()
{
  CcConsole::writeLine(getName() + CcGlobalStrings::Seperators::Space + getVersion().getVersionString());
  CcConsole::writeLine("Command:");
  CcConsole::writeLine(getName() + CcGlobalStrings::Seperators::Space + "[Operators] Request");
  CcConsole::writeLine("");
  CcConsole::writeLine("Operators:");
  m_oArguments.writeHelp(CcConsole::getOutStream());
}
