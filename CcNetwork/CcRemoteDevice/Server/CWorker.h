/*
 * This file is part of CWorker.
 *
 * CWorker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CWorker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CWorker.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CWorker
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CWorker
 **/
#ifndef H_CWorker_H_
#define H_CWorker_H_

#include "CcRemoteDevice.h"
#include "CcByteArray.h"
#include "IWorker.h"

class CcRemoteDeviceServer;

namespace NRemoteDeviceServer
{

/**
 * @brief CWorker implementation
 *        Main class wich is loaded to start Application.
 */
class CcRemoteDeviceSHARED CWorker : public IWorker
{
public:
  /**
   * @brief Constructor
   */
  CWorker(CcRemoteDeviceServer* pServer):
    m_pServer(pServer),
    m_oData(1024)
  { CCUNUSED(m_pServer); }

  /**
   * @brief Destructor
   */
  virtual ~CWorker() = default;

  virtual void run() override;

  CcByteArray& getData()
  { return m_oData; }

private:
  CcRemoteDeviceServer* m_pServer;
  CcByteArray           m_oData;
};

}

#endif // H_CWorker_H_
