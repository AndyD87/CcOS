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
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of class CcWindowsNetworkDevice
 */
#include "CcWindowsNetworkDevice.h"
#include "CcWindowsNetworkStack.h"
#include "CcWmiInterface.h"

CcWindowsNetworkDevice::CcWindowsNetworkDevice(CcWindowsNetworkStack* pParent, uint32 uiSystemDeviceId):
  m_pParent(pParent),
  m_uiSystemDeviceId(uiSystemDeviceId)
{
  refreshInterfaces();
}

bool CcWindowsNetworkDevice::isConnected()
{
  return false;
}

void CcWindowsNetworkDevice::refreshInterfaces()
{
  // Get all devices
  CcString sQuery = "SELECT * FROM Win32_NetworkAdapter WHERE DeviceID = " + CcString::fromNumber(m_uiSystemDeviceId);
  if (m_pParent->getWmiInterface().open())
  {
    CcWmiResult oResult = m_pParent->getWmiInterface().query(sQuery);
    if(oResult.size() == 1)
    {
      CcTableRow& oRow = oResult.at(0);
      // Verify correct IP
      uint32 uiId = oRow["DeviceID"].getUint32();
      if (uiId == m_uiSystemDeviceId)
      {
        CcString sMac = oRow["MACAddress"].getString();
        m_oAddress.setMac(sMac);
        m_sName = oRow["NetConnectionID"].getString();
      }
      sQuery = "SELECT * FROM Win32_NetworkAdapterConfiguration WHERE Index = " + CcString::fromNumber(m_uiSystemDeviceId);
      oResult = m_pParent->getWmiInterface().query(sQuery);
      if (oResult.size() == 1)
      {
        CcTableRow& oConfigRow = oResult.at(0);
        bool bIpEnabled = oConfigRow["IPEnabled"].getBool();
        if (bIpEnabled)
        {
          CcVariantList oIpAddresses = oConfigRow["IPAddress"].getVariantList();
          CcVariantList oSubnets = oConfigRow["IPSubnet"].getVariantList();
          if (oIpAddresses.size() > 0 &&
              oIpAddresses.size() == oSubnets.size())
          {
            CcVariantList::iterator oIterator = oSubnets.begin();
            for (CcVariant& oIpAddress : oIpAddresses)
            {
              CcIpInterface oInterface;
              if (oInterface.oIpAddress.setIp(oIpAddress.getString()))
              {
                oInterface.pDevice = this;
                oInterface.setSubnet(oIterator->getString());
                m_oInterfaces.add(oInterface);
              }
              oIterator++;
            }
          }
        }
      }
    }
  }
}
