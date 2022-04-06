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
 * @brief     Implementation of Class CcM3U
 */
#include "Playlist/CcM3U.h"
#include "CcFile.h"
#include "CcHttpClient.h"

CcM3U::CcM3U(const CcUrl& sUrl)
{
  open(sUrl);
}

CcM3U::~CcM3U()
{
}

CcStatus CcM3U::open(const CcUrl& sUrl)
{
  CcStringList oDataLines;
  CcStatus oSuccess = false;
  if (sUrl.getProtocol() == "http" || 
      sUrl.getProtocol() == "https")
  {
    CcHttpClient oClient;
    oClient.setUrl(sUrl);
    if (oClient.execGet())
    {
      CcString sData = oClient.getByteArray();
      oDataLines = sData.splitLines(true);
    }
  }
  else if(CcFile::isFile(sUrl.getPath()))
  {
    CcFile oFile(sUrl.getPath());
    if (oFile.open(EOpenFlags::Read))
    {
      CcString sData = oFile.readAll();
      oDataLines = sData.splitLines(true);
      oFile.close();
    }
  }
  if (oDataLines.size())
  {
    parseLines(oDataLines);
  }
  return oSuccess;
}

#define EXT_M3U                       "#EXTM3U"
#define EXT_INF                       "#EXTINF"
#define EXT_X_VERSION                 "#EXT-X-VERSION:"
#define EXT_X_MEDIA                   "#EXT-X-MEDIA"
#define EXT_X_INDEPENDENT_SEGMENTS    "#EXT-X-INDEPENDENT-SEGMENTS"
#define EXT_X_STREAM_INF              "#EXT-X-STREAM-INF:"
#define EXT_X_TARGETDURATION          "#EXT-X-TARGETDURATION:"
#define EXT_X_MEDIA_SEQUENCE          "#EXT-X-MEDIA-SEQUENCE:"
#define EXT_X_DISCONTINUITY_SEQUENCE  "#EXT-X-DISCONTINUITY-SEQUENCE:"
#define EXT_X_PROGRAM_DATE_TIME       "#EXT-X-PROGRAM-DATE-TIME:"

void CcM3U::parseLines(const CcStringList& oLines)
{
  CcStringList::const_iterator oIterator = oLines.begin();
  while(oIterator != oLines.end())
  {
    CcString& sLine = *oIterator;
    if (sLine.length() > 0)
    {
      if (sLine[0] == '#')
      {
        if (sLine.startsWith(EXT_M3U))
        {
          // Correct format
        }
        else if (sLine.startsWith(EXT_X_VERSION))
        {
          CcString sVersion = sLine.substr(CCLENGTHOFSTRING(EXT_X_VERSION));
          m_uiVersion = sVersion.toUint16();
        }
        else if (sLine.startsWith(EXT_X_MEDIA))
        {

        }
        else if (sLine.startsWith(EXT_X_INDEPENDENT_SEGMENTS))
        {

        }
        else if (sLine.startsWith(EXT_X_STREAM_INF))
        {

        }
        else if (sLine.startsWith(EXT_INF))
        {
          CItem oItem;
          oItem.eItemType = EItemType::File;
          oItem.Data.pFileItem = new CFileInf();
          oItem.Data.pFileItem->sName = "Name";

          oIterator++;
          if (oIterator != oLines.end())
          {
            CcString& sFileLine = *oIterator;
            oItem.Data.pFileItem->sPath = sFileLine;
          }
        }
      }
      else
      {
        CItem oItem;
        oItem.eItemType = EItemType::File;
        oItem.Data.pFileItem = new CFileInf();
        oItem.Data.pFileItem->sPath = sLine;

      }
    }
    else
    {
      // Empty Line, do nothing
    }
    oIterator++;
  }
}