##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(watchdog_pi)
SET(PACKAGE_NAME watchdog_pi)
SET(PLUGIN_SOURCE_DIR .)
MESSAGE (STATUS "*** Building ${PACKAGE_NAME} ***")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "0")

INCLUDE("cmake/PluginConfigure.cmake")

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out


SET(SRC_WATCHDOG
            src/watchdog_pi.h
            src/watchdog_pi.cpp
            src/watchdog_pi.h
            src/watchdog_pi.cpp
            src/WatchdogDialog.h
            src/WatchdogDialog.cpp
            src/WatchdogPrefsDialog.h
            src/WatchdogPrefsDialog.cpp
            src/Alarm.h
            src/Alarm.cpp
            src/WatchdogUI.h
            src/WatchdogUI.cpp
            src/icons.h
            src/icons.cpp
	)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WATCHDOG})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
