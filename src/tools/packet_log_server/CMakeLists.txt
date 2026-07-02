#
# This file is part of Project SkyFire https://www.projectskyfire.org.
# See LICENSE.md file for Copyright information
#

add_executable(packetlogserver
  Main.cpp
)

if( UNIX )
  install(TARGETS packetlogserver DESTINATION bin)
elseif( WIN32 )
  install(TARGETS packetlogserver RUNTIME DESTINATION ".")
endif()
