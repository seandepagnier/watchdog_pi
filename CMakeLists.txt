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
#-----------------------------------------------------------------------------
# Frontend2 Author: Jon Gough  
# ---------------------------------------------------------------------------
# CMake-CI Number: 0.0.004  
# ---------------------------------------------------------------------------
# ----- Modify section below to include all the details for your plugin ----- ##

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

SET(VERBOSE_NAME Watchdog)
# Search "getcommonname"in weather_routing_pi.cpp
set(COMMON_NAME "Weatherfax")
set(TITLE_NAME "Watchdog")
set(PACKAGE_CONTACT "Sean d'Epagnier")
SET(PACKAGE "watchdog_pi")

set(VERSION_MAJOR "2")
set(VERSION_MINOR "4")
set(VERSION_PATCH "7")
set(VERSION_TWEAK "0")
set(VERSION_DATE "2020/04/27")
set(OCPN_MIN_VERSION "ov50")
set(OCPN_API_VERSION_MAJOR "1")
set(OCPN_API_VERSION_MINOR "16")

set(PARENT "opencpn")
#set(GIT_USER "seandepagnier")
#set(GIT_USER "jongough")
set(GIT_USER "rgleason")

set(GIT_REPOSITORY_NAME "watchdog_pi")
message(STATUS "CIRCLECI: ${CIRCLECLI}, Env CIRCLECI: $ENV{CIRCLECI}")
message(STATUS "TRAVIS: ${TRAVIS}, Env TRAVIS: $ENV{TRAVIS}")

## Allows setup of a local webserver with git for testing
if($ENV{CIRCLECI})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{CIRCLE_BRANCH}")
elseif($ENV{TRAVIS})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{TRAVIS_BRANCH}")
elseif($ENV{APPVEYOR})
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  set(GIT_REPOSITORY_BRANCH "$ENV{APPVEYOR_REPO_BRANCH}")
else()
  set(GIT_REPOSITORY_HOST "github.com")
  set(GIT_REPOSITORY_DIR "${GIT_USER}/")
  # Get the current working branch
  execute_process(
    COMMAND git rev-parse --abbrev-ref HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_REPOSITORY_BRANCH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    )
  if(${GIT_REPOSITORY_BRANCH} EQUAL "")
    message(STATUS "Setting default GIT repository branch - master")
    set(GIT_REPOSITORY_BRANCH "master")
  endif()
endif()
message(STATUS "Git Branch: ${GIT_REPOSITORY_BRANCH}")

#set(CLOUDSMITH_USER "sean-depagnier")
#set(CLOUDSMITH_USER "jon-gough")
set(CLOUDSMITH_USER "rick-gleason")

#set(CLOUDSMITH_BASE_REPOSITORY "${GIT_REPOSITORY_NAME}")
#set(CLOUDSMITH_BASE_REPOSITORY "weatherfax_pi")  #Jon Gough's customization
#set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Sean's customization
set(CLOUDSMITH_BASE_REPOSITORY "opencpn-plugins")  #Rick's customization

# Set Cloudsmith upload repository suffix for each catalog
set(PROD "prod") #Standard Repos
set(BETA "beta") #Standard Repos
set(ALPHA "alpha") #Standard Repos

# Set Plugin info-url, summary and description
set(XML_INFO_URL "https://opencpn.org/OpenCPN/plugins/weatherfax.html")
set(XML_SUMMARY "Open fax image files or decode audio faxes and overlay on chart")
set(XML_DESCRIPTION "Simple image overlay calibration, Converts any image image into a raster chart.  Db for HF radio fax stations via SSB and Metro URL internet retrieval.")


##
## ----- Modify above to include all the plugin details --------- ##
## ----- Modify below for plugin special requirements------------ ##
##

#------Settings
SET(BUNDLE_DATA FALSE)  #from wd
SET(CMAKE_MACOSX_RPATH "ON")  #from WD
option(PLUGIN_USE_SVG "Use SVG graphics" ON)  
#OPTION(WATCHDOG_USE_SVG "Use SVG graphics" ON) #from WD
SET(CMAKE_CXX_STANDARD 11)  #also from WD

# Prefer libGL.so to libOpenGL.so, see CMP0072
set(OpenGL_GL_PREFERENCE "LEGACY")
#------ End Settings

if(NOT ${PACKAGE} MATCHES "(.*)_pi")
  set(PACKAGE_NAME ${PACKAGE}_pi)
  set(PACKAGE_FILE_NAME "${PACKAGE}_pi")
else(NOT ${PACKAGE} MATCHES "(.*)_pi")
  set(PACKAGE_NAME ${PACKAGE})
  set(PACKAGE_FILE_NAME "${PACKAGE}")
endif(NOT ${PACKAGE} MATCHES "(.*)_pi")
string(TOUPPER "${PACKAGE}" TITLE_NAME)

project(${PACKAGE_NAME} VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK})
message(STATUS "PROJECT_VERSION: ${PROJECT_VERSION}")

