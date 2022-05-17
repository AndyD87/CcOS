/**
 * @copyright  Andreas Dirmeier (C) 2022
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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxDbus
 */
#include "ILinuxDbus.h"
#include "CcKernel.h"
#include "CcList.h"

#include <iostream>
#include <stdio.h>
#include <dbus/dbus.h>

class ILinuxDbus::CPrivate
{
public:

  static bool resultToArg(DBusMessageIter oIter, CcVariantList& oResult)
  {
    bool bSuccess = true;
    int current_type;
    while (bSuccess == true &&
           (current_type = dbus_message_iter_get_arg_type (&oIter)) != DBUS_TYPE_INVALID)
    {
      switch(current_type)
      {
        case DBUS_TYPE_ARRAY:
        {
          DBusMessageIter oSubIter;
          dbus_message_iter_recurse(&oIter, &oSubIter);
          CcVariantList oVariantList;
          bSuccess = resultToArg(oSubIter, oVariantList);
          oResult.append(CcVariant(oVariantList));
          break;
        }
        case DBUS_TYPE_VARIANT:
        {
          DBusMessageIter oSubIter;
          dbus_message_iter_recurse(&oIter, &oSubIter);
          bSuccess = resultToArg(oSubIter, oResult);
          break;
        }
        case DBUS_TYPE_UINT32:
        {
          uint32 uiValue;
          dbus_message_iter_get_basic(&oIter, &uiValue);
          oResult.append(CcVariant(uiValue));
          break;
        }
        case DBUS_TYPE_BYTE:
        {
          char pPath;
          dbus_message_iter_get_basic(&oIter,&pPath);
          oResult.append(CcVariant(pPath));
          break;
        }
        case DBUS_TYPE_OBJECT_PATH:
          CCFALLTHROUGH;
        case DBUS_TYPE_STRING:
        {
          char* pPath;
          dbus_message_iter_get_basic(&oIter,&pPath);
          oResult.append(CcString(pPath));
          break;
        }
        default:
          CCDEBUG("Unknown dbus type");
          bSuccess = false;
          break;
      }
      dbus_message_iter_next (&oIter);
    }
    return bSuccess;
  }

  DBusConnection* pConnection = nullptr;
  DBusError       oError;
};

ILinuxDbus::ILinuxDbus(const CcString& sBus, const CcString& sPath, const CcString& sInterface) :
  m_sBus(sBus),
  m_sPath(sPath),
  m_sInterface(sInterface)
{
  CCNEW(m_pPrivate, CPrivate);
}

ILinuxDbus::~ILinuxDbus()
{
  disconnect();
  CCDELETE(m_pPrivate);
}

bool ILinuxDbus::call(const CcString& sMethod, const CcLinuxDbusArguments& oArgs, CcLinuxDbusArguments& oResult)
{
  return callInt(sMethod, oArgs, oResult, getBus(), getPath(), getInterface());
}

bool ILinuxDbus::call(const CcString& sMethod,
          const CcLinuxDbusArguments& oArgs,
          CcLinuxDbusArguments& oResult,
          const CcString& sPathAppend,
          const CcString& sIfcAppend)
{
  CcString sPath = getPath();
  sPath.appendPath(sPathAppend);
  CcString sInterface = getInterface();
  sInterface.append(sIfcAppend);
  return callInt(sMethod, oArgs, oResult, getBus(), sPath, sInterface);
}

bool ILinuxDbus::callInt(const CcString& sMethod,
          const CcLinuxDbusArguments& oArgs,
          CcLinuxDbusArguments& oResult,
          const CcString& sBus,
          const CcString& sPath,
          const CcString& sIfc)
{
  CCUNUSED(oArgs);
  bool bSuccess = false;
  DBusMessage* dbus_msg = nullptr;
  DBusMessage* dbus_reply = nullptr;
  DBusMessageIter oIter;
  CcList<DBusMessageIter> oIterList;
  if ( nullptr == (dbus_msg = ::dbus_message_new_method_call(sBus.getCharString(),
                                                             sPath.getCharString(),
                                                             sIfc.getCharString(),
                                                             sMethod.getCharString()
                                                            )
                  )
     )
  {
    CCDEBUG("ILinuxDbus::call failed to create method");
  }
  else
  {
    bSuccess = true;
    for(const CcVariant& oArg : oArgs)
    {
      oIterList.appendDefault();
      dbus_message_iter_init_append(dbus_msg, &oIterList.last());
      switch (oArg.getType())
      {
        case CcVariant::EType::String:
        {
          const char* pString = oArg.getStringRef().getCharString();
          if (!dbus_message_iter_append_basic(&oIterList.last(), DBUS_TYPE_STRING, &pString))
          {
            bSuccess = false;
          }
          break;
        }
        default:
          bSuccess = false;
          break;
      }
    }
    if (  bSuccess &&
          nullptr == ( dbus_reply = dbus_connection_send_with_reply_and_block
                      ( m_pPrivate->pConnection,
                        dbus_msg,
                        DBUS_TIMEOUT_USE_DEFAULT,
                        &m_pPrivate->oError
                      )
                    )
       )
    {
      bSuccess = false;
      CCERROR(m_pPrivate->oError.name);
      CCERROR(m_pPrivate->oError.message);
    }
    else
    {
      // Request succeeded, now get the content
      if(dbus_reply &&
         dbus_message_iter_init (dbus_reply, &oIter))
      {
        bSuccess = CPrivate::resultToArg(oIter, oResult);
      }
      else
      {
        bSuccess = false;
      }
    }
  }
  if(dbus_msg)
  {
    dbus_message_unref(dbus_msg);
  }
  return bSuccess;
}

bool ILinuxDbus::connect()
{
  bool bSuccess = false;
  if(m_pPrivate->pConnection)
  {
    disconnect();
  }
  // Connect to D-Bus
  if ( nullptr == (m_pPrivate->pConnection = dbus_bus_get(DBUS_BUS_SYSTEM, &m_pPrivate->oError)) )
  {
    CCERROR(m_pPrivate->oError.name);
    CCERROR(m_pPrivate->oError.message);
  }
  else
  {
    CCMONITORNEW(m_pPrivate->pConnection);
    bSuccess = true;
  }
  return bSuccess;
}

void ILinuxDbus::disconnect()
{
  if(m_pPrivate->pConnection)
  {
    dbus_connection_unref(m_pPrivate->pConnection);
    CCMONITORDELETE(m_pPrivate->pConnection);
    m_pPrivate->pConnection = nullptr;
  }
}

CcLinuxDbusArguments ILinuxDbus::property(const CcString& sProperty,
                                          const CcString& sInterfaceAppend,
                                          const CcString& sPathAppend)
{
  CcLinuxDbusArguments oArgs;
  CcString sInterface = getInterface();
  sInterface.append(sInterfaceAppend);
  oArgs.append(sInterface);
  oArgs.append(sProperty);
  CcLinuxDbusArguments oResult;
  CcString sPath = getPath();
  if(sPathAppend.startsWith(sPath))
  {
    sPath.appendPath(sPathAppend.substr(sPath.length()));
  }
  else
  {
    sPath.appendPath(sPathAppend);
  }
  callInt("Get", oArgs, oResult, getBus(), sPath, "org.freedesktop.DBus.Properties");
  return oResult;
}
