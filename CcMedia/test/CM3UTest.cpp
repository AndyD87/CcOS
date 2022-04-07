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
 * @brief     Implemtation of class CM3UTest
 */
#include "CM3UTest.h"
#include "CcString.h"
#include "Playlist/CcM3U.h"
#include "CcFile.h"

CM3UTest::CM3UTest() :
  CcTest("CM3UTest")
{
  appendTestMethod("Setup test env", &CM3UTest::generateTestFiles);
  appendTestMethod("Test basics", &CM3UTest::testBasic);
  appendTestMethod("Test stream download", &CM3UTest::testStreamLoad);
}

CM3UTest::~CM3UTest()
{
}

bool CM3UTest::generateTestFiles()
{
  bool bSuccess = true;
  m_sTestDir = CcTestFramework::getTemporaryDir();
  m_sMasterFilePath.set(m_sTestDir);
  m_sChuncksFilePath.set(m_sTestDir);
  m_sStreamFile.set(m_sTestDir);
  m_sStreamFileDownload.set(m_sTestDir);

  m_sMasterFilePath.appendPath("master.m3u8");
  m_sChuncksFilePath.appendPath("chunks.m3u8");
  m_sStreamFile.appendPath("stream.ts");
  m_sStreamFileDownload.appendPath("stream.down.ts");

  CcFile oMasterFile(m_sMasterFilePath);
  if (oMasterFile.open(EOpenFlags::Write))
  {
    CcFile oChunckFile(m_sChuncksFilePath);
    if (oChunckFile.open(EOpenFlags::Write))
    {
      CcFile oStreamFile(m_sStreamFile);
      if (oStreamFile.open(EOpenFlags::Write))
      {
        bSuccess &= oMasterFile.writeLine("#EXTM3U");
        bSuccess &= oMasterFile.writeLine("#EXT-X-VERSION:3");
        bSuccess &= oMasterFile.writeLine("#EXT-X-INDEPENDENT-SEGMENTS");
        bSuccess &= oMasterFile.writeLine("#EXT-X-STREAM-INF:BANDWIDTH=1672000,AVERAGE-BANDWIDTH=1460800,CODECS=\"avc1.4d401f,mp4a.40.2\",RESOLUTION=640x360,FRAME-RATE=50.000");
        bSuccess &= oMasterFile.writeLine("chunks.m3u8");
        bSuccess &= oMasterFile.writeLine("#EXT-X-STREAM-INF:BANDWIDTH=1034000,AVERAGE-BANDWIDTH=910800,CODECS=\"avc1.4d401e,mp4a.40.2\",RESOLUTION=480x270,FRAME-RATE=50.000");
        bSuccess &= oMasterFile.writeLine("chunks.m3u8");
        bSuccess &= oMasterFile.writeLine("#EXT-X-STREAM-INF:BANDWIDTH=4224000,AVERAGE-BANDWIDTH=3660800,CODECS=\"avc1.640020,mp4a.40.2\",RESOLUTION=1280x720,FRAME-RATE=50.000");
        bSuccess &= oMasterFile.writeLine("chunks.m3u8");
        bSuccess &= oMasterFile.writeLine("#EXT-X-STREAM-INF:BANDWIDTH=6520800,AVERAGE-BANDWIDTH=5640800,CODECS=\"avc1.64002a,mp4a.40.2\",RESOLUTION=1920x1080,FRAME-RATE=50.000");
        bSuccess &= oMasterFile.writeLine("chunks.m3u8");
        bSuccess &= oMasterFile.writeLine("#EXT-X-STREAM-INF:BANDWIDTH=2182400,AVERAGE-BANDWIDTH=1900800,CODECS=\"avc1.4d401f,mp4a.40.2\",RESOLUTION=960x540,FRAME-RATE=50.000");
        bSuccess &= oMasterFile.writeLine("chunks.m3u8");

        bSuccess &= oChunckFile.writeLine("#EXTM3U");
        bSuccess &= oChunckFile.writeLine("#EXT-X-VERSION:3");
        bSuccess &= oChunckFile.writeLine("#EXT-X-TARGETDURATION:4");
        bSuccess &= oChunckFile.writeLine("#EXT-X-MEDIA-SEQUENCE:4581547");
        bSuccess &= oChunckFile.writeLine("#EXT-X-DISCONTINUITY-SEQUENCE:2595");
        bSuccess &= oChunckFile.writeLine("#EXT-X-PROGRAM-DATE-TIME:2022-04-06T10:09:49.360Z");
        bSuccess &= oChunckFile.writeLine("#EXTINF:4.00000");
        bSuccess &= oChunckFile.writeLine("stream.ts");

        bSuccess &= oStreamFile.writeLine("I am a video :)");

        bSuccess &= oStreamFile.close();
      }
      else
      {
        bSuccess = false;
      }
      bSuccess &= oChunckFile.close();

    }
    else
    {
      bSuccess = false;
    }
    bSuccess &= oMasterFile.close();
  }
  else
  {
    bSuccess = false;
  }
  return bSuccess;
}

void CM3UTest::cleanup()
{
  CcTestFramework::removeTemporaryDir();
}

bool CM3UTest::testBasic()
{
  bool bRet = false;
  if (CcFile::isFile(m_sMasterFilePath) &&
      CcFile::isFile(m_sChuncksFilePath))
  {
    CcM3U oReader(m_sMasterFilePath);
    if (oReader.getStreamCount() > 0)
    {
      bRet = true;
    }
  }
  return bRet;
}

bool CM3UTest::testStreamLoad()
{
  bool bRet = false;
  CcM3U oReader(m_sMasterFilePath);
  if (oReader.getStreamCount() > 0)
  {
    size_t uiBestStream = oReader.getStreamBest();
    if (uiBestStream == 3)
    {
      bRet = oReader.downloadStream(uiBestStream, m_sStreamFileDownload);
      if (bRet)
      {
        // Check if files are identical
      }
    }
  }
  return bRet;
}
