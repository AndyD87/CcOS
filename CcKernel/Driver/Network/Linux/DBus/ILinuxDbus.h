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
 * @subpage   ILinuxDbus
 *
 * @page      ILinuxDbus
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2015
 * @par       Language: C++11
 * @brief     Class ILinuxDbus
 */

#ifndef H_ILinuxDbus_H_
#define H_ILinuxDbus_H_

#include "CcBase.h"
#include "CcString.h"
#include "CcLinuxDbusArguments.h"
#include "CcLinuxDbusArguments.h"

struct DBusConnection;

class ILinuxDbus
{
public:
  ILinuxDbus(const CcString& sBus, const CcString& sPath, const CcString& sInterface);
  virtual ~ILinuxDbus();

  const CcString& getBus()
  { return m_sBus; }
  const CcString& getPath()
  { return m_sPath; }
  const CcString& getInterface()
  { return m_sInterface; }

  bool call(const CcString& sMethod, const CcLinuxDbusArguments& oArgs, CcLinuxDbusArguments& oResult);
  bool call(const CcString& sMethod,
            const CcLinuxDbusArguments& oArgs,
            CcLinuxDbusArguments& oResult,
            const CcString& sPathAppend,
            const CcString& sIfcAppend);
  bool connect();
  void disconnect();

  CcLinuxDbusArguments property(const CcString& sProperty,
                                const CcString& sPathAppend,
                                const CcString& sInterfaceAppend);

private:
  bool callInt(const CcString& sMethod,
            const CcLinuxDbusArguments& oArgs,
            CcLinuxDbusArguments& oResult,
            const CcString& sBus,
            const CcString& sPath,
            const CcString& sIfc);
  class CPrivate;
  CPrivate* m_pPrivate;
  CcString m_sBus;
  CcString m_sPath;
  CcString m_sInterface;
};

#endif // H_ILinuxDbus_H_
