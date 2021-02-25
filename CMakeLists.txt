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
# Frontend2: v1.0.155.0   9.02/2021   Frontend2 Author: Jon Gough  
# ---------------------------------------------------------------------------

## ----- When changing this file do NOT change the order in which sections occur        ----- ##
## ----- Changes should only be made between the section blocks that identify where     ----- ##
## ----- these changes should be. The whole configuration process relies on this        ----- ##
## ----- sequence to be successful                                                      ----- ##
## ----- Modify section below to include all the details for your plugin ----- ##
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
set(PACKAGE "watchdog_pi")
set(SHORT_DESCRIPTION "Implement watchdog ability for opencpn")
set(LONG_DESCRIPTION "Multiple Configurable alarms for  changing conditions for boundaries, cog, landfall, ais, wind, nmea, weather, pypilot, wind, deadman, etc.")

set(VERSION_MAJOR "2")
set(VERSION_MINOR "4")
set(VERSION_PATCH "22")
set(VERSION_TWEAK "24")
set(VERSION_DATE "24/2/2021") # DD/MM/YYYY format
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")
set(TP_COMMENT " Release for O52")											 

set(PARENT "opencpn")

# The next line allows for setting up your own local webserver with git on it for testing purposes, the default is github.com
# This line is turned on in pluginconfigure.cmake because the flatpak build and building locally depends on it.
# set(GIT_REPOSITORY_SERVER "github.com")

#No longer needed, queried.
#set(GIT_USER "rgleason")
#set(GIT_REPOSITORY_NAME "watchdog_pi")

# Specifies Cloudsmith Upload repository suffix
set(PROD "prod") # Standard Repos
set(BETA "beta") # Standard Repos
set(ALPHA "alpha") # Standard Repos

# Set if  Cloudsmith Base Repository name does not match your Git Repository name.
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins") #Rick's customization
set(CLOUDSMITH_BASE_REPOSITORY "watchdog")  #without the _pi

# Set the following to use OpenCPN cloudsmith repositories instead of own repositories
set(CLOUDSMITH_USER "opencpn")
#set(CLOUDSMITH_USER "rick-gleason")

# Set Plugin info-url, summary and description									  
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/watchdog.html")
set(XML_SUMMARY ${SHORT_DESCRIPTION})
set(XML_DESCRIPTION ${LONG_DESCRIPTION})

##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##


option(PLUGIN_USE_SVG "Use SVG graphics" ON)  
set(CMAKE_CXX_STANDARD 11)

# Use local version of GLU library
set(USE_LOCAL_GLU TRUE)
# Prefer libGL.so to libOpenGL.so, see CMP0072

set(OpenGL_GL_PREFERENCE "LEGACY")

# Don't use local version of GLU library
# set(USE_LOCAL_GLU FALSE)

#option(USE_GL "Enable OpenGL support" OFF)
option(USE_GL "Enable OpenGL support" ON)  #Turn ON, only fix found for GL_texture2d, glDisable, GL_BLEND  errors
message(STATUS "${CMLOC}USE_GL: ${USE_GL}")

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE "Release"
	CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "${CMLOC}Build type: ${CMAKE_BUILD_TYPE}")

##
## ----- The statements below are used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
##

project(${PACKAGE})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake;")

##=======================================
include("PluginSetup")
##=======================================
## ----- The above is used to setup standard variables that are required by the CMAKE process - do not remove ----- ##
## ----- Modify section below if there are special requirements for the plugin ----- ##
##

set(CMAKE_VERBOSE_MAKEFILE "Activate verbose mode for make files" ON)

option(Plugin_CXX11 "Use c++11" OFF)


## ----- Modify section above if there are special requirements for the plugin ----- ##
## ----- do not change next section - needed to configure build process ----- ##
#========================================================
include("PluginConfigure")
#========================================================
## ----- do not change section above - needed to configure build process ----- ##
## ----- Change below to match project requirements for source, headers, etc. ----- ##


add_definitions(-DUSE_S57)

#IF(PLUGIN_USE_SVG)
 # ADD_DEFINITIONS(-DWATCHDOG_USE_SVG)
#ENDIF(PLUGIN_USE_SVG)

IF(PLUGIN_USE_SVG)
    MESSAGE(STATUS "${CMLOC}SVG support enabled...")
    IF(NOT QT_ANDROID)
        ADD_DEFINITIONS(-DPLUGIN_USE_SVG)
    ENDIF(NOT QT_ANDROID)
ENDIF(PLUGIN_USE_SVG)

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

