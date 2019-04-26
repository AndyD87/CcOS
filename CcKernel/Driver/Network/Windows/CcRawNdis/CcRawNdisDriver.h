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
 * @page      Network_Windows
 * @subpage   CcRawNdisDriver
 *
 * @page      CcRawNdisDriver
 * @author    Andreas Dirmeier
 * @par       Language: C++11
 */

#ifndef _CcRawNdisDriver_H_
#define _CcRawNdisDriver_H_

#include "IDriver.h"

/**
 * @brief Generate SM32F407V CPU Device
 */
class CcRawNdisDriver : public IDriver
{
public:
  /**
   * @brief Constructor
   */
  CcRawNdisDriver();

  /**
   * @brief Destructor
   */
  virtual ~CcRawNdisDriver();
  
  virtual CcStatus entry() override;
  virtual CcStatus unload() override;
private: // Types
  class CPrivate;
private:
  CPrivate* m_pPrivate;
};

#endif /* _CcRawNdisDriver_H_ */
