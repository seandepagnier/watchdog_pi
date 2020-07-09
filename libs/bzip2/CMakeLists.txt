#
#  Provide a TinyXML 3.0-style linkable targetocpn::tinyxml which
#  provides all necessary libraries, headers and defines required to
#  use tinyxml
#
#  Probes for an available, system implementation and uses it if 
#  is available. If not, builds the library from bundled sources.

cmake_minimum_required(VERSION 3.1.0)


if (TARGET ocpn::bzip2)
  return ()
endif ()

include(ExternalProject)

add_library(_BZIP2 INTERFACE)
add_library(ocpn::bzip2 ALIAS _BZIP2)

find_package(BZip2)
if (BZIP2_FOUND)
  message(${CMLOC} "Using system bzip2")
  target_include_directories(_BZIP2 INTERFACE ${BZIP2_INCLUDE_DIR})
  target_link_libraries(_BZIP2 INTERFACE ${BZIP2_LIBRARIES})
else ()
  message(${CMLOC} "Using bundled bzip2")
  set (BZ2_LIBRARY ${CMAKE_CURRENT_SOURCE_DIR}/bzip2-1.0.6/libbz2.a)
  set (BZ2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bzip2-1.0.6)

  ExternalProject_Add(libbzip2
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bzip2-1.0.6
    CONFIGURE_COMMAND /usr/bin/true
    BUILD_COMMAND make -f Makefile-libbz2_so
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND  /bin/true
  )
  add_library(BZIP2 STATIC IMPORTED)
  add_dependencies(BZIP2 libbzip2)
  set_target_properties(BZIP2 PROPERTIES IMPORTED_LOCATION ${BZ2_LIBRARY})
  set_target_properties(
    BZIP2 PROPERTIES INTERFACE_INCLUDE_DIRECTORIES ${BZ2_DIR}
  )
  target_link_libraries(_BZIP2 INTERFACE BZIP2)
endif ()
