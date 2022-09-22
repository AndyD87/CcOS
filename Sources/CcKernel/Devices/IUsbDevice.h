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
#include "IIoDevice.h"
#include "CcVector.h"
#include "CcByteArray.h"
#include "CcEvent.h"
#include "CcBufferList.h"

#define  UsbRequestTarget_DEVICE                       0x00U
#define  UsbRequestTarget_INTERFACE                    0x01U
#define  UsbRequestTarget_ENDPOINT                     0x02U
#define  UsbRequestTarget_MASK                         0x03U

#define  UsbRequestType_STANDARD                          0x00U
#define  UsbRequestType_CLASS                             0x20U
#define  UsbRequestType_VENDOR                            0x40U
#define  UsbRequestType_MASK                              0x60U

#define  UsbRequest_GET_STATUS                             0x00U
#define  UsbRequest_CLEAR_FEATURE                          0x01U
#define  UsbRequest_SET_FEATURE                            0x03U
#define  UsbRequest_SET_ADDRESS                            0x05U
#define  UsbRequest_GET_DESCRIPTOR                         0x06U
#define  UsbRequest_SET_DESCRIPTOR                         0x07U
#define  UsbRequest_GET_CONFIGURATION                      0x08U
#define  UsbRequest_SET_CONFIGURATION                      0x09U
#define  UsbRequest_GET_INTERFACE                          0x0AU
#define  UsbRequest_SET_INTERFACE                          0x0BU
#define  UsbRequest_SYNCH_FRAME                            0x0CU

#define  UsbDescriptorType_DEVICE                           0x01U
#define  UsbDescriptorType_CONFIGURATION                    0x02U
#define  UsbDescriptorType_STRING                           0x03U
#define  UsbDescriptorType_ENDPOINT                         0x05U
#define  UsbDescriptorType_DEVICE_QUALIFIER                 0x06U
#define  UsbDescriptorType_OTHER_SPEED_CONFIGURATION        0x07U
#define  UsbDescriptorType_BOS                              0x0FU

