##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   
## License:     GPLv3
##---------------------------------------------------------------------------

message(STATUS "Processing PluginGL.cmake")
# for weather_routing
#SET(SRC_PLUGINGL
#          src/plugingl/pidc.cpp  
#          src/plugingl/pi_shaders.cpp
#          src/plugingl/TexFont.cpp
#          src/plugingl/qtstylesheet.cpp
#          )


# changed for watchdog
SET(SRC_PLUGINGL
          src/GL/gl.h  
          src/GL/glext.h
 )

if(UNIX)
    add_definitions("-fpic")
endif(UNIX)

message(STATUS "PROJECT_SOURCE_DIR: ${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/GL)   #added for Watchdog

ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINGL STATIC ${SRC_PLUGINGL})
TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINGL )
message(STATUS "Add Library ${PACKAGE_NAME}_LIB_PLUGINGL")  #added for Watchdog

