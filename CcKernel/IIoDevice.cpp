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
 * @brief     Class IIoDevice
 */

#include "IIoDevice.h"
#include "CcByteArray.h"
#include "CcArray.h"
#include "CcString.h"
#include "CcBufferList.h"
#include "CcGlobalStrings.h"

#define DEFAULT_BUFFER_SIZE 128 //!< default buffer size for reading and writing from and to CharArray

CcStatus IIoDevice::ioControl(uint32 cmd, const void *argument)
{
  // do nothing implementation by inheriting class;
  CCUNUSED(cmd);
  CCUNUSED(argument);
  return false;
}

void* IIoDevice::getStdFile()
{
  return nullptr;
}

size_t IIoDevice::readArray(CcByteArray& oOutputArray, bool bDoResize)
{
  size_t uiReceived = read(oOutputArray.getArray(), oOutputArray.size());
  if (bDoResize == true)
  {
    if (uiReceived <= oOutputArray.size())
      oOutputArray.resize(uiReceived);
    else
      oOutputArray.resize(0);
  }
  return uiReceived;
}

CcByteArray IIoDevice::readAll(size_t uiBufSize)
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
      uiReceived = read(oBuffer.getArray(), uiBufSize);
  }
  return oReturn;
}

CcStatus IIoDevice::writeArray(const CcByteArray& oArray)
{
  bool bRet = true;
  size_t uiDataSend = 0;
  while (uiDataSend < oArray.size())
  {
    size_t uiDataLeft = oArray.size() - uiDataSend;
    size_t uiLastSend = write(oArray.getArray(uiDataSend), uiDataLeft);
    if (uiLastSend == SIZE_MAX)
    {
      bRet = false;
      break;
    }
    else
    {
      uiDataSend += uiLastSend;
    }
  }
  return bRet;
}

CcStatus IIoDevice::writeString(const CcString& oArray)
{
  bool bRet = true;
  size_t uiDataSend = 0;
  while (uiDataSend < oArray.length())
  {
    size_t uiDataLeft = oArray.length() - uiDataSend;
    size_t uiLastSend = write(oArray.getCharString() + uiDataSend, uiDataLeft);
    if (uiLastSend == SIZE_MAX)
    {
      bRet = false;
      break;
    }
    else
    {
      uiDataSend += uiLastSend;
    }
  }
  return bRet;
}

CcStatus IIoDevice::writeLine(const CcString& oArray)
{
  bool bRet = true;
  bRet &= writeString(oArray);
  bRet &= writeString(CcGlobalStrings::EolOs);
  return bRet;
}

CcStatus IIoDevice::writeBufferList(const CcBufferList& oArray)
{
  CcStatus bRet;
  for (const CcByteArray& oBuffer : oArray)
  {
    bRet = writeArray(oBuffer);
    if (!bRet) break;
  }
  return bRet;
}

