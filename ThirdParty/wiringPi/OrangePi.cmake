# Download sources
CcGitClone(${WRINIGPI_SOURCE_DIR} https://github.com/orangepi-xunlong/wiringOP.git
                                  https://coolcow.de/projects/ThirdParty/wiringOP.git
)

# Setup source files as made in Makefile
set(SOURCE_FILES
  source/wiringPi/ads1115.c
  source/wiringPi/bmp180.c
  source/wiringPi/drcNet.c
  source/wiringPi/drcSerial.c
  source/wiringPi/ds18b20.c
  source/wiringPi/htu21d.c
  source/wiringPi/max31855.c
  source/wiringPi/max5322.c
  source/wiringPi/mcp23008.c
  source/wiringPi/mcp23016.c
  source/wiringPi/mcp23017.c
  source/wiringPi/mcp23s08.c
  source/wiringPi/mcp23s17.c
  source/wiringPi/mcp3002.c
  source/wiringPi/mcp3004.c
  source/wiringPi/mcp3422.c
  source/wiringPi/mcp4802.c
  source/wiringPi/oled.c
  source/wiringPi/OrangePi.c
  source/wiringPi/pcf8574.c
  source/wiringPi/pcf8591.c
  source/wiringPi/piHiPri.c
  source/wiringPi/piThread.c
  source/wiringPi/pseudoPins.c
  source/wiringPi/rht03.c
  source/wiringPi/sn3218.c
  source/wiringPi/softPwm.c
  source/wiringPi/softServo.c
  source/wiringPi/softTone.c
  source/wiringPi/sr595.c
  source/wiringPi/w25q64.c
  source/wiringPi/wiringPi.c
  source/wiringPi/wiringPiI2C.c
  source/wiringPi/wiringPiSPI.c
  source/wiringPi/wiringSerial.c
  source/wiringPi/wiringShift.c
  source/wiringPi/wpiExtensions.c
)


# Setup source directory for include
include_directories(source/wiringPi)

# Setup default definitions, board specific includes has to be done before, look at CMakeLists.txt
add_definitions(-D_GNU_SOURCE -DCONFIG_ORANGEPI)

# Create directory
add_library(wiringPi SHARED ${SOURCE_FILES})

# Setup version from wiringPi Version
file(READ source/VERSION WIRINGPI_VERSION)
string(STRIP ${WIRINGPI_VERSION} WIRINGPI_VERSION)
set_target_properties(wiringPi PROPERTIES VERSION ${WIRINGPI_VERSION})

# Setup depending libs
target_link_libraries(wiringPi PUBLIC m pthread rt crypt)
