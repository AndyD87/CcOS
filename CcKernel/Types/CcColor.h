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
 * @brief     Class CcColor
 */
#pragma once

#include "CcBase.h"

class CcString;

/**
 * @brief Class for describing a common color value with red, green, blue and alpha
 */
class CcKernelSHARED CcColor
{
public:
  /**
   * @brief Enum for arangement of Colors in a single value.
   */
  enum class EColorType
  {
    RGB = 0,
    ARGB,
    BGR,
  };

  CcColor() = default;
  ~CcColor() = default;
  
  /**
   * @brief Copyconstructor
   */
  CcColor(const CcColor& rColor)
    { operator=(rColor);}

  /**
   * @brief Create Object with initiating Color
   * @param color: RGB value stored in a 16bit value
   * @param type: Type of color arrangement in color-value, default RGB
   */
  CcColor(uint16 color, EColorType type = EColorType::RGB);

  /**
   * @brief Create Object with initiating Color
   * @param color: RGB value stored in a 32bit value
   * @param type: Type of color arrangement in color-value, default RGB
   */
  CcColor(uint32 color, EColorType type = EColorType::RGB);

  /**
   * @brief Create Object with initiating Color
   * @param R: Red value
   * @param G: Green value
   * @param B: Blue value
   * @param A: Alpha value for transparency;
   */
  CcColor(uint8 R, uint8 G, uint8 B, uint8 A=0xff);

  /**
   * @brief Copy color values from another object to this
   * @param rColor: Color to copy from
   * @return Handle to this
   */
  CcColor& operator=(const CcColor& rColor);

  /**
   * @brief Set 16 bit color in type defined format
   * @param color:  16 bit color to set
   * @param type:   Format of color
   */
  void setColor(uint16 color, EColorType type = EColorType::RGB);

  /**
   * @brief Set 32 bit color in type defined format
   * @param color:  32 bit color to set
   * @param type:   Format of color
   */
  void setColor(uint32 color, EColorType type = EColorType::RGB);

  /**
   * @brief Set color values directly.
   * @param R: Value for red
   * @param G: Value for green
   * @param B: Value for blue
   * @param A: Value for alpha
   */
  void setColor(uint8 R, uint8 G, uint8 B, uint8 A=0xff);
  //! @return Get red value
  uint8 getR() const { return m_R;}
  //! @return Get green value
  uint8 getG() const { return m_G;}
  //! @return Get blue value
  uint8 getB() const { return m_B;}
  //! @return Get alpha value
  uint8 getA() const { return m_A;}

  //! @param R: Set red value
  void setColorR(uint8 R)
  {m_R=R;}
  //! @param G: Set green value
  void setColorG(uint8 G)
  {m_G=G;}
  //! @param B: Set blue value
  void setColorB(uint8 B)
  {m_B=B;}
  //! @param A: Set alpha value
  void setColorA(uint8 A)
  {m_A=A;}

  /**
   * @brief Get current value as 16 bit value in defined format
   * @param type: Target format of 16 bit output
   * @return Formated value
   */
  uint16 getColor16Bit( EColorType type = EColorType::RGB);

  /**
   * @brief Get current value as 32 bit value in defined format
   * @param type: Target format of 32 bit output
   * @return Formated value
   */
  uint32 getColor32Bit( EColorType type = EColorType::RGB);

  /**
   * @brief Get value as \#xxx string
   * @return CSS formated string.
   */
  CcString getCssString() const;

  uint8 m_R=0;    //!< Red value
  uint8 m_G=0;    //!< Green value
  uint8 m_B=0;    //!< Blue value
  uint8 m_A=0xff; //!< Alpha value
};
