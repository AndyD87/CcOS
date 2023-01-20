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
 * @brief     Class CcImageRaw
 */
#pragma once

#include "CcBase.h"
#include "CcMedia.h"
#include "CcByteArray.h"
#include "CcSize.h"

/**
 * @brief Raw image with 32bit per pixel
 */
class CcMediaSHARED CcImageRaw
{
public:
  /**
   * @brief Definition of pixel as unioin.
   */
  typedef union
  {
    uint32 uiValue;     //!< Uint32 value of pixel
    uint8  pValue[4];   //!< All 4 values as byte arra
    struct
    {
      uint8 R;          //!< Red value of pixel
      uint8 G;          //!< Green value of pixel
      uint8 B;          //!< Blue value of pixel
      uint8 A;          //!< Alpha value of pixel
    } RGBA;             //!< Pixel values as color by name
  } CPixel;

  /**
   * @brief Constructor with init of 0x0 pixel buffer
   */
  CcImageRaw();

  /**
   * @brief Create image with pixelbuffer
   * @param uiX: X dimension of image
   * @param uiY: Y dimension of image
   */
  CcImageRaw(uint32 uiX, uint32 uiY);
  CCDEFINE_CONSTRUCTOR_TO_OPERATORS(CcImageRaw)
  ~CcImageRaw();

  /**
   * @brief Copy content of another raw image to this
   * @param oToCopy: Image to duplicate
   * @return Handle to this
   */
  CcImageRaw& operator=(const CcImageRaw& oToCopy);

  /**
   * @brief Move content from another raw image to this
   * @param oToMove: Image to move from
   * @return Handle to this
   */
  CcImageRaw& operator=(CcImageRaw&& oToMove);

  /**
   * @brief Resize pixelbuffer to new format
   * @param uiX: X value of new map
   * @param uiY: Y value of new map
   */
  void resize(uint32 uiX, uint32 uiY);

  /**
   * @brief Get full pixel on coordinate
   * @param uiX: X value of coordinate
   * @param uiY: Y value of coordinate
   * @return Reference to pixel in area.
   */
  CPixel& getPixel(uint32 uiX, uint32 uiY);
  const CPixel& getPixel(uint32 uiX, uint32 uiY) const;

  /**
   * @brief Get Red value of pixel on coordinate
   * @param uiX: X value of coordinate
   * @param uiY: Y value of coordinate
   * @return Reference to Red value of pixel in area.
   */
  uint8& getPixelR(uint32 uiX, uint32 uiY) const;

  /**
   * @brief Get Green value of pixel on coordinate
   * @param uiX: X value of coordinate
   * @param uiY: Y value of coordinate
   * @return Reference to Green value of pixel in area.
   */
  uint8& getPixelG(uint32 uiX, uint32 uiY) const;

  /**
   * @brief Get Blue value of pixel on coordinate
   * @param uiX: X value of coordinate
   * @param uiY: Y value of coordinate
   * @return Reference to Blue value of pixel in area.
   */
  uint8& getPixelB(uint32 uiX, uint32 uiY) const;

  /**
   * @brief Get Alpha value of pixel on coordinate
   * @param uiX: X value of coordinate
   * @param uiY: Y value of coordinate
   * @return Reference to Alpha value of pixel in area.
   */
  uint8& getPixelA(uint32 uiX, uint32 uiY) const;

  /**
   * @brief Set color values of pixel on specific coordinate
   * @param X: X value of coordinate
   * @param Y: Y value of coordinate
   * @param R: Red value to set of pixel on coordinate
   * @param G: Green value to set of pixel on coordinate
   * @param B: Blue value to set of pixel on coordinate
   * @param A: Alpha value to set of pixel on coordinate
   */
  void setPixel(uint32 X, uint32 Y, uint8 R, uint8 G, uint8 B, uint8 A);

  //! @return Get dimension in X direction
  inline uint32 getX() const
  { return m_oSize.getWidth(); }
  //! @return Get dimension in Y direction
  inline uint32 getY() const
  { return m_oSize.getHeight(); }
  //! @return Get width of image
  inline uint32 getWidth() const
  { return m_oSize.getWidth(); }
  //! @return Get height of image
  inline uint32 getHeight() const
  { return m_oSize.getHeight(); }
  //! @return Get number of all pixels
  inline uint64 getPixelCount() const
  { return static_cast<uint64>(getHeight()) * getWidth(); }
  //! @return Size of pixel buffer
  size_t getBufferSize()
  { return m_uiBufferSize; }
  //! @return Pixel buffer
  CPixel* getBuffer()
  { return m_pData; }

  /**
   * @brief Clear buffer and set to null
   */
  void clear();
private:
  CcSize      m_oSize;
  size_t      m_uiBufferSize = 0;
  CPixel*     m_pData = nullptr;
};
