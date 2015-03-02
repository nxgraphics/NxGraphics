# NXMOCAP CMake build file
cmake_minimum_required(VERSION 2.8)

INCLUDE(CheckIncludeFiles)

project(NxMocap)

set(NxMocap_LIBRARY NxMocap)

set(NxMocap_src
./NxMocap.cpp		
./NxMocap_Device.cpp
./NxMocap_DeviceKinect.cpp
./NxMocap_Log.cpp
./NxMocap_Mysql.cpp	
./NxMocap_NetStreamClient.cpp
./NxMocap_NetStreamServer.cpp
./NxMocap_Plateform.cpp
./NxMocap_Player.cpp
./NxMocap_PlayerBvh.cpp
./NxMocap_PlayerKinect.cpp
./NxMocap_PlayerPolhemus.cpp
./NxMocap_Recorder.cpp
./NxMocap_RecorderPolhemus.cpp
./NxMocap_Skeleton.cpp
./NxMocap_User.cpp
./NxMocap_Utilities.cpp
./NxMocapPreCompiled.cpp		
./NxMocap.h
./NxMocap_Devices.h
./NxMocap_Headers.h
./NxMocap_Log.h
./NxMocap_Mysql.h
./NxMocap_Plateform.h
./NxMocap_Player.h
./NxMocap_Prerequisites.h
./NxMocap_Recorder.h
./NxMocap_Singleton.h
./NxMocap_Skeleton.h
./NxMocap_Stream.h
./NxMocap_Utilities.h
./NxMocap_Vector3.h
./NxMocapPrecompiled.h
)

SOURCE_GROUP("Header Files" FILES 
./NxMocap.h
./NxMocap_Devices.h
./NxMocap_Headers.h
./NxMocap_Log.h
./NxMocap_Mysql.h
./NxMocap_Plateform.h
./NxMocap_Player.h
./NxMocap_Prerequisites.h
./NxMocap_Recorder.h
./NxMocap_Singleton.h
./NxMocap_Skeleton.h
./NxMocap_Stream.h
./NxMocap_Utilities.h
./NxMocap_Vector3.h
./NxMocap_Precompiled.h
)

SOURCE_GROUP("Source Files" FILES 
./NxMocap.cpp		
./NxMocap_Device.cpp
./NxMocap_DeviceKinect.cpp
./NxMocap_Log.cpp
./NxMocap_Mysql.cpp	
./NxMocap_NetStreamClient.cpp
./NxMocap_NetStreamServer.cpp
./NxMocap_Plateform.cpp
./NxMocap_Player.cpp
./NxMocap_PlayerBvh.cpp
./NxMocap_PlayerKinect.cpp
./NxMocap_PlayerPolhemus.cpp
./NxMocap_Recorder.cpp
./NxMocap_RecorderPolhemus.cpp
./NxMocap_Skeleton.cpp
./NxMocap_User.cpp
./NxMocap_Utilities.cpp
./NxMocap_PreCompiled.cpp)


#--------------------------
if( MSVC )

set(NxMocap_src
${NxMocap_src}
./NxMocap_DeviceKinectOpenNi2.cpp
./NxMocap_DeviceKinectMicrosoft.cpp
)

elseif(APPLE)

#set(NxMocap_src ${NxMocap_src} ./NxVideo_Mov_Player2.cpp )
elseif(ANDROID)	
	message( "NxMocap detected Android !" )
	 

else()

endif()

#-------------------
include_directories(../nxdeps/include) 
# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

set(NxMocap_DEF "NXMOCAP_NONCLIENT_BUILD;" )
#set(NxMocap_LINK_FLAGS "" )

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

if( MSVC )
	#openNI
	CHECK_INCLUDE_FILES("OpenNi.h" HAVE_OPENNI_H)
	IF(NOT HAVE_OPENNI_H)
		IF(NOT DEFINED OPENNI2_INCLUDE)
		# env is not set
		SET(OPENNI2_INCLUDE $ENV{OPENNI2_INCLUDE})
		IF(NOT DEFINED OPENNI2_INCLUDE)
		MESSAGE("OPENNI2_INCLUDE should be set, Did you install openNi Framework ?")
		ENDIF()
	ENDIF()
INCLUDE_DIRECTORIES(${OPENNI2_INCLUDE})
MESSAGE("OPENNI2_INCLUDE : ${OPENNI2_INCLUDE}")
ENDIF()


# openNI lib
IF(WIN32)
	SET(OPEN_LIBRARY_NAME OpenNI2)
ELSE()
	SET(OPEN_LIBRARY_NAME OpenNI2)
ENDIF()

FIND_LIBRARY(OPEN_NI_LIBRARY NAMES ${OPEN_LIBRARY_NAME} PATHS ${OPENNI2_LIB})

IF(NOT OPEN_NI_LIBRARY)
	SET(OPENNI2_LIB $ENV{OPENNI2_LIB})
	IF(NOT DEFINED OPENNI2_LIB)
		# env is not set
		MESSAGE("OPEN_NI_LIB should be set, Did you install openni framework ? ")
	ELSE()
		FIND_LIBRARY(OPEN_NI_LIBRARY NAMES OpenNI2 PATHS ${OPENNI2_LIB})
	ENDIF()
