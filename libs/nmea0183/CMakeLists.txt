##---------------------------------------------------------------------------
## Author:      Sean D'Epagnier
## Copyright:   2015
## License:     GPLv3+
##---------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::nmea0183)
    return ()
endif ()

# add_library(_NMEA0183 INTERFACE)
# add_library(ocpn::nmea0183 ALIAS _nmea)

# find_package(nmea0183)

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

SET(SRC_NMEA0183
    src/latlong.cpp
    src/long.cpp
    src/nmea0183.cpp
    src/response.cpp
    src/rmb.cpp
    src/sentence.cpp
    src/talkerid.cpp
    src/rmc.cpp
    src/hexvalue.cpp
    src/lat.cpp
    src/expid.cpp
    src/wpl.cpp
    src/rte.cpp
    src/hdt.cpp
    src/hdg.cpp
    src/hdm.cpp
    src/gll.cpp
    src/vtg.cpp
    src/gga.cpp
    src/gsv.cpp
    src/dbt.cpp
    src/dpt.cpp
    src/mtw.cpp
    src/mwv.cpp
    src/vhw.cpp
    src/vwr.cpp
    src/zda.cpp
    src/rsa.cpp
    src/mwd.cpp
    src/vwt.cpp
    src/mta.cpp
    src/vlw.cpp
    src/mda.cpp
    src/xdr.cpp
)

#if (NOT wxWidgets_INCLUDE_DIRS)
#    include(SimpleWxConfig)
#    SimpleWxConfig()
#endif ()

add_library(NMEA0183 STATIC ${SRC_NMEA0183})
add_library(ocpn::nmea0183 ALIAS NMEA0183)

set_property(TARGET NMEA0183 PROPERTY COMPILE_FLAGS "${OBJ_VISIBILITY}")
target_include_directories(NMEA0183 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(NMEA0183 PRIVATE ${wxWidgets_INCLUDE_DIRS})

#originally
#INCLUDE_DIRECTORIES(src/nmea0183)
#ADD_LIBRARY(${PACKAGE_NAME}_LIB_PLUGINNMEA0183 STATIC ${SRC_NMEA0183})
#TARGET_LINK_LIBRARIES( ${PACKAGE_NAME} ${PACKAGE_NAME}_LIB_PLUGINNMEA0183 )
