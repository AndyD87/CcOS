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
 * @page      CcHttpWebframework
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpWebframework
 **/
#ifndef CcHttpWebframework_H_
#define CcHttpWebframework_H_

#include "CcBase.h"
#include "CcApp.h"
#include "IHttpProvider.h"
#include "CcHttpServer.h"

class CcRestApiApplication;
class CcRestApiSystem;
class IHttpUserControl;

namespace CcHttp
{
namespace Application
{
namespace RestApiWebframework
{
class CcHttpWebframeworkIndex;
/**
 * @brief CcHttpWebframework impelmentation
 *        Main class wich is loaded to start Application.
 */
class CcHttpWebframework : public CcHttpServer
{
public:
  /**
   * @brief Constructor
   */
  CcHttpWebframework(bool bNoUi = s_bNoUiDefault);

  /**
   * @brief Destructor
   */
  virtual ~CcHttpWebframework();

  CcRestApiApplication& getRestApiApplication();
  CcRestApiSystem& getRestApiSystem();
  CcHttpWebframeworkIndex* getIndex();

  void setHttpUserControl(IHttpUserControl* pUserControl);

public: // Member
  static const bool s_bNoUiDefault;

private:
  class CPrivate;
private:
  CPrivate* m_pPrivate = nullptr;  //!< Private data for applicaton.
};
}
}
}
#endif /* CcHttpWebframework_H_ */
