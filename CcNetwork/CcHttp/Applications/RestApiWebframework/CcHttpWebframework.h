/*
 * This file is part of CcHttpWebframework.
 *
 * CcHttpWebframework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcHttpWebframework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcHttpWebframework.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @file
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpWebframework
 **/
#pragma once

#include "CcBase.h"
#include "CcApp.h"
#include "IHttpProvider.h"
#include "CcHttpServer.h"

class CcRestApiApplication;
class CcRestApiSystem;
class IHttpUserControl;

namespace NHttp
{
namespace Application
{
namespace RestApiWebframework
{
class CcHttpWebframeworkIndex;
/**
 * @brief CcHttpWebframework implementation
 *        Main class wich is loaded to start Application.
 */
class CcHttpSHARED CcHttpWebframework : public CcHttpServer
{
public:
  /**
   * @brief Constructor
   */
  CcHttpWebframework(CcHttpServerConfig* pConfig, bool bNoUi = s_bNoUiDefault);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpWebframework();

  //! @return Get handle of rest api application
  CcRestApiApplication& getRestApiApplication();
  //! @return Get handle of rest api system
  CcRestApiSystem& getRestApiSystem();

  //! @return Get index of http framework
  CcHttpWebframeworkIndex* getIndex();

  /**
   * @brief Overwrite default user control with custem user control
   * @param pUserControl: User control to use now
   */
  void setHttpUserControl(IHttpUserControl* pUserControl);

public: // Member
  //! If false no html data needs to be created
  static const bool s_bNoUiDefault;

private:
  class CPrivate;
private:
  CPrivate* m_pPrivate = nullptr;  //!< Private data for applicaton.
};
}
}
}