ENDIF()

 


# --------------------------------------------------------------
# NITE2
CHECK_INCLUDE_FILES("NiTE.h" HAVE_NITE_H)
IF(NOT HAVE_NITE_H)
IF(NOT DEFINED NITE2_INCLUDE)
# env is not set
SET(NITE2_INCLUDE $ENV{NITE2_INCLUDE})
IF(NOT DEFINED NITE2_INCLUDE)
MESSAGE("NITE2_INCLUDE should be set, Did you install NiTE2 Framework ?")
ENDIF()
ENDIF()
INCLUDE_DIRECTORIES(${NITE2_INCLUDE})
MESSAGE("NITE2_INCLUDE : ${NITE2_INCLUDE}")
ENDIF()


#NiTE2 lib
IF(WIN32)
SET(NITE2_LIBRARY_NAME NiTE2)
ELSE()
SET(NITE2_LIBRARY_NAME NiTE2)
ENDIF()

FIND_LIBRARY(NiTE2_LIBRARY NAMES ${NITE2_LIBRARY_NAME} PATHS ${NITE2_LIB})

IF(NOT NiTE2_LIBRARY)
SET(NiTE2_LIB $ENV{NiTE2_LIB})
IF(NOT DEFINED NiTE2_LIB)
# env is not set
MESSAGE("OPEN_NI_LIB should be set, Did you install openni framework ? ")
ELSE()
FIND_LIBRARY(NiTE2_LIBRARY NAMES NiTE2 PATHS ${NiTE2_LIB})
ENDIF()
ENDIF()

# --------------------------------------------------------------
# KINECTSDK INCLUDES
CHECK_INCLUDE_FILES("NuiApi.h" HAVE_NUIAPI_H)
IF(NOT HAVE_NUIAPI_H)
IF(NOT DEFINED KINECTSDK_INCLUDE)
# env is not set
SET(KINECTSDK_INCLUDE $ENV{KINECTSDK10_DIR})
IF(NOT DEFINED KINECTSDK_INCLUDE)
MESSAGE("KINECTSDK should be set, Did you install Kinect sdk  Framework ?")
ENDIF()
ENDIF()
INCLUDE_DIRECTORIES("${KINECTSDK_INCLUDE}inc")
MESSAGE("KINECTSDK_INCLUDE : ${KINECTSDK_INCLUDE}")
ENDIF()


#KINECTSDK lib
IF(WIN32)
	SET(KINECTSDK_LIBRARY_NAME Kinect10)
ELSE()
	SET(KINECTSDK_LIBRARY_NAME Kinect10)
ENDIF()

FIND_LIBRARY(KINECTSDK_LIBRARY NAMES ${KINECTSDK_LIBRARY_NAME} PATHS ${KINECTSDK_LIB})

IF(NOT KINECTSDK_LIBRARY)
	SET(KINECTSDK_LIB $ENV{KINECTSDK10_DIR})
	IF(NOT DEFINED KINECTSDK_LIB)
	# env is not set
	MESSAGE("KINECTSDK10_DIR should be set, Did you install kinect sdk framework ? ")
	ELSE()
	FIND_LIBRARY(KINECTSDK_LIBRARY NAMES Kinect10 PATHS "${KINECTSDK_LIB}lib/x86/")
	ENDIF()
ENDIF()

 

#=====================



 

# from http://www.cmake.org/pipermail/cmake/2007-November/017372.html

SET( NxMocap_LINK_FLAGS "${NxMocap_LINK_FLAGS} /DELAYLOAD:\"OpenNi2.dll\";\"NiTE2.dll\"" )

SET( NxMocap_LINK_FLAGS "${NxMocap_LINK_FLAGS} /NODEFAULTLIB:libcmt.lib" )

SET( NxMocap_LINK_FLAGS "${NxMocap_LINK_FLAGS} /LIBPATH:\"../NxDeps/lib/${CMAKE_BUILD_TYPE}/\"" )

SET( NxMocap_DEF "${NxMocap_DEF}WIN32;_WINDOWS;_USRDLL;")
 


SET( NxMocap_EXTRALIBS 
ws2_32.lib
debug libmysql_d optimized libmysql
${OPEN_NI_LIBRARY}
${NiTE2_LIBRARY}
${KINECTSDK_LIBRARY}
delayimp.lib
)

add_library(${NxMocap_LIBRARY} SHARED ${NxMocap_src})
else()
add_library(${NxMocap_LIBRARY} STATIC ${NxMocap_src})
endif()
 
set_target_properties(${NxMocap_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxMocap_DEF}" LINK_FLAGS_RELEASE "${NxMocap_LINK_FLAGS}" )

set_target_properties(${NxMocap_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxMocap_DEF}" LINK_FLAGS_DEBUG "${NxMocap_LINK_FLAGS}" )


target_link_libraries(${NxMocap_LIBRARY} ${NxMocap_EXTRALIBS})



