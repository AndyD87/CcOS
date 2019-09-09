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
 * @page      CcKernel
 * @subpage   CcApp
 *
 * @page      CcApp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcApp
 */
#ifndef H_CCAPPLICATION_H_
#define H_CCAPPLICATION_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "IThread.h"
#include "CcStringList.h"
#include "CcKernelBase.h"
#include "CcUuid.h"
#include "CcHandle.h"
#include "CcStatus.h"

class CcVersion;
class CcApp;

#ifdef _MSC_VER
template class CcKernelSHARED CcHandle<CcApp>;
#endif

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
  CcApp(const CcString& sAppName);

  /**
   * @brief Constructor
   * @param sAppName: Application name
   * @param oUuid: Uuid for unique identfication if application has one.
   */
  CcApp(const CcString& sAppName, const CcUuid& oUuid);

  /**
   * @brief Destructor
   */
  virtual ~CcApp();

  /**
   * @brief Get Version of current application.
   * @return current Version
   */
  virtual CcVersion getVersion() const;

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

private:
  void initApp();

private:
  CcUuid m_oId;                         //!< Unique Id for identify application
};

#endif // H_CcAPPLICATION_H_
