#
# This file is part of Project SkyFire https://www.projectskyfire.org. 
# See COPYRIGHT file for Copyright information
#

add_subdirectory(genrev)
add_subdirectory(server)

if(TOOLS)
  add_subdirectory(tools)
endif(TOOLS)

if(PACKET_LOG_SERVER)
  add_subdirectory(tools/packet_log_server)
endif()

if(SKYFIRE_BUILD_TESTS)
  add_subdirectory(tests)
endif()

