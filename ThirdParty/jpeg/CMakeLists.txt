if(NOT DEFINED CCOS_THIRDPARTY_JPEG_ACTIVE OR NOT DEFINED CCOS_BUILDLEVEL)
elseif(${CCOS_THIRDPARTY_JPEG_ACTIVE} GREATER ${CCOS_BUILDLEVEL})
  # Copyright (C) 2007-2012 LuaDist.
  # Created by Peter Kapec, David Manura
  # Redistribution and use of this file is allowed according to the terms of the MIT license.
  # For details see the COPYRIGHT file distributed with LuaDist.
  # Please note that the package source code is licensed under its own license.

  set(CURRENT_PROJECT jpeg)

  set ( HEADERS jerror.h jmorecfg.h jpeglib.h jconfig.h )

  set ( SRC jmemnobs.c jaricom.c jcapimin.c jcapistd.c jcarith.c jccoefct.c jccolor.c
    jcdctmgr.c jchuff.c jcinit.c jcmainct.c jcmarker.c jcmaster.c jcomapi.c jcparam.c
    jcprepct.c jcsample.c jctrans.c jdapimin.c jdapistd.c jdarith.c jdatadst.c jdatasrc.c
    jdcoefct.c jdcolor.c jddctmgr.c jdhuff.c jdinput.c jdmainct.c jdmarker.c jdmaster.c
    jdmerge.c jdpostct.c jdsample.c jdtrans.c jerror.c jfdctflt.c jfdctfst.c jfdctint.c
    jidctflt.c jidctfst.c jidctint.c jquant1.c jquant2.c jutils.c jmemmgr.c cderror.h
    cdjpeg.h jdct.h jinclude.h jmemsys.h jpegint.h jversion.h transupp.h )

  if(WINDOWS)
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
  endif()

  CcAddLibrary ( ${CURRENT_PROJECT} STATIC ${SRC} ${HEADERS} )

  set_property( TARGET ${CURRENT_PROJECT} PROPERTY FOLDER "CcOS/ThirdParty")

  #################
  # Post configuration steps
  #################
  CcOSTargetIncludeDirs( ${CURRENT_PROJECT} )
  # do nothing, do not install static lib!
  CcOSLibSettings(${CURRENT_PROJECT} FALSE FALSE)
endif()
