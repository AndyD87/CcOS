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
 * @file
 *
 * @author    Andreas Dirmeier
 * @copyright  Andreas Dirmeier (C) 2022
 * @par       Language: C++11
 * @brief     Class ILinuxDbus
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "CcLinuxDbusArguments.h"

struct DBusConnection;

/**
 * @brief Basic linux dbus interface for common access with read and writes.
 */
class ILinuxDbus
{
public:
  /**
   * @brief Create interface to target bus, path and interface
   * @param sBus:       Target bus on system
   * @param sPath:      Target path on bus
   * @param sInterface: Target interface to query
   */
  ILinuxDbus(const CcString& sBus, const CcString& sPath, const CcString& sInterface);
  virtual ~ILinuxDbus();

  //! @return Get target bus
  const CcString& getBus()
  { return m_sBus; }
  //! @return Get target path
  const CcString& getPath()
  { return m_sPath; }
  //! @return Get target interface
  const CcString& getInterface()
  { return m_sInterface; }

  /**
   * @brief Call to bus interface
   * @param sMethod:  Method to execute
   * @param oArgs:    Arguments to pass to method
   * @param[in,out] oResult:  Result output
   * @return True if execution succeded
   */
  bool call(const CcString& sMethod, const CcLinuxDbusArguments& oArgs, CcLinuxDbusArguments& oResult);
  /**
   * @brief Call to bus sub interfaces on bus
   * @param sMethod:  Method to execute
   * @param oArgs:    Arguments to pass to method
   * @param[in,out] oResult:  Result output
   * @param sPathAppend:  Append this path to interface path before execution
   * @param sIfcAppend:   Append this string to target interface before execution
   * @return True if execution succeded
   */
  bool call(const CcString& sMethod,
            const CcLinuxDbusArguments& oArgs,
            CcLinuxDbusArguments& oResult,
            const CcString& sPathAppend,
            const CcString& sIfcAppend);

  /**
   * @brief Connect to bus
   * @return True if connection established.
   */
  bool connect();

  /**
   * @brief Disconnect from bus if connected.
   */
  void disconnect();

  /**
   * @brief Query property from bus:
   * @param sProperty:        Name of property to query
   * @param sPathAppend:      Append this path to interface path before query
   * @param sInterfaceAppend: Append this string to target interface before query
   * @return Property values as Dbus Arguments.
   */
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
