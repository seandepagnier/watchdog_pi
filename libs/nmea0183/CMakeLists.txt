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
    src/dbt.cpp
    src/dbt.hpp
    src/dpt.cpp
    src/dpt.hpp
    src/expid.cpp
    src/gga.cpp
    src/gga.hpp
    src/gll.cpp
    src/gll.hpp
    src/gsv.cpp
    src/gsv.hpp
    src/hdg.cpp
    src/hdg.hpp
    src/hdm.cpp
    src/hdm.hpp
    src/hdt.cpp
    src/hdt.hpp
    src/hexvalue.cpp
    src/lat.cpp
    src/latlong.cpp			   
    src/LatLong.hpp
    src/long.cpp
    src/mda.cpp
    src/mda.hpp
    src/mta.cpp
    src/mta.hpp
    src/mtw.cpp
    src/mtw.hpp
    src/mwd.cpp
    src/mwd.hpp
    src/mwv.cpp
    src/mwv.hpp
    src/nmea0183.cpp
    src/nmea0183.h
    src/nmea0183.hpp
    src/response.cpp
    src/Response.hpp
    src/rmb.cpp
    src/RMB.hpp
    src/rmc.cpp			   
    src/RMC.HPP
    src/rsa.cpp
    src/rsa.hpp
    src/rte.cpp
    src/rte.hpp
    src/SatInfo.h
    src/sentence.cpp
    src/Sentence.hpp
    src/talkerid.cpp
    src/vhw.cpp
    src/vhw.hpp
    src/vlw.cpp
    src/vlw.hpp
    src/vtg.cpp
    src/vtg.hpp
    src/vwr.cpp
    src/vwr.hpp
    src/vwt.cpp
    src/vwt.hpp
    src/wpl.cpp
    src/wpl.hpp
    src/xdr.cpp
    src/xdr.hpp
	src/xte.hpp
    src/xte.hpp
    src/zda.cpp
    src/zda.hpp
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