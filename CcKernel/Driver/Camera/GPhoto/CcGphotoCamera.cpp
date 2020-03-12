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
 * @brief     Implementation of Class CcGGphotoCamera
 */

#include "CcGphotoCamera.h"
#include "CcKernel.h"
#include "IThread.h"
#include "CcByteArray.h"
#include "CcKernel.h"
#include "CcProcess.h"
#include "CcFile.h"
#include "CcFileSystem.h"
#include <stdio.h>

CCEXTERNC_BEGIN
#include "gphoto2/gphoto2-camera.h"
CCEXTERNC_END

class CcGphotoCamera::CPrivate
{
public:
	bool										bOpen				= false;
	GPContext*							pContext		= nullptr;
	Camera*									pCamera			= nullptr;
	CameraAbilitiesList*		pAbilities	= nullptr;
	static GPPortInfoList*	s_pPortInfoList;
	static size_t						s_uiRefCount;

	CPrivate()
	{
		s_uiRefCount++;
		if (s_pPortInfoList == nullptr)
		{
			/* Load all the port drivers we have... */
			int ret = gp_port_info_list_new (&s_pPortInfoList);
			if (ret >= GP_OK)
			{
				ret = gp_port_info_list_load (s_pPortInfoList);
				if (ret >= GP_OK)
				{
					gp_port_info_list_count (s_pPortInfoList);
				}
			}
		}

		pContext = gp_context_new();
		CCMONITORNEW(pContext);
		gp_context_set_error_func(pContext, CPrivate::ctx_error_func, this);
		gp_context_set_status_func(pContext, CPrivate::ctx_status_func, this);
		gp_camera_new (&pCamera);
		CCMONITORNEW(pCamera);

		gp_abilities_list_new (&pAbilities);
		CCMONITORNEW(pAbilities);
		if(pAbilities == nullptr)
		{
		}
	}

	~CPrivate()
	{
		CCMONITORDELETE(pAbilities);
		gp_abilities_list_free(pAbilities);
		CCMONITORDELETE(pCamera);
		gp_camera_exit (pCamera, pContext);
		CCMONITORDELETE(pContext);
		gp_context_unref(pContext);

		s_uiRefCount--;
		if(s_uiRefCount == 0)
		{
			gp_port_info_list_free(s_pPortInfoList);
			s_pPortInfoList = nullptr;
		}
	}

	static void ctx_error_func (GPContext *pContext, const char *str, void *data)
	{
		CCUNUSED(pContext);
		CCUNUSED(data);
		CCUNUSED(str);
		CCERROR(CcString("Context: ")+str);
	}

	static void ctx_status_func (GPContext *pContext, const char *str, void *data)
	{
		CCUNUSED(pContext);
		CCUNUSED(data);
		CCUNUSED(str);
		CCINFO(CcString("Context: ")+str);
	}
};

GPPortInfoList* CcGphotoCamera::CPrivate::s_pPortInfoList = nullptr;
size_t CcGphotoCamera::CPrivate::s_uiRefCount = 0;

CcGphotoCamera::CcGphotoCamera(const CcStringPair& oListItem)
{
	CCNEW(m_pPrivate, CPrivate);
	open(oListItem);
}

CcGphotoCamera::CcGphotoCamera(const CcString& sName, const CcString& sConnection)
{
	CCNEW(m_pPrivate, CPrivate);
	open(sName, sConnection);
}

CcGphotoCamera::~CcGphotoCamera()
{
	close();
	CCDELETE(m_pPrivate);
}

CcImageData CcGphotoCamera::getImage()
{
	return CcImageData();
}

CcString CcGphotoCamera::captureTo(const CcString& sPath, const CcString& sName, bool bAutoIncrement)
{
	CcString sOutput;
	CcString sNewName = sName;
	if(bAutoIncrement)
	{
		sNewName = CcFileSystem::getNextFreeFilename(sPath, sName, ".jpg");
	}
	sOutput = sPath;
	sOutput.appendPath(sName + ".jpg");
	CcString sCamPath;
	CcString sCamFilename;
	if(capture(sCamPath, sCamFilename))
	{
		if(!fileDownload(sCamPath, sCamFilename, sOutput))
		{
			sOutput = "";
		}
	}
	else
	{
		sOutput = "";
	}
	return sOutput;
}

