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
 * @page      IRestApi
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IRestApi
 */
#ifndef _IRestApi_H_
#define _IRestApi_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcList.h"

class CcHttpWorkData;

/**
 * @brief IRestApi impelmentation
 */
class CcHttpSHARED IRestApi
{
public:
  /**
   * @brief Constructor
   */
  IRestApi(IRestApi* pParent, const CcString& sPath);

  /**
   * @brief Destructor
   */
  virtual ~IRestApi();

  virtual bool exec(CcStringList& oPath, CcHttpWorkData& oData);
  virtual bool run(CcHttpWorkData& oData);
  const CcString& getPath() const
    { return m_sPath; }
  IRestApi* getProvider(const CcString& sPath);

  void appendProvider(IRestApi* pChild)
    { m_oChilds.append(pChild); }
  void removeProvider(IRestApi* pChild)
    { m_oChilds.removeItem(pChild); }

private:
  IRestApi*         m_pParent;
  CcString          m_sPath;
  CcList<IRestApi*> m_oChilds;
};

#endif /* _IRestApi_H_ */
