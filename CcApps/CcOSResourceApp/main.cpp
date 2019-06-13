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

void printHelp ()
{
  CcConsole::writeLine("Usage: CcOSResource [Options] -i [input file] -o [output file, without .h .c] -n [name of var]");
  CcConsole::writeLine("Options:");
  CcConsole::writeLine("  -a        Append to File, not overwrite as default set");
  CcConsole::writeLine("  -s        String mode to generate a const char* source");
  CcConsole::writeLine("");
  CcConsole::writeLine("For example, jquery will be transfered in source file like this:");
  CcConsole::writeLine("  CcOSResource.exe -i jquery-3.4.1.min.js -o jquery-3.4.1.min.js -n g_Jquery_3_4_1_Min");
}

void writeHexMode(CcFile& oInputFile, CcFile& oOutputFile, CcFile& oOutputHeader, const CcString& sResourceName)
{
  oOutputFile.writeLine("// Resource file generated from CcOSResource");
  oOutputFile.writeLine("#include \"CcBase.h\"");
  oOutputFile.writeLine("CCEXTERNC const unsigned char " + sResourceName + "[] = {");
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
        oOutputFile.writeString(CcGlobalStrings::EolOs);
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
  oOutputFile.writeLine("CCEXTERNC const char* " + sResourceName + " = \"\\");
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
            if (uiLastTransferPos + 1 < uiLastTransfer)
            {
              if (pTransferBuffer[uiLastTransferPos + 1] == '\n')
              {
                uiLastTransferPos++;
                uiAllSize++;
                oOutputFile.write("\\r\\n\\", 5);
                oOutputFile.writeString(CcGlobalStrings::EolOs);
              }
              else
              {
                oOutputFile.write("\\r\\", 3);
                oOutputFile.writeString(CcGlobalStrings::EolOs);
              }
            }
            else
            {
              char cTemp;
              if (1 == oInputFile.read(&cTemp, 1))
              {
                if (cTemp == '\n')
                {
                  uiAllSize++;
                  oOutputFile.write("\\r\\n\\", 5);
                  oOutputFile.writeString(CcGlobalStrings::EolOs);
                }
                else
                {
                  oOutputFile.write("\\r\\", 3);
                  oOutputFile.writeString(CcGlobalStrings::EolOs);
                }
              }
              else
              {
                oOutputFile.write("\\r\\", 3);
                oOutputFile.writeString(CcGlobalStrings::EolOs);
              }
            }
          }
          else if (pTransferBuffer[uiLastTransferPos] == '\n' && bLastWasReturn == false)
          {
            oOutputFile.write("\\n\\", 3);
            oOutputFile.writeString(CcGlobalStrings::EolOs);
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
  oOutputFile.writeLine("CCEXTERNC size_t " + sResourceName + "_Length = " + CcString::fromNumber(uiAllSize) + "; ");
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
    CcFile oOutputHeader(sOutputFile + ".h");
    if (oOutputHeader.open(eOpenMode))
    {
      CcFile oOutputFile(sOutputFile + ".cpp");
      if (oOutputFile.open(eOpenMode))
      {
        CcFile oInputFile(sInputFile);
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
        CcConsole::writeLine("Failed to open output file: " + sInputFile + ".c");
      }

      oOutputHeader.close();;
    }
    else
    {
      CcConsole::writeLine("Failed to open output header: " + sInputFile + ".h");
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
