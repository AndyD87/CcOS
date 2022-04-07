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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcM3U
 */
#pragma once

#include "CcMedia.h"
#include "CcUrl.h"
#include "CcSize.h"
#include "CcList.h"
#include "CcMapCommon.h"
#include "CcStringList.h"
#include "CcDateTime.h"

/**
 * @brief Example Class implementation
 */
class CcM3U
{
public:
  enum class EItemType
  {
    File,
    ExtXMediaType,
    ExtXStreamInf,
  };

  class CExtInf
  {
  public:
    float fLength = 0.0;
    CcString sName;
    CcString sPath;
  };

  class CFileInf
  {
  public:
    CExtInf     ExtInf;
    CcDateTime  DateInf;
    float       TimeZone;
    CcString sPath;
  };

  class CExtXMediaType
  {
  public:
    CcString sType;
    CcString sGroupId;
    CcString sName;
    CcString sLanguage;
    CcString sUri;
  };

  class CExtXStreamInf
  {
  public:
    CcString     sUri;
    CcStringList oCodecs;
    uint64       uiBandwidth = 0;
    uint64       uiAverageBandwidth = 0;
    CcString     sSubtitles;
    float        fFrameRate = 0.0;
    CcSize       oResoulution;

    CcString     sAudio;
    CcString     sVideo;
  };

  CcMediaSHARED CcM3U() = default;

  /**
   * @brief Constructor
   */
  CcMediaSHARED CcM3U(const CcUrl& sUrl);

  /**
   * @brief Destructor
   */
  CcMediaSHARED virtual ~CcM3U();

  CcStatus CcMediaSHARED open(const CcUrl& sUrl);

  size_t   CcMediaSHARED getStreamCount() const
  { return m_oStreams.size(); }
  size_t   CcMediaSHARED getStreamBest() const;
  CcStatus CcMediaSHARED downloadStream(size_t uiIndex, const CcString& sFile);

private:
  CcStatus parseLines(const CcStringList& oLines);
  static CcStringMap getValuePares(const CcString& sLine, size_t uiOffset);

  CcStatus parseExtStreamInf(CExtXStreamInf& oData, CcStringMap& oMap);
private:
  CcUrl m_oUrl;
  bool m_bIsPath = false;

  uint16_t m_uiVersion = 1;
  bool     m_bIndependentSegments = false;

  CcList<CExtXStreamInf> m_oStreams;
  CcList<CExtXMediaType> m_oMedias;
  CcList<CFileInf      > m_oFiles;
};
