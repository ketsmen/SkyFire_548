#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
#

set(packetlogserver_SRCS
  Main.cpp
)

if( WIN32 )
  list(APPEND packetlogserver_SRCS packetlogserver.rc)
endif()

add_executable(packetlogserver
  ${packetlogserver_SRCS}
)

if( WIN32 )
  target_include_directories(packetlogserver PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if( UNIX )
  install(TARGETS packetlogserver DESTINATION bin)
elseif( WIN32 )
  install(TARGETS packetlogserver RUNTIME DESTINATION ".")
endif()
