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
 * @page      CcHttp
 * @subpage   IHttpPathProvider
 *
 * @page      IHttpPathProvider
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class IHttpPathProvider
 */
#ifndef _IHttpPathProvider_H_
#define _IHttpPathProvider_H_

#include "CcBase.h"
#include "CcHttp.h"
#include "IHttpProvider.h"

/**
 * @brief Example Class impelmentation
 */
class CcHttpSHARED IHttpPathProvider : public IHttpProvider
{
public:
  /**
   * @brief Constructor
   */
  IHttpPathProvider(const CcString& sPath);

  /**
   * @brief Destructor
   */
  virtual ~IHttpPathProvider();

  virtual bool checkResponsible(const CcHttpWorkData &oData) const override;
  
  void setPath(const CcString& sPath)
    { m_sPath = sPath; }
  const CcString& getPath()
    { return m_sPath; }

protected:
  void setCanStartWith( bool bOnOff )
    { m_bCanStartWith = bOnOff;}

private:
  CcString m_sPath;
  bool m_bCanStartWith;
  static CcString m_s404Msg;
};

#endif /* _IHttpPathProvider_H_ */
