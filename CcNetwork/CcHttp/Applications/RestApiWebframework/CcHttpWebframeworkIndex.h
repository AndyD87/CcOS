/*
 * This file is part of CcHttpWebframeworkIndex.
 *
 * CcHttpWebframeworkIndex is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CcHttpWebframeworkIndex is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with CcHttpWebframeworkIndex.  If not, see <http://www.gnu.org/licenses/>.
 **/
/**
 * @page      CcHttpWebframeworkIndex
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://coolcow.de
 * @par       Language   C++ ANSI V3
 * @brief     Class CcHttpWebframeworkIndex
 **/
#ifndef H_CcHttpWebframeworkIndex_H_
#define H_CcHttpWebframeworkIndex_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpProvider.h"
#include "CcStringList.h"

namespace CcHttp
{
namespace Application
{
namespace RestApiWebframework
{
  class CcHttpWebframework;
/**
 * @brief CcHttpWebframeworkIndex implementation
 *        Main class wich is loaded to start Application.
 */
class CcHttpSHARED CcHttpWebframeworkIndex : public IHttpProvider
{
public:
  /**
   * @brief Constructor
   */
  CcHttpWebframeworkIndex();

  /**
   * @brief Destructor
   */
  virtual ~CcHttpWebframeworkIndex();

  virtual bool checkResponsible(const CcHttpWorkData& oData) const override;
  virtual CcStatus execGet(CcHttpWorkData& oData) override;

  void setTitle(const CcString& sTitle)
  {
    m_sTitle = sTitle;
  }
  void addScript(const CcString& sScript)
  {
    m_oScripts.append(sScript);
  }
  void addLoadableScript(const CcString& sScript)
  {
    m_oLoadableScripts.append(sScript);
  }
  void addStylesheet(const CcString& sStylesheet)
  {
    m_oStylesheets.append(sStylesheet);
  }

private:
  class CPrivate;
private:
  CPrivate*     m_pPrivate  = nullptr;  //!< Private data for applicaton.
  CcString      m_sTitle    = "CcOSWebframework";
  CcStringList  m_oScripts;
  CcStringList  m_oLoadableScripts;
  CcStringList  m_oStylesheets;
};
}
}
}
#endif // H_CcHttpWebframeworkIndex_H_
