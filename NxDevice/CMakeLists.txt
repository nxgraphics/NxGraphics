# NXDEVICE CMake build file
cmake_minimum_required(VERSION 2.6)

project(NxDevice)

set(NxDevice_LIBRARY NxDevice)

set(NxDevice_src
./NxDevice.cpp		
./NxDevice_Hardware.cpp		
./NxDevice_Log.cpp
./NxDevice_Utilities.cpp		
./NxDevice_Midi_Manager.cpp		
./NxDevice_Plateform.cpp		
./NxDevicePrecompiled.cpp		
./NxDevice.h	
./NxDevice_Hardware.h
./NxDevice_Headers.h
./NxDevice_Log.h
./NxDevice_Midi.h
./NxDevice_Plateform.h	
./NxDevice_Prerequisites.h
./NxDevice_Singleton.h
./NxDevice_Stable_Headers.h
./NxDevicePreCompiled.h
../nxdeps/include/PortMidi/pm_common/pmutil.c
../nxdeps/include/PortMidi/pm_common/portmidi.c
../nxdeps/include/PortMidi/porttime/porttime.c
../nxdeps/include/OscPack/Ip/IpEndpointName.cpp
../nxdeps/include/OscPack/osc/OscOutboundPacketStream.cpp
../nxdeps/include/OscPack/osc/OscPrintReceivedElements.cpp
../nxdeps/include/OscPack/osc/OscReceivedElements.cpp
../nxdeps/include/OscPack/osc/OscTypes.cpp
../nxdeps/include/TinyXml/tinystr.cpp
../nxdeps/include/TinyXml/tinyxml.cpp
../nxdeps/include/TinyXml/tinyxmlerror.cpp
../nxdeps/include/TinyXml/tinyxmlparser.cpp
)



 

SOURCE_GROUP("LIBRAIRIES\\MIDI\\Sources" FILES 
../nxdeps/include/PortMidi/pm_common/pmutil.c
../nxdeps/include/PortMidi/pm_common/portmidi.c
../nxdeps/include/PortMidi/porttime/porttime.c
)

SOURCE_GROUP("LIBRAIRIES\\OSC\\Sources" FILES 
../nxdeps/include/OscPack/Ip/IpEndpointName.cpp
../nxdeps/include/OscPack/osc/OscOutboundPacketStream.cpp
../nxdeps/include/OscPack/osc/OscPrintReceivedElements.cpp
../nxdeps/include/OscPack/osc/OscReceivedElements.cpp
../nxdeps/include/OscPack/osc/OscTypes.cpp
)

SOURCE_GROUP("LIBRAIRIES\\XML\\Sources" FILES 
../nxdeps/include/TinyXml/tinystr.cpp
../nxdeps/include/TinyXml/tinyxml.cpp
../nxdeps/include/TinyXml/tinyxmlerror.cpp
../nxdeps/include/TinyXml/tinyxmlparser.cpp
)



SOURCE_GROUP("Header Files" FILES 
./NxDevice.h	
./NxDevice_Hardware.h
./NxDevice_Headers.h
./NxDevice_Log.h
./NxDevice_Midi.h
./NxDevice_Plateform.h	
./NxDevice_Prerequisites.h
./NxDevice_Singleton.h
./NxDevice_Stable_Headers.h
./NxDevicePreCompiled.h
)


if( MSVC )

set(NxDevice_src
${NxDevice_src}
./NxDevice_Device_Dmx.cpp	
./NxDevice_Device_Midi.cpp	
./NxDevice_Device_Osc.cpp
./NxDevice_Device_OscOutputMsg.cpp
../nxdeps/include/SIMPLEDMX/timeddmx.h
../nxdeps/include/SIMPLEDMX/timedDmx_Interval.h
../nxdeps/include/SIMPLEDMX/TIMEDDMX.cpp
../nxdeps/include/PortMidi/pm_win/pmwin.c
../nxdeps/include/PortMidi/pm_win/pmwinmm.c
../nxdeps/include/PortMidi/porttime/ptwinmm.c
../nxdeps/include/OscPack/Ip/Win32/NetworkingUtils.cpp
../nxdeps/include/OscPack/Ip/Win32/UdpSocket.cpp

)

SOURCE_GROUP("Source Files\\Devices" FILES 
./NxDevice_Device_Dmx.cpp	
./NxDevice_Device_Midi.cpp	
./NxDevice_Device_Osc.cpp
./NxDevice_Device_OscOutputMsg.cpp
)

