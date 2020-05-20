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
 * @page      Linux
 * @subpage   CcLinuxDbusArguments
 *
 * @page      CcLinuxDbusArguments
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class CcLinuxDbusArguments
 */

#ifndef H_CcLinuxDbusArguments_H_
#define H_CcLinuxDbusArguments_H_

#include "CcBase.h"
#include "CcVariantList.h"

class CcLinuxDbusArguments : public CcVariantList
{
public:
  CcLinuxDbusArguments();
  ~CcLinuxDbusArguments();

  bool succeeded()
  { return m_bSuccess; }
  void setSuccess(bool bSuccess)
  { m_bSuccess = bSuccess; }
private:
  bool m_bSuccess;
};

#endif // H_CcLinuxDbusArguments_H_
