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
 * @page      CcMedia
 * @subpage   CcImage
 *
 * @page      CcImage
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcImage
 */
#ifndef H_CcImage_H_
#define H_CcImage_H_

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
   * @brief Convert current Image
   * @param Type: Type Image has to be converted to.
   * @param Settings: Setting of target Image to convert to
   * @return true if conversion succeeded.
   */
  bool convert(EImageType Type, void* Settings);

  bool setBuffer(const CcByteArray& oToCopy, EImageType eType = EImageType::Unknown);

  CcImageRaw getRaw();

  static void init();
  static EImageType findType(const CcByteArray& oData);
  static NImage::IImageConverter* getConverter(EImageType eType);
  static void registerConverter(NImage::IImageConverter* pConverter);
  static bool deregisterConverter(NImage::IImageConverter* pConverter);

private:
  static size_t                               m_iInit;
  static CcMutex                              m_oConverterListLock;
  static CcVector<NImage::IImageConverter*>   m_pConverterList;
  static CcImage                              m_oImage;
};

#endif // H_CcImage_H_
