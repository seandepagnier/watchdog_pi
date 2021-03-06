cmake_minimum_required(VERSION 3.1.0)

if (TARGET ocpn::nmea0183)
    return ()
endif ()

if (NOT CMAKE_MODULE_PATH)
  set (CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../cmake)
endif ()

SET(SRC
    src/hexvalue.cpp
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
    src/GPwpl.cpp
    src/GPwpl.hpp
    src/apb.cpp
    src/apb.hpp
    src/mwd.cpp
    src/mwd.hpp
    src/mwv.cpp
    src/mwv.hpp
    src/nmea0183.cpp
    src/nmea0183.hpp
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
