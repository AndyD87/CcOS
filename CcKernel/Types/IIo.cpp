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
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IIo
 */

#include "IIo.h"
#include "CcByteArray.h"
#include "CcArray.h"
#include "CcString.h"
#include "CcStringUtil.h"
#include "CcBufferList.h"
#include "CcGlobalStrings.h"

#define DEFAULT_BUFFER_SIZE 128 //!< default buffer size for reading and writing from and to CharArray

IIo& IIo::operator<<(const CcString& toAdd)
{
  write(toAdd.getCharString(), toAdd.length());
  return *this;
}

IIo& IIo::operator<<(const char *toAdd)
{
  write(toAdd, CcStringUtil::strlen(toAdd));
  return *this;
}

CcStatus IIo::ioControl(uint32 cmd, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  // do nothing implementation by inheriting class;
  CCUNUSED(cmd);
  CCUNUSED(pInArg);
  CCUNUSED(uiInSize);
  CCUNUSED(pOutArg);
  CCUNUSED(uiOutSize);
  CCUNUSED(puiWritten);
  return false;
}

CcStatus IIo::ioControl(EControls eControl, const void *pInArg, size_t uiInSize, void *pOutArg, size_t uiOutSize, size_t* puiWritten)
{
  return ioControl(static_cast<uint32>(eControl), pInArg, uiInSize, pOutArg, uiOutSize, puiWritten);
}

void* IIo::getStdFile()
{
  return nullptr;
}

size_t IIo::readArray(CcByteArray& oOutputArray, bool bDoResize)
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

CcByteArray IIo::readAll(size_t uiBufSize)
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

CcStatus IIo::writeArray(const CcByteArray& oArray)
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

CcStatus IIo::writeString(const CcString& oArray)
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

CcStatus IIo::writeLine(const CcString& oArray)
{
  bool bRet = true;
  bRet &= writeString(oArray);
  bRet &= writeString(CcGlobalStrings::EolOs);
  return bRet;
}

CcStatus IIo::writeBufferList(const CcBufferList& oArray)
{
  CcStatus bRet;
  for (const CcByteArray& oBuffer : oArray)
  {
    bRet = writeArray(oBuffer);
    if (!bRet) break;
  }
  return bRet;
}
