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
 * @page      CcRestApiSystem
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiSystem
 */
#ifndef H_CcRestApiSystem_H_
#define H_CcRestApiSystem_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "CcRestApiMemory.h"
#include "CcRestApiDevices.h"

/**
 * @brief CcRestApiSystem implementation
 */
class CcHttpSHARED CcRestApiSystem : public IRestApi
{
public:
  /**
   * @brief Constructor
   */
  CcRestApiSystem(IRestApi* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiSystem();

  virtual bool get(CcHttpWorkData& oData) override;

  CcRestApiDevices& getDevices()
    { return m_oDevices; }
private:
  CcRestApiMemory m_oMemory;
  CcRestApiDevices m_oDevices;
};

#endif // H_CcRestApiSystem_H_
