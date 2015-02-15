#include "NxGraphicsPreCompiled.h"
#include "NxSceneShadows.h"

#ifdef __ANDROID__
#include "OgreRTShaderSystem.h"

#include "NxMeshManager.h"
#endif 

namespace Nx {

	/*
	Ogre::MaterialPtr createRTSSMaterial(const std::string& matName, bool enableLight)
	{
//#ifdef OGREKIT_USE_RTSHADER_SYSTEM
		Ogre::RTShader::ShaderGenerator* shaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		//GK_ASSERT(shaderGenerator);

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(matName,
			Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);

		if (material.isNull())
		{
			material = Ogre::MaterialManager::getSingleton().create(matName,
				Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}

		material->setLightingEnabled(enableLight);

		shaderGenerator->createShaderBasedTechnique(matName,
			Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);      
		shaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME, matName);
		if (material->getNumTechniques() > 1)
		{
			material->getTechnique(0)->getPass(0)->setVertexProgram(
				material->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
			material->getTechnique(0)->getPass(0)->setFragmentProgram(
				material->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
		}

		//GK_ASSERT(!material->getTechnique(0)->getPass(0)->getVertexProgram().isNull());
		//GK_ASSERT(!material->getTechnique(0)->getPass(0)->getFragmentProgram().isNull());

		return material;
//#else
//		return Ogre::MaterialPtr();
//#endif
	}
	*/


Nx3DSceneDefault::Nx3DSceneDefault( const Nx3DSceneDesc & SceneDesc ) : Nx3D_Scene( SceneDesc )
{
	mRtt = SceneDesc.mTarget;
	mLayerType = SceneDesc.mType;
	mWindow = SceneDesc.mRenderWindow; 
}

Nx3DSceneDefault::~Nx3DSceneDefault()
{
 
}

void Nx3DSceneDefault::CreateScene()
{
	static int SceneIndex = 0;
	//mSceneManager = Ogre::Root::getSingleton().createSceneManager( ST_GENERIC, "SceneManager"+Ogre::StringConverter::toString(SceneIndex) );

	//bool EnableShadows = false;

	mSceneManager = Ogre::Root::getSingleton().createSceneManager("OctreeSceneManager", "SceneManager"+Ogre::StringConverter::toString(SceneIndex) );

	mSceneManager = NxEngine::getSingleton().GetContentManager()->GetContentManager();


	LogMsg( "Created Scene Type : " + mSceneManager->getTypeName() );
	mSceneManager->setAmbientLight( ColourValue( 0.5, 0.5, 0.5) );
 

	// dont use... create material copies.. for maxime marion install no texture opacity animation( or take material copy )

	//if(EnableShadows)
	//{
	//	ShadowsCapabilities();
	//	ShadowsInitialize( mSceneManager );
	//}

	LogMsg("creating physx engine...");
	mScenePhysics = new NxScenePhysics( this ); 
	mScenePhysics->CreatePhysicsEngine3D();
	
	LogMsg("creating physx engine. DONE..");

	mScenePhysics->SetDebugPhysics3D( false );

	//NxScene3DManager::getSingleton().DebugPhysics3D( "127.0.0.1" );
 
	NxNode * CameraSceneNode = CreateNxNode( "CameraEditorNode" );
	NxCharacterController * CameraEditor = CameraSceneNode->CreateNxController( "CameraEditor" );

	if( mRtt )
	{
		LogMsg("Creating 3D Scene With RTT mode");
		mViewport = mRtt->addViewport( CameraEditor->GetCameraFps());//, 5 ); // ! attension multiople scenes
		//mViewport->setAutoUpdated(false);
		//mViewport->getTarget()->setAutoUpdated( false );
	}
	else // non rtt mode
	{
		// provided mWindow
		LogMsg("Creating 3D Scene With NxRenderWindow");
		LogMsg( "Num Viewports : " + Ogre::StringConverter::toString( mWindow->GetWindow()->getNumViewports()) );
 
		//mViewport = mWindow-> GetWindow()->addViewport( CameraEditor->GetCameraFps(), mWindow->GetWindow()->getNumViewports() );
		
		//mWindow->GetWindow()->getViewport(0)->setCamera( CameraEditor->GetCameraFps() );
		mViewport = mWindow->GetWindow()->getViewport(0);
		mViewport->setCamera( CameraEditor->GetCameraFps() );
		//mViewport = mWindow-> GetWindow()->addViewport( CameraEditor->GetCameraFps(), 1 );


		
		// how to access camera and create setcustom matrix... from jni


		//mViewport = NxEngine::getSingleton().GetNxViewport()->GetViewport();//  Window()->getViewport(0);//CameraEditor->GetCameraFps()->getViewport();// - NxEngine::getSingleton().GetNxWindow()->getViewport(0);
		//mViewport->setCamera(  CameraEditor->GetCameraFps()  );
		//CameraEditor->GetCameraFps()->setPosition( Ogre::Vector3(0,0,2000));
	}






 
	LogMsg("creating Shadows  ");
	//  DeferredShadingSystem * mSystem = new DeferredShadingSystem( mViewport, mSceneManager, CameraEditor->GetCameraFps() );
	//  mSystem->initialize();

	//if( EnableShadows ) ShadowsPrepare();


	LogMsg("Creating Gizmo...");
	mSceneTools = new NxSceneTools( this, CameraEditor->GetCameraFps()  );
	
	LogMsg("Creating Gizmo : Done");
 /////////////////


	//RTShader::ShaderGenerator::getSingleton().addSceneManager(  mSceneManager   );
	 /*
	Ogre::MeshPtr NxMesh = NxMeshFactory::getSingleton().CreateBox( "dede", Nx::Vector3(15,15,15), Nx::Vector3(5,5,5) );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)) {
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	Ogre::SceneNode* mNode = mSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
	Ogre::Entity * mEntity = mSceneManager->createEntity( "yeah", NxMesh );
	mNode->attachObject( mEntity );

	 

	mNode->setPosition( Ogre::Vector3(0,0,-100));

	Ogre::Light* light = mSceneManager->createLight( "MainLight" );
	light->setPosition(0, 40, 2000);

	CameraEditor->GetCameraFps()->setPosition( Ogre::Vector3(0,0,0));
	 */

	/*
	Ogre::MeshPtr NxMesh = NxMeshFactory::getSingleton().CreateBox( "dede", Nx::Vector3(50,50,15), Nx::Vector3(20,20,20) );

	unsigned short src, dest;
	if (!NxMesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest)) {
		NxMesh->buildTangentVectors(VES_TANGENT, src, dest);		
	}

	Ogre::SceneNode* mNode = mSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
	Ogre::Entity * mEntity = mSceneManager->createEntity( "yeah", NxMesh );
	mNode->attachObject( mEntity );
	mNode->setPosition( Ogre::Vector3(0,0,  -400));

	//CameraEditor->GetCameraFps()->setPosition( Ogre::Vector3(0,0,  200));
	GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,0, 300 ) );

	Ogre::MaterialPtr testmaterial = createRTSSMaterial("tester", true );
	mEntity->setMaterial( testmaterial   );

	Ogre::SceneNode* mNodeLight = mSceneManager->getRootSceneNode()->createChildSceneNode("lightNode");
	Ogre::Light* light = mSceneManager->createLight( "MainLight" );
	light->setPosition(0, 40, 0);
	mNodeLight->attachObject( light );
	*/
  
///////////////////

	//mViewport->setMaterialScheme(  RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	//RTShader::ShaderGenerator::getSingleton().invalidateScheme( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME );
 

	NxEngine::getSingleton().AddFrameListener(this);

	

	SceneIndex++;
	 

/////



	//RTShader::ShaderGenerator::getSingleton().invalidateScheme( RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME );


	//mViewport->setMaterialScheme(RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
 
	//GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,0, 0 ) );
	 
	 
	// Create shader based technique for scene manager A.
	//RTShader::ShaderGenerator::getSingleton().createShaderBasedTechnique("MyMaterial", MaterialManager::DEFAULT_SCHEME_NAME, mSceneManager->getName() + RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	//RTShader::ShaderGenerator::getSingleton().addSceneManager(mSceneManager   );
	//mViewport->setMaterialScheme(mSceneManager->getName() + RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	//Ogre::RTShader::ShaderGenerator::getSingletonPtr()->addSceneManager(mSceneManager );

//#ifdef __ANDROID__
//	// Grab the shader generator pointer.
//	Ogre::RTShader::ShaderGenerator * mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
//	// Set the scene manager.
//	mShaderGenerator->addSceneManager(mSceneManager);
//
//	mViewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
//
//	mShaderGenerator->invalidateScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
//#endif

}

void Nx3DSceneDefault::DeleteScene()
{
	//Ogre::Root::getSingleton().removeFrameListener( this ); 
	Ogre::Root::getSingleton()._fireFrameStarted();

	//mViewport->getTarget()->setAutoUpdated(false);
	mSceneManager->destroyAllCameras();
 
	Ogre::Root::getSingletonPtr()->destroySceneManager(mSceneManager);
	Ogre::ResourceGroupManager * mngr = Ogre::ResourceGroupManager::getSingletonPtr();

	bool FindResource = false ;
	Ogre::StringVector GroupNames = mngr->getResourceGroups() ;
	for( int i = 0 ; i <  GroupNames.size() ; i++ ) {
		Ogre::LogManager::getSingleton().logMessage("Found Resource Group : " + GroupNames[i]);
		if( GroupNames[i].compare( GetResourceGroupName() ) == 0 ) {
			FindResource = true ;
		}  
	}

 	if( FindResource ) {
		if(  mngr->isResourceGroupLoaded( GetResourceGroupName() ) ) {
			mngr->unloadResourceGroup( GetResourceGroupName() , false );
			mngr->destroyResourceGroup(GetResourceGroupName());
			Ogre::LogManager::getSingleton().logMessage("NxGraphics: Ogremax unloaded and destroyed resource group: " + GetResourceGroupName() );
		}
		else {
			mngr->clearResourceGroup( GetResourceGroupName() );
			mngr->removeResourceLocation( GetResourceGroupName() ,  ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
			mngr->destroyResourceGroup(GetResourceGroupName());
			Ogre::LogManager::getSingleton().logMessage("NxGraphics: Ogremax destroyed resource group : " + GetResourceGroupName() );
		}
	}
	else {
		Ogre::LogManager::getSingleton().logMessage("NxGraphics: Ogremax Could not Find resource group : " + GetResourceGroupName() );
	}
}

}