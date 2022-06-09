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
 * @brief     Create Access to an USB-Hid-Device
 */
#pragma once

#include "CcBase.h"
#include "CcString.h"
#include "IDevice.h"
#include "CcVector.h"

/**
 * @brief Class for communication with a USB-HIDevice
 *        Informations from https://www.beyondlogic.org/usbnutshell/usb5.shtml
 */
class CcKernelSHARED IUsbDevice : public IDevice
{
public:
  enum class EType
  {
    Hid = 0,
  };

  #pragma pack(push, 1)
    struct CcKernelSHARED SDeviceDescriptor
    {
      public:
      uint8   uiLength         = 0x12;    //< Size of descriptor 18 byte
      uint8   uiDescriptorType = 0x01;    //< Device Descriptor
      uint16  uiBcd            = 0;       //< USB Specification Number which device complies too
      uint8   uiDeviceClass    = 0xff;    //< Class Code
                                          //<  - 0x00=Each interface specifies it's own device code
                                          //<  - 0xff=The class is vendor specific
      uint8   uiDeviceSubClass = 0;       //< Sub-Class information
      uint8   uiDeviceProtocol = 0;       //< Protocoll information
      uint8   uiMaxPacketSize = 0;        //< Maximum Packet Size of value 8, 16, 32 or 64
      uint16  uiVendorId = 0;             //< Vendor ID assigned by USB Org
      uint16  uiProductId = 0;            //< Product ID assigned by Manufacturer
      uint16  uiBcdDevice = 0;            //< Device release number
      uint8   uiManufacturerStringIdx = 0;//< Index of Manufacturer String Descriptor
      uint8   uiProductStringIdx = 0;     //< Index of Product String Descriptor
      uint8   uiSerialNumberStringIdx = 0;//< Index of Serial Number String Descriptor
      uint8   uiNumConfigurations = 0;    //< Number of Possible Configurations
    };
    
    struct CcKernelSHARED SConfigDescriptor
    {
      public:
      uint8   uiLength = 0;               //< Size of descriptor in bytes
      uint8   uiDescriptorType = 0x02;    //< Config Descriptor
      uint16  uiTotalLength    = 0;       //< Total length in bytes of data returned
      uint8   uiNumInterfaces  = 0xff;    //< Number of Interfaces
      uint8   uiConfigurationValue = 0;   //< Value to use as an argument to select this configuration
      uint8   uiConfiguration = 0;        //< Index of String Descriptor describing this configuration
      uint8   uiAttributes = 0;           //< D7 Reserved, set to 1. (USB 1.0 Bus Powered)
                                          //< D6 Self Powered
                                          //< D5 Remote Wakeup
                                          //< D4..0 Reserved, set to 0.
      uint8   uiMaxPower = 0;             //< Maximum Power Consumption in 2mA units
    };
    
    struct CcKernelSHARED SInterfaceDescriptor
    {
      public:
      uint8   uiLength         = 0x00;    //< Size of descriptor in bytes
      uint8   uiDescriptorType = 0x04;    //< Interface Descriptor
      uint8   uiInterfaceNumber = 0;      //< Number of Interface
      uint8   uiAlternateSetting = 0;     //< Value used to select alternative setting
      uint8   uiNumEndpoints = 0;         //< Number of Endpoints used for this interface
      uint8   uiInterfaceClass = 0;       //< Class Code (Assigned by USB Org)
      uint8   uiInterfaceSubClass = 0;    //< Subclass Code (Assigned by USB Org)
      uint8   uiInterfaceProtocol = 0;    //< Protocol Code (Assigned by USB Org)
      uint8   uiInterfaceIdx = 0;         //< Index of String Descriptor Describing this interface
    };
    
    struct CcKernelSHARED SEndpointDescriptor
    {
      public:
      uint8   uiLength         = 0x00;    //< Size of descriptor in bytes
      uint8   uiDescriptorType = 0x05;    //< Endpoint Descriptor
      uint8   uiEndpointAddress = 0;      //< Endpoint Address
                                          //< Bits 0..3b Endpoint Number.
                                          //< Bits 4..6b Reserved. Set to Zero
                                          //< Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)
      uint8   uiAttributes = 0;           //< Bits 0..1 Transfer Type
                                          //<   00 = Control
                                          //<   01 = Isochronous
                                          //<   10 = Bulk
                                          //<   11 = Interrupt
                                          //< Bits 2..7 are reserved. If Isochronous endpoint,
                                          //< Bits 3..2 = Synchronisation Type (Iso Mode)
                                          //<   00 = No Synchonisation
                                          //<   01 = Asynchronous
                                          //<   10 = Adaptive
                                          //<   11 = Synchronous
                                          //< Bits 5..4 = Usage Type (Iso Mode)
                                          //<   00 = Data Endpoint
                                          //<   01 = Feedback Endpoint
                                          //<   10 = Explicit Feedback Data Endpoint
                                          //<   11 = Reserved
      uint16  wMaxPacketSize = 0;         //< Maximum Packet Size this endpoint is capable of sending or receiving
      uint8   uInterval = 0;              //< Interval for polling endpoint data transfers. Value in frame counts. 
                                          //< Ignored for Bulk & Control Endpoints. 
                                          //< Isochronous must equal 1 and field may range from 1 to 255 for interrupt endpoints.
    };
    
    struct CcKernelSHARED SStringDescriptor
    {
      public:
      uint8   uiLength         = 0x00;    //< Size of descriptor in bytes
      uint8   uiDescriptorType = 0x03;    //< String Descriptor
      char    pString[3];                  //< String described
    };
  #pragma pack(pop)
  class CcKernelSHARED CDeviceDescriptor : public SDeviceDescriptor
  {
    public:
    class CcKernelSHARED CConfigDescriptor : public SConfigDescriptor
    {
      public:
      CConfigDescriptor();
      ~CConfigDescriptor();
      class CcKernelSHARED CInterfaceDescriptor : public SInterfaceDescriptor
      {
        public:
        class CcKernelSHARED CEndpointDescriptor : public SEndpointDescriptor
        {
          public:
          CEndpointDescriptor();
          ~CEndpointDescriptor();
        };
        CInterfaceDescriptor();
        ~CInterfaceDescriptor();

        private:
        CcVector<CEndpointDescriptor> oEndPoints;
      };
      
      private:
      CcVector<CInterfaceDescriptor> oInterfaces;
    };
    
    class CcKernelSHARED CStringDescriptor : public SStringDescriptor
    {
      public:
      CStringDescriptor();
      ~CStringDescriptor();
    };


    CDeviceDescriptor(uint16 uiVid, uint16 uiPid);
    ~CDeviceDescriptor();

  private:
    CcVector<CConfigDescriptor> oConfigs;
    CcVector<SStringDescriptor*> oStrings;
  };

  /**
   * @brief Constructor
   */
  IUsbDevice();

  /**
   * @brief Destructor
   */
  virtual ~IUsbDevice();

  virtual CcStatus loadDeviceDescriptor(const CDeviceDescriptor& oDescriptor) = 0;
};
