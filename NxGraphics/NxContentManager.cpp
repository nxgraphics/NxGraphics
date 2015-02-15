#include "NxGraphicsPreCompiled.h"
#include <Overlay/OgreOverlaySystem.h>

namespace Nx {

	NxContentManager::NxContentManager( NxSceneType Type, const std::string & Name )
	{
		mSceneManager = Root::getSingleton().createSceneManager( (Ogre::SceneType) Type, Name );
	}

	NxContentManager::~NxContentManager()
	{
		Root::getSingleton().destroySceneManager( mSceneManager );
	}

	NxCamera * NxContentManager::CreateCamera( const std::string & name )
	{
		return new NxCamera( this, name );
	}

	void NxContentManager::AddRenderQueueListener( Ogre::OverlaySystem * system ) {
		mSceneManager->addRenderQueueListener(system);
	}

	void NxContentManager::ClearSpecialCaseRenderQueues() {
		mSceneManager->clearSpecialCaseRenderQueues();
	}

	void NxContentManager::AddSpecialCaseRenderQueue( unsigned char queue ) {
		mSceneManager->addSpecialCaseRenderQueue( queue );
	}

	void NxContentManager::SetSpecialCaseRenderQueueMode( NxSpecialCaseRenderQueueMode mode ) {
		mSceneManager->setSpecialCaseRenderQueueMode( ( SceneManager::SpecialCaseRenderQueueMode) mode );
	}
 
	Ogre::SceneManager * NxContentManager::GetContentManager()
	{
		return mSceneManager;
	}


}