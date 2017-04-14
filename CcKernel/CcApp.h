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
 * @page      CcApp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcApp
 */
#ifndef CCAPPLICATION_H_
#define CCAPPLICATION_H_

#include "CcBase.h"
#include "CcKernelBase.h"
#include "CcThreadObject.h"
#include "CcStringList.h"
#include "CcKernelBase.h"
#include "CcUuid.h"
#include "CcHandle.h"

// forward declaration
class CcApp;

/**
 * @brief Every Application should inherit this class to keep executions
 *
 */
class CcKernelSHARED CcApp : public CcThreadObject
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
   * @brief Exit Code of application can updated from external and internal.
   * @param iExitCode: new exit code. Preferd values should come from EError
   */
  inline void setExitCode(int32 iExitCode)
    { m_iExitCode = iExitCode; }

  /**
   * @brief Get Exit Code wich is currently stored in application.
   *
   * @return int32
   */
  int32 getExitCode() const
    { return m_iExitCode; }

  /**
   * @brief Start Application and wait for execution is done.
   *        Other than start(), it will not create a new thread, because we are waiting on it.
   * @return Exit code wich was set during execution.
   *         For CcXXX applications, this code will be part of EError
   */
  uint32 exec(void);

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
    { CcThreadObject::setName(sNewName); }

private:
  CcUuid m_oId;                         //!< Unique Id for identify application
  int32 m_iExitCode = 0;                //!< Exit code wich will be returned if application ends
};

#ifdef WIN32
template class CcKernelSHARED CcHandle<CcApp>;
#endif
typedef class CcHandle<CcApp> CcAppHandle;

#endif /* CCAPPLICATION_H_ */
