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
#include "CcMapCommon.h"
#include "CcStringUtil.h"

#define EXT_M3U                       "#EXTM3U"
#define EXT_INF                       "#EXTINF:"
#define EXT_X_VERSION                 "#EXT-X-VERSION:"
#define EXT_X_MEDIA                   "#EXT-X-MEDIA"
#define EXT_X_INDEPENDENT_SEGMENTS    "#EXT-X-INDEPENDENT-SEGMENTS"
#define EXT_X_STREAM_INF              "#EXT-X-STREAM-INF:"
#define EXT_X_TARGETDURATION          "#EXT-X-TARGETDURATION:"
#define EXT_X_MEDIA_SEQUENCE          "#EXT-X-MEDIA-SEQUENCE:"
#define EXT_X_DISCONTINUITY_SEQUENCE  "#EXT-X-DISCONTINUITY-SEQUENCE:"
#define EXT_X_PROGRAM_DATE_TIME       "#EXT-X-PROGRAM-DATE-TIME:"

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
  m_oUrl = sUrl;
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
    oSuccess = parseLines(oDataLines);
  }
  return oSuccess;
}

size_t  CcM3U::getStreamBest() const
{
  size_t uiStream = SIZE_MAX;
  CcSize oLastBestSize;
  size_t uiCnt = 0;
  for (const CExtXStreamInf& oStream : m_oStreams)
  {
    if (oStream.oResoulution > oLastBestSize)
    {
      oLastBestSize = oStream.oResoulution;
      uiStream = uiCnt;
    }
    uiCnt++;
  }
  return uiStream;
}

CcStatus CcM3U::downloadStream(size_t uiIndex, const CcString& sFile)
{
  CcStatus oStatus = false;
  CCUNUSED(uiIndex);
  CCUNUSED(sFile);
  if (uiIndex < m_oStreams.size())
  {
    CExtXStreamInf& oInfData = m_oStreams[uiIndex];
    if (oInfData.sAudio.length() == 0 &&
        oInfData.sVideo.length() == 0)
    {
      CcUrl oNewUrl = m_oUrl;
      oNewUrl.setPath(oInfData.sUri);

      CcM3U oStreamReader;
      if ((oStatus = oStreamReader.open(oNewUrl)) == true)
      {

      }
    }
  }
  else
  {
    oStatus = EStatus::OutOfRange;
  }
  return oStatus;
}

CcStatus CcM3U::parseLines(const CcStringList& oLines)
{
  CcStatus oSuccess;
  CcStringList::const_iterator oIterator = oLines.begin();
  CExtInf oActiveExtInf;
  CcDateTime oActiveDateInf;
  while(oSuccess && oIterator != oLines.end())
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
        else if (sLine.startsWith(EXT_X_INDEPENDENT_SEGMENTS))
        {
          m_bIndependentSegments = true;
        }
        else if (sLine.startsWith(EXT_X_MEDIA))
        {
          CCDEBUG("");
        }
        else if (sLine.startsWith(EXT_X_STREAM_INF))
        {
          CcStringMap oList = getValuePares(sLine, CCLENGTHOFSTRING(EXT_X_STREAM_INF));
          CExtXStreamInf oInfData;
          oSuccess = parseExtStreamInf(oInfData, oList);
          if (oInfData.sAudio.length() == 0 &&
              oInfData.sVideo.length() == 0)
          {
            oIterator++;
            oInfData.sUri = *oIterator;
          }
          m_oStreams.append(oInfData);
        }
        else if (sLine.startsWith(EXT_INF))
        {
          CcStringMap oList = getValuePares(sLine, CCLENGTHOFSTRING(EXT_INF));
          if (oList.size() > 0)
            oActiveExtInf.fLength = oList[0].getKey().toFloat();
          else
            oActiveExtInf.fLength = 0.0;
          if (oList.size() > 1)
            oActiveExtInf.sName = oList[1].getKey();
          else
            oActiveExtInf.sName = "";
        }
        else if (sLine.startsWith(EXT_X_PROGRAM_DATE_TIME))
        {
          CcStringMap oList = getValuePares(sLine, CCLENGTHOFSTRING(EXT_X_PROGRAM_DATE_TIME));
          if(oList.size() > 0)
            oActiveDateInf.fromIso8601(oList[0].getKey());
        }
      }
      else
      {
        CFileInf oFileItem;
        oFileItem.ExtInf = oActiveExtInf;
        CcString& sFileLine = *oIterator;
        oFileItem.sPath = sFileLine;
        m_oFiles.append(oFileItem);
      }
    }
    else
    {
      // Empty Line, do nothing
    }
    oIterator++;
  }
  return oSuccess;
}

CcStringMap CcM3U::getValuePares(const CcString& sLine, size_t uiOffset)
{
  CcStringMap oReturn;
  while (uiOffset < sLine.length())
  {
    size_t uiFoundComma = CcStringUtil::findCharEscapedSkipQuotes(sLine, ',', '\\', '"', uiOffset);
    size_t uiFoundColon = CcStringUtil::findCharEscapedSkipQuotes(sLine, '=', '\\', '"', uiOffset, uiFoundComma - uiOffset);
    if (uiFoundColon < uiFoundComma)
    {
      CcString sKey = sLine.substr(uiOffset, uiFoundColon - uiOffset);
      CcString sValue = sLine.substr(uiFoundColon+1, uiFoundComma - (uiFoundColon + 1));
      oReturn.append(sKey, CcStringUtil::stripQuotes(sValue));
    }
    else
    {
      CcString sKey = sLine.substr(uiOffset, uiFoundComma - uiOffset);
      oReturn.append(CcStringUtil::stripQuotes(sKey), "");
    }
    if(uiFoundComma < sLine.length())
      uiOffset = uiFoundComma + 1;
    else
      uiOffset = SIZE_MAX;
  }
  return oReturn;
}

CcStatus CcM3U::parseExtStreamInf(CExtXStreamInf& oData, CcStringMap& oMap)
{
  CcStatus oStatus;
  for (CcStringPair& oPair : oMap)
  {
    if (oPair.getKey().compare("BANDWIDTH"))
    {
      oData.uiBandwidth = oPair.getValue().toUint64();
    }
    else if (oPair.getKey().compare("AVERAGE-BANDWIDTH"))
    {
      oData.uiAverageBandwidth = oPair.getValue().toUint64();
    }
    else if (oPair.getKey().compare("CODECS"))
    {
      oData.oCodecs = oPair.getValue().split(',');
    }
    else if (oPair.getKey().compare("RESOLUTION"))
    {
      CcStringList oValues = oPair.getValue().split('x');
      if (oValues.size() == 2)
      {
        oData.oResoulution.setWidth(oValues[0].toUint32());
        oData.oResoulution.setHeight(oValues[1].toUint32());
      }
    }
    else if (oPair.getKey().compare("FRAME-RATE"))
    {
      oData.fFrameRate = oPair.getValue().toFloat();
    }
    else
    {
      CCDEBUG("Unhandled key");
    }
  }
  return oStatus;
}