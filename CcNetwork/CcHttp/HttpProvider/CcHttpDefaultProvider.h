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
 *
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcHttpDefaultProvider
 */
#ifndef H_CcHttpDefaultProvider_H_
#define H_CcHttpDefaultProvider_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpProvider.h"

/**
 * @brief Example Class implementation
 */
class CcHttpSHARED CcHttpDefaultProvider : public IHttpProvider
{
public:
  /**
   * @brief Constructor
   */
  CcHttpDefaultProvider();

  /**
   * @brief Destructor
   */
  virtual ~CcHttpDefaultProvider();

  virtual bool checkResponsible(const CcHttpWorkData &) const override
    { return true; }

  virtual CcStatus execGet(CcHttpWorkData& oData) override;
private:
  static CcString m_s404Msg;
};

#endif // H_CcHttpDefaultProvider_H_
