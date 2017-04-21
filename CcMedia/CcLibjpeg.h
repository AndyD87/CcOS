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
 * @subpage   CcLibjpeg
 *
 * @page      CcLibjpeg
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcLibjpeg
 */
#ifndef CcLibjpeg_H_
#define CcLibjpeg_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcByteArray.h"
#include "CcImage.h"

/**
 * @brief Example Class impelmentation
 */
class CcLibjpeg : public CcImage
{
public:
  /**
   * @brief Constructor
   */
  CcLibjpeg( void );

  /**
   * @brief Destructor
   */
  virtual ~CcLibjpeg( void );

};

#endif /* CcLibjpeg_H_ */