#define UsbFeature_EP_HALT                             0x00U
#define UsbFeature_REMOTE_WAKEUP                       0x01U
#define UsbFeature_TEST_MODE                           0x02U


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
      uint8   uiLength         = sizeof(SDeviceDescriptor);    //< Size of descriptor 18 byte
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
      uint8   uiLength;               //< Size of descriptor in bytes
      uint8   uiDescriptorType;       //< Config Descriptor
      uint16  uiTotalLength;          //< Total length in bytes of data returned
      uint8   uiNumInterfaces;        //< Number of Interfaces
      uint8   uiConfigurationValue;   //< Value to use as an argument to select this configuration
      uint8   uiConfiguration;        //< Index of String Descriptor describing this configuration
      uint8   uiAttributes;           //< D7 Reserved, set to 1. (USB 1.0 Bus Powered)
                                          //< D6 Self Powered
                                          //< D5 Remote Wakeup
                                          //< D4..0 Reserved, set to 0.
      uint8   uiMaxPower;             //< Maximum Power Consumption in 2mA units
      
      SConfigDescriptor() = delete;
      ~SConfigDescriptor() = delete;

      void init()
      {
        uiLength = sizeof(SConfigDescriptor);
        uiDescriptorType = 0x02;
      }
    };
    
    struct CcKernelSHARED SInterfaceDescriptor
    {
      public:
      uint8   uiLength;               //< Size of descriptor in bytes
      uint8   uiDescriptorType;       //< Interface Descriptor
      uint8   uiInterfaceNumber;      //< Number of Interface
      uint8   uiAlternateSetting;     //< Value used to select alternative setting
      uint8   uiNumEndpoints;         //< Number of Endpoints used for this interface
      uint8   uiInterfaceClass;       //< Class Code (Assigned by USB Org)
      uint8   uiInterfaceSubClass;    //< Subclass Code (Assigned by USB Org)
      uint8   uiInterfaceProtocol;    //< Protocol Code (Assigned by USB Org)
      uint8   uiInterfaceStringIdx;         //< Index of String Descriptor Describing this interface

      SInterfaceDescriptor() = delete;
      ~SInterfaceDescriptor() = delete;
      void init()
      {
        uiLength = sizeof(SInterfaceDescriptor);
        uiDescriptorType = 0x04;
      }
    };
    
    struct CcKernelSHARED SInterfaceAssociationDescriptor
    {
      public:
      uint8   uiLength;               //< Size of descriptor in bytes
      uint8   uiDescriptorType;       //< Interface Descriptor
      uint8   uiFirstInterface;       //< Number of Interface
      uint8   uiInterfaceCount;       //< Value used to select alternative setting
      uint8   uiDeviceClass;          //< Number of Endpoints used for this interface
      uint8   uiDeviceSubClass = 0;   //< Sub-Class information
      uint8   uiDeviceProtocol = 0;   //< Protocoll information
      uint8   uiInterfaceStringIdx;   //< Index of String Descriptor Describing this interface

      SInterfaceAssociationDescriptor() = delete;
      ~SInterfaceAssociationDescriptor() = delete;
      void init()
      {
        uiLength = sizeof(SInterfaceAssociationDescriptor);
        uiDescriptorType = 0x0B;
      }
    };
    
    struct CcKernelSHARED SEndpointDescriptor
    {
      public:
      uint8   uiLength;                   //< Size of descriptor in bytes
      uint8   uiDescriptorType;           //< Endpoint Descriptor
      uint8   uiEndpointAddress;          //< Endpoint Address
                                          //< Bits 0..3b Endpoint Number.
                                          //< Bits 4..6b Reserved. Set to Zero
                                          //< Bits 7 Direction 0 = Out, 1 = In (Ignored for Control Endpoints)
      uint8   uiAttributes;               //< Bits 0..1 Transfer Type
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
      uint16  wMaxPacketSize;             //< Maximum Packet Size this endpoint is capable of sending or receiving
      uint8   uInterval;                  //< Interval for polling endpoint data transfers. Value in frame counts. 
                                          //< Ignored for Bulk & Control Endpoints. 
                                          //< Isochronous must equal 1 and field may range from 1 to 255 for interrupt endpoints.
                                          
      SEndpointDescriptor() = delete;
      ~SEndpointDescriptor() = delete;
      void init()
      {
        uiLength = sizeof(SEndpointDescriptor);
        uiDescriptorType = 0x05;
      }
    };
    
    struct CcKernelSHARED SFunctionalDescriptor
    {
      public:
      uint8   uiLength;         //< Size of descriptor in bytes
      uint8   uiDescriptorType; //< Size of descriptor in bytes
      union IUsbDevice
      {
        struct
        {
          uint8 uiRaw[1];
        } Raw;
      } Data;
      
      SFunctionalDescriptor() = delete;
      ~SFunctionalDescriptor() = delete;
      void init(uint8 uiSize)
      {
        uiLength         = uiSize;
        uiDescriptorType = 0x24;
      }
    };

    struct CcKernelSHARED SStringDescriptor
    {
      public:
      uint8   uiLength         = 0x00;    //< Size of descriptor in bytes
      uint8   uiDescriptorType = 0x03;    //< String Descriptor
      uint16 pString[1];                  //< Unicode string described
    };

    struct CcKernelSHARED SRequest
    {
    public:
      uint8_t   bmRequest;
      uint8_t   bRequest;
      uint16_t  wValue;
      uint16_t  wIndex;
      uint16_t  wLength;
      uint8     uiData[7];
    };
  #pragma pack(pop)
  
  enum class CcKernelSHARED EEnpointState
  {
    Idle,
    Setup,
    DataIn,
    DataOut,
    StateIn,
    StateOut,
  };
  
  class CcKernelSHARED CConfigDescriptor
  {
    public:
    CConfigDescriptor();
    ~CConfigDescriptor();

    class CEndpointInfo
    {
      public:
      EEnpointState         eState = EEnpointState::Idle;
      CcByteArray           oInBuffer;
      CcBufferList          oBufferList;
      uint32                uiMaxBufferList=64;
      CcEvent               oOnChange;
      SEndpointDescriptor*  pDescriptor;

      void reset();
    };

    class CcKernelSHARED CInterfaceInfo
    {
      public:
      CcEvent                oOnRequest;
      CcEvent                oOnReadDone;
      SInterfaceDescriptor*  pDescriptor;
    };

    SInterfaceDescriptor* createInterface(const CcEvent& oOnRequest, const CcEvent& oOnReadDone);
    SInterfaceAssociationDescriptor* createInterfaceAssociation(uint8 uiInterfaceCount);
    SEndpointDescriptor* createEndpoint(bool bInOut, uint8 uiAttributes, uint16 wMaxPacketSize, uint8 uInterval, const CcEvent& oOnChange, bool bCreateNewId = true);
    SFunctionalDescriptor* createFunctional(uint8 uiSize);
    void setupInterfaces();

    uint8                     getNextInterfaceId();
    uint8                     getNextEndpointId();
    uint8                     getLastEndpointId();
    SConfigDescriptor*        getConfig()
    { return m_oBuffer.cast<IUsbDevice::SConfigDescriptor>(0);}
    SInterfaceDescriptor*     getInterface(size_t uiIndex)
    { return m_oBuffer.cast<IUsbDevice::SInterfaceDescriptor>(m_oInterfaces[uiIndex]); }
    CInterfaceInfo&           getInterfaceInfo(size_t uiIndex)
    { return m_oInterfaceConfigs[uiIndex]; }
    size_t                    getInterfaceCount() const
    { return m_oInterfaces.size(); }
    SEndpointDescriptor*      getEndpoint(size_t uiIndex)
    { return m_oBuffer.cast<IUsbDevice::SEndpointDescriptor>(m_oEndPoints[uiIndex]); }
    CEndpointInfo&            getEndpointInfo(size_t uiIndex)
    { return m_oEndPointConfigs[uiIndex]; }
    CcVector<CEndpointInfo>&  getEndpointInfos()
    { return m_oEndPointConfigs; }
    size_t                    getEndpointCount() const
    { return m_oEndPoints.size(); }
    SFunctionalDescriptor*    getFunction(size_t uiIndex)
    { return m_oBuffer.cast<IUsbDevice::SFunctionalDescriptor>(m_oEndPoints[uiIndex]); }

    #ifdef _MSC_VER
    //  template class CcVector<IUsbDevice::CConfigDescriptor::CEndpointInfo>;
    #endif

    typedef class CcKernelSHARED CcVector<CEndpointInfo>  CEndpointInfoList;
    typedef class CcKernelSHARED CcVector<CInterfaceInfo> CInterfaceInfoList;
    typedef class CcKernelSHARED CcVector<uint32>         CIdList;

    private:
    CcByteArray               m_oBuffer;
    CIdList                   m_oEndPoints;
    CEndpointInfoList         m_oEndPointConfigs;
    CIdList                   m_oInterfaces;
    CInterfaceInfoList        m_oInterfaceConfigs;
    CIdList                   m_oFunctions;
  };
  
  class CcKernelSHARED CDeviceDescriptor : public SDeviceDescriptor
  {
  public:
    CDeviceDescriptor();
    ~CDeviceDescriptor();

    uint8 findEndpoint(uint8 uiEndpoint, CConfigDescriptor** pConfig = nullptr);
    uint8 findInterface(uint8 uiEndpoint, CConfigDescriptor** pConfig = nullptr);

    CConfigDescriptor& getActiveConfig()
    { return oConfigs[uiActiveConfig]; }
    CcVector<CConfigDescriptor>& getConfigs()
    { return oConfigs; }
    CcVector<SStringDescriptor*>& getStrings()
    { return oStrings; }

    CConfigDescriptor& createConfig();
    IUsbDevice::SStringDescriptor& createString(const CcString& sValue);

    typedef class CcKernelSHARED CcVector<CConfigDescriptor>  CConfigDescriptorList;
    typedef class CcKernelSHARED CcVector<SStringDescriptor*> CStringDescriptorList;
  private:
    CConfigDescriptorList   oConfigs;
    CStringDescriptorList   oStrings;
    size_t uiActiveConfig = SIZE_MAX;
  };

  /**
   * @brief Constructor
   */
  IUsbDevice();

  /**
   * @brief Destructor
   */
  virtual ~IUsbDevice();

  virtual size_t read(uint8 uiEndpoint, uint8* pBuffer, uint16 uiSize) = 0;
  virtual size_t write(uint8 uiEndpoint, const uint8* pBuffer, uint16 uiSize) = 0;
  virtual void stallEp(uint8 uiEndpoint) = 0;
  virtual void ctrlSendStatus() = 0;
  virtual void ctrlReceiveStatus() = 0;

  CDeviceDescriptor& getDeviceDescriptor()
  { return m_oDeviceDescriptor; }
  const CDeviceDescriptor& getDeviceDescriptor() const
  { return m_oDeviceDescriptor; }

  void sendError();
  static void debugMessage(const char* pMessage);
protected:
  CDeviceDescriptor     m_oDeviceDescriptor;
  EEnpointState         m_eEp0State    = EEnpointState::Idle;
  #ifdef HS
    uint16                m_uiEp0MaxSize = 0x200;
  #else
    uint16                m_uiEp0MaxSize = 0x40;
  #endif
};
