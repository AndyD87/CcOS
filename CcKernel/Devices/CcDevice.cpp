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
 * @brief     Implementation of class CcDevice
 */

#include "Devices/CcDevice.h"
#include "Devices/CcDeviceCamera.h"
#include "Devices/CcDeviceClock.h"
#include "Devices/CcDeviceCpu.h"
#include "Devices/CcDeviceDisplay.h"
#include "Devices/CcDeviceEeprom.h"
#include "Devices/CcDeviceGpioPin.h"
#include "Devices/CcDeviceGpioPort.h"
#include "Devices/CcDeviceGrayscaleMap.h"
#include "Devices/CcDeviceHdd.h"
#include "Devices/CcDeviceI2C.h"
#include "Devices/CcDeviceIoDevice.h"
#include "Devices/CcDeviceLed.h"
#include "Devices/CcDeviceNetwork.h"
#include "Devices/CcDevicePixMap.h"
#include "Devices/CcDeviceSdCard.h"
#include "Devices/CcDeviceSpi.h"
#include "Devices/CcDeviceTimer.h"
#include "Devices/CcDeviceTouch.h"
#include "Devices/CcDeviceUart.h"
#include "Devices/CcDeviceUsb.h"
#include "Devices/CcDeviceUsbHid.h"
#include "Devices/CcDeviceWlan.h"
#include "Devices/CcDeviceWlanAccessPoint.h"
#include "Devices/CcDeviceWlanClient.h"

