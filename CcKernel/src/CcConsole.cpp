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
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Implementation of Class CcConsole
 */
#include "CcConsole.h"
#include "CcStdIn.h"
#include "CcStdOut.h"
#include "CcString.h"
#include "CcArray.h"
#include "CcByteArray.h"
#include "CcIODevice.h"

CcIODevice* CcConsole::s_Input = new CcStdIn();
CcIODevice* CcConsole::s_Output = new CcStdOut();

size_t CcConsole::read(char* buffer, size_t size)
{
  if (s_Input != nullptr)
    return s_Input->read(buffer, size);
  else
    return ~static_cast<size_t>(0);
}

size_t CcConsole::write(const char* buffer, size_t size)
{
  if (s_Input != nullptr)
    return s_Output->write(buffer, size);
  else
    return ~static_cast<size_t>(0);
}

size_t CcConsole::readArray(CcByteArray& oOutputBuffer)
{
  size_t uiReceived = read(oOutputBuffer.getArray(), oOutputBuffer.size());
  if (uiReceived <= oOutputBuffer.size())
    oOutputBuffer.resize(uiReceived);
  else
    oOutputBuffer.resize(0);
  return uiReceived;
}

CcByteArray CcConsole::readAll(size_t uiBufSize)
{
  CcByteArray oReturn;
  CcArray<char> oBuffer(uiBufSize);
  size_t uiReceived = 0;
  size_t uiReceivedAll = 0;
  uiReceived = read(oBuffer.address(), uiBufSize);
  while (uiReceived > 0 && uiReceived != SIZE_MAX)
  {
    uiReceivedAll += uiReceived;
    oReturn.append(oBuffer.address(), uiReceived);
    if (uiReceivedAll < uiBufSize)
      break;
    else
      uiReceived = read(oBuffer.address(), uiBufSize - uiReceivedAll);
  }
  return oReturn;
}

CcString CcConsole::readLine()
{
  CcByteArray oArray = s_Input->readAll();
  if (oArray.last() == '\n')
    oArray.remove(oArray.size() - 1);
  if (oArray.last() == '\r')
    oArray.remove(oArray.size() - 1);
  return CcString(oArray);
}

void CcConsole::writeLine(const CcString& sOutput)
{
  s_Output->write(sOutput.getCharString(), sOutput.length());
  s_Output->write("\n", 1);
}

void CcConsole::writeArray(const CcByteArray& oArray)
{
  size_t uiDataSend = 0;
  while (uiDataSend < oArray.size())
  {
    size_t uiDataLeft = oArray.size() - uiDataSend;
    size_t uiLastSend = write(oArray.getArray(uiDataSend), uiDataLeft);
    if (uiLastSend == SIZE_MAX)
    {
      break;
    }
    else
    {
      uiDataSend += uiLastSend;
    }
  }
}

void CcConsole::writeString(const CcString& oArray)
{
  size_t uiDataSend = 0;
  while (uiDataSend < oArray.length())
  {
    size_t uiDataLeft = oArray.length() - uiDataSend;
    size_t uiLastSend = write(oArray.getCharString() + uiDataSend, uiDataLeft);
    if (uiLastSend == SIZE_MAX)
    {
      break;
    }
    else
    {
      uiDataSend += uiLastSend;
    }
  }
}
