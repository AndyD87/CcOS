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
 * @brief     Class CcApp
 */
#pragma once

#include "CcBase.h"
#include "IThread.h"
#include "CcStringList.h"
#include "CcUuid.h"
#include "CcHandle.h"
#include "CcStatus.h"
#include "CcVersion.h"

class CcApp;

template class CcKernelSHARED CcHandle<CcApp>;

//! Define a handle for applications.
typedef CcHandle<CcApp> CcAppHandle;

/**
 * @brief Every Application should inherit this class to keep executions
 *
 *        Application wich will inherit CcApp will automatically check the
 *        Environmentvariable CCOS_APPS_NO_IO_BUFFERING to disable io buffering
 *        of stdin, stdout and stderr.
 */
class CcKernelSHARED CcApp : public IThread
{
public:
  /**
   * @brief Constructor
   *        Application will be constructed anonymous
   */
  CcApp();

  /**
   * @brief Constructor
   * @param sAppName: Application name
   */
  CcApp(const CcString& sAppName, const CcVersion& oVersion = {}, const CcUuid& oUuid = {});

  /**
   * @brief Destructor
   */
  virtual ~CcApp();

  /**
   * @brief Start Application and wait for execution is done.
   *        Other than start(), it will not create a new thread, because we are waiting on it.
   * @return Exit code wich was set during execution.
   *         For CcXXX applications, this code will be part of EStatus
   */
  virtual CcStatus exec();

  /**
   * @brief Get unique id wich was passed during creation of application
   * @return Unique id
   */
  const CcUuid& getUuid()
    { return m_oId; }

  /**
   * @brief Get Version of current application.
   * @return current Version
   */
  const CcVersion& getVersion() const;

protected:
  /**
   * @brief Update unique id from inheriting class.
   *        Could be useful if application is changing it's type during running.
   * @param oNewUuid: New value to update Uuid with
   * @return void
   */
  inline void setUuid(const CcUuid& oNewUuid)
  { m_oId = oNewUuid; }

  /**
   * @brief Update application name from inheriting class.
   *        Could be useful if application is changing it's name during running.
   * @param sNewName: New value to update Name
   * @return void
   */
  inline void setName(const CcString& sNewName)
  { IThread::setName(sNewName); }

  inline void setVersion(const CcVersion& oVersion)
  { m_oVersion = oVersion; }

private:
  void initApp();

private:
  CcVersion m_oVersion;                    //!< Verstion value of the application
  CcUuid    m_oId;                         //!< Unique Id for identify application
};
