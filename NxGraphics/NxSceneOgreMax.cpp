#include "NxGraphicsPreCompiled.h"

#include "../nxdeps/include/OgreMax/OgreMaxScene.hpp"
using namespace OgreMax;

#ifdef USE_RTSHADER_SYSTEM
#include <RTShaderSystem/OgreRTShaderSystem.h>
// Remove the comment below in order to make the RTSS use valid path for writing down the generated shaders.
// If cache path is not set - all shaders are generated to system memory.
//#define _RTSS_WRITE_SHADERS_TO_DISK
#endif

namespace Nx {

	class OgreMaxSceneCallback : public OgreMax::OgreMaxSceneCallback 
	{
	public :
		Nx3DSceneOgreMax * CurrentScene ;

		OgreMaxSceneCallback( Nx3DSceneOgreMax * ScenePtr ): CurrentScene( ScenePtr )
		{

		}

		~OgreMaxSceneCallback() {}
		void StartedLoad(const OgreMaxScene* scene) {}
		void CreatedSceneManager(const OgreMaxScene* scene, Ogre::SceneManager* sceneManager)
		{
			//if( sceneManager->getShadowTechnique() == Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE )
			//{
			//	Log( "Creating Shadows PSSM" );
			//	if(!CurrentScene->ShadowsCapabilities()){ Log( "---- Shader Model Not Supported !!" );  }
			//	CurrentScene->ShadowsInitialize( sceneManager );
			//}

		}
		void CreatedExternal(const OgreMaxScene* scene, const OgreMax::Types::ExternalItem& item)
		{
			//Log("[ CreatedExternal User Data ]" );
			//Log("CreatedExternal User Data name : " + item.name );
			//Log("CreatedExternal User Data Type : " + item.type );
			//Log("CreatedExternal User Data userData : " + item.userData );
			//Log("CreatedExternal User Data userData : " + item.userDataReference );
		}
		void CreatedExternalUserData(const OgreMaxScene* scene, const OgreMax::Types::ExternalUserData& userData) 
		{
			//Log("[ CreatedExternalUserData User Data ]" );
			//Log("CreatedExternalUserData User Data name : " + userData.name );
			//Log("CreatedExternalUserData User Data Type : " + userData.type );
			//Log("CreatedExternalUserData User Data userData : " + userData.userData );
		}
		void LoadedUserData(const OgreMaxScene* scene, const Ogre::String& userDataReference, const Ogre::String& userData)
		{
			//Log("LoadedUserData User Data userDataReference : " + userDataReference );
			//Log("LoadedUserData User Data userData : " + userData );
		}

		void LoadingResourceLocations(const OgreMaxScene* scene) {}
		void LoadedResourceLocations(const OgreMaxScene* scene, const OgreMaxScene::ResourceLocations& resourceLocations) {}
		void LoadingRenderTexture(const OgreMaxScene* scene, OgreMax::Types::RenderTextureParameters& renderTextureParameters) {}
		void LoadingSceneFile(const OgreMaxScene* scene, const Ogre::String& fileName, Ogre::String& resourceGroupName) {}
		void LoadingSkyBox(const OgreMaxScene* scene, OgreMax::Types::SkyBoxParameters& skyBoxParameters) {}
		void LoadingSkyDome(const OgreMaxScene* scene, OgreMax::Types::SkyDomeParameters& skyDomeParameters) {}
		void LoadingSkyPlane(const OgreMaxScene* scene, OgreMax::Types::SkyPlaneParameters& skyPlaneParameters) {}
		void LoadingEntity(const OgreMaxScene* scene, OgreMax::Types::EntityParameters& parameters) {}
		void LoadingPlane(const OgreMaxScene* scene, OgreMax::Types::PlaneParameters& parameters) {}
		void LoadingNodeAnimation( const OgreMaxScene* scene, Types::NodeAnimationParameters& parameters )
		{

		}
		void CreatedLight(const OgreMaxScene* scene, Ogre::Light* light)
		{
			//light->setAttenuation(13, 1.0, 0.35, 0.44);
			// need a shader....
			// http://www.ogre3d.org/forums/viewtopic.php?f=2&t=56268&p=381780&hilit=setAttenuation#p381780
			CreatedMovableObject(scene, light);

			//Log("[ Created Light ]" );
			//Log("Created Light getAttenuationRange : " + Ogre::StringConverter::toString( light->getAttenuationRange() ) );


		}

		void CreatedCamera(const OgreMaxScene* scene, Ogre::Camera* camera)
		{
			CreatedMovableObject(scene, camera);
		}

		void CreatedMesh(const OgreMaxScene* scene, Ogre::Mesh* mesh)
		{

			//build tangents manually in ogremax

			//Log("Building tangent vectors");
			//unsigned short src, dest;
			//if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
			//{
			//	mesh->buildTangentVectors(VES_TANGENT, src, dest);		
			//}

		}



