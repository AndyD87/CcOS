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
 * @brief     Implemtation of class CcStatus
 */
#include "CcStatus.h"
#include "CcGlobalStrings.h"

#ifdef LINUX
  #include "errno.h"
#elif defined(WINDOWS)
  #include <windows.h>
#endif

CcStatus& CcStatus::operator=(const CcStatus& oError)
{
  m_eError = oError.m_eError;
  return *this;
}

CcStatus& CcStatus::operator=(EStatus eError)
{
  m_eError = eError;
  return *this;
}

CcStatus& CcStatus::operator=(bool bTrueFalse)
{
  if (bTrueFalse)
    m_eError = EStatus::AllOk;
  else
    m_eError = EStatus::Error;
  return *this;
}

CcStatus& CcStatus::operator=(int iErrorCode)
{
  m_eError = static_cast<EStatus>(iErrorCode);
  return *this;
}

CcStatus& CcStatus::operator=(uint uiErrorCode)
{
  m_eError = static_cast<EStatus>(uiErrorCode);
  return *this;
}

#ifdef WINDOWS
CcStatus& CcStatus::operator=(unsigned long iErrorCode)
{
  setSystemError(static_cast<uint32>(iErrorCode));
  return *this;
}
#endif

const CcString& CcStatus::getString() const
{
  switch(m_eError)
  {
    case EStatus::AllOk:
      return CcGlobalStrings::Error::NoError;
    case EStatus::NotPermitted:
      return CcGlobalStrings::Error::NotPermitted;
    case EStatus::AccessDenied:
      return CcGlobalStrings::Error::AccessDenied;
    default:
      return CcGlobalStrings::Error::UndefinedError;
  }
}

