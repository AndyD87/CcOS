/*
 * This file is part of CcConvertImage.
 *
 * CcConvertImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcConvertImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcConvertImage.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcConvertImage
 **/
#pragma once

#include "CcApp.h"
#include "CcString.h"
#include "CcImage.h"

/**
 * @brief CcConvertImage implementation
 *        Main class wich is loaded to start Application.
 */
class CcConvertImage : public CcImage
{
public:
  /**
   * @brief Constructor
   */
  CcConvertImage();

  /**
   * @brief Destructor
   */
  ~CcConvertImage();

  bool loadImage(const CcString& sImage);
};
