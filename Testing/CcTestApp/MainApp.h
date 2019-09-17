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
 * @page      CcTestApp
 * @subpage   CcApps
 *
 * @page      CcTestApp
 * @copyright Andreas Dirmeier (C) 2017
 * @author    Andreas Dirmeier
 * @par       Web:
 * @par       Language: C++11
 * @brief     Class MainApp
 *
 *  Example GUI-Application with Menue Structure and Application Loader
 */
#ifndef H_MAINAPP_H_
#define H_MAINAPP_H_

#include "CcBase.h"
#include "CcGuiApplication.h"

class CcPushButton;

class MainApp: public CcGuiApplication
{
public:
  MainApp();
  virtual ~MainApp();

  //virtual void run() override;

private:
  CcPushButton* m_pButton;
};

#endif // H_MAINAPP_H_
