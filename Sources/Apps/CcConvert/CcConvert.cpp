/*
 * This file is part of CcConvert.
 *
 * CcConvert is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConvert is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConvert.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implemtation of class CcConvert
 */
#include "CcConvert.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcSystem.h"
#include "CcServiceSystem.h"
#include "CcService.h"
#include "CcConvert.h"
#include "CcConsole.h"
#include "CcOSVersion.h"
#include "CcMessageFormat.h"
#include "CcConvertImage.h"

#ifdef WINDOWS
  #include "Shell/CcWinRawConsole.h"
#endif

CcConvert::CcConvert() :
  CcApp("CcConvert", { CCOS_VERSION_COMMA }),
  m_oArguments(
    {
      {"-input",     CcVariant::EType::String, "", "Input file"},
      {"-output",    CcVariant::EType::String, "", "Output file"},
      {"-type",      CcVariant::EType::String, "", "Select one of the next conversion options:\n"
                                                   " img2pixmap"}
    }
  )
{
}

CcConvert::~CcConvert()
{
}

void CcConvert::run()
{
  CcStatus oStatus;
  if (m_oArguments.getValue("-h").getBool())
  {
    writeHelp();
  }
  else if (m_oArguments.getUnparsed().size() == 0)
  {
    CcString sInput  = m_oArguments.getValue("-input").getString();
    CcString sOutput = m_oArguments.getValue("-output").getString();
    CcString sType   = m_oArguments.getValue("-type").getString();
    if (sInput.length() && sOutput.length() && sType.length())
    {
      if (sType == "img2pixmap")
      {
        CcConvertImage oImage;
        if (oImage.loadFile(sInput))
        {
          CcFile oFile(sOutput);
          if (oFile.open(EOpenFlags::Write))
          {
            oFile.writeArray(oImage.convert(EImageType::Pixmap));
            oFile.close();
            setExitCode(EStatus::NoError);
          }
        }
        else
        {
          CcConsole::writeLine(CcMessageFormat::formatErrorMessage("Input is not a image"));
          setExitCode(EStatus::CommandInvalidParameter);
        }
      }
    }
    else
    {
      CcConsole::writeLine(CcMessageFormat::formatErrorMessage("Missing parameter"));
      setExitCode(EStatus::CommandRequiredParameter);
      writeHelp();
    }
  }
  else
  {
    CcConsole::writeLine(CcMessageFormat::formatErrorMessage("Unknown parameter"));
    oStatus = EStatus::CommandRequiredParameter;
    writeHelp();
  }
  if (getExitCode() && !oStatus)
  {
    setExitCode(oStatus);
  }
  CCVERBOSE("CcConvert stopped, code " + CcString::fromNumber(getExitCode().getErrorUint()));
}

CcStatus CcConvert::parseArguments(int iArgs, char** pArgs)
{
  return m_oArguments.parse(iArgs, pArgs);
}

void CcConvert::writeHelp()
{
  CcConsole::writeLine(getName() + CcGlobalStrings::Seperators::Space + getVersion().getVersionString());
  CcConsole::writeLine("Command:");
  CcConsole::writeLine(getName() + CcGlobalStrings::Seperators::Space + "-input <file> -output <file> -type <convert>");
  CcConsole::writeLine("");
  CcConsole::writeLine("Operators:");
  m_oArguments.writeHelp(CcConsole::getOutStream());
}
