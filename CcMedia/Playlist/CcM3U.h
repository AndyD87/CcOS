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
#include "CcRectangle.h"
#include "CcList.h"

/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CcM3U
{
public:
  enum class EItemType
  {
    File,
    ExtXIndependentSements,
    ExtXMediaType,
    ExtXStreamInf,
  };

  class CcMediaSHARED CFileInf
  {
  public:
    float fLength;
    CcString sName;
    CcString sPath
  };

  class CcMediaSHARED CExtXMediaType
  {
  public:
    CcString sType;
    CcString sGroupId;
    CcString sName;
    CcString sLanguage;
    CcString sUri;
  };

  class CcMediaSHARED CExtXStreamInf
  {
  public:
    CcString    sPath;
    CcString    sCodecs;
    uint64      uiBandwidth;
    uint64      uiAverageBandwidth;
    CcString    sAudio;
    CcString    sSubtitles;
    float       fFrameRate;
    CcRectangle oResoulution;
  };

  class CcMediaSHARED CItem
  {
  public:
    EItemType eItemType;
    union
    {
      CExtXStreamInf* pExtXStreamInf;
      CFileInf*       pFileItem;
    } Data;
  };

  typedef class CcMediaSHARED CcList<CItem> CItemList;

  /**
   * @brief Constructor
   */
  CcM3U(const CcUrl& sUrl);

  /**
   * @brief Destructor
   */
  virtual ~CcM3U();

  CcStatus open(const CcUrl& sUrl);

private:
  void parseLines(const CcStringList& oLines);

private:
  CcUrl m_oUrl;
  bool m_bIsPath = false;

  uint16_t m_uiVersion = 1;
  CItemList m_oItems;
};
