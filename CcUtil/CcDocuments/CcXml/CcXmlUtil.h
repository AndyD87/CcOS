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
 * @subpage   CcXmlUtil
 *
 * @page      CcXmlUtil
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Class CcXmlUtil
 *            This Class is a Static Class. It is used for Operations on XmlNodes.
 **/
#ifndef H_CcXmlUtil_H_
#define H_CcXmlUtil_H_

#include "CcDocument.h"
#include "CcBase.h"
#include "CcXmlNode.h"

/**
 * @brief Class implementation
 */
class CcDocumentsSHARED CcXmlUtil 
{
public:
  static bool getBoolFromNodeValue(const CcXmlNode& oNode, bool bSetIfInvalid = false, bool* pbOk = nullptr);
  static const CcString& getStringFromBool(bool bSetIfInvalid);
};

#endif // H_CcXmlUtil_H_
