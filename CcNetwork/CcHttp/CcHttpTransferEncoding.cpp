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
 * @brief     Implementation of Class CcHttpTransferEncoding
 */
#include "CcHttpTransferEncoding.h"
#include "CcHttpGlobalStrings.h"
#include "CcKernel.h"
#include "CcGlobalStrings.h"
#include "CcStringList.h"

const uint32 CcHttpTransferEncoding::Normal   = 0;
const uint32 CcHttpTransferEncoding::Chunked  = 1;
const uint32 CcHttpTransferEncoding::Compress = 2;
const uint32 CcHttpTransferEncoding::Deflate  = 4;
const uint32 CcHttpTransferEncoding::Gzip     = 6;
const uint32 CcHttpTransferEncoding::Identity = 16;

void CcHttpTransferEncoding::setFlag(uint32 uiFlag)
{
  m_uiFlags = uiFlag;
}

void CcHttpTransferEncoding::addFlag(uint32 uiFlag)
{
  m_uiFlags |= uiFlag;
}

void CcHttpTransferEncoding::parseLine(const CcString& sLine)
{
  CcStringList oSplitted = sLine.split(CcGlobalStrings::Seperators::Comma);
  m_uiFlags = 0;
  for (CcString& sData : oSplitted)
  {
    parseValue(sData.trim());
  }
}

CcString CcHttpTransferEncoding::getLine()
{
  CcString sLine;
  sLine.append("Transfer-Encoding: ", sizeof("Transfer-Encoding: ")-1);
  if (isChunked())
  {
    sLine.append("chunked, ");
  }
  if (isCompressed())
  {
    sLine.append("compressed, ");
  }
  if (isDeflate())
  {
    sLine.append("deflate, ");
  }
  if (isGzip())
  {
    sLine.append("gzip, ");
  }
  if (isIdentity())
  {
    sLine.append("identity, ");
  }
  sLine.remove(sLine.length() - 2, 2);
  return sLine;
}

void CcHttpTransferEncoding::parseValue(const CcString &sData)
{
  if (sData.compareInsensitve("chunked"))
  {
    m_uiFlags |= Chunked;
  }
  if (sData.compareInsensitve("compress"))
  {
    m_uiFlags |= Compress;
  }
  if (sData.compareInsensitve("deflate"))
  {
    m_uiFlags |= Deflate;
  }
  if (sData.compareInsensitve("gzip"))
  {
    m_uiFlags |= Gzip;
  }
  if (sData.compareInsensitve("identity"))
  {
    m_uiFlags |= Identity;
  }
}
