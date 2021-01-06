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
#include "IIo.h"
#include "CcGlobalStrings.h"
#include "CcMutex.h"

CcMutex*    CcConsole::s_pLock(nullptr);
CcStdIn*    CcConsole::s_pInput(nullptr);
CcStdOut*   CcConsole::s_pOutput(nullptr);

void CcConsole::init()
{
  CCNEW(s_pInput, CcStdIn);
  CCNEW(s_pOutput, CcStdOut);
  CCNEW(s_pLock, CcMutex);
}

void CcConsole::deinit()
{
  // Try to lock, some is possibly accessing it
  if(s_pLock)
  {
    s_pLock->lock();
    CCDELETE(s_pInput);
    CCDELETE(s_pOutput);
    s_pLock->unlock();
    CCDELETE(s_pLock);
  }
}

void CcConsole::setInputDevice(CcStdIn* pInDev)
{
  s_pInput = pInDev;
}

void CcConsole::setOutputDevice(CcStdOut* pOutDev)
{
  s_pOutput = pOutDev;
}

size_t CcConsole::read(void* pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if (s_pInput != nullptr)
  {
    s_pLock->lock();
    uiRead = s_pInput->read(pBuffer, uSize);
    s_pLock->unlock();
  }
  return uiRead;
}

size_t CcConsole::write(const void* pBuffer, size_t uSize)
{
  size_t uiWritten = SIZE_MAX;
  if (s_pOutput != nullptr)
  {
    s_pLock->lock();
    uiWritten = s_pOutput->write(pBuffer, uSize);
    s_pLock->unlock();
  }
  return uiWritten;
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

size_t CcConsole::readAll(CcByteArray& oReturn, size_t uiBufSize)
{
  oReturn.clear();
  CcArray<char> oBuffer(uiBufSize);
  size_t uiReceived = SIZE_MAX;
  size_t uiReceivedAll = 0;
  uiReceived = read(oBuffer.getArray(), uiBufSize);
  if(uiReceived < uiBufSize ) uiReceivedAll = 0;
  while (uiReceived > 0 && uiReceived != SIZE_MAX)
  {
    uiReceivedAll += uiReceived;
    oReturn.append(oBuffer.getArray(), uiReceived);
    if (uiReceivedAll < uiBufSize)
      break;
    else
      uiReceived = read(oBuffer.getArray(), uiBufSize - uiReceivedAll);
  }
  return uiReceivedAll;
}

size_t CcConsole::readLine(CcString& sReturn)
{
  sReturn.clear();
  CcArray<char> oBuffer(256);
  size_t uiReceived = 0;
  size_t uiReceivedAll = SIZE_MAX;
  uiReceived = read(oBuffer.getArray(), 256);
  if(uiReceived < 256 ) uiReceivedAll = 0;
  while (uiReceived > 0 && uiReceived != SIZE_MAX)
  {
    uiReceivedAll += uiReceived;
    sReturn.append(oBuffer.getArray(), uiReceived);
    if (sReturn.contains(CcGlobalStrings::EolShort))
    {
      size_t uiPos = sReturn.find(CcGlobalStrings::EolShort);
      if (uiPos < sReturn.length())
      {
        if (uiPos > 0 && sReturn[uiPos - 1] == CcGlobalStrings::EolLong[0])
          uiPos--;
        sReturn.remove(uiPos, sReturn.length());
      }
      break;
    }
    else
      uiReceived = read(oBuffer.getArray(), 256);
  }
  return uiReceivedAll;
}

size_t CcConsole::readLineHidden(CcString& sReturn)
{
  sReturn.clear();
  CcArray<char> oBuffer(256);
  size_t uiReceived = 0;
  size_t uiReceivedAll = SIZE_MAX;
  uiReceived = s_pInput->readHidden(oBuffer.getArray(), 256);
  if (uiReceived < 256) uiReceivedAll = 0;
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
      uiReceived = s_pInput->readHidden(oBuffer.getArray(), 256);
  }
  return uiReceivedAll;
}

void CcConsole::writeLine(const CcString& sOutput)
{
  if (s_pOutput != nullptr)
  {
    s_pLock->lock();
    s_pOutput->writeLine(sOutput);
    s_pLock->unlock();
  }
}

void CcConsole::writeSameLine(const CcString& sOutput)
{
  if (s_pOutput != nullptr)
  {
    s_pLock->lock();
    s_pOutput->write(CcGlobalStrings::EolCr.getCharString(), 1);
    s_pOutput->write(sOutput.getCharString(), sOutput.length());
    s_pLock->unlock();
  }
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

CcStdOut* CcConsole::getOutStream()
{
  return s_pOutput;
}

CcStdIn* CcConsole::getInStream()
{
  return s_pInput;
}
