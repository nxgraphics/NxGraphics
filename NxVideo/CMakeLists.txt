# NXVIDEO CMake build file
cmake_minimum_required(VERSION 2.6)

project(NxVideo)

set(NxVideo_LIBRARY NxVideo)

set(NxVideo_src
./NxVideo.cpp		
./NxVideo_Log.cpp
./NxVideo_Plateform.cpp	
./NxVideo_Player.cpp 
./NxVideo_Recorder.cpp
./NxVideo_Utilities.cpp
./NxVideoPreCompiled.cpp
./NxVideo_Buffer.cpp
./NxVideo_Grabber.cpp
./NxVideo_Grabber_Quicktime.cpp
./NxVideo.h		
./NxVideo_Grabber.h		
./NxVideo_Headers.h	
./NxVideo_Log.h
./NxVideo_Plateform.h
./NxVideo_Player.h
./NxVideo_Prerequisites.h
./NxVideo_Singleton.h
./NxVideo_Utilities.h	
./NxVideo_Video.h
./NxVideo_Format.h
./NxVideoPreCompiled.h
)

SOURCE_GROUP("Header Files" FILES 
./NxVideo.h		
./NxVideo_Grabber.h		
./NxVideo_Headers.h	
./NxVideo_Log.h
./NxVideo_Plateform.h
./NxVideo_Player.h
./NxVideo_Prerequisites.h
./NxVideo_Singleton.h
./NxVideo_Utilities.h	
./NxVideo_Video.h
./NxVideo_Format.h
./NxVideoPreCompiled.h
)

SOURCE_GROUP("Source Files\\Buffers" FILES 
./NxVideo_Buffer.cpp
)

SOURCE_GROUP("Source Files\\Camera Input" FILES 
./NxVideo_Grabber.cpp
./NxVideo_Grabber_Quicktime.cpp
)



if( MSVC )

	set(NxVideo_src
	${NxVideo_src}
	./NxVideo_Grabber_DirectShow.cpp
	./NxVideo_Grabber_PointGrey.cpp
	./NxVideo_Grabber_VFW.cpp
	./NxVideo_Avi_Player.cpp
	./NxVideo_Avi_Recorder.cpp
	./NxVideo_Mov_Player2.cpp
	./NxVideo_Theora_Player.cpp
	../nxdeps/include/VideoInput/videoInput.cpp
	)

	SOURCE_GROUP("LIBRAIRIES\\VIDEOINPUT\\Sources" FILES 
	../nxdeps/include/VideoInput/videoInput.cpp
	)

	SOURCE_GROUP("Source Files\\Camera Input" FILES 
	./NxVideo_Grabber_DirectShow.cpp
	./NxVideo_Grabber_PointGrey.cpp
	./NxVideo_Grabber_VFW.cpp
	)

 
elseif(APPLE)

	#set to compile in 32bit mode only.. because of quicktime.. didnt manage to do it auatomaticly
	#set(CMAKE_OSX_ARCHITECTURES i386 CACHE STRING "do not build for 64-bit" FORCE)
	set(ARCH i386)
	#set(WORD_SIZE 32)

	set(NxVideo_src 
	${NxVideo_src}
	./NxVideo_Mov_Player2.cpp
	./NxVideo_Theora_Player.cpp
	)
	
elseif(ANDROID)	
	message( "NxVideo detected Android !" )
	set(NxVideo_src 
	${NxVideo_src}
	./NxVideo_Theora_Player.cpp
	)

else()
	message( "NxVideo Linux" )
	set(NxVideo_src 
	${NxVideo_src}
	./NxVideo_Theora_Player.cpp
	)

endif()


# --------- Setup the Executable output Directory -------------
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )

# --------- Setup the Executable output Directory -------------
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/../Lib )
 
set(NxVideo_DEF "NXVIDEO_NONCLIENT_BUILD;" )
 
# warning level
IF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)" )  
   # Use the lowest warning level for visual studio.
   SET(CMAKE_CXX_WARNING_LEVEL 0)
   IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     STRING(REGEX REPLACE "/W[0-4]" "/W0"
            CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
   ELSE(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
     SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W0")
   ENDIF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
ENDIF(CMAKE_BUILD_TOOL MATCHES "(msdev|devenv)" )

include_directories(../NxDeps/include)
include_directories(../NxDeps/include/libtheora)


if( MSVC )

	include_directories(../NxDeps/include/DxSdkFeb2010/DirectShow/BaseClasses) 
	include_directories(../NxDeps/include/Quicktime/CIncludes)
	include_directories(../NxDeps/include/GLEW/include)
 
	# from http://www.cmake.org/pipermail/cmake/2007-November/017372.html

	set( NxVideo_LINK_FLAGS "${NxVideo_LINK_FLAGS} /DELAYLOAD:\"PGRFlyCapture.dll\"" )
	set( NxVideo_LINK_FLAGS "${NxVideo_LINK_FLAGS} /NODEFAULTLIB:libcmt.lib" )
	set( NxVideo_LINK_FLAGS "${NxVideo_LINK_FLAGS} /LIBPATH:\"../NxDeps/lib/${CMAKE_BUILD_TYPE}/\"" )



	set( NxVideo_DEF "${NxVideo_DEF}WIN32;_WINDOWS;_USRDLL;")



	SET( NxVideo_EXTRALIBS 
	CVClient.lib
	QTMLClient.lib
	strmbase.lib
	ws2_32.lib
	winmm.lib
	vfw32.lib
	PGRFlyCapture.lib
	delayimp.lib
	debug libtheora_d.lib optimized libtheora.lib
	debug vorbis_d.lib optimized vorbis.lib
	debug ogg_d.lib optimized ogg.lib
	)

	add_library(${NxVideo_LIBRARY} SHARED ${NxVideo_src})

elseif(APPLE)	
	include_directories(../NxDeps/include/libtheora)
	set(NxVideo_EXTRALIBS /System/Library/Frameworks/QuickTime.framework )
	set(OSX_ARCHITECTURES "i386")
	message(STATUS "Set the variable CMAKE_OSX_ARCHITECTURE to i386 because QuickTime is i386 only.")
	add_library(${NxVideo_LIBRARY} STATIC ${NxVideo_src})

else()

	#linux or android
	add_definitions(-D__TMS320C6X__) #for ogg
	#disable multichar warning
	add_definitions(-Wno-multichar)
	add_library(${NxVideo_LIBRARY} STATIC ${NxVideo_src})
 

endif()
 

set_target_properties(${NxVideo_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxVideo_DEF}" LINK_FLAGS_RELEASE "${NxVideo_LINK_FLAGS}" )

set_target_properties(${NxVideo_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxVideo_DEF}" LINK_FLAGS_DEBUG "${NxVideo_LINK_FLAGS}" )

target_link_libraries(${NxVideo_LIBRARY} ${NxVideo_EXTRALIBS})

install(TARGETS ${NxVideo_LIBRARY}
        RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/Bin/${CMAKE_BUILD_TYPE}"
        LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/Lib/${CMAKE_BUILD_TYPE}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/Lib/${CMAKE_BUILD_TYPE}")