bool CcGphotoCamera::open(const CcString& sName, const CcString& sConnection)
{
  CCUNUSED(sConnection);
  close();
  m_pPrivate->bOpen = false;
  if(GP_OK == gp_abilities_list_load (m_pPrivate->pAbilities, m_pPrivate->pContext))
  {
    CameraAbilities	a;
    int iModel = gp_abilities_list_lookup_model (m_pPrivate->pAbilities, sName.getCharString());
    if(iModel > GP_OK)
    {
      if(GP_OK == gp_abilities_list_get_abilities (m_pPrivate->pAbilities, iModel, &a))
      {
        m_pPrivate->bOpen = GP_OK == gp_camera_set_abilities (m_pPrivate->pCamera, a);
        if(m_pPrivate->bOpen)
        {
          m_pPrivate->bOpen = false;
          int p = gp_port_info_list_lookup_path (CPrivate::s_pPortInfoList, sConnection.getCharString());
          if(GP_ERROR_UNKNOWN_PORT == p)
          {
            CCERROR("Port not found");
          }
          else
          {
            GPPortInfo	pi;
            if(GP_OK >= gp_port_info_list_get_info (CPrivate::s_pPortInfoList, p, &pi))
            {
              m_pPrivate->bOpen = GP_OK >= gp_camera_set_port_info (m_pPrivate->pCamera, pi);
            }
          }
        }
      }
    }
  }
  return m_pPrivate->bOpen;
}

void CcGphotoCamera::close()
{
  if(m_pPrivate->bOpen)
    m_pPrivate->bOpen = false;
}

bool CcGphotoCamera::isOpen()
{
  return m_pPrivate->bOpen;
}

bool CcGphotoCamera::capture(CcString& sFolder, CcString& sFile)
{
  bool bSuccess = false;
  CameraFilePath pCamera_file_path;
  int retval = gp_camera_capture(
                  m_pPrivate->pCamera,
                  GP_CAPTURE_IMAGE,
                  &pCamera_file_path,
                  m_pPrivate->pContext
  );
  if(retval >= GP_OK)
  {
    sFolder = pCamera_file_path.folder;
    sFile = pCamera_file_path.name;
    bSuccess = true;
  }
  return bSuccess;
}

bool CcGphotoCamera::fileDownload(const CcString &sFolder, const CcString& sFile, const CcString& sLocal)
{
	bool bSuccess = false;
	CcFile oFile(sLocal);
	if(oFile.open(EOpenFlags::Write))
	{
		FILE* pFile = (FILE*)oFile.getStdFile();
		int iFile = fileno(pFile);
		CameraFile *file;
		int retval = gp_file_new_from_fd(&file, iFile);
		if(retval >= GP_OK)
		{
			retval = gp_camera_file_get(m_pPrivate->pCamera,
																	sFolder.getCharString(),
																	sFile.getCharString(),
																	GP_FILE_TYPE_NORMAL,
																	file,
																	m_pPrivate->pContext);
			if(retval >= GP_OK)
			{
				bSuccess = true;
			}
		}
		oFile.close();
	}
	return bSuccess;
}

bool CcGphotoCamera::fileDelete(const CcString &sFolder, const CcString &sFile)
{
	bool bSuccess = false;
	int retval = gp_camera_file_delete(m_pPrivate->pCamera,
															sFolder.getCharString(),
															sFile.getCharString(),
															m_pPrivate->pContext);
	if(retval >= GP_OK)
	{
		bSuccess = true;
	}
	return bSuccess;
}

CcStringMap CcGphotoCamera::getAvailable()
{
  CcStringMap   oList;
  CameraList*   list;
  GPContext*    pContext = gp_context_new(); /* see pContext.c */
  const char*   name, *value;

  if(pContext == nullptr)
  {
    CCERROR("Failed to create GPContext");
  }
  else
  {
    int ret = gp_list_new (&list);
    if(ret != GP_OK)
    {
      CCERROR("Failed to create CameraList");
    }
    else
    {
      gp_list_reset (list);
      int iCount = gp_camera_autodetect (list, pContext);
      if(iCount)
      {
        for (int i = 0; i < iCount; i++)
        {
          gp_list_get_name  (list, i, &name);
          gp_list_get_value (list, i, &value);
          oList.append(name, value);
        }
      }
      gp_list_free(list);
    }
  }
  return oList;
}