//! @param uiError: Set system error and add system error flags
CcStatus& CcStatus::setSystemError(uint32 uiError)
{
  // If unknown error occured, please add it to this list and select or create an expecting enum
  // in EStatus.
  switch (uiError)
  {
    case 0:    /* No error */
      m_eError = EStatus::AllOk;
      break;
    #ifdef LINUX
    case EHOSTUNREACH:    /* No route to host */
      m_eError = EStatus::NetworkHostNotReachable;
      break;
    case EPERM:       /* Operation not permitted */
      m_eError = EStatus::NotPermitted;
      break;
    case ENOENT:      /* No such file or directory */
    case ESRCH:       /* No such process */
    case EINTR:       /* Interrupted system call */
    case EIO:         /* I/O error */
    case ENXIO:       /* No such device or address */
    case E2BIG:       /* Arg list too long */
    case ENOEXEC:     /* Exec format error */
    case EBADF:       /* Bad file number */
    case ECHILD:      /* No child processes */
    case EAGAIN:      /* Try again */
    case ENOMEM:      /* Out of memory */
    case EACCES:      /* Permission denied */
      m_eError = EStatus::UserAccessDenied;
      break;
    case EFAULT:      /* Bad address */
    case ENOTBLK:     /* Block device required */
    case EBUSY:       /* Device or resource busy */
    case EEXIST:      /* File exists */
    case EXDEV:       /* Cross-device link */
    case ENODEV:      /* No such device */
    case ENOTDIR:     /* Not a directory */
    case EISDIR:      /* Is a directory */
    case EINVAL:      /* Invalid argument */
    case ENFILE:      /* File table overflow */
    case EMFILE:      /* Too many open files */
    case ENOTTY:      /* Not a typewriter */
    case ETXTBSY:     /* Text file busy */
    case EFBIG:       /* File too large */
    case ENOSPC:      /* No space left on device */
    case ESPIPE:      /* Illegal seek */
    case EROFS:       /* Read-only file system */
    case EMLINK:      /* Too many links */
    case EPIPE:       /* Broken pipe */
    case EDOM:        /* Math argument out of domain of func */
    case ERANGE:      /* Math result not representable */
    case EDEADLK:     /* Resource deadlock would occur */
    case ENAMETOOLONG:/* File name too long */
    case ENOLCK:      /* No record locks available */
    case ENOSYS:      /* Function not implemented */
    case ENOTEMPTY:   /* Directory not empty */
    case ELOOP:       /* Too many symbolic links encountered */
    case ENOMSG:      /* No message of desired type */
    case EIDRM:       /* Identifier removed */
    case ECHRNG:      /* Channel number out of range */
    case EL2NSYNC:    /* Level 2 not synchronized */
    case EL3HLT:      /* Level 3 halted */
    case EL3RST:      /* Level 3 reset */
    case ELNRNG:      /* Link number out of range */
    case EUNATCH:     /* Protocol driver not attached */
    case ENOCSI:      /* No CSI structure available */
    case EL2HLT:      /* Level 2 halted */
    case EBADE:       /* Invalid exchange */
    case EBADR:       /* Invalid request descriptor */
    case EXFULL:      /* Exchange full */
    case ENOANO:      /* No anode */
    case EBADRQC:     /* Invalid request code */
    case EBADSLT:     /* Invalid slot */
    case EBFONT:      /* Bad font file format */
    case ENOSTR:      /* Device not a stream */
    case ENODATA:     /* No data available */
    case ETIME:       /* Timer expired */
    case ENOSR:       /* Out of streams resources */
    case ENONET:      /* Machine is not on the network */
    case ENOPKG:      /* Package not installed */
    case EREMOTE:     /* Object is remote */
    case ENOLINK:     /* Link has been severed */
    case EADV:        /* Advertise error */
    case ESRMNT:      /* Srmount error */
    case ECOMM:       /* Communication error on send */
    case EPROTO:      /* Protocol error */
    case EMULTIHOP:   /* Multihop attempted */
    case EDOTDOT:     /* RFS specific error */
    case EBADMSG:     /* Not a data message */
    case EOVERFLOW:   /* Value too large for defined data type */
    case ENOTUNIQ:    /* Name not unique on network */
    case EBADFD:      /* File descriptor in bad state */
    case EREMCHG:     /* Remote address changed */
    case ELIBACC:     /* Can not access a needed shared library */
    case ELIBBAD:     /* Accessing a corrupted shared library */
    case ELIBSCN:     /* .lib section in a.out corrupted */
    case ELIBMAX:     /* Attempting to link in too many shared libraries */
    case ELIBEXEC:    /* Cannot exec a shared library directly */
    case EILSEQ:      /* Illegal byte sequence */
    case ERESTART:    /* Interrupted system call should be restarted */
    case ESTRPIPE:    /* Streams pipe error */
    case EUSERS:      /* Too many users */
    case ENOTSOCK:    /* Socket operation on non-socket */
    case EDESTADDRREQ:/* Destination address required */
    case EMSGSIZE:    /* Message too long */
    case EPROTOTYPE:  /* Protocol wrong type for socket */
    case ENOPROTOOPT: /* Protocol not available */
    case EPROTONOSUPPORT: /* Protocol not supported */
    case ESOCKTNOSUPPORT: /* Socket type not supported */
    case EOPNOTSUPP:  /* Operation not supported on transport endpoint */
    case EPFNOSUPPORT:/* Protocol family not supported */
    case EAFNOSUPPORT:/* Address family not supported by protocol */
    case EADDRINUSE:  /* Address already in use */
    case EADDRNOTAVAIL:   /* Cannot assign requested address */
    case ENETDOWN:        /* Network is down */
    case ENETUNREACH:     /* Network is unreachable */
    case ENETRESET:       /* Network dropped connection because of reset */
    case ECONNABORTED:    /* Software caused connection abort */
    case ECONNRESET:      /* Connection reset by peer */
    case ENOBUFS:         /* No buffer space available */
    case EISCONN:         /* Transport endpoint is already connected */
    case ENOTCONN:        /* Transport endpoint is not connected */
    case ESHUTDOWN:       /* Cannot send after transport endpoint shutdown */
    case ETOOMANYREFS:    /* Too many references: cannot splice */
    case ETIMEDOUT:       /* Connection timed out */
    case ECONNREFUSED:    /* Connection refused */
    case EHOSTDOWN:       /* Host is down */
    case EALREADY:        /* Operation already in progress */
    case EINPROGRESS:     /* Operation now in progress */
    case ESTALE:          /* Stale NFS file handle */
    case EUCLEAN:         /* Structure needs cleaning */
    case ENOTNAM:         /* Not a XENIX named type file */
    case ENAVAIL:         /* No XENIX semaphores available */
    case EISNAM:          /* Is a named type file */
    case EREMOTEIO:       /* Remote I/O error */
    #elif defined(WINDOWS)
      case ERROR_FILE_NOT_FOUND:
      case ERROR_PATH_NOT_FOUND:
        m_eError = EStatus::FSFileNotFound; break;
    #endif
    default:
      m_eError = static_cast<EStatus>(static_cast<uint32>(EStatus::SystemError) | uiError); 
      CCDEBUG("Unknown error code: " + CcString::fromNumber(uiError));
  }
  return *this;
}

/**
 * @brief Method for convert status to bool.
 * @param[out] bLeft: True if oStatus == 0
 * @param oStatus:    Status to convert
 */
void operator&=(bool& bLeft, const CcStatus& oStatus )
{
  bLeft = bLeft & (oStatus.getError() == EStatus::AllOk);
}

/**
 * @brief Method for convert status to int.
 * @param[out] iLeft: oStatus value as int
 * @param oStatus:    Status to convert
 */
void operator&=(int& iLeft, const CcStatus& oStatus )
{
  iLeft = iLeft & static_cast<int>(oStatus.getError());
}
