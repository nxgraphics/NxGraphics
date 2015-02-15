# NXNETWORK CMake build file
cmake_minimum_required(VERSION 2.6)

project(NxNetwork)

set(NxNetwork_LIBRARY NxNetwork)

set(NxNetwork_src
./NxNetwork.cpp	
./NxNetwork_Client.cpp		
./NxNetwork_Server.cpp
./NxNetwork_Plateform.cpp
./NxNetwork_Log.cpp
./NxNetwork_Utilities.cpp	
./NxNetworkPreCompiled.cpp
./NxNetwork_HttpClient.cpp
./NxNetwork_Singleton.h
./NxNetwork_Log.h
./NxNetwork_Utilities.h
./NxNetworkPreCompiled.h
./NxNetwork_Server.h
./NxNetwork_Client.h
./NxNetwork_Plateform.h
./NxNetwork.h
./NxNetwork_HttpClient.h
)

# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# warning level
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")
   # Use the lowest warning level for visual studio.
   SET(CMAKE_CXX_WARNING_LEVEL 0)
   IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W0"
            CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
   ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)")

   
include_directories(../nxdeps/include/curl) 

set(NxNetwork_DEF "NXNETWORK_NONCLIENT_BUILD;" )

set(NxNetwork_LINK_FLAGS "" )

if( MSVC )
 
# from http://www.cmake.org/pipermail/cmake/2007-November/017372.html	 
SET(NxNetwork_DEF "${NxNetwork_DEF}WIN32;_WINDOWS;_USRDLL;CURL_STATICLIB;BUILDING_LIBCURL;")
SET(NxNetwork_Deps "../NxDeps/lib/release/" )

SET( NxNetwork_EXTRALIBS 
ws2_32.lib
../NxDeps/lib/libcurl
)

else()


endif()

SET( ${NxNetwork_LIBRARY} "${NxNetwork_LIBRARY}" LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"/out/library)
set(NxNetwork_DEF "${NxNetwork_DEF}_CRT_SECURE_NO_WARNINGS;")
if( MSVC )
add_library(${NxNetwork_LIBRARY} SHARED ${NxNetwork_src})
else()
add_library(${NxNetwork_LIBRARY} STATIC ${NxNetwork_src})
endif()


set_target_properties(${NxNetwork_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxNetwork_DEF}" LINK_FLAGS_RELEASE "${NxNetwork_LINK_FLAGS}")
target_link_libraries(${NxNetwork_LIBRARY} ${NxNetwork_EXTRALIBS})



