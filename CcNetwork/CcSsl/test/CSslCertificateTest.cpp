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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implemtation of class CSslCertificateTest
 */
#include "CSslCertificateTest.h"
#include "CcKernel.h"
#include "CcFile.h"
#include "CcString.h"
#include "CcSslCertificate.h"
#include "Hash/CcSslHmac.h"

class CSslCertificateTestPrivate
{
public:
  CcString sTempDir;
  CcString sPrivateKeyFile;
  CcString sPublicKeyFile;
};

CSslCertificateTest::CSslCertificateTest()
{
  appendTestMethod("Remove test files if existing", &CSslCertificateTest::deleteTestFiles);
  appendTestMethod("Test create Keys", &CSslCertificateTest::testCreate);

  m_pPrivate = new CSslCertificateTestPrivate();
  CCMONITORNEW(m_pPrivate);
  m_pPrivate->sTempDir = CcKernel::getTempDir();
  m_pPrivate->sPrivateKeyFile = m_pPrivate->sTempDir;
  m_pPrivate->sPublicKeyFile = m_pPrivate->sTempDir;
  m_pPrivate->sPrivateKeyFile.appendPath("Private.crt");
  m_pPrivate->sPublicKeyFile.appendPath("Public.crt");
}

CSslCertificateTest::~CSslCertificateTest()
{
  deleteTestFiles();
  CCDELETE(m_pPrivate);
}

bool CSslCertificateTest::deleteTestFiles()
{
  bool bRet = true;
  if(CcFile::exists(m_pPrivate->sPrivateKeyFile))
  {
    bRet &= CcFile::remove(m_pPrivate->sPrivateKeyFile);
  }
  if(CcFile::exists(m_pPrivate->sPublicKeyFile))
  {
    bRet &= CcFile::remove(m_pPrivate->sPublicKeyFile);
  }
  return bRet;
}

bool CSslCertificateTest::testCreate()
{
  bool bSuccess = true;
  CcSslCertificate oCert;
  // Do it more than once, because we had a SIGSEV
  bSuccess &= oCert.create(m_pPrivate->sPrivateKeyFile, m_pPrivate->sPublicKeyFile);
  bSuccess &= deleteTestFiles();
  bSuccess &= oCert.create(m_pPrivate->sPrivateKeyFile, m_pPrivate->sPublicKeyFile);
  bSuccess &= deleteTestFiles();
  bSuccess &= oCert.create(m_pPrivate->sPrivateKeyFile, m_pPrivate->sPublicKeyFile);
  bSuccess &= deleteTestFiles();
  bSuccess &= oCert.create(m_pPrivate->sPrivateKeyFile, m_pPrivate->sPublicKeyFile);
  bSuccess &= deleteTestFiles();
  return true;
}
