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
 * @page      CcRestApiApplicationMenu
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcRestApiApplicationMenu
 */
#ifndef _CcRestApiApplicationMenu_H_
#define _CcRestApiApplicationMenu_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "CcList.h"

class CcRestApiApplication;

/**
 * @brief CcRestApiApplicationMenu impelmentation
 */
class CcHttpSHARED CcRestApiApplicationMenu : public IRestApi
{
public:
  typedef struct CcHttpSHARED
  {
    CcString sName;
    CcString sLink;
  } SEntry;
  /**
   * @brief Constructor
   */
  CcRestApiApplicationMenu(CcRestApiApplication* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiApplicationMenu();

  virtual bool get(CcHttpWorkData& oData) override;

  void append(const CcString& sName, const CcString& sLink);

private:
  CcList<SEntry>* m_pItems;
};

#endif /* _CcRestApiApplicationMenu_H_ */
