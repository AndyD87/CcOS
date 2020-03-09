/*
 * @copyright  Andreas Dirmeier (C) 2015
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
 * @par       Web:      http://coolcow.de/projects/CcOS
 * @par       Language: C++11
 * @brief     Implementation of Class CcV4LDriver
 */
#include "CcV4LDriver.h"
#include "CcString.h"
#include "CcStatic.h"
#include "CcFile.h"

#include <stdio.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/videodev2.h>
#include <libv4l2.h>

struct v4l_buffer {
        void   *start;
        size_t length;
};

static void xioctl(int fh, int request, void *arg)
{
  int r;

  do
  {
    r = v4l2_ioctl(fh, request, arg);
  } while (r == -1 && ((errno == EINTR) || (errno == EAGAIN)));

  if (r == -1)
  {
          fprintf(stderr, "error %d\n", errno);
  }
}


CcV4LDriver::CcV4LDriver()
{
}

CcV4LDriver::~CcV4LDriver()
{
}

CcStatus CcV4LDriver::entry()
{
  CcStatus oStatus(false);
  int fd = v4l2_open("/dev/video0", O_RDWR | O_NONBLOCK, 0);
  if (fd < 0)
  {
  }
  else
  {
    struct v4l2_format              fmt;
    struct v4l2_buffer              buf;
    struct v4l2_requestbuffers      req;
    CcStatic_memsetZeroObject(fmt);
    CcStatic_memsetZeroObject(buf);
    CcStatic_memsetZeroObject(req);

    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    fmt.fmt.pix.width       = 640;
    fmt.fmt.pix.height      = 480;
    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_RGB24;
    fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
    xioctl(fd, VIDIOC_S_FMT, &fmt);
    if (fmt.fmt.pix.pixelformat != V4L2_PIX_FMT_RGB24)
    {
      CCERROR("Libv4l didn't accept RGB24 format. Can't proceed.\\n");
    }
    else
    {
      uint32 n_buffers;
      req.count = 2;
      req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      req.memory = V4L2_MEMORY_MMAP;
      xioctl(fd, VIDIOC_REQBUFS, &req);

      CCNEWARRAYTYPE(buffers, v4l_buffer, req.count);
      for (n_buffers = 0; n_buffers < req.count; ++n_buffers)
      {
        CcStatic_memsetZeroObject(buf);
        buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory      = V4L2_MEMORY_MMAP;
        buf.index       = n_buffers;

        xioctl(fd, VIDIOC_QUERYBUF, &buf);

        buffers[n_buffers].length = buf.length;
        buffers[n_buffers].start = v4l2_mmap(NULL, buf.length,
                      PROT_READ | PROT_WRITE, MAP_SHARED,
                      fd, buf.m.offset);

        if (MAP_FAILED == buffers[n_buffers].start)
        {
          perror("mmap");
        }
      }

      for (uint32 i = 0; i < n_buffers; ++i)
      {
        CcStatic_memsetZeroObject(buf);
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;
        buf.index = i;
        xioctl(fd, VIDIOC_QBUF, &buf);
      }
      v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

      xioctl(fd, VIDIOC_STREAMON, &type);
      for (uint32 i = 0; i < 20; i++)
      {
        int r;
        do
        {
          fd_set fds;
          FD_ZERO(&fds);
          FD_SET(fd, &fds);

          /* Timeout. */
          struct timeval tv;
          tv.tv_sec = 2;
          tv.tv_usec = 0;

          r = select(fd + 1, &fds, NULL, NULL, &tv);
        } while ((r == -1 && (errno = EINTR)));
        if (r == -1)
        {
          CCERROR("select");
          break;
        }
        else
        {
          CcStatic_memsetZeroObject(buf);
          buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
          buf.memory = V4L2_MEMORY_MMAP;
          xioctl(fd, VIDIOC_DQBUF, &buf);

          CcString sFile("pic-");
          sFile += CcString::fromNumber(i);
          sFile += ".ppm";
          CcFile oPicture(sFile);
          if(oPicture.open(EOpenFlags::Write))
          {
            CcString sData = "P6\n";
            sData += CcString::fromNumber(fmt.fmt.pix.width);
            sData += " ";
            sData += CcString::fromNumber(fmt.fmt.pix.height);
            sData += "\n255\n";
            oPicture.writeString(sData);
            oPicture.write(buffers[buf.index].start, buf.bytesused);
            oPicture.close();
          }

          xioctl(fd, VIDIOC_QBUF, &buf);
        }
      }

      CCDELETEARR(buffers);
    }

    v4l2_close(fd);
  }
  return oStatus;
}

CcStatus CcV4LDriver::unload()
{
  return true;
}
