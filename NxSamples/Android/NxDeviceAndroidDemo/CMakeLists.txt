 
cmake_minimum_required(VERSION 2.6)
project(NxDeviceAndroidDemo)

SET(NDKOUT "${CMAKE_SOURCE_DIR}/NxGraphicsSdk/NxSamples/NxDeviceAndroidDemo")
file(MAKE_DIRECTORY "${NDKOUT}")
file(MAKE_DIRECTORY "${NDKOUT}/jni")
file(MAKE_DIRECTORY "${NDKOUT}/assets")
file(MAKE_DIRECTORY "${NDKOUT}/res")
file(MAKE_DIRECTORY "${NDKOUT}/src")
file(MAKE_DIRECTORY "${NDKOUT}/src/nx")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/NxDevice.java" DESTINATION "${NDKOUT}/src/nx")
file(WRITE "${NDKOUT}/default.properties" "target=${ANDROID_TARGET}")
file(WRITE "${NDKOUT}/jni/Application.mk" "APP_ABI := ${ANDROID_ABI}\nAPP_STL := gnustl_static ")
configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/AndroidManifest.xml.in" "${NDKOUT}/AndroidManifest.xml" @ONLY)


function(getNxDevice str )
	set( ${str} "my declarations here" PARENT_SCOPE)
endfunction()




SET(test " ")  

getNxDevice( test )

message( "resuklt"  ${test} )

set( NXDEVICE_INCLUDE ${test} )

configure_file("${CMAKE_SOURCE_DIR}/CMake/Templates/Android.mk.in" "${NDKOUT}/jni/Android.mk" @ONLY)

install(DIRECTORY "${CMAKE_SOURCE_DIR}/NxSamples/Android/media/res/" DESTINATION "${NDKOUT}/res")