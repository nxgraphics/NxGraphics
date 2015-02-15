#include "NxGraphicsPreCompiled.h"

extern "C" 
{
  #include "../nxdeps/include/Lua/Include/lua.h"
  #include "../nxdeps/include/Lua/Include/lauxlib.h"
  #include "../nxdeps/include/Lua/Include/lualib.h"
}

//LuaBind scripting
/*
#include "../Dependencies/LUABIND_07/luabind/luabind.hpp"
#include "../Dependencies/LUABIND_07/luabind/class.hpp"
#include "../Dependencies/LUABIND_07/luabind/function.hpp"
#include "../Dependencies/LUABIND_07/luabind/object.hpp"
#include "../Dependencies/LUABIND_07/luabind/out_value_policy.hpp"
#include "../Dependencies/LUABIND_07/luabind/return_reference_to_policy.hpp"
#include "../Dependencies/LUABIND_07/luabind/operator.hpp"
using namespace luabind;
*/

//#include "..\nxdevice\NxDevice_Headers.h"

// Prototype this before operator.hpp so it can be found for tostring() operator.
std::ostream& operator<<( std::ostream& stream, const Ogre::Entity ent );


using namespace Ogre;

namespace Nx {

#define LUA_CONST_START( class ) { object g = globals(L); object table = g[#class];
#define LUA_CONST( class, name ) table[#name] = class::name
#define LUA_CONST_END }

NxGraphics& getApplication()
{
	return NxGraphics::getSingleton();
 
}

void  NxGraphics_Export bindEngine( lua_State* L )  
{
	//module(L)
	//[
	//	class_<NxGraphics>("NxGraphics") 
	//	.def("GetRenderer", &NxGraphics::GetEngineManager )
	//	.def("GetDevices", &NxGraphics::GetDeviceManager )
	//	.def("OpenScript", (void( NxGraphics:: * )( Ogre::String ))&NxGraphics::OpenScript )
	//];
}

void  NxGraphics_Export bindMixer( lua_State* L ) // Video mixer
{
	/*module(L)
	[
		class_<MixerManager>("MixerManager") 
		//.def("CreateNxLayerVideo", ( NxLayerVideo *( MixerManager::*)(int , const std::string &  ))&MixerManager::CreateNxLayerVideo )
		//.def("CreateNxLayer3D", (void( MixerManager::*)( int, Ogre::String, int, int, int ))&MixerManager::CreateNxLayer3D )
		//.def("SetLayerOpacity", (void( MixerManager::*)( float, int ))&MixerManager::SetLayerOpacity )
	];*/
 
}

void  NxGraphics_Export bindDevice( lua_State* L ) // renderer
{
	//module(L)
	//[
	//	class_<NxDeviceManager>("NxDeviceManager")  
	//];
}

void  NxGraphics_Export bindRenderer( lua_State* L ) // renderer
{
	//module(L)
	//[
	//	class_<NxEngine>("NxEngine") 	
	//];
}

void  NxGraphics_Export bindApplication( lua_State* L )//application
{
	//module(L)
	//[
	//	def( "getApplication",    getApplication )
	//];
}

void bindLua( lua_State* L ) // lua cpp class definitions
{
	/*
	Log( "bindLua renderer" );
	bindRenderer( L );
	Log( "bindLua device" );
	bindDevice( L );
	Log( "bindLua mixer" );
    bindMixer( L );
	Log( "bindLua engine" );
	bindEngine( L );
	Log( "bindLua application" );
	bindApplication( L );
	*/
}
 

} // NxGraphics_Namespace 