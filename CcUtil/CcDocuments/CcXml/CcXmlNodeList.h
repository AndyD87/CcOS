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
 * @page      CcXml
 * @subpage   CcXmlNodeList
 *
 * @page      CcXmlNodeList
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web: http://adirmeier.de/CcOS
 * @version   0.01
 * @date      2016-04
 * @par       Language   C++ ANSI V3
 * @brief     Class CcXmlNodeList
 */
#ifndef CcXmlNodeList_H_
#define CcXmlNodeList_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcList.h"
#include "CcXmlNode.h"

#ifdef WIN32
template class CcDocumentsSHARED CcList<CcXmlNode>;
#endif

/**
 * @brief Example Class impelmentation
 */
class CcDocumentsSHARED CcXmlNodeList : public CcList<CcXmlNode>
{
public:
  /**
   * @brief Constructor
   */
  CcXmlNodeList( void );

  /**
   * @brief Destructor
   */
  virtual ~CcXmlNodeList( void );
};

#endif /* CcXmlNodeList_H_ */