		void CreatedEntity( const OgreMaxScene * scene, Ogre::Entity* entity ) 
		{
			CreatedMovableObject(scene, entity);
		}
		void CreatedParticleSystem(const OgreMaxScene* scene, Ogre::ParticleSystem* particleSystem)
		{
			CreatedMovableObject( scene, particleSystem );
		}
		void CreatedBillboardSet(const OgreMaxScene* scene, Ogre::BillboardSet* billboardSet)
		{
			CreatedMovableObject(scene, billboardSet);
		}
		void CreatedPlane(const OgreMaxScene* scene, const Ogre::Plane& plane, Ogre::Entity* entity)
		{
			CreatedMovableObject(scene, entity);
		}
		void CreatedMovableObject(const OgreMaxScene* scene, Ogre::MovableObject* object) {}
		void CreatedNodeAnimation(const OgreMaxScene* scene, Ogre::SceneNode* Node, Ogre::Animation* animation) {}
		void CreatedNodeAnimationTrack(const OgreMaxScene* scene, Ogre::SceneNode* node, Ogre::AnimationTrack* animationTrack, bool enabled, bool looping) 
		{
		}
		void CreatedNodeAnimationState(const OgreMaxScene* scene, Ogre::SceneNode* node, Ogre::AnimationState* animationState) 
		{
			CurrentScene->NodeAnimationList.push_back( animationState );
			//Log("SceneNode has animation : "+ animationState->getAnimationName() );

		}
		Ogre::Camera* GetRenderTextureCamera(const OgreMaxScene* scene, const OgreMax::Types::RenderTextureParameters& renderTextureParameters) {return 0;}
		void CreatedRenderTexture(const OgreMaxScene* scene, const OgreMax::Types::LoadedRenderTexture* renderTexture) {}
		void StartedCreatingNode(const OgreMaxScene* scene, Ogre::SceneNode* node) {}
		void FinishedCreatingNode(const OgreMaxScene* scene, Ogre::SceneNode* node) {}
		void CreatedTerrain(const OgreMaxScene* scene, const Ogre::String& dataFile) {}
		void HandleObjectExtraData(Types::ObjectExtraDataPtr objectExtraData)
		{
			if( objectExtraData->HasUserData() )
			{
				//Log("---- new HandleObjectExtraData User Data ----" );
				//Log("---- HandleObjectExtraData userData : " + objectExtraData->userData );
				//Log("---- HandleObjectExtraData ID : " + objectExtraData->id );
				//Log("---- HandleObjectExtraData userDataReference : " + objectExtraData->userDataReference );
			}
			else
			{
				//Log("----> HandleObjectExtraData has no User Data" );
			}
		}
		void UpdatedLoadProgress(const OgreMaxScene* scene, Ogre::Real progress) {}
		void FinishedLoad(const OgreMaxScene* scene, bool success) {}
		void CreatingShadowTextures(const OgreMaxScene* scene, Types::ShadowParameters& shadowParameters){}
	};


Nx3DSceneOgreMax::Nx3DSceneOgreMax( const Nx3DSceneDesc & SceneDesc ) : Nx3D_Scene( SceneDesc ), mSceneOgreMax(0), mSceneOgreMaxCallback(0)
{
	mRtt = SceneDesc.mTarget;
	mLayerType = Nx3D_Scene_Ogremax ;
	mSceneFile = SceneDesc.mScenePath ;
}

Nx3DSceneOgreMax::~Nx3DSceneOgreMax()
{
	 
}

std::string Nx3DSceneOgreMax::GetNxSceneFileFullPath()
{
	return mSceneFile.c_str(); 
}

void Nx3DSceneOgreMax::CreateScene()
{

	/*
	static int Index = 0;
	Index++;

	mSceneOgreMax = new OgreMaxScene();

	SetResourceGroupName( "OgreMaxResources_" + Ogre::StringConverter::toString( Index ));

	mSceneOgreMaxCallback = new OgreMaxSceneCallback( this );
	mSceneOgreMax->Load( mSceneFile , 0, OgreMax::OgreMaxScene::NO_OPTIONS, 0, 0, mSceneOgreMaxCallback, GetResourceGroupName() );

	mSceneManager = mSceneOgreMax->GetSceneManager();
 
	if( !mSceneManager ){ Log("NxGraphics: Ogremax scene manager is Null" ); return ; }
 
	Vector3 CamPos;
	Quaternion CamOri;
	// find first camera if any, and take the position and orientation.
	SceneManager::CameraIterator it = mSceneManager->getCameraIterator();
	while(it.hasMoreElements())
	{
		Ogre::Camera * TmpCamera = it.getNext(); 
		CamPos = TmpCamera->getDerivedPosition();
		CamOri = TmpCamera->getDerivedOrientation();
		Log("Found camera "+ TmpCamera->getName()  + " pos : " + Ogre::StringConverter::toString( CamPos.x ) + " " + Ogre::StringConverter::toString(  CamPos.y ) + " " + Ogre::StringConverter::toString(  CamPos.z )    );
		break;
	}

	CreatePhysicsEngine3D();

	SetDebugPhysics3D( false );

	NxScene3DManager::getSingleton().DebugPhysics3D( "127.0.0.1" );

	std::string PathCopy = mSceneFile ;
	NxGraphicsUtilities::SplitFilepath( PathCopy );

	Log( "Looking for : PhysxScene.xml..." );
	Log( "Loading physx path :  " + std::string( PathCopy + "/PhysxScene.xml" )  );

	LoadPhysxScene( std::string( PathCopy + "/PhysxScene.xml" ), nScene, NXU::FT_XML );
	Log("Parsing physx actors...");
	CreateActors();
	Log("Creating physx actors...");
	//CreatePhysxActors();

	ParseUserData();


	NxScene3DObject * CameraSceneNode = CreateNxNode( "CameraEditorNode" );
	NxCharacterController * CameraEditor = CameraSceneNode->CreateNxController( "CameraEditor" );

	// RTT mode.
	if( mRtt )
	{
		mViewport = mRtt->addViewport( CameraEditor->GetCameraFps() );
		mViewport->setAutoUpdated(false);
		mViewport->getTarget()->setAutoUpdated( false );
	}
	else // non rtt mode
	{
		NxEngine::getSingleton().GetNxWindow()->getViewport(0)->setCamera( CameraEditor->GetCameraFps() );
		mViewport = NxEngine::getSingleton().GetNxWindow()->getViewport(0);
	}
	
	mViewport->setBackgroundColour( mSceneOgreMax->GetBackgroundColor() ) ; 
	mViewport->setClearEveryFrame(true); 
	//External_Viewport->setOverlaysEnabled(false);



	SceneCallback = new Nx3D_SceneCallback( this );
    nScene->setUserContactReport( SceneCallback );// Contact Reports
	nScene->setUserTriggerReport( SceneCallback );// Trigger Reports
 */
	//nPhysxEnable = true ;
}

void Nx3DSceneOgreMax::DeleteScene()
{
	/*
	Log("Removing frame Listener");
	//Ogre::Root::getSingleton().removeFrameListener( this ); 
	//Ogre::Root::getSingleton()._fireFrameStarted();
	Ogre::Root::getSingleton().detachRenderTarget(mViewport->getTarget());
	mViewport->getTarget()->setAutoUpdated(false);
	if( mSceneOgreMax )
	{
		Log("Removing SceneOgreMax");
		mSceneOgreMax->Destroy();
		delete mSceneOgreMax ;
		mSceneOgreMax = 0 ;
		Log("Removing SceneOgreMax Done");
	}

	Log("destroyAllCameras");


	//detroy all NxCameras here!
	mSceneManager->destroyAllCameras();

	Log("destroySceneManager");
	if( mSceneManager )
	{
		Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
		mSceneManager = 0;
	}
	Log("destroySceneManager done");
	Ogre::ResourceGroupManager * mngr = Ogre::ResourceGroupManager::getSingletonPtr();

	bool FindResource = false ;
	Ogre::StringVector GroupNames = mngr->getResourceGroups() ;
	for( int i = 0 ; i <  GroupNames.size() ; i++ )
	{
		Ogre::LogManager::getSingleton().logMessage("Found Resource Group : " + GroupNames[i]);
		if( GroupNames[i].compare( GetResourceGroupName() ) == 0 )
		{
			FindResource = true ;
		}  
	}

	Log("isResourceGroupLoaded ... ");
 	if( FindResource )
	{
		if(  mngr->isResourceGroupLoaded( GetResourceGroupName() ) )
		{
			Log("unloadResourceGroup ... ");
			mngr->unloadResourceGroup( GetResourceGroupName() , false );
			mngr->destroyResourceGroup(GetResourceGroupName());
		}
		else
		{
			Log("clearResourceGroup ... ");
			mngr->clearResourceGroup( GetResourceGroupName() );
			mngr->removeResourceLocation( GetResourceGroupName(), ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mngr->destroyResourceGroup(GetResourceGroupName());
		}
	}
	else
	{
		Log("NxGraphics: Ogremax Could not Find resource group : " + GetResourceGroupName() );
	}

	Log("Nx3DSceneOgreMax::DeleteScene() Done");
	*/
}
 

}