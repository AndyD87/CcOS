# - Try to find LibWIRINGPI
# Once done this will define
#  WIRINGPI_FOUND - System has LibWIRINGPI
#  WIRINGPI_INCLUDE_DIRS - The LibWIRINGPI include directories
#  WIRINGPI_LIBRARIES - The libraries needed to use LibWIRINGPI
#  WIRINGPI_DEFINITIONS - Compiler switches required for using LibWIRINGPI

find_package(PkgConfig QUIET)
pkg_check_modules(PackageCheck_WIRINGPI QUIET wiringPi)
set(WIRINGPI_DEFINITIONS ${PackageCheck_WIRINGPI})

find_path(WIRINGPI_INCLUDE_DIR wiringPi.h
          HINTS ${PackageCheck_WIRINGPI_INCLUDEDIR} ${PackageCheck_WIRINGPI_INCLUDE_DIRS}
)


find_library(WIRINGPI_LIBRARY NAMES wiringpi
             HINTS ${PackageCheck_WIRINGPI_LIBDIR} ${PackageCheck_WIRINGPI_LIBRARY_DIRS}
)

if (WIRINGPI_INCLUDE_DIR AND WIRINGPI_LIBRARY)
  set(WIRINGPI_LIBRARIES    ${WIRINGPI_LIBRARY})
  set(WIRINGPI_INCLUDE_DIRS ${WIRINGPI_INCLUDE_DIR} )
  set(WIRINGPI_FOUND TRUE)

  if(ZeroMQ_FOUND)
    add_library(CcOS::ThirdParty::wiringPi INTERFACE IMPORTED)
    set_property(TARGET CcOS::ThirdParty::wiringPi PROPERTY INTERFACE_LINK_LIBRARIES ${WIRINGPI_LIBRARY})
    set_property(TARGET CcOS::ThirdParty::wiringPi PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${WIRINGPI_INCLUDE_DIR}")
  endif()
endif()
