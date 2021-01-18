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
 * @brief     Class CcRestApiApplicationStatus
 */
#ifndef H_CcRestApiApplicationStatus_H_
#define H_CcRestApiApplicationStatus_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IRestApi.h"
#include "CcListCommon.h"
#include "CcMapCommon.h"

class CcString;
class CcRestApiApplication;

/**
 * @brief CcRestApiApplicationStatus implementation
 */
class CcHttpSHARED CcRestApiApplicationStatus : public IRestApi
{
public:
  /**
   * @brief Status publisher interface, to append an status to http overview page.
   */
  class CcHttpSHARED IPublisher
  {
  public:
    virtual ~IPublisher() = default;
    /**
     * @brief Generate a CcStringMap with Key as Title and Value as output of status
     * @return Generated StringMap
     */
    virtual CcStringMap getStatus() = 0;
  };

  /**
   * @brief Constructor
   */
  CcRestApiApplicationStatus(CcRestApiApplication* pParent);

  /**
   * @brief Destructor
   */
  virtual ~CcRestApiApplicationStatus();

  virtual bool get(CcHttpWorkData& oData) override;

  void appendPublisher(IPublisher* pPublisher)
    { m_oPublishers.append(pPublisher); }
private:
  CcVoidList m_oPublishers;
};

#endif // H_CcRestApiApplicationStatus_H_
