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
 * @brief     Class CcImage
 */
#pragma once

#include "CcBase.h"
#include "CcMedia.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "Types/CcImageData.h"
#include "CcVector.h"
#include "CcMutex.h"
#include "CcImageRaw.h"

namespace NImage
{
  class IImageConverter;
}

#ifdef _MSC_VER
template class CcMediaSHARED CcVector<NImage::IImageConverter*>;
#endif
/**
 * @brief Example Class implementation
 */
class CcMediaSHARED CcImage : public CcImageData
{
public:

  /**
   * @brief Constructor
   */
  CcImage();

  /**
   * @brief Destructor
   */
  virtual ~CcImage();

  /**
   * @brief Read an Imagefile to Buffer
   * @param sPathToFile: Path to File
   * @return true if Image so successfully loaded
   */
  bool loadFile(const CcString& sPathToFile);

  /**
   * @brief Write image to file.
   * @param sPathToFile: Path to File
   * @return true if Image so successfully loaded
   */
  bool writeFile(const CcString& sPathToFile);

  /**
   * @brief Convert current Image to an other type
   * @param Type: Type Image has to be converted to.
   * @param Settings: *Reserved Setting of target Image to convert to
   * @return Data of new image format
   */
  CcByteArray convert(EImageType Type, void* Settings = nullptr);

  /**
   * @brief Insert image buffer.
   *        If no type was set, all registered converter will be queried
   *        to determine it's correct type.
   * @param oToCopy: Buffer to import
   * @param eType:   Known type or EImageType::Unknown to search for known
   * @return True if Buffer was set and type was found
   */
  bool setBuffer(const CcByteArray& oToCopy, EImageType eType = EImageType::Unknown);

  /**
   * @brief Convert this image to raw image.
   * @return Raw image or empty image if no usable conversion was found
   */
  CcImageRaw getRaw();

  /**
   * @brief Initialize static image instance and setup all common converters.
   */
  static void init();

  /**
   * @brief Find type of image by raw binary input
   * @param oData: Input data
   * @return Found type or EImageType::Unknown if not.
   */
  static EImageType findType(const CcByteArray& oData);

  /**
   * @brief Find converter for converting raw image to target type.
   * @param eType: Converter for target type
   * @return Pointer to found converter interface
   */
  static NImage::IImageConverter* getConverter(EImageType eType);

  /**
   * @brief Register converter at global image instance
   * @param pConverter: Converter to register
   */
  static void registerConverter(NImage::IImageConverter* pConverter);

  /**
   * @brief Remove converter from global image instance
   * @param pConverter: Converter to remove
   */
  static bool deregisterConverter(NImage::IImageConverter* pConverter);

private:
  static size_t                               m_iInit;
  static CcMutex                              m_oConverterListLock;
  static CcVector<NImage::IImageConverter*>   m_pConverterList;
  static CcImage                              m_oImage;
};
