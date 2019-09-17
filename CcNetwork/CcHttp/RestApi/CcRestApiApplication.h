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
 * @page      CcRestApiApplication
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiApplication
 */
#ifndef H_CcRestApiApplication_H_
#define H_CcRestApiApplication_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "CcRestApiApplicationMenu.h"
#include "CcRestApiApplicationFooter.h"

/**
 * @brief CcRestApiApplication implementation
 */
class CcHttpSHARED CcRestApiApplication : public IRestApi
{
public:
  /**
   * @brief Constructor
   */
  CcRestApiApplication(IRestApi* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiApplication();

  virtual bool get(CcHttpWorkData& oData) override;

  CcRestApiApplicationMenu& getMenu()
    { return m_oMenu; }
  CcRestApiApplicationFooter& getFooter()
    { return m_oFooter; }

private:
  CcRestApiApplicationMenu m_oMenu;
  CcRestApiApplicationFooter m_oFooter;
};

#endif // H_CcRestApiApplication_H_
