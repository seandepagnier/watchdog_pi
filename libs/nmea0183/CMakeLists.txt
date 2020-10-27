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

SET(SRC
#   Sentences available in O-nmea Lib
    src/hexvalue.cpp
    src/dbt.hpp
    src/dbt.cpp
    src/dpt.hpp
    src/dpt.cpp
    src/LatLong.hpp
    src/latlong.cpp
    src/lat.cpp
    src/long.cpp
    src/expid.cpp
    src/hdt.hpp
    src/hdt.cpp
    src/hdg.hpp
    src/hdg.cpp
    src/hdm.hpp
    src/hdm.cpp
    src/gll.hpp
    src/gll.cpp
    src/gga.hpp
    src/gga.cpp
    src/gsv.hpp
    src/gsv.cpp
#   Not Available in watchdog nmea lib
#    src/GPwpl.cpp
#    src/GPwpl.hpp
#    src/apb.cpp
#    src/apb.hpp
    src/mwd.cpp
    src/mwd.hpp
    src/mwv.cpp
    src/mwv.hpp
    src/nmea0183.cpp
    src/nmea0183.hpp
#    src/nmea0183.h
    src/Response.hpp
    src/response.cpp
    src/rmb.cpp
    src/RMB.hpp
    src/rmc.cpp
    src/RMC.HPP
    src/rte.hpp
    src/rte.cpp
    src/Sentence.hpp
    src/sentence.cpp
    src/talkerid.cpp
    src/vtg.hpp
    src/vtg.cpp
    src/wpl.hpp
    src/wpl.cpp
    src/xte.cpp
    src/xte.hpp

#   Sentences not available in O-nmea Lib
#   Senetences neeeded for Watchdog to work
    src/mda.hpp
    src/mda.cpp
    src/mta.hpp
    src/mta.cpp
    src/mtw.hpp
    src/mtw.cpp
    src/rsa.hpp
    src/rsa.cpp
    src/SatInfo.h
    src/vhw.hpp
    src/vhw.cpp
    src/vlw.hpp
    src/vlw.cpp
    src/vwr.hpp
    src/vwr.cpp
    src/vwt.hpp
    src/vwt.cpp
    src/xdr.hpp
    src/xdr.cpp
    src/zda.hpp
    src/zda.cpp
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