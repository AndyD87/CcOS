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

CCEXTERNC_BEGIN
#include "gphoto2/gphoto2-camera.h"
CCEXTERNC_END

class CcGphotoCamera::CPrivate
{
public:
	GPContext* context = nullptr;
	Camera*    camera  = nullptr;
	CameraAbilitiesList		*abilities    = nullptr;
	static GPPortInfoList	*portinfolist;

	CPrivate()
	{
		if (!portinfolist)
		{
			/* Load all the port drivers we have... */
			int ret = gp_port_info_list_new (&portinfolist);
			if (ret >= GP_OK)
			{
				ret = gp_port_info_list_load (portinfolist);
				if (ret >= GP_OK)
				{
					gp_port_info_list_count (portinfolist);
				}
			}
		}

		context = gp_context_new();
		CCMONITORNEW(context);
		gp_context_set_error_func(context, CPrivate::ctx_error_func, this);
		gp_context_set_status_func(context, CPrivate::ctx_status_func, this);
		gp_camera_new (&camera);
		CCMONITORNEW(camera);
		context = gp_context_new();

		gp_abilities_list_new (&abilities);
		if(abilities == nullptr)
		{
		}
	}

	~CPrivate()
	{
		CCMONITORDELETE(abilities);
		gp_abilities_list_free(abilities);
		CCMONITORDELETE(camera);
		gp_camera_exit (camera, context);
		gp_camera_free(camera);
		CCMONITORDELETE(context);
		gp_context_unref(context);
	}

	static void ctx_error_func (GPContext *context, const char *str, void *data)
	{
		CCUNUSED(context);
		CCUNUSED(data);
		CCERROR(CcString("Context: ")+str);
	}

	static void ctx_status_func (GPContext *context, const char *str, void *data)
	{
		CCUNUSED(context);
		CCUNUSED(data);
		CCINFO(CcString("Context: ")+str);
	}
};

GPPortInfoList* CcGphotoCamera::CPrivate::portinfolist = nullptr;

CcGphotoCamera::CcGphotoCamera()
{
	CCNEW(m_pPrivate, CPrivate);
}

CcGphotoCamera::~CcGphotoCamera()
{
	close();
	CCDELETE(m_pPrivate);
}

bool CcGphotoCamera::open(const CcString& sName, const CcString& sConnection)
{
  CCUNUSED(sConnection);
  bool bSuccess = false;
  if(GP_OK == gp_abilities_list_load (m_pPrivate->abilities, m_pPrivate->context))
  {
    CameraAbilities	a;
    int iModel = gp_abilities_list_lookup_model (m_pPrivate->abilities, sName.getCharString());
    if(iModel > GP_OK)
    {
      if(GP_OK == gp_abilities_list_get_abilities (m_pPrivate->abilities, iModel, &a))
      {
        bSuccess = GP_OK == gp_camera_set_abilities (m_pPrivate->camera, a);
        if(bSuccess)
        {
          bSuccess = false;
          int p = gp_port_info_list_lookup_path (CPrivate::portinfolist, sConnection.getCharString());
          if(GP_ERROR_UNKNOWN_PORT == p)
          {
            CCERROR("Port not found");
          }
          else
          {
            GPPortInfo	pi;
            if(GP_OK >= gp_port_info_list_get_info (CPrivate::portinfolist, p, &pi))
            {
              bSuccess = GP_OK >= gp_camera_set_port_info (m_pPrivate->camera, pi);
            }
          }
        }
      }
    }
  }
  return bSuccess;
}

void CcGphotoCamera::close()
{

}

bool CcGphotoCamera::capture()
{
  bool bSuccess = true;
  CameraFilePath camera_file_path;
  while(bSuccess)
  {
    int retval = gp_camera_capture(
                    m_pPrivate->camera,
                    GP_CAPTURE_IMAGE,
                    &camera_file_path,
                    m_pPrivate->context
    );
    if(retval >= GP_OK)
    {
      bSuccess = true;
    }
    else
    {
      bSuccess = false;
    }
  }
  return bSuccess;
}

CcStringMap CcGphotoCamera::getAvailable()
{
  CcStringMap   oList;
  CameraList*   list;
  GPContext*    context = gp_context_new(); /* see context.c */
  const char*   name, *value;

  if(context == nullptr)
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
      int iCount = gp_camera_autodetect (list, context);
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
