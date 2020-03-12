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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief    Development default CLI-Application for testing new Implementations
 */

#include "CcBase.h"
#include "CcKernel.h"
#include "CcConsole.h"
#include "Types/CcArguments.h"
#include "CcFile.h"
#include "CcGlobalStrings.h"

#define TRANSFER_SIZE 10240
#define LINE_SIZE 32

bool g_bStringMode = false;
bool g_bAppendMode = false;
bool g_bAlwaysOverwrite = true;
bool g_bOsNewLine = false;

void printHelp ()
{
  CcConsole::writeLine("Usage: CcOSResource [Options] -i [input file] -o [output file, without .h .c] -n [name of var]");
  CcConsole::writeLine("Options:");
  CcConsole::writeLine("  -a        Append to File, not overwrite as default set");
  CcConsole::writeLine("  -s        String mode to generate a const char* source");
  CcConsole::writeLine("  -os       Newline in string should depend on os");
  CcConsole::writeLine("  -ow       Overwrite existing files only if changed");
  CcConsole::writeLine("");
  CcConsole::writeLine("For example, javascript lib will be transfered in source file like this:");
  CcConsole::writeLine("  CcOSResource.exe -i lib.min.js -o lib.min.js -n g_LibMin");
}

const CcString& getEol()
{
  // If required we can add an additional flag to change output file format here
  return CcGlobalStrings::EolOs;
}

CcString getEolText()
{
  if (g_bOsNewLine)
  {
    if (CcGlobalStrings::EolOs == CcGlobalStrings::EolShort)
    {
      return "\\n";
    }
    else if (CcGlobalStrings::EolOs == CcGlobalStrings::EolLong)
    {
      return "\\r\\n";
    }
    else
    {
      return "\\n";
    }
  }
  else
  {
    return "\\n";
  }
}

void writeHexMode(CcFile& oInputFile, CcFile& oOutputFile, CcFile& oOutputHeader, const CcString& sResourceName)
{
  oOutputFile.writeLine("// Resource file generated from CcOSResource");
  oOutputFile.writeLine("#include \"CcBase.h\"");
  oOutputFile.writeLine("const unsigned char " + sResourceName + "[] = {");
  unsigned char pTransferBuffer[TRANSFER_SIZE];
  size_t uiLastTransfer = 0;
  size_t uiAllSize = 0;
  do
  {
    uiLastTransfer = oInputFile.read(pTransferBuffer, TRANSFER_SIZE);
    if (uiLastTransfer != 0 && uiLastTransfer <= TRANSFER_SIZE)
    {
      size_t uiLastTransferPos = 0;
      while (uiLastTransferPos < uiLastTransfer)
      {
        for (int j = 0; j < LINE_SIZE && uiLastTransferPos < uiLastTransfer; j++, uiAllSize++, uiLastTransferPos++)
        {
          if (uiAllSize != 0)
          {
            oOutputFile.write(", ", 2);
          }
          oOutputFile.write("0x", 2);
          oOutputFile.writeString(CcString::fromNumber(pTransferBuffer[uiLastTransferPos], 16));
        }
        oOutputFile.writeString(getEol());
      }
    }
  } while (uiLastTransfer == TRANSFER_SIZE);
  oOutputFile.writeLine("};");
  oOutputFile.writeLine("CCEXTERNC size_t " + sResourceName + "_Size = " + CcString::fromNumber(uiAllSize) + "; ");
  oInputFile.close();;

  oOutputHeader.writeLine("// Resource file generated from CcOSResource");
  oOutputHeader.writeLine("#include \"CcBase.h\"");
  oOutputHeader.writeLine("CCEXTERNC const unsigned char " + sResourceName + "[" + CcString::fromNumber(uiAllSize) + "];");
  oOutputHeader.writeLine("CCEXTERNC size_t " + sResourceName + "_Size; ");
}

