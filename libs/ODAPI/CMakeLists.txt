# ~~~
# Summary:      ODAPI headers library cmake setup
# License:      GPLv3+
# Copyright (c) 2023 Dave Register
#
# ~~~

cmake_minimum_required(VERSION 3.5)

if (TARGET ocpn::odapi)
  return ()
endif ()


add_library(OCPN_ODAPI INTERFACE)
add_library(ocpn::odapi ALIAS OCPN_ODAPI)
target_include_directories(
  OCPN_ODAPI INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)

