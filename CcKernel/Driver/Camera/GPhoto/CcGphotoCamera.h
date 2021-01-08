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
 * @page      CcNetwork
 * @subpage   CcGphotoCamera
 *
 * @page      CcGphotoCamera
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      https://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcGphotoCamera
 */

#ifndef H_CcGphotoCamera_H_
#define H_CcGphotoCamera_H_

#include "CcBase.h"
#include "CcGphoto.h"
#include "CcMapCommon.h"
#include "Devices/ICamera.h"

class CcGphotoSHARED CcGphotoCamera : public ICamera
{
public:
  CcGphotoCamera(const CcStringPair& oListItem);
  CcGphotoCamera(const CcString& sName, const CcString& sConnection);
  ~CcGphotoCamera();

  virtual CcImageData getImage() override;
  virtual CcString captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement = false) override;

  bool open(const CcStringPair& oListItem)
    { return open(oListItem.getKey(), oListItem.getValue());}
  bool open(const CcString& sName, const CcString& sConnection);
  void close();
  bool isOpen();
  bool capture(CcString& sFolder, CcString& sFile);
  bool fileDownload(const CcString& sFolder, const CcString& sFile, const CcString& sLocal);
  bool fileDelete(const CcString& sFolder, const CcString& sFile);

  static CcStringMap getAvailable();
private:
  class CPrivate;
  CPrivate * m_pPrivate = nullptr;
};

#endif // H_CcGphotoCamera_H_