void writeStringMode(CcFile& oInputFile, CcFile& oOutputFile, CcFile& oOutputHeader, const CcString& sResourceName)
{
  oOutputFile.writeLine("// Resource file generated from CcOSResource");
  oOutputFile.writeLine("#include \"CcBase.h\"");
  oOutputFile.writeLine("const char* " + sResourceName + " = \"\\");
  char pTransferBuffer[TRANSFER_SIZE];
  size_t uiLastTransfer = 0;
  size_t uiAllSize = 0;
  do
  {
    uiLastTransfer = oInputFile.read(pTransferBuffer, TRANSFER_SIZE);
    if (uiLastTransfer != 0 && uiLastTransfer <= TRANSFER_SIZE)
    {
      size_t uiLastTransferPos = 0;
      bool bLastWasReturn = false;
      while (uiLastTransferPos < uiLastTransfer)
      {
        for (int j = 0; j < LINE_SIZE && uiLastTransferPos < uiLastTransfer; j++, uiAllSize++, uiLastTransferPos++)
        {
          if (pTransferBuffer[uiLastTransferPos] == '"')
          {
            bLastWasReturn = false;
            oOutputFile.write("\\\"", 2);
          }
          else if (pTransferBuffer[uiLastTransferPos] == '\\')
          {
            bLastWasReturn = false;
            oOutputFile.write("\\\\", 2);
          }
          else if (pTransferBuffer[uiLastTransferPos] == '\r')
          {
            bLastWasReturn = true;
            uiAllSize += (getEolText().length() / 2) - 1;
            oOutputFile.writeString(getEolText());
            oOutputFile.write("\\", 1);
            oOutputFile.writeString(getEol());
          }
          else if (pTransferBuffer[uiLastTransferPos] == '\n' && bLastWasReturn == true)
          {
            uiAllSize--;
            bLastWasReturn = false;
          }
          else if (pTransferBuffer[uiLastTransferPos] == '\n')
          {
            uiAllSize += (getEolText().length() / 2) - 1;
            oOutputFile.writeString(getEolText());
            oOutputFile.write("\\", 1);
            oOutputFile.writeString(getEol());
          }
          else
          {
            bLastWasReturn = false;
            oOutputFile.write(pTransferBuffer + uiLastTransferPos, 1);
          }
        }
      }
    }
  } while (uiLastTransfer == TRANSFER_SIZE);
  oOutputFile.writeLine("\";");
  oOutputFile.writeLine("size_t " + sResourceName + "_Length = " + CcString::fromNumber(uiAllSize) + "; ");
  oInputFile.close();

  oOutputHeader.writeLine("// Resource file generated from CcOSResource");
  oOutputHeader.writeLine("#include \"CcBase.h\"");
  oOutputHeader.writeLine("CCEXTERNC const char* " + sResourceName + ";");
  oOutputHeader.writeLine("CCEXTERNC size_t " + sResourceName + "_Length; ");
}

