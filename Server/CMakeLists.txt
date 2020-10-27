project (Server_MT)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    )

set(SOURCES server_multithreaded.cc 
            src/Logging_Server.cc
            src/Thread_Per_Connection_Logging_Server.cc
    )

add_executable (Server_MT ${SOURCES})
target_link_libraries(Server_MT ${ACE_LINK_LIB} ${TLV_LIB})
