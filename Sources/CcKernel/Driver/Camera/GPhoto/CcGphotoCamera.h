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
 * @brief     Class CcGphotoCamera
 */
#pragma once

#include "CcBase.h"
#include "CcGphoto.h"
#include "CcMapCommon.h"
#include "Devices/ICamera.h"

/**
 * @brief Use externel libgphoto2 and create a camera interface for CcOS.
 */
class CcGphotoSHARED CcGphotoCamera : public ICamera
{
public:
  /**
   * @brief Create camara with name and connection stored as pair.
   *        Available cameras can be queried with getAvailable.
   * @param oListItem: Pair with name as key and connection as value
   */
  CcGphotoCamera(const CcStringPair& oListItem);

  /**
   * @brief Create camera with known name and connection
   *        Available cameras can be queried with getAvailable.
   * @param sName:        Name of camera
   * @param sConnection:  Connection to the camara
   */
  CcGphotoCamera(const CcString& sName, const CcString& sConnection);
  ~CcGphotoCamera();

  virtual CcImageData getImage() override;
  virtual CcString captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement = false) override;

  /**
   * @brief Create camara with name and connection stored as pair.
   *        Available cameras can be queried with getAvailable.
   * @param oListItem: Pair with name as key and connection as value
   * @return True if connection could be opened successfully
   */
  bool open(const CcStringPair& oListItem)
  { return open(oListItem.getKey(), oListItem.getValue());}

  /**
   * @brief Create camera with known name and connection
   *        Available cameras can be queried with getAvailable.
   * @param sName:        Name of camera
   * @param sConnection:  Connection to the camara
   * @return True if connection could be opened successfully
   */
  bool open(const CcString& sName, const CcString& sConnection);

  /**
   * @brief Close camera connection.
   */
  void close();

  /**
   * @brief Check if camera connection is already established.
   * @return True if connection is valid.
   */
  bool isOpen();

  /**
   * @brief Caputre a image from camera and store it in file on camera
   * @param sFolder:  Folder on camera picture was stored
   * @param sFile:    File of picture on camera
   * @return True if capture and sore succeeded.
   */
  bool capture(CcString& sFolder, CcString& sFile);

  /**
   * @brief Download exisiting file from camea
   * @param sFolder:  Folder on camera
   * @param sFile:    File on camera
   * @param sLocal:   Target file on local machine
   * @return True if successfully downloaded
   */
  bool fileDownload(const CcString& sFolder, const CcString& sFile, const CcString& sLocal);

  /**
   * @brief Delete file on camera
   * @param sFolder:  Folder on camera
   * @param sFile:    File on camera
   * @return True if successfully deleted
   */
  bool fileDelete(const CcString& sFolder, const CcString& sFile);

  /**
   * @brief Get a list of available and connected cameras.
   * @return List of cameras with name and connection.
   */
  static CcStringMap getAvailable();
private:
  class CPrivate;
  CPrivate * m_pPrivate = nullptr;
};
