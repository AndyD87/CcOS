/*
 *
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
 * @page      Camera
 * @subpage   CcGphotoDriver
 *
 * @page      CcGphotoDriver
 * @author    Andreas Dirmeier
 * @copyright Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcGphotoDriver
 */
#ifndef H_CcGphotoDriver_H_
#define H_CcGphotoDriver_H_

#include "IDriver.h"
#include "Devices/ICamera.h"
#include "CcVector.h"

/**
 * @brief Example Class impelmentation
 */
class CcGphotoDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  CcGphotoDriver();

  /**
   * @brief Destructor
   */
  virtual ~CcGphotoDriver();

  virtual CcStatus entry() override;
  virtual CcStatus unload() override;

private:
  CcVector<ICamera*> m_pCameras;
};

#endif // _CcGphotoDriver_H_
