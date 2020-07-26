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

class CcConsole::CPrivate
{
public:
  static CcMutex  s_oLock;
  static CcStdIn  s_Input;
  static CcStdOut s_Output;
  static CcStdIn*   s_pInput;
  static CcStdOut*  s_pOutput;
};

CcMutex  CcConsole::CPrivate::s_oLock;
CcStdIn  CcConsole::CPrivate::s_Input;
CcStdOut CcConsole::CPrivate::s_Output;
CcStdIn*   CcConsole::CPrivate::s_pInput(&s_Input);
CcStdOut*  CcConsole::CPrivate::s_pOutput(&s_Output);

void CcConsole::init()
{
  CPrivate::s_pInput  = &CPrivate::s_Input;
  CPrivate::s_pOutput = &CPrivate::s_Output;
}

void CcConsole::deinit()
{
}

void CcConsole::setInputDevice(CcStdIn* pInDev)
{
  CPrivate::s_pInput = pInDev;
}

void CcConsole::setOutputDevice(CcStdOut* pOutDev)
{
  CPrivate::s_pOutput = pOutDev;
}

size_t CcConsole::read(void* pBuffer, size_t uSize)
{
  size_t uiRead = SIZE_MAX;
  if (CPrivate::s_pInput != nullptr)
  {
    CPrivate::s_oLock.lock();
    uiRead = CPrivate::s_pInput->read(pBuffer, uSize);
    CPrivate::s_oLock.unlock();
  }
  return uiRead;
}

size_t CcConsole::write(const void* pBuffer, size_t uSize)
{
  size_t uiWritten = SIZE_MAX;
  if (CPrivate::s_pOutput != nullptr)
  {
    CPrivate::s_oLock.lock();
    uiWritten = CPrivate::s_pOutput->write(pBuffer, uSize);
    CPrivate::s_oLock.unlock();
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
  CcString sReturn;
  CcArray<char> oBuffer(256);
  size_t uiReceived = 0;
  size_t uiReceivedAll = 0;
  uiReceived = CPrivate::s_pInput->readHidden(oBuffer.getArray(), 256);
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
      uiReceived = CPrivate::s_pInput->readHidden(oBuffer.getArray(), 256);
  }
  return sReturn;
}

void CcConsole::writeLine(const CcString& sOutput)
{
  if (CPrivate::s_pOutput != nullptr)
  {
    CPrivate::s_oLock.lock();
    CPrivate::s_pOutput->writeLine(sOutput);
    CPrivate::s_oLock.unlock();
  }
}

void CcConsole::writeSameLine(const CcString& sOutput)
{
  if (CPrivate::s_pOutput != nullptr)
  {
    CPrivate::s_oLock.lock();
    CPrivate::s_pOutput->write(CcGlobalStrings::EolCr.getCharString(), 1);
    CPrivate::s_pOutput->write(sOutput.getCharString(), sOutput.length());
    CPrivate::s_oLock.unlock();
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
  return CPrivate::s_pOutput;
}

CcStdIn* CcConsole::getInStream()
{
  return CPrivate::s_pInput;
}
