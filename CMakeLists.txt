##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(watchdog_pi)
SET(PACKAGE_NAME watchdog_pi)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

SET(VERSION_MAJOR "1")
SET(VERSION_MINOR "1")

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_WATCHDOG
            src/watchdog_pi.cpp
            src/watchdog_pi.cpp
            src/WatchdogDialog.cpp
            src/WatchdogPrefsDialog.cpp
            src/Alarm.cpp
            src/WatchdogUI.cpp
            src/icons.cpp
	)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WATCHDOG})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