# left  src/jsoncpp in case needed	
#SET(LIBSRC
#			src/plugingl/pi_shaders.cpp
#			src/plugingl/pidc.cpp
#			src/plugingl/qtstylesheet.cpp
#			src/plugingl/TexFont.cpp
#			src/tinyxml/tinyxml.cpp
#			src/tinyxml/tinyxmlerror.cpp
#			src/tinyxml/tinyxmlparser.cpp
#    )

set(OCPNSRC ocpnsrc/TexFont.cpp)

set(OCPNHDRS ocpninclude/ocpn_plugin.h ocpninclude/pluginmanager.h ocpninclude/TexFont.h)

set(EXTSRC)

set(EXTINCLUDE ${EXTINCLUDE} extinclude)
set(EXTINCLUDE ${EXTINCLUDE} extinclude/ODAPI.h)

set(LIBSSRC libs/wxJSON/src/jsonreader.cpp libs/wxJSON/src/jsonval.cpp libs/wxJSON/src/jsonwriter.cpp)



#added for android  - removed these files aame are found in src/GL
set(LIBHDRS
    libs/GL/gl.h
    libs/GL/gl_private.h
    libs/GL/glext.h
    libs/GL/glu.h
    libs/wx/include/json_defs.h
    libs/wx/include/jsonreader.h
    libs/wx/include/jsonval.h
    libs/wx/include/jsonwriter.h)


							   
add_definitions(-DPLUGIN_USE_SVG)

if(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" OFF)
else(APPLE)
    option(PI_JSON_SCHEMA_VALIDATOR "Use JSON Schema validator" ON)
endif(APPLE)

#if(PI_JSON_SCHEMA_VALIDATOR)
#    message(STATUS "${CMLOC}Using JSON Schema validation - warning this changes the 'flavour' of the executable and it may not be compatible with OpenCPN")
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

include_directories(BEFORE ${PROJECT_SOURCE_DIR}/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/glu/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/${EXTINCLUDE})
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/libs/wx/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpninclude/wxcurl/include)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/lz4)
include_directories(BEFORE ${PROJECT_SOURCE_DIR}/ocpnsrc/bitmaps)

## ----- Change above to match project requirements ----- ##
## Statement below is required to collect all the set( headers and SRCS  - Adjust as required
## This add_library  can be used in conditionals below.
##==========================================================================
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${OCPNSRC} ${EXTSRC} ${LIBSSRC} ${SRC_NMEA0183})
##==========================================================================
#ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${EXTSRC} ${LIBHDRS} ${SRC_JSON} ${SRC_NMEA0183})
#ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${SRC_JSON} ${SRC_NMEA0183} )
#ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${EXTSRC} ${LIBSSRC}  ${LIBHDRS} ${SRC_JSON} ${SRC_NMEA0183} )

add_subdirectory(libs/tinyxml)
target_link_libraries(${PACKAGE_NAME} ocpn::tinyxml)

#add_subdirectory("libs/jsoncpp")
#target_link_libraries(${PACKAGE_NAME} ocpn::jsoncpp)

add_subdirectory("libs/wxJSON")
target_link_libraries(${PACKAGE_NAME} ocpn::wxjson)

add_subdirectory(libs/bzip2)
target_link_libraries(${PACKAGE_NAME} ocpn::bzip2)

#add_subdirectory(libs/nmea0183)
#target_link_libraries(${PACKAGE_NAME} ocpn::nmea0183)
#had errors,  trying PluginNmeaCmake

#needed only if src/glshim project should be included.
if(QT_ANDROID)
  target_link_libraries(${PACKAGE_NAME}  GL_static)
endif(QT_ANDROID)  

add_definitions(-DTIXML_USE_STL)

#JSON Validator: Every plugin that uses JSON for passing messages should use a validator.  A JSON validator is much like an XML validator, the validator allows the system to ignore non-conformant messages.  The JSON validator changes the OSABI flavor of the Linux executable so there is code to change it back.

 if(UNIX AND NOT APPLE AND NOT QT_ANDROID)
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
endif(UNIX AND NOT APPLE AND NOT QT_ANDROID)


## ----- Change above to match project requirements ----- ##
#These are NOT used in Testplugin, used for other plugins
##=======================================
#include("PluginGL")
include("PluginNMEA") #not for wxfax
include("PluginJSON")  #not for wxfax  #commented out using libs
##=======================================
## ----- Change above to match project requirements ----- ##


## ----- do not change next section - needed to configure build process ----- ##
##=======================================
include("PluginInstall")
include("PluginLocalization")
include("PluginPackage")
##=======================================
## ----- do not change section above - needed to configure build process ----- ##

message(STATUS "${CMLOC}include directories: ")
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "${CMLOC}dir='${dir}'")
endforeach()
