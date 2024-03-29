/**
 * @copyright  Andreas Dirmeier (C) 2022
 *
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
 * @brief     Implementation of Class CcLinuxUser
 */
#include "CcLinuxUser.h"
#include "CcKernel.h"
#include "crypt.h"
#include "pwd.h"
#include "grp.h"
#include "shadow.h"
#include "errno.h"
#include "CcStatic.h"
#include "CcStringUtil.h"

CcLinuxUser::CcLinuxUser(const CcString& Username , const CcString& HomeDir, uint32 iUserId, bool bIsOwner):
  CcUser(Username)
{
  m_sUsername = Username;
  m_sHomeDir  = HomeDir;
  m_uiId = static_cast<uint32>(iUserId);
  if(bIsOwner)
  {
    int iTempNum = 0;
    // get number of items
    int iRet = getgrouplist(Username.getCharString(), 0, nullptr, &iTempNum);
    if(iRet < 0)
    {
      CCNEWARRAYTYPE(piGroups, gid_t, iTempNum);
      int iGroupNum = iTempNum;
      iRet = getgrouplist(Username.getCharString(), 0, piGroups, &iGroupNum);
      for(int i =0; i< iGroupNum && iRet > 0; i++)
      {
        CcGroup oGroup("", piGroups[i]);
        m_GroupList.append(oGroup);
      }
      CCDELETEARR(piGroups);
    }
  }
}

CcLinuxUser::~CcLinuxUser()
{
}

bool CcLinuxUser::login(const CcPassword &Password)
{
  bool bSuccess = false;
  struct passwd *pw;
  struct spwd *sp;
  char *encrypted, *correct;
  // get user structure
  pw = getpwnam(m_sUsername.getCharString());
  endpwent();
  // get additionaly the shadow structure for user
  sp = getspnam(m_sUsername.getCharString());
  endspent();
  if(sp != 0)
  {
    // if shadow exist, use shadow salt, otherwise passwd salt
    correct = sp ? sp->sp_pwdp : pw->pw_passwd;
    // encrypt password
    encrypted = crypt(Password.getString().getCharString(), correct);
    //compare generated password with stored password in previously read struct
    bSuccess = CcStringUtil::strcmp(encrypted, correct) ? false : true;  // cmp=0 == true
  }
  return bSuccess;
}
