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
 * @brief     Implemtation of class CcTestUtility
 */
#include "CcTestUtility.h"
#include "CcStatus.h"
#include "CcString.h"
#include "CcFile.h"
#include "CcByteArray.h"
#include "CcStatic.h"

CcStatus CcTestUtility::generateAndVerifyFile(const CcString& sPath, uint64 uiSize, uint64 uiPattern)
{
  CcFile oFile(sPath);
  uint64 uiTestPattern = uiPattern;
  CcStatus oSuccess = oFile.open(EOpenFlags::Write);
  if(oSuccess)
  {
    uint64 uiWritten = 0;
    CcByteArray oBuffer(CCMIN(uiSize, 1024*sizeof(uiPattern)));
    uint64* pBuffer = CCVOIDPTRCAST(uint64*, oBuffer.getArray());
    // Write buffered for performance reason
    for(;oSuccess && uiWritten + oBuffer.size() <= uiSize; uiWritten += oBuffer.size())
    {
      for(size_t uiBufferWritten = 0; uiBufferWritten < oBuffer.size(); uiBufferWritten += sizeof(uiPattern))
      {
        pBuffer[uiBufferWritten/sizeof(uiPattern)] = uiBufferWritten + uiPattern;
        uiPattern++;
      }
      oSuccess = oFile.writeArray(oBuffer);
    }
    // Write last pattern in single step
    for(;oSuccess && uiWritten + sizeof(uiPattern) <= uiSize; uiWritten += sizeof(uiPattern))
    {
      if(sizeof(uiPattern) != oFile.write(&uiPattern, sizeof(uiPattern)))
      {
        oSuccess = EStatus::FileSystemError;
      }
      uiPattern++;
    }
    // Write last bytes <8
    if(oSuccess && uiWritten < uiSize)
    {
      oFile.write(&uiPattern, uiSize - uiWritten);
    }
    if(oSuccess)
    {
      oSuccess = oFile.close();
    }
    if(oSuccess)
    {
      oSuccess = oFile.open(EOpenFlags::Read);
    }
    if(oSuccess)
    {
      uint64 uiRead = 0;
      // Verify buffered
      for(;oSuccess && uiRead + oBuffer.size() <= uiSize; uiRead += oBuffer.size())
      {
        if(oBuffer.size() != oFile.readArray(oBuffer, false))
        {
          oSuccess = EStatus::FileSystemError;
        }
        else
        {
          for(size_t uiVerified = 0; oSuccess && uiVerified < oBuffer.size(); uiVerified += sizeof(uiTestPattern))
          {
            if(pBuffer[uiVerified/sizeof(uiTestPattern)] != uiVerified + uiTestPattern)
            {
              oSuccess = EStatus::Error;
            }
            uiTestPattern++;
          }
        }
      }
      // Verify single step
      for(;oSuccess && uiRead + sizeof(uiPattern) <= uiSize; uiRead += sizeof(uiTestPattern))
      {
        if(sizeof(uiTestPattern) != oFile.read(pBuffer, sizeof(uiTestPattern)))
        {
          oSuccess = EStatus::FileSystemError;
        }
        if(oSuccess &&
           uiTestPattern != pBuffer[0])
        {
          oSuccess = EStatus::Error;
        }
        uiTestPattern++;
      }
      // Verify last bytes <8
      if(oSuccess && uiRead < uiSize)
      {
        size_t uiSizeLeft = static_cast<size_t>(uiSize - uiRead);
        CcStatic::memset(pBuffer, 0, sizeof(uiTestPattern));
        if(uiSizeLeft == oFile.read(pBuffer, uiSizeLeft))
        {
          uint64 uiTestPatternBuffer=0;
          CcStatic::memcpy(&uiTestPatternBuffer, &uiTestPattern, uiSizeLeft);
          if(CcStatic::memcmp(&uiTestPatternBuffer, pBuffer, uiSizeLeft) != 0)
          {
            oSuccess = EStatus::Error;
          }
        }
        else
        {
          oSuccess = EStatus::FileSystemError;
        }
      }
    }
    oFile.close();
  }
  return oSuccess;
}
