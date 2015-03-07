
if( MSVC )
 
elseif(APPLE)
 
elseif(ANDROID)	
	message( "preparing Android Sdk" )
add_subdirectory(Android)
	 
else()

endif()