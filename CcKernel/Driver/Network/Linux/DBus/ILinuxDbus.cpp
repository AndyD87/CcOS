/**
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcLinuxDbus
 */
#include "ILinuxDbus.h"
#include "CcKernel.h"

#include <iostream>
#include <stdio.h>
#include <dbus/dbus.h>
#include <NetworkManager/NetworkManager.h>

class ILinuxDbus::CPrivate
{
public:
  DBusConnection* pConnection = nullptr;
  DBusError       oError;
};

ILinuxDbus::ILinuxDbus(const CcString &sBus, const CcString &sPath, const CcString &sInterface) :
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

bool ILinuxDbus::call(const CcString& sMethod, const CcLinuxDbusArguments& oArgs, CcLinuxDbusResult& oResult)
{
  CCUNUSED(oArgs);
  bool bSuccess = false;
  DBusMessage* dbus_msg = nullptr;
  DBusMessage* dbus_reply = nullptr;
  if ( nullptr == (dbus_msg = ::dbus_message_new_method_call(getBus().getCharString(),
                                                             getPath().getCharString(),
                                                             getInterface().getCharString(),
                                                             sMethod.getCharString()
                                                            )
                  )
     )
  {
    CCDEBUG("ILinuxDbus::call failed to create method");
  }
  else
  {
    if ( nullptr == ( dbus_reply = dbus_connection_send_with_reply_and_block
                      ( m_pPrivate->pConnection,
                        dbus_msg,
                        DBUS_TIMEOUT_USE_DEFAULT,
                        &m_pPrivate->oError
                      )
                    )
       )
    {
      ::dbus_message_unref(dbus_msg);
      CCERROR(m_pPrivate->oError.name);
      CCERROR(m_pPrivate->oError.message);
    }
    else
    {
      bSuccess = true;
      // Request succeeded, now get the content
      DBusMessageIter oIter;
      if(dbus_message_iter_init (dbus_reply, &oIter))
      {
        int current_type;
        while ((current_type = dbus_message_iter_get_arg_type (&oIter)) != DBUS_TYPE_INVALID)
        {
          switch(current_type)
          {
            case DBUS_TYPE_ARRAY:
            {
              DBusMessageIter oSubIter;
              dbus_message_iter_recurse(&oIter, &oSubIter);
              int array_type = 0;
              while ((array_type = dbus_message_iter_get_arg_type (&oSubIter)) != DBUS_TYPE_INVALID)
              {
                switch(array_type)
                {
                  case DBUS_TYPE_OBJECT_PATH:
                  {
                    char* pPath;
                    dbus_message_iter_get_basic(&oSubIter,&pPath);
                    oResult.append(CcString(pPath));
                    break;
                  }
                  default:
                    break;
                }
                dbus_message_iter_next (&oSubIter);
              }
              break;
            }
            case DBUS_TYPE_OBJECT_PATH:
              break;
            default:
              break;
          }
          dbus_message_iter_next (&oIter);
        }
        //if ( !::dbus_message_get_args(dbus_reply, &m_pPrivate->oError, DBUS_TYPE_ARRAY, DBUS_TYPE_ARRAY, &dbus_result, &dbus_result_len, DBUS_TYPE_INVALID) )
        //{
        //  CCERROR(m_pPrivate->oError.name);
        //  CCERROR(m_pPrivate->oError.message);
        //}
      }
    }
  }
  if(dbus_msg)
  {
    ::dbus_message_unref(dbus_msg);
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
    CCMONITORNEW(m_pPrivate->pConnection);
    CCERROR(m_pPrivate->oError.name);
    CCERROR(m_pPrivate->oError.message);
  }
  else
  {
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
