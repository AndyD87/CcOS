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
 * @page      CcGui
 * @subpage   CcQt
 *
 * @page      CcQt
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Helper tools
 */
#ifndef H_CcQt_H_
#define H_CcQt_H_

#define ToQPoint(CCPOINT)   QPoint(CCPOINT.getX(), CCPOINT.getY())
#define ToQSize(CCSIZE)     QSize(CCSIZE.getWidth(), CCSIZE.getHeight())
#define ToQString(CCSTRING) QString::fromUtf8(CCSTRING.getCharString(), static_cast<int>(CCSTRING.length()))

#define ToCcPoint(QPOINT)   CcPoint(QPOINT.x(), QPOINT.y())
#define ToCcSize(QSIZE)     CcSize(QSIZE.width(), QSIZE.height())
#define ToCcString(QSTRING) CcString::fromUtf8(QSTRING.getCharString(), static_cast<int>(QSTRING.length()))

#define ToQWidget(SUBSYSTEM) static_cast<QWidget*>(SUBSYSTEM)

#endif // H_CcQt_H_
