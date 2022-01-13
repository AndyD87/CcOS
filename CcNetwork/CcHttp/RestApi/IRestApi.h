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
 * @brief     Class IRestApi
 */
#pragma once

#include "CcBase.h"
#include "CcHttp.h"
#include "CcString.h"
#include "CcStringList.h"
#include "CcVector.h"

class CcHttpWorkData;

class IRestApi;
class IHttpUser;

#ifdef _MSC_VER
template class CcHttpSHARED CcVector<IRestApi*>;
#endif

/**
 * @brief IRestApi implementation
 */
class CcHttpSHARED IRestApi
{
public:
  /**
   * @brief Constructor
   */
  IRestApi(IRestApi* pParent, const CcString& sPath);

  /**
   * @brief Destructor
   */
  virtual ~IRestApi();

  /**
   * @brief Execute a get request, it is called from execPath if not filtered
   *        by inherited class.
   *        If no implementation done, request will be forwarded to @ref custom
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool get(CcHttpWorkData& oData);

  /**
   * @brief Execute a post request, it is called from execPath if not filtered
   *        by inherited class.
   *        If no implementation done, request will be forwarded to @ref custom
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool post(CcHttpWorkData& oData);

  /**
   * @brief Execute a put request, it is called from execPath if not filtered
   *        by inherited class.
   *        If no implementation done, request will be forwarded to @ref custom
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool put(CcHttpWorkData& oData);

  /**
   * @brief Execute a del request, it is called from execPath if not filtered
   *        by inherited class.
   *        If no implementation done, request will be forwarded to @ref custom
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool del(CcHttpWorkData& oData);

  /**
   * @brief Execute a patch request, it is called from execPath if not filtered
   *        by inherited class.
   *        If no implementation done, request will be forwarded to @ref custom
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool patch(CcHttpWorkData& oData);

  /**
   * @brief Final call if no matching method handler was found or implemented.
   * @param[in,out] oData: Incoming data and data to fill for transmit.
   * @return True if Packet was handled successfully
   */
  virtual bool custom(CcHttpWorkData& oData);

  /**
   * @brief This method can be overloaded to check user authorization.
   *        It will be forwarded to parent until first implementation.
   *        The target method handles access to all it's child elements.
   * @param oData: Work data to determine user informations from.
   * @return True if user has the authorization to continue.
   */
  virtual bool checkAuth(CcHttpWorkData& oData);

  /**
   * @brief This method can be overloaded to get access to current user interface
   *        if available.
   *        It will be forwarded to parent until first implementation.
   * @param oData: Data from http request to determine correct user.
   * @return Current user or null if user could not be detcted.
   */
  virtual IHttpUser* getUser(CcHttpWorkData& oData);

  /**
   * @brief Execute a receive packet. The path is already extracted to a string list and
   *        can easily be compared for every subfolder.
   * @param oPath:  List of folders in path. It will be reduced by one on next child.
   * @param oData:  Http data to work on
   * @return True if packet was successfully excuted.
   */
  virtual bool execPath(CcStringList& oPath, CcHttpWorkData& oData);

  /**
   * @brief If user authorization is required, overload this method and return true.
   * @return True if auth is required and @ref checkAuth will be called
   */
  virtual bool isAuthRequired();

  //! @return Get parent node in api, if null we are on root node.
  IRestApi* getParent() const
  { return m_pParent; }
  //! @return Get path of this object for parent to determin the responsibilty.
  const CcString& getPath() const
  { return m_sPath; }
  /**
   * @brief Search in children list for path.
   * @param sPath: Path to search fo.
   * @return Next found provier or nullptr if not found
   */
  IRestApi* getProvider(const CcString& sPath);
  
  //! @param sPath: Path to set for this object
  void setPath(const CcString& sPath)
  { m_sPath = sPath; }

  /**
   * @brief Generate list of all childs and write it to client.
   * @param oData: Http data to work on.
   */
  void sendList(CcHttpWorkData& oData);

  /**
   * @brief Send method not found error.
   * @param oData: Data to work on.
   */
  void sendMethodNotFound(CcHttpWorkData& oData);

  /**
   * @brief Send auth required error.
   * @param oData: Data to work on.
   */
  void sendAuthRequired(CcHttpWorkData& oData);

  /**
   * @brief Add child to this object
   * @param pChild: Child to add.
   */
  void appendProvider(IRestApi* pChild)
  { m_oChilds.append(pChild); }

  /**
   * @brief Remove existing provieder by handle.
   * @param pChild: Child to search for.
   * @return True if provider was found and remvoed
   */
  bool removeProvider(IRestApi* pChild)
  { return m_oChilds.removeItem(pChild); }

protected:
  //! @return Get current list of chileds
  CcVector<IRestApi*>& getChilds()
  { return m_oChilds; }

private:
  IRestApi*           m_pParent = nullptr;
  CcString            m_sPath;
  CcVector<IRestApi*> m_oChilds;
};
