##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::nmea0183)
    return ()
endif ()

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

SET(SRC_NMEA0183
    src/nmea0183/latlong.cpp
    src/nmea0183/long.cpp
    src/nmea0183/nmea0183.cpp
    src/nmea0183/response.cpp
    src/nmea0183/rmb.cpp
    src/nmea0183/sentence.cpp
    src/nmea0183/talkerid.cpp
    src/nmea0183/rmc.cpp
    src/nmea0183/hexvalue.cpp
    src/nmea0183/lat.cpp
    src/nmea0183/expid.cpp
    src/nmea0183/wpl.cpp
    src/nmea0183/rte.cpp
    src/nmea0183/hdt.cpp
    src/nmea0183/hdg.cpp
    src/nmea0183/hdm.cpp
    src/nmea0183/gll.cpp
    src/nmea0183/vtg.cpp
    src/nmea0183/gga.cpp
    src/nmea0183/gsv.cpp
    src/nmea0183/dbt.cpp
    src/nmea0183/dpt.cpp
    src/nmea0183/mtw.cpp
    src/nmea0183/mwv.cpp
    src/nmea0183/vhw.cpp
    src/nmea0183/vwr.cpp
    src/nmea0183/zda.cpp
    src/nmea0183/rsa.cpp
    src/nmea0183/mwd.cpp
    src/nmea0183/vwt.cpp
    src/nmea0183/mta.cpp
    src/nmea0183/vlw.cpp
    src/nmea0183/mda.cpp
    src/nmea0183/xdr.cpp
)

if (NOT wxWidgets_INCLUDE_DIRS)
    include(SimpleWxConfig)
    SimpleWxConfig()
endif ()

add_library(NMEA0183 STATIC ${SRC})
add_library(ocpn::nmea0183 ALIAS NMEA0183)

set_property(TARGET NMEA0183 PROPERTY COMPILE_FLAGS "${OBJ_VISIBILITY}")
target_include_directories(NMEA0183 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(NMEA0183 PRIVATE ${wxWidgets_INCLUDE_DIRS})

#originally
# INCLUDE_DIRECTORIES(src/nmea0183)
#
# ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINNMEA0183 STATIC ${SRC_NMEA0183})
# TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINNMEA0183 )
