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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief      Create Access to an USB-Hid-Device through Windows API
 */
#include "USB/WinUSBHid.h"
#include "CcString.h"
#include <stdio.h>

WinUSBHid::WinUSBHid(void) :
  m_DeviceHandle(nullptr)
{
}

WinUSBHid::~WinUSBHid(void)
{
}

void WinUSBHid::GetDeviceCapabilities(void)
{
  PHIDP_PREPARSED_DATA  PreparsedData = {0};
  HIDP_CAPS   hidCaps;          //!< Stored Device-Settings
  //get informations of HID
  if (HidD_GetPreparsedData(m_DeviceHandle, &PreparsedData))
  {
    //extract important informations of HID
    if (CCSUCCESS == HidP_GetCaps(PreparsedData, &hidCaps))
    {
      //get Usage to identify the type of HID
      m_Info.usage = hidCaps.Usage + hidCaps.UsagePage;
      //save buffer size for in- and output to members
      m_Info.m_uiReportInputSize = hidCaps.InputReportByteLength;
      m_Info.m_uiReportOutputSize = hidCaps.OutputReportByteLength;
      wchar_t sTemp[100]; //temporary Buffer for return values
      // Request product String of HID and save to Member
      if (HidD_GetProductString(m_DeviceHandle, sTemp, 100))
      {
        m_Info.m_sProductString = sTemp;
      }
      // Request Manufacturer String of HID and save to Member
      if (HidD_GetManufacturerString(m_DeviceHandle, sTemp, 100))
      {
        m_Info.m_sVendorString = sTemp;
      }
      // Request product Serialnumber of HID and save to Member
      if (HidD_GetSerialNumberString(m_DeviceHandle, sTemp, 100))
      {
        m_Info.m_sSerialString = sTemp;
      }
    }
    //No need for PreparsedData any more, so free the memory it's using.
    HidD_FreePreparsedData(PreparsedData);
  }
}

bool WinUSBHid::connect(void)
{
  BOOL         bSuccess(FALSE);
  BOOL         LastDevice(FALSE);
  uint16              MemberIndex = 0;
  HIDD_ATTRIBUTES     Attributes  = {0, 0, 0, 0};
  DWORD               DeviceUsage;
  DWORD               Length = 0;
  DWORD               Required;
  HANDLE              hDevInfo;

  SP_DEVICE_INTERFACE_DATA          devInfoData;
  PSP_DEVICE_INTERFACE_DETAIL_DATA  detailData=nullptr;
  m_DeviceHandle=INVALID_HANDLE_VALUE; //Baustelle
  HidD_GetHidGuid(&m_HidGuid);  
  
  // API function: SetupDiGetClassDevs
  // Returns: a handle to a device information set for all installed devices.
  // Requires: the GUID returned by GetHidGuid.
  hDevInfo=SetupDiGetClassDevs 
    (&m_HidGuid, 
    nullptr, 
    nullptr, 
    DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
  devInfoData.cbSize = sizeof(devInfoData);
  
  do
  {
    //Get all Devices Plugged in
    bSuccess = SetupDiEnumDeviceInterfaces
      (hDevInfo, 
      0, 
      &m_HidGuid, 
      MemberIndex, 
      &devInfoData);

    if (bSuccess != FALSE)
    {
      // collect Information from opened device
      bSuccess = SetupDiGetDeviceInterfaceDetail(
        hDevInfo,
        &devInfoData,
        nullptr,
        0,
        &Length,
        nullptr);
      if (bSuccess)
      {
        // allocate memory depending on Length requested from Interface Detail
        detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(Length);
        if (detailData != nullptr)
        {
          //Set cbSize in the detailData structure.
          detailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
          //Call the function again, this time passing it the returned buffer size.
          bSuccess = SetupDiGetDeviceInterfaceDetail(
            hDevInfo,
            &devInfoData,
            detailData,
            Length,
            &Required,
            nullptr);
          if (bSuccess)
          {
            // Open a handle to the device.
            m_DeviceHandle = CreateFile(
              detailData->DevicePath,
              0,
              FILE_SHARE_READ | FILE_SHARE_WRITE,
              (LPSECURITY_ATTRIBUTES) nullptr,
              OPEN_EXISTING,
              0,
              nullptr);
            //Set the Size to the number of bytes in the structure.
            Attributes.Size = sizeof(Attributes);
            if (HidD_GetAttributes(m_DeviceHandle, &Attributes))
            {
              //Is it the desired device?
              bSuccess = false;
              if (Attributes.VendorID == m_Info.vid && Attributes.ProductID == m_Info.pid)
              {
                //Both the Vendor ID and Product ID match.
                bSuccess = TRUE;
                //Collect Data for HID-Communication Settings
                GetDeviceCapabilities();
                if (m_Info.usage != 0)
                {
                  DeviceUsage = m_Info.usage;
                  // Check if Device is the required one, like Mouse = Mouse
                  if (m_Info.usage == DeviceUsage)
                  {
                    bSuccess = TRUE;
                  }
                  else
                  {
                    bSuccess = FALSE;
                  }
                }
                if (bSuccess == TRUE)
                {
                  //Target Found, create read- and write-handles to it
                  m_WriteHandle = CreateFile
                    (detailData->DevicePath,
                    GENERIC_WRITE,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    (LPSECURITY_ATTRIBUTES) nullptr,
                    OPEN_EXISTING,
                    0,
                    nullptr);
                  m_ReadHandle = CreateFile
                    (detailData->DevicePath,
                    GENERIC_READ,
                    FILE_SHARE_READ | FILE_SHARE_WRITE,
                    (LPSECURITY_ATTRIBUTES) nullptr,
                    OPEN_EXISTING,
                    0,
                    nullptr);
                }
              } //if (Attributes.ProductID == ProductID && Attributes.VendorID == VendorID)
              else
              {
                CloseHandle(m_DeviceHandle);
              }
            }
          }
        }  //if (Result != 0)
        else
        {
          //SetupDiEnumDeviceInterfaces returned 0, so there are no more devices to check.
          LastDevice = TRUE;
        }
        free(detailData);
      }
      MemberIndex = MemberIndex + 1;
    }
  } while ((LastDevice == FALSE) && (bSuccess == FALSE));
  //Clean up requested Buffers
  SetupDiDestroyDeviceInfoList(hDevInfo);
  if (bSuccess == FALSE)
    return false;
  else
    return true;
}

size_t WinUSBHid::write(const char* cBuffer, size_t iLength)
{
  BOOL  bSuccess(FALSE);
  DWORD dwBytesWritten;
  // @todo chk for invalid_handle
  if(getReportOutputSize() == iLength)
  {
    bSuccess =WriteFile(
        m_WriteHandle,
        cBuffer,
        (DWORD)iLength,
        &dwBytesWritten,
        nullptr);
  }
  if (bSuccess == FALSE)
    return false;
  else
    return true;
}

size_t WinUSBHid::read(char* cBuffer, size_t iLength)
{
  BOOL  bSuccess(FALSE);
  DWORD dwBytesWritten;
  // @todo chk for invalid_handle
  if(getReportInputSize() == iLength)
  {
    bSuccess =ReadFile(
        m_ReadHandle,
        cBuffer,
        (DWORD)iLength,
        &dwBytesWritten,
        nullptr);
  }
  if (bSuccess == FALSE)
    return false;
  else
    return true;
}
