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
 * @brief     Implementation of Class CcConsole
 */
#include "CcConsole.h"
#include "CcStdIn.h"
#include "CcStdOut.h"
#include "CcString.h"
#include "CcArray.h"
#include "CcByteArray.h"
#include "IIoDevice.h"
#include "CcGlobalStrings.h"

CcStdIn* CcConsole::s_Input = new CcStdIn();
CcStdOut* CcConsole::s_Output = new CcStdOut();

size_t CcConsole::read(void* pBuffer, size_t uSize)
{
  if (s_Input != nullptr)
    return s_Input->read(pBuffer, uSize);
  else
    return ~static_cast<size_t>(0);
}

size_t CcConsole::write(const void* pBuffer, size_t uSize)
{
  if (s_Output != nullptr)
    return s_Output->write(pBuffer, uSize);
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
  uiReceived = read(oBuffer.getArray(), uiBufSize);
  while (uiReceived > 0 && uiReceived != SIZE_MAX)
  {
    uiReceivedAll += uiReceived;
    oReturn.append(oBuffer.getArray(), uiReceived);
    if (uiReceivedAll < uiBufSize)
      break;
    else
      uiReceived = read(oBuffer.getArray(), uiBufSize - uiReceivedAll);
  }
  return oReturn;
}

CcString CcConsole::readLine()
{
  CcString sReturn;
  CcArray<char> oBuffer(256);
  size_t uiReceived = 0;
  size_t uiReceivedAll = 0;
  uiReceived = read(oBuffer.getArray(), 256);
  while (uiReceived > 0 && uiReceived != SIZE_MAX)
  {
    uiReceivedAll += uiReceived;
    sReturn.append(oBuffer.getArray(), uiReceived);
    if (sReturn.contains(CcGlobalStrings::EolShort))
    {
      size_t uiPos = sReturn.find(CcGlobalStrings::EolShort);
      if (uiPos < sReturn.length())
      {
        sReturn.remove(uiPos, sReturn.length());
      }
      break;
    }
    else
      uiReceived = read(oBuffer.getArray(), 256);
  }
  return sReturn;
}

CcString CcConsole::readLineHidden()
{
  CcArray<char> oBuffer(1024); // @todo magic number
  CcString sData;
  size_t uiReceived = 0;
  do
  {
    uiReceived = s_Input->readHidden(oBuffer.getArray(), 1024); // @todo magic number
    if (uiReceived < oBuffer.size())
    {
      sData.append(oBuffer.getArray(), uiReceived);
    }
  } while (uiReceived == 1024);// @todo magic number
  if (sData.length() > 0 && sData.last() == '\n')
  {
    sData.remove(sData.length() - 1);
    writeString(CcGlobalStrings::EolOs);
  }
  if (sData.length() > 0 && sData.last() == '\r')
    sData.remove(sData.length() - 1);
  return sData;
}

void CcConsole::writeLine(const CcString& sOutput)
{
  if(s_Output != nullptr)
    s_Output->writeLine(sOutput);
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

void CcConsole::disableBuffering()
{
  CcStdIn::disableBuffer();
  CcStdOut::disableBuffer();
}
