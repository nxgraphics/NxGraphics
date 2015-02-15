#include "NxGraphicsPreCompiled.h"

extern "C" 
{
  #include "../nxdeps/include/Lua/Include/lua.h"
  #include "../nxdeps/include/Lua/Include/lauxlib.h"
  #include "../nxdeps/include/Lua/Include/lualib.h"
}

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

namespace Nx {

lua_State * L;

typedef struct lua_State lua_State;
void bindLua( lua_State* L );

template<> NxScriptManager* NxSingleton<NxScriptManager>::ms_Singleton = 0;

NxScriptManager::NxScriptManager()
{
	/*
	L = NULL;
	L = lua_open();
	if( L == NULL ){ Log( "NxGraphics Scripting system Failed !" ); }
	luaL_openlibs(L);
	luabind::open(L);
	bindLua( L );
	*/
}

NxScriptManager::~NxScriptManager()
{

}

NxScriptManager& NxScriptManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

bool NxScriptManager::LoadFile( const std::string & FileName )
{
	int res = luaL_loadfile(L, FileName.c_str() ) || lua_pcall( L, 0 ,0 ,0 );
	return res;
}

//lua_State * NxScriptManager::GetLuaState()
//{
//	return L ;
//}
// end LUA


//can handle javascript files ?????

//using namespace v8; // google v8 javascript
class NxGraphics_Export NxGraphicsScriptingEngineV8 : public NxSingleton<NxGraphicsScriptingEngineV8>
{
public :
	NxGraphicsScriptingEngineV8();
	~NxGraphicsScriptingEngineV8();
	static NxGraphicsScriptingEngineV8& getSingleton(void);
	void CreateScriptingEngineV8();
	//v8::Persistent<v8::Context> mV8Context;
};

template<> NxGraphicsScriptingEngineV8* NxSingleton<NxGraphicsScriptingEngineV8>::ms_Singleton = 0;

NxGraphicsScriptingEngineV8::NxGraphicsScriptingEngineV8()
{

}

NxGraphicsScriptingEngineV8::~NxGraphicsScriptingEngineV8()
{

}

NxGraphicsScriptingEngineV8& NxGraphicsScriptingEngineV8::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

/*
v8::Handle<v8::Value> logCallback(const v8::Arguments &args)
{
	v8::HandleScope scope;
    V8TutorialBase *objPtr = externalToClassPtr<V8TutorialBase>(args.Data());
    if(args.Length() != 1)
    {
        ostringstream strstream;
        strstream<<"Got unexpected number of arguments. Expecting 1, got "<<args.Length()<<endl;
        objPtr->log(strstream.str());
    }
    else
    {
        String::Utf8Value val(Local<String>::Cast(args[0]));
        objPtr->log(*val);    
    }

    return v8::Null();
}
*/

void NxGraphicsScriptingEngineV8::CreateScriptingEngineV8()
{

/*
	// Create a stack-allocated handle scope. 
	HandleScope handle_scope;
	// Create a new context. 
	Handle<Context> context = Context::New();
	// Enter the created context for compiling and 
	// running the hello world script.
	Context::Scope context_scope(context);
	// Create a string containing the JavaScript source code. 
	Handle<v8::String> source = v8::String::New("'Hello' + ', World!'");
	// Compile the source code. 
	Handle<Script> script = Script::Compile(source);
	// Run the script to get the result. 
	Handle<Value> result = script->Run();
	// Convert the result to an ASCII string and print it. 
	v8::String::AsciiValue ascii(result);
	Log("Resultat : " + Ogre::String( *ascii ) );
 */

}

} //NxGraphics_Namespace 

