#include "NxGraphicsPreCompiled.h"
#include "NxMeshManager.h"

namespace Nx {

template<> NxScene3DManager* NxSingleton<NxScene3DManager>::ms_Singleton = 0;

NxScene3DManager::NxScene3DManager() 
{
	new NxMeshManager();
	new NxScene3DPhysicsManager();
}

NxScene3DManager::~NxScene3DManager() {

}

bool NxScene3DManager::DeleteNx3DScene( Nx3D_Scene * Scene )
{
	switch( Scene->GetSceneType() )
	{
		case Nx3D_Scene_Ogremax :
		{
			 ((Nx3DSceneOgreMax * ) Scene)->DeleteScene();
			  Scene3DList.remove( Scene );
			  return true ;
		}
		break ;
		case Nx3D_Scene_Default :
		{
			 ((Nx3DSceneDefault* ) Scene)->DeleteScene();
			  Scene3DList.remove( Scene );
			  return true ;
		}
		break ;
		default :
			LogMsg("could nor remove nx3dscene, type doesnt exist");
		break;
	}
	return false;
}

Nx3D_Scene * NxScene3DManager::CreateNx3DScene( const Nx3DSceneDesc & SceneDesc )
{
	Nx3D_Scene * mNx3DScene ;

	switch( SceneDesc.mType ) {
	case Nx3D_Scene_Default : 
		{
			mNx3DScene = new Nx3DSceneDefault( SceneDesc );
			Scene3DList.push_back(  mNx3DScene  );
			mNx3DScene->CreateScene();	
		}
		break ;
	case Nx3D_Scene_Ogremax : 
		{	
			mNx3DScene = new Nx3DSceneOgreMax( SceneDesc );
			Scene3DList.push_back(  mNx3DScene   );
			mNx3DScene->CreateScene();	
		} 
		break ;
	default :
		LogMsg("NxGraphics 3D manager : Could not find any 3D scene for this type !");		
		break;
	}

	return mNx3DScene ;
}

Nx3D_Scene * NxScene3DManager::GetNx3DScene( int index )
{
	if( ( index + 1 ) > Scene3DList.size() ){ return 0; }
	std::list< Nx3D_Scene * >::iterator i = Scene3DList.begin();
	std::advance(i, index );
	// now index points to the nth member of the list (counting from zero)
	return  *i ;
}

size_t NxScene3DManager::GetNumNxScenes()
{
	return Scene3DList.size();
}

NxScene3DManager& NxScene3DManager::getSingleton(void)
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}




/*
NxReal UpdateTime()
{
    NxReal deltaTime;
	static __int64 gTime, gLastTime;
    __int64 freq;
    QueryPerformanceCounter((LARGE_INTEGER *)&gTime);  // Get current count
    QueryPerformanceFrequency((LARGE_INTEGER *)&freq); // Get processor freq
    deltaTime = (double)(gTime - gLastTime)/(double)freq;
    gLastTime = gTime;
    return deltaTime;
}
*/
Nx3D_Scene * NxScene3DManager::GetNx3DSceneFromOgreSceneManager( Ogre::SceneManager * SceneMgr )
{
	Nx3D_Scene * Tmp = 0;
	for( std::list< Nx3D_Scene *  >::iterator iter =  Scene3DList.begin(); iter !=  Scene3DList.end(); iter++) {
		Nx3D_Scene * CurrentScene = *iter; 
		if( SceneMgr == CurrentScene->GetNxSceneManager() ) {
			Tmp = CurrentScene;
			break;
		}
	}
	if( !Tmp ) LogMsg("GetNx3DSceneFromOgreSceneManager : no scene to return ");
	return Tmp;
}




/*
Ogre::Real RoundValue( Ogre::Real value, Ogre::uint8 ndigits = 0)
{
    Ogre::Real f = 1.0;

    while (ndigits--)
        f = f * 10.0;

    value *= f;

    if (value >= 0.0)
        value = std::floor(value + 0.5);
    else
        value = std::ceil(value - 0.5);

    value /= f;

    return value;
}
*/




}//namespace
	 