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

void printHelp ()
{
  CcConsole::writeLine("Usage: CcOSRessource [Options] -i [input file] -o [output file, without .h .c] -n [name of var]");
  CcConsole::writeLine("Options:");
  CcConsole::writeLine("  -a        Append to File, not overwrite as default set");
  CcConsole::writeLine("");
  CcConsole::writeLine("For example, jquery will be transfered in source file like this:");
  CcConsole::writeLine("  CcOSResource.exe -i jquery-3.4.1.min.js -o jquery-3.4.1.min.js -n g_Jquery_3_4_1_Min");
}

int run(const CcString& sInputFile, const CcString& sOutputFile, const CcString& sRessourceName, bool bAppend = false)
{
  EOpenFlags eOpenMode = EOpenFlags::Overwrite;
  if(bAppend) eOpenMode = EOpenFlags::Append;
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
          oOutputFile.writeLine("// Resource file generated from CcOSResource");
          oOutputFile.writeLine("#include \"CcBase.h\"");
          oOutputFile.writeLine("unsigned char " + sRessourceName + "[] = {");
          unsigned char pTransferBuffer[TRANSFER_SIZE];
          size_t uiLastTransfer = 0;
          size_t uiAllSIze = 0;
          do
          {
            uiLastTransfer = oInputFile.read(pTransferBuffer, TRANSFER_SIZE);
            if (uiLastTransfer != 0 && uiLastTransfer <= TRANSFER_SIZE)
            {
              size_t uiLastTransferPos = 0;
              while (uiLastTransferPos < uiLastTransfer)
              {
                for (int j = 0; j < LINE_SIZE && uiLastTransferPos <= uiLastTransfer; j++, uiAllSIze++, uiLastTransferPos++)
                {
                  if (uiAllSIze != 0)
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
          oOutputFile.writeLine("size_t " + sRessourceName + "_Size = " + CcString::fromNumber(uiAllSIze) + "; ");
          oInputFile.close();;

          oOutputHeader.writeLine("// Resource file generated from CcOSResource");
          oOutputHeader.writeLine("#include \"CcBase.h\"");
          oOutputHeader.writeLine("extern unsigned char " + sRessourceName + "["+CcString::fromNumber(uiAllSIze)+"];");
          oOutputHeader.writeLine("extern size_t " + sRessourceName + "_Size; ");
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
  CcString sRessourceName;
  if (oArguments.size() > 1)
  {
    for (size_t uiArgument = 1; uiArgument < oArguments.size(); uiArgument++)
    {
      if (oArguments[uiArgument] == "-h")
      {
        printHelp();
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
          uiArgument++;
        }
      }
      else if (oArguments[uiArgument] == "-n")
      {
        if (oArguments.size() >= uiArgument + 1)
        {
          sRessourceName = oArguments[uiArgument + 1];
          uiArgument++;
        }
      }
      else
      {
        printHelp();
      }
    }
    iRet = run(sInputFile, sOutputFile, sRessourceName);
  }
  else
  {
    printHelp();
  }

  return iRet;
}