SOURCE_GROUP("LIBRAIRIES\\DMX\\Sources" FILES 
../nxdeps/include/SIMPLEDMX/TIMEDDMX.cpp
)

SOURCE_GROUP("LIBRAIRIES\\DMX\\Headers" FILES 
../nxdeps/include/SIMPLEDMX/timeddmx.h
../nxdeps/include/SIMPLEDMX/timedDmx_Interval.h
)

SOURCE_GROUP("LIBRAIRIES\\MIDI\\Sources" FILES 
../nxdeps/include/PortMidi/pm_win/pmwin.c
../nxdeps/include/PortMidi/pm_win/pmwinmm.c
../nxdeps/include/PortMidi/porttime/ptwinmm.c
)

SOURCE_GROUP("LIBRAIRIES\\OSC\\Sources" FILES 
../nxdeps/include/OscPack/Ip/Win32/NetworkingUtils.cpp
../nxdeps/include/OscPack/Ip/Win32/UdpSocket.cpp
)




 
elseif(APPLE)

#set(NxDevice_src ${NxDevice_src} ./NxDevice_XXXXXXX.cpp )

else()

set(NxDevice_src
${NxDevice_src}
./NxDevice_Device_Midi.cpp	
./NxDevice_Device_Osc.cpp
./NxDevice_Device_OscOutputMsg.cpp
../nxdeps/include/PortMidi/pm_linux/finddefault.c
../nxdeps/include/PortMidi/pm_linux/pmlinux.c
../nxdeps/include/PortMidi/porttime/ptlinux.c
../nxdeps/include/OscPack/Ip/posix/NetworkingUtils.cpp
../nxdeps/include/OscPack/Ip/posix/UdpSocket.cpp
)



endif()



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

 

include_directories(../nxdeps/include) 
include_directories(../nxdeps/include/OscPack)
include_directories(../nxdeps/include/portmidi/pm_common)
include_directories(../nxdeps/include/portmidi/porttime)
include_directories(../nxdeps/include/FTDI)

set(NxDevice_DEF "NXDEVICE_NONCLIENT_BUILD;" )

set(NxDevice_LINK_FLAGS "" )

if( MSVC )

SET( NxDevice_DEF "${NxDevice_DEF}FTD2XX_EXPORTS;NXDEVICE_NONCLIENT_BUILD;_WINDLL" )
SET( NxDevice_Deps "../NxDeps/lib/release/" )

SET( NxDevice_EXTRALIBS 
ws2_32
winmm
../NxDeps/lib/release/ftd2xx
)


elseif(APPLE)

 set(NxDevice_DEF "${NxDevice_DEF}__MACOSX_CORE__;")
	
        SET(NxDevice_EXTRALIBS /System/Library/Frameworks/CoreAudio.framework )
        SET(NxDevice_EXTRALIBS /System/Library/Frameworks/AudioUnit.framework ${NxDevice_EXTRALIBS})
	SET(NxDevice_EXTRALIBS /System/Library/Frameworks/CoreMidi.framework ${NxDevice_EXTRALIBS})
        SET(NxDevice_EXTRALIBS /System/Library/Frameworks/ApplicationServices.framework ${NxDevice_EXTRALIBS})

        # Some versions of OSX may need the AudioToolbox framework. Add it if it's found.
        FIND_LIBRARY(AUDIOTOOLBOX_LIBRARY NAMES AudioToolbox PATHS ~/Library/Frameworks
                           /Library/Frameworks
                           /System/Library/Frameworks
                    )
IF(AUDIOTOOLBOX_LIBRARY)
SET(NxDevice_EXTRALIBS ${AUDIOTOOLBOX_LIBRARY} ${NxDevice_EXTRALIBS})
ENDIF()
 
else()

ADD_DEFINITIONS(-DOSC_HOST_LITTLE_ENDIAN)


 
endif()

 
if( MSVC )
add_library(${NxDevice_LIBRARY} SHARED ${NxDevice_src})
else()
add_library(${NxDevice_LIBRARY} STATIC ${NxDevice_src})
endif()


set_target_properties(${NxDevice_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxDevice_DEF}" LINK_FLAGS_RELEASE "${NxDevice_LINK_FLAGS}")

set_target_properties(${NxDevice_LIBRARY} PROPERTIES COMPILE_DEFINITIONS "${NxDevice_DEF}" LINK_FLAGS_DEBUG "${NxDevice_LINK_FLAGS}")

target_link_libraries(${NxDevice_LIBRARY} ${NxDevice_EXTRALIBS})