set(PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(STATUS "${VERBOSE_NAME} Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_TWEAK}")
message(STATUS "OPCN_FLATPAK: ${OCPN_FLATPAK}")

#------Settings
# Don't use local version of GLU library
set(USE_LOCAL_GLU FALSE)

# Question still needed? on or off?
# option(USE_GL "Enable OpenGL support" ON)
message(STATUS "USE_GL: ${USE_GL}")
#------End Settings


if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "" "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(PKG_NVR ${PACKAGE_NAME}-${PROJECT_VERSION})
set(PKG_URL "https://dl.cloudsmith.io/public/--pkg_repo--/raw/names/--name--/versions/--version--/--filename--")

#===========================================================
INCLUDE("cmake/PluginSetup.cmake")
#===========================================================
if(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}-$ENV{OCPN_TARGET}")
else(DEFINED ENV{OCPN_TARGET})
    set(PACKAGING_NAME "${PKG_NVR}_${PKG_TARGET}-${PKG_TARGET_VERSION}")
endif(DEFINED ENV{OCPN_TARGET})
message(STATUS "PACKAGING_NAME: ${PACKAGING_NAME}")

set(PLUGIN_NAME ${PACKAGE}-plugin-${PKG_TARGET}-${PKG_TARGET_VERSION})

set(CMAKE_VERBOSE_MAKEFILE ON)

option(Plugin_CXX11 "Use c++11" OFF)

if(Plugin_CXX11)
  message(STATUS "Attempting to use c++11")
  include(CheckCXXCompilerFlag)
  check_cxx_compiler_flag("-std=c++11" COMPILER_SUPPORTS_CXX11)
  check_cxx_compiler_flag("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
  if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(Plugin_USE_CXX11 ON)
  elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set(Plugin_USE_CXX11 ON)
  else()
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
    set(Plugin_USE_CXX11 OFF)
  endif()
else()
  set(Plugin_USE_CXX11 OFF)
endif()

if(Plugin_USE_CXX11)
  message(STATUS "Allowing use of c++11")
endif()

message(STATUS "CMAKE version: ${CMAKE_VERSION}")
if(CMAKE_VERSION VERSION_GREATER 3.4)
  set(ENABLE_CLANG_TIDY
      OFF
      CACHE BOOL "Add clang-tidy automatically to builds")
  if(ENABLE_CLANG_TIDY)
    find_program(
      CLANG_TIDY_EXE
      NAMES "clang-tidy"
      PATHS /usr/local/opt/llvm/bin)
    if(CLANG_TIDY_EXE)
      message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
      # For more, see http://clang.llvm.org/extra/clang-tidy/ set(CLANG_TIDY_CHECKS "-*,modernize-*")
      set(CLANG_TIDY_CHECKS "-*,performance-*")
      set(CMAKE_CXX_CLANG_TIDY
          "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS};-header-filter='${CMAKE_SOURCE_DIR}/*'"
          CACHE STRING "" FORCE)
    else()
      message(AUTHOR_WARNING "clang-tidy not found!")
      set(CMAKE_CXX_CLANG_TIDY
          ""
          CACHE STRING "" FORCE) # delete it
    endif()
  endif()
endif()

if(CMAKE_VERSION VERSION_GREATER 3.9)
  set(ENABLE_CPPCHECK
      OFF
      CACHE BOOL "Add cppcheck automatically to builds")
  if(ENABLE_CPPCHECK)
    find_program(CPPCHECK_EXECUTABLE NAMES "cppcheck")
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXECUTABLE})
  endif()
endif()

#
# ----- Modify section above for special plugin requirements----- ##
#
## --Section above is direct from Weatherfax located above SET(SRC_WEATHERFAX  ---##
#===========================================================
INCLUDE("cmake/PluginConfigure.cmake")
#===========================================================
# ----- Change below to match your project requirements ----- ##
# ++++++++++++++Section below is direct from Watchdog +++++++++##

# needed?
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
  	        src/AIS_Targent_Info.h
			Alarm.h
			ConfigurationDialog.h
			EditAlarmDialog.h
			icons.h
			msvcdefs.h
			NewAlarmDialog.h
			ocpn_plugin.h
			ODAPI.h
			signalk_client.h
			Texfont.h
			version.h
			watchdog_pi.h
			WatchdogDialog.h
			WatchdogPropertiesDialog.h
			WatchdogUI.h
			wddc.h
			WeatherPanel.h
			WindPanel.h
			wxWTranslateCatalog.h
)


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
ADD_LIBRARY(${PACKAGE_NAME} SHARED ${SRCS} ${HDRS} ${SRC_JSON} ${SRC_NMEA0183} ${SRC_LTINYXML})
# ENDIF(TINYXML_FOUND)
ADD_DEFINITIONS(-DTIXML_USE_STL)


# +++++Section above is direct from Watchdog +++++++++++++##
#
#----INCLUDE CMAKE #3 & #4 -----
INCLUDE("cmake/PluginNMEA.cmake") #not for wxfax
INCLUDE("cmake/PluginJSON.cmake")  #not for wxfax
INCLUDE("cmake/PluginXML.cmake")
#INCLUDE("cmake/PluginGL.cmake")

# ----- Do not change - needed to build app ----- ##
#
#INCLUDE CMAKE #5,6 & 7 -----
INCLUDE("cmake/PluginInstall.cmake")
INCLUDE("cmake/PluginLocalization.cmake")
INCLUDE("cmake/PluginPackage.cmake")