int run(const CcString& sInputFile, const CcString& sOutputFile, const CcString& sResourceName)
{
  EOpenFlags eOpenMode = EOpenFlags::Overwrite;
  if(g_bAppendMode) eOpenMode = EOpenFlags::Append;
  int iResult = -1;
  if (CcFile::exists(sInputFile))
  {
    CcString sOutputFilePathH(sOutputFile + ".h");
    CcString sOutputFilePathC(sOutputFile + ".c");
    CcString sOutputFilePathTempH(sOutputFilePathH);
    CcString sOutputFilePathTempC(sOutputFilePathC);
    while ( CcFile::exists(sOutputFilePathTempH) ||
            CcFile::exists(sOutputFilePathTempC))
    {
      sOutputFilePathTempH += ".tmp";
      sOutputFilePathTempC += ".tmp";
    }
    CcFile oOutputHeader(sOutputFilePathTempH);
    CcFile oOutputFile(sOutputFilePathTempC);
    CcFile oInputFile(sInputFile);
    if (oOutputHeader.open(eOpenMode))
    {
      if (oOutputFile.open(eOpenMode))
      {
        if (oInputFile.open(EOpenFlags::Read))
        {
          iResult = 0;
          if (g_bStringMode == false)
          {
            writeHexMode(oInputFile, oOutputFile, oOutputHeader, sResourceName);
          }
          else
          {
            writeStringMode(oInputFile, oOutputFile, oOutputHeader, sResourceName);
          }
        }
        else
        {
          CcConsole::writeLine("Failed to open input file: " + sInputFile);
        }
        oOutputFile.close();;
      }
      else
      {
        CcConsole::writeLine("Failed to open output file: " + sOutputFilePathTempC);
      }
      oOutputHeader.close();
    }
    else
    {
      CcConsole::writeLine("Failed to open output header: " + sOutputFilePathTempH);
    }
    if (oOutputHeader.exists() && oOutputFile.exists())
    {
      if (sOutputFilePathTempC != sOutputFilePathC)
      {
        if (g_bAlwaysOverwrite == false)
        {
          if (CcFile::compare(sOutputFilePathTempH, sOutputFilePathH))
          {
            CcFile::remove(sOutputFilePathTempH);
            CcFile::remove(sOutputFilePathTempC);
          }
          else
          {
            CcFile::remove(sOutputFilePathH);
            CcFile::remove(sOutputFilePathC);
            CcDateTime oMaxTimout = CcKernel::getUpTime();
            oMaxTimout.addSeconds(2);
            while (!CcFile::move(sOutputFilePathTempH, sOutputFilePathH) && oMaxTimout > CcKernel::getUpTime()) CcKernel::sleep(100);
            while (!CcFile::move(sOutputFilePathTempC, sOutputFilePathC) && oMaxTimout > CcKernel::getUpTime()) CcKernel::sleep(100);
          }
        }
        else
        {
          CcFile::remove(sOutputFilePathH);
          CcFile::remove(sOutputFilePathC);
          CcDateTime oMaxTimout = CcKernel::getUpTime();
          oMaxTimout.addSeconds(2);
          while (!CcFile::move(sOutputFilePathTempH, sOutputFilePathH) && oMaxTimout > CcKernel::getUpTime()) CcKernel::sleep(100);
          while (!CcFile::move(sOutputFilePathTempC, sOutputFilePathC) && oMaxTimout > CcKernel::getUpTime()) CcKernel::sleep(100);
        }
      }
    }
  }
  else
  {
    CcConsole::writeLine("Input file does not exist: " + sInputFile);
  }
  return iResult;
}

int main(int argc, char **argv)
{
  int iRet = 0;
  CcArguments oArguments(argc, argv);
  CcKernel::initCLI();

  CcString sOutputFile;
  CcString sInputFile;
  CcString sResourceName;
  if (oArguments.size() > 1)
  {
    for (size_t uiArgument = 1; uiArgument < oArguments.size(); uiArgument++)
    {
      if (oArguments[uiArgument] == "-h")
      {
        printHelp();
      }
      else if (oArguments[uiArgument] == "-a")
      {
        g_bAppendMode = true;
      }
      else if (oArguments[uiArgument] == "-s")
      {
        g_bStringMode = true;
      }
      else if (oArguments[uiArgument] == "-o")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sOutputFile = oArguments[uiArgument+1];
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-ow")
      {
        g_bAlwaysOverwrite = false;
      }
      else if (oArguments[uiArgument] == "-i")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sInputFile = oArguments[uiArgument + 1];
          if (sOutputFile.length() == 0)
          {
            sOutputFile = sInputFile;
          }
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-n")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sResourceName = oArguments[uiArgument + 1];
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-os")
      {
        g_bOsNewLine = true;
      }
      else
      {
        printHelp();
      }
    }
    iRet = run(sInputFile, sOutputFile, sResourceName);
  }
  else
  {
    printHelp();
  }

  return iRet;
}
