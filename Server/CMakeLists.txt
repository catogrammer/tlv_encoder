project (Server_MT)

set(SOURCES server_multithreaded.cc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable (Server_MT ${SOURCES})
target_link_libraries(Server_MT ${ACE_LINK_LIB})
