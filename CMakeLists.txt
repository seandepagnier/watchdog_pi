# ---------------------------------------------------------------------------
# Frontend2 Author: jongough aka. Jon Gough    License:wxWidgets License
# ---------------------------------------------------------------------------
##---------------------------------------------------------------------------
## Author:      Sean d'Epagnier
##***************************************************************************
## *   Copyright (C) 2010 by Sean d'Epagnier                              *
## *   License:     GPLv3+
## *   This program is free software; you can redistribute it and/or modify  *
## *   it under the terms of the GNU General Public License as published by  *
## *   the Free Software Foundation; either version 2 of the License, or     *
## *   (at your option) any later version.                                   *
## *                                                                         *
## *   This program is distributed in the hope that it will be useful,       *
## *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
## *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
## *   GNU General Public License for more details.                          *
## *                                                                         *
## *   You should have received a copy of the GNU General Public License     *
## *   along with this program; if not, write to the                         *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx,                                       *
## *   xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx                                       *
## ***
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# Frontend2: v1.0.120
# ---------------------------------------------------------------------------

## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##

##
##----- Modify section below to include all the details for your plugin ----- ##
##

set(CMLOC "CMakeLists: ")

# define minimum cmake version
cmake_minimum_required(VERSION 3.1.1)
if(COMMAND cmake_policy)
    if(POLICY CMP0043)
        cmake_policy(SET CMP0043 NEW)
    endif(POLICY CMP0043)
    cmake_policy(SET CMP0048 NEW)
    if(POLICY CMP0077)
        cmake_policy(SET CMP0077 NEW)
    endif(POLICY CMP0077)
endif(COMMAND cmake_policy)

# define plugin name, owner and versions
SET(VERBOSE_NAME "Watchdog")
set(COMMON_NAME "Watchdog")  # Search "getcommonname"in watchdog_pi.cpp
set(TITLE_NAME "Watchdog")
set(PACKAGE_CONTACT "Sean d'Epagnier")
SET(PACKAGE "watchdog_pi")

set(VERSION_MAJOR "2")
set(VERSION_MINOR "4")
set(VERSION_PATCH "13")
set(VERSION_TWEAK "0")
set(VERSION_DATE "7/01/2020")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")

#Used to set up a local webserver with git loaded, for testing purposes.
set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
#set(GIT_USER "rgleason")
#set(GIT_REPOSITORY_NAME "watchdog_pi")
# Set Cloudsmith upload repository suffix for each catalog
set(PROD "prod")   #Standard Repos
set(BETA "beta")   #Standard Repos
set(ALPHA "alpha") #Standard Repos

# set(CLOUDSMITH_USER "sean-depagnier")
# set(CLOUDSMITH_USER "rick-gleason")
set(CLOUDSMITH_USER "opencpn")

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
# set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
# set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins") #Rick's customization
set(CLOUDSMITH_BASE_REPOSITORY "watchdog")  #without the pi

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/watchdog.html")
set(XML_SUMMARY "Implement watchdog ability for opencpn")
set(XML_DESCRIPTION "Multiple Configurable alarms for  changing conditions for boundaries, cog, landfall, ais, wind, nmea, weather, pypilot, wind, deadman, etc.")

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

SET(BUNDLE_DATA FALSE)  #from wd
SET(CMAKE_MACOSX_RPATH "ON")  #from WD

option(PLUGIN_USE_SVG "Use SVG graphics" ON)  

set(CMAKE_CXX_STANDARD 11)

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")

# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

# Question still needed? on or off?
# option(USE_GL "Enable OpenGL support" OFF)
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE
        "Release"
        CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

#
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(watchdog_pi)

## ----- The statement below is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
#===========================================================
INCLUDE("cmake/PluginSetup.cmake")
#===========================================================
##
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##
##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

## ----- Modify section above if there are special requirements for the plugin ----- ##

## ----- do not change next section - needed to configure build process ----- ##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
## ----- do not change section above - needed to configure build process ----- ##
# ----- Change below to match project requirements for source, headers, etc. ----- ##

add_definitions(-DUSE_S57)

# ----Remember to Add your project Set(SRCS and set(HDRS  etc..	
# ++++++++++++++Section below is direct from Watchdog +++++++++##

IF(WATCHDOG_USE_SVG)
  ADD_DEFINITIONS(-DWATCHDOG_USE_SVG)
ENDIF(WATCHDOG_USE_SVG)

SET(SRCS
            src/watchdog_pi.cpp
            src/WatchdogDialog.cpp
            src/WatchdogPropertiesDialog.cpp
            src/ConfigurationDialog.cpp
            src/NewAlarmDialog.cpp
            src/EditAlarmDialog.cpp
            src/Alarm.cpp
            src/signalk_client.cpp
            src/WatchdogUI.cpp
            src/icons.cpp
            src/ODAPI.h
            src/wddc.cpp
            src/TexFont.cpp
	)
	
SET(HDRS
  	        src/AIS_Target_Info.h
			src/Alarm.h
			src/ConfigurationDialog.h
			src/EditAlarmDialog.h
			src/icons.h
			src/msvcdefs.h
			src/NewAlarmDialog.h
			src/ODAPI.h
			src/signalk_client.h
			src/TexFont.h
			src/watchdog_pi.h
			src/WatchdogDialog.h
			src/WatchdogPropertiesDialog.h
			src/WatchdogUI.h
			src/wddc.h
			src/WeatherPanel.h
			src/WindPanel.h
			src/wxWTranslateCatalog.h

            api-16/ocpn_plugin.h
 )

