##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3+
##---------------------------------------------------------------------------
 
# define minimum cmake version
CMAKE_MINIMUM_REQUIRED(VERSION 2.6.2)

PROJECT(watchdog_pi)
SET(PACKAGE_NAME watchdog_pi)
SET(VERBOSE_NAME Watchdog)
SET(TITLE_NAME WATCHDOG)
SET(CPACK_PACKAGE_CONTACT "Sean D'Epagnier ")

SET(BUNDLE_DATA FALSE)

INCLUDE( VERSION.cmake )
SET(PLUGIN_VERSION "${PLUGIN_VERSION_MAJOR}.${PLUGIN_VERSION_MINOR}.${PLUGIN_VERSION_PATCH}" )

SET(VERSION_MAJOR ${PLUGIN_VERSION_MAJOR})
SET(VERSION_MINOR "${PLUGIN_VERSION_MINOR}${PLUGIN_VERSION_PATCH}")
SET(VERSION_DATE ${PLUGIN_VERSION_DATE})

INCLUDE("cmake/PluginConfigure.cmake")

SET(SRC_WATCHDOG
            src/watchdog_pi.cpp
            src/watchdog_pi.cpp
            src/WatchdogDialog.cpp
            src/WatchdogPrefsDialog.cpp
            src/Alarm.cpp
            src/WatchdogUI.cpp
            src/icons.cpp

            src/wxJSON/jsonreader.cpp
            src/wxJSON/jsonval.cpp
            src/wxJSON/jsonwriter.cpp

            src/wddc.cpp
            src/TexFont.cpp
	)
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WATCHDOG})

INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
