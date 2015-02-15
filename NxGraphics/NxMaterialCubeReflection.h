#include "NxGraphicsPreCompiled.h"
/*
#include <OgreRenderTargetListener.h>
#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgreTechnique.h>
#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreTexture.h>
#include <OgreHardwarePixelBuffer.h>
using namespace Ogre;*/

namespace Nx {

class NxCubeMapListener : public RenderTargetListener
{
public:
	Ogre::Entity * mEntity;
	Ogre::Camera * mCubeCamera;
	RenderTarget* mTargets[6];
	Ogre::SceneManager * mSceneMgr;


	NxCubeMapListener( const std::string name, Entity * Ent ) : mEntity( Ent )
	{
		mSceneMgr = Ent->getParentSceneNode()->getCreator();
		createCubeMap();

		Ogre::MaterialPtr mat = Ogre::MaterialManager::getSingleton().create( name ,Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME );
		Ogre::TextureUnitState * State=  mat->getTechnique(0)->getPass(0)->createTextureUnitState();
		State->setCubicTextureName("dyncubemap", true);
		State->setTextureAddressingMode( Ogre::TextureUnitState::TAM_CLAMP   );
		State->setEnvironmentMap(true, Ogre::TextureUnitState::ENV_REFLECTION );
		Ent->setMaterialName(name);
	}

	void testCapabilities( const RenderSystemCapabilities* caps )
	{
        if (!caps->hasCapability(RSC_CUBEMAPPING))
        {
			OGRE_EXCEPT(Exception::ERR_NOT_IMPLEMENTED, "Your graphics card does not support cube mapping, "
				"so you cannot run this sample. Sorry!", "CubeMappingSample::testCapabilities");
        }
	}
 
    void preRenderTargetUpdate(const RenderTargetEvent& evt)
    {
		mCubeCamera->setPosition( mEntity->getParentSceneNode()->_getDerivedPosition() );

		mEntity->setVisible(false);  // hide the entity
		// point the camera in the right direction based on which face of the cubemap this is
		mCubeCamera->setOrientation(Ogre::Quaternion::IDENTITY);
		if (evt.source == mTargets[0]) mCubeCamera->yaw(Ogre::Degree(-90));
		else if (evt.source == mTargets[1]) mCubeCamera->yaw(Ogre::Degree(90));
		else if (evt.source == mTargets[2]) mCubeCamera->pitch(Ogre::Degree(90));
		else if (evt.source == mTargets[3]) mCubeCamera->pitch(Ogre::Degree(-90));
		else if (evt.source == mTargets[5]) mCubeCamera->yaw(Ogre::Degree(180));
    }

    void postRenderTargetUpdate(const RenderTargetEvent& evt)
    {
        mEntity->setVisible(true);  // unhide the entity
    }

	void createCubeMap()
	{
		// create the camera used to render to our cubemap
		mCubeCamera = mSceneMgr->createCamera("CubeMapCamera");
		mCubeCamera->setFOVy(Ogre::Degree(90));
		mCubeCamera->setAspectRatio(1);
		mCubeCamera->setFixedYawAxis(false);
		mCubeCamera->setNearClipDistance(5);

		// create our dynamic cube map texture
		TexturePtr tex = TextureManager::getSingleton().createManual("dyncubemap",
			ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_CUBE_MAP, 128, 128, 0, Ogre::PF_R8G8B8, TU_RENDERTARGET);

		// assign our camera to all 6 render targets of the texture (1 for each direction)
		for (unsigned int i = 0; i < 6; i++)
		{
			mTargets[i] = tex->getBuffer(i)->getRenderTarget();
			mTargets[i]->addViewport(mCubeCamera)->setOverlaysEnabled(false);
			mTargets[i]->addListener(this);
		}
	}

};

class NxMaterialCubeMap
{
public :
	NxMaterialCubeMap( const std::string & name, Entity * Ent )
	{
		new NxCubeMapListener( name, Ent );
	}

};




}