add_definitions(-DPLUGIN_USE_SVG)

if(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
else(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
endif(APPLE)
																					 

if(PI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
    add_definitions(-DPI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Adding JSON Schema validation")

    set(EXTSRC ${EXTSRC} extsrc/json-schema-validator/json-schema-draft7.json.cpp extsrc/json-schema-validator/json-uri.cpp extsrc/json-schema-validator/json-validator.cpp)
    set(EXTINCLUDE ${EXTINCLUDE} extinclude/nlohmann/json.hpp extinclude/json-schema-validator/json-schema.hpp)
    include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude/json-schema-validator)
else(PI_JSON_SCHEMA_VALIDATOR)
    message(STATUS "${CMLOC}Not using JSON Schema validation - warning invalid JSON 'may' cause unexpected crashes")
endif(PI_JSON_SCHEMA_VALIDATOR)


##=============================================================
# Modify/configure 'Include' Directories below for the Plugin
##=============================================================
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/extinclude)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
# include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

##================================================================================
## Required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##================================================================================	
#    SET(EXTSRC  ${EXTSRC}   etc   Can be in an if statement as below
##================================================================================			

#INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/libaudiofile)
#INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/plugingl)
INCLUDE_DIRECTORIES(api-16)

# ----- Change above to match project requirements ----- ##
#--------REMOVE LN 194 to LN 296 from Testplugin_pi---------------
#------- Change below to match project requirements --------------#

#----INCLUDE CMAKE -----
# INCLUDE("cmake/PluginNMEA.cmake")  #from WD
# INCLUDE("cmake/PluginJSON.cmake")  #from WD
#INCLUDE("cmake/PluginXML.cmake")  #WD not used
#INCLUDE("cmake/PluginGL.cmake")  #WD not used
#----INCLUDE CMAKE -----

#IF(UNIX)
# INCLUDE("cmake/FindTinyXML.cmake")
# FIND_PACKAGE(TinyXML QUIET)
#ENDIF(UNIX)

#IF(TINYXML_FOUND)
# message (STATUS "Building with system tinyxml")
# INCLUDE_DIRECTORIES(${TINYXML_INCLUDE_DIR})
# ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRC_WATCHDOG} ${SRC_JSON} ${SRC_NMEA0183})
# TARGET_LINK_LIBRARIES(${PACKAGE_NAME} ${TINYXML_LIBRARIES})
#ELSE(TINYXML_FOUND)
# message (STATUS "Building with embedded tinyxml")
# INCLUDE_DIRECTORIES(BEFORE ${PROJECT_SOURCE_DIR}/src/tinyxml/)
# SET(SRC_LTINYXML
#            src/tinyxml/tinyxml.cpp
#            src/tinyxml/tinyxmlparser.cpp
#            src/tinyxml/tinyxmlerror.cpp
# )

if(UNIX)
    add_definitions("-fpic")
endif(UNIX)

ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${SRC_JSON} ${SRC_NMEA0183} ${SRC_LTINYXML})
# ENDIF(TINYXML_FOUND)

ADD_DEFINITIONS(-DTIXML_USE_STL)

# +++++Section above is direct from Watchdog +++++++++++++##
# Was commented out

#JSON Validator: Every plugin that uses JSON for passing messages should use a validator.  A JSON validator is much like an XML validator, the validator allows the system to ignore non-conformant messages.  The JSON validator changes the OSABI flavor of the Linux executable so there is code to change it back.

if(UNIX AND NOT APPLE)
    if(PI_JSON_SCHEMA_VALIDATOR)
        # Change OS/ABI to be UNIX - System V to make it run with OCPN
        set(lib_name lib${PACKAGE_NAME}.so)
        message(STATUS "${CMLOC}lib-name: ${lib_name}")
        find_file(elfedit elfedit)
        if(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Will ensure OS/ABI is set to 0 to match OCPN")
            add_custom_command(
                TARGET ${PACKAGE_NAME}
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                DEPENDS ${PACKAGE_NAME}
                COMMENT " Running post build action on ${lib_name}."
                COMMAND sh -c 'elfedit --input-osabi=Linux --output-osabi=none ${lib_name} || (echo 0) ')
        else(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
            message(STATUS "${CMLOC}Cannot correct OS/ABI to match OCPN if it is generated incorrectly. Package binutils needed")
        endif(NOT (${elfedit} STREQUAL "elfedit-NOTFOUND"))
    endif(PI_JSON_SCHEMA_VALIDATOR)
endif(UNIX AND NOT APPLE)

##================================================================================
## ----- Change above to match project requirements ----- ##
#These are NOT used in Testplugin, used for other plugins
#INCLUDE("cmake/PluginGL.cmake")
INCLUDE("cmake/PluginNMEA.cmake") #not for wxfax
INCLUDE("cmake/PluginJSON.cmake")  #not for wxfax
INCLUDE("cmake/PluginXML.cmake")

##
## ----- Change above to match project requirements ----- ##
##
## ----- Do not change - needed to build app ----- ##
#INCLUDE CMAKE #5,6 & 7 -----
INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
