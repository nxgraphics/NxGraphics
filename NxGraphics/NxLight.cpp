#include "NxGraphicsPreCompiled.h"
#include "NxConversions.h"

 
/// Main light billboard set
Ogre::BillboardSet *mBillboardSet = 0;
/// Camera used for rendering the shadow map and as light frustum
Ogre::Camera       *mLightCamera = 0;
/// Light camera scene node
Ogre::SceneNode    *mLightCameraSN = 0;

// Knot scene node
Ogre::SceneNode    *mKnotSN = 0;



// Current cookie selected
int mCurrentCookie = 0;

static int mCounter = 0;


class LightShaftsListener : public Ogre::FrameListener
{
public:
    SceneManager *mSceneMgr;
    Ogre::Real mKeyBuffer;
	bool mRotateEnable;
	bool mRotateKnot;
	
    LightShaftsListener( SceneManager *sm) : mSceneMgr(sm), mKeyBuffer(-1), mRotateEnable(true), mRotateKnot(false)
    {
    }

    bool frameStarted(const FrameEvent &e)
    {
  
		updatePosition(e);
        return true;
    }

	void updatePosition(const FrameEvent &e)
	{
		// Just a simple circular trajectory
		const Ogre::Real& SimulationTime = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		Ogre::Real Radius = 8;

		if (!mRotateEnable)
		{
			Radius = 0;
		}

		//mLightCameraSN->setPosition(Ogre::Math::Sin(SimulationTime/1000)*Radius, mLightCameraSN->getPosition().y, Ogre::Math::Cos(SimulationTime/1000)*Radius);


		// Set the the scene node direction to 0,KnotHeight,0 point
		///float height = 4;
		//Ogre::Vector3 NormalisedDirection = (Ogre::Vector3(0,height,0)-mLightCameraSN->getPosition()).normalisedCopy();
		//mLightCameraSN->setDirection(NormalisedDirection, Ogre::Node::TS_WORLD);

		// Upload current position to light shafts materials
		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
		getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uLightPosition", mLightCameraSN->getPosition());

		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShaftsDepth"))->
		getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uLightPosition", mLightCameraSN->getPosition());
 
		// We've to update the texture projection matrix
        updateTextureProjectionMatrix();


	}

	void updateTextureProjectionMatrix()
	{
		const Ogre::Matrix4 PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE(
               0.5,    0,    0,  0.5,
               0,   -0.5,    0,  0.5,
               0,      0,    1,    0,
               0,      0,    0,    1);
		
		Ogre::Matrix4 TexViewProj = PROJECTIONCLIPSPACE2DTOIMAGESPACE_PERSPECTIVE * mLightCamera->getProjectionMatrixWithRSDepth() * mLightCamera->getViewMatrix(); 
		
		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
			getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant( "uTexViewProj", TexViewProj );
	}
};


	// Just for change all entities materials in depth rtt
	class LightDepthMapRttListener : public Ogre::RenderTargetListener
	{
	public:
		LightDepthMapRttListener(Ogre::SceneManager *sm) : mSceneMgr(sm)
		{
		}

		void preRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
		{
			Ogre::SceneManager::MovableObjectIterator EntityIterator = mSceneMgr->getMovableObjectIterator("Entity");
			Ogre::Entity * CurrentEntity = NULL;
			unsigned int k = 0;

			mMaterials.empty();

			mBillboardSet->setVisible(false);

			while( EntityIterator.hasMoreElements() )
			{
				CurrentEntity = static_cast<Ogre::Entity *>(EntityIterator.peekNextValue());
				for( k = 0; k < CurrentEntity->getNumSubEntities(); k++ )
				{
					mMaterials.push(CurrentEntity->getSubEntity(k)->getMaterialName());
					CurrentEntity->getSubEntity(k)->setMaterialName("LightShaftsDepth");
				}

				EntityIterator.moveNext();
			}
		}

		void postRenderTargetUpdate(const Ogre::RenderTargetEvent& evt)
		{
			Ogre::SceneManager::MovableObjectIterator EntityIterator = mSceneMgr->getMovableObjectIterator("Entity");
			Ogre::Entity * CurrentEntity = NULL;
			unsigned int k = 0;

			mBillboardSet->setVisible(true);

			while( EntityIterator.hasMoreElements() )
			{
				CurrentEntity = static_cast<Ogre::Entity *>(EntityIterator.peekNextValue());
				for( k = 0; k < CurrentEntity->getNumSubEntities(); k++ )
				{
					CurrentEntity->getSubEntity(k)->setMaterialName(mMaterials.front());
					mMaterials.pop();
				}

				EntityIterator.moveNext();
			}
		}

		std::queue<std::string> mMaterials;	
		Ogre::SceneManager *mSceneMgr;
	};


	bool createLightShafts(Ogre::BillboardSet* Billboard, Ogre::Camera *LightCamera, const int &NumberOfPlanes)
    {
	    // Calculate the distance between planes
	    float DistanceBetweenPlanes = (LightCamera->getFarClipDistance() - LightCamera->getNearClipDistance())/NumberOfPlanes;

		// Get frustum corners to calculate near/far planes dimensions
		const Ogre::Vector3 *FrustumCorners = LightCamera->getWorldSpaceCorners();

		// Calcule near and far planes dimensions
		float NearWidth  = (FrustumCorners[0] - FrustumCorners[1]).length(),
		      NearHeigth = (FrustumCorners[1] - FrustumCorners[2]).length(),
			  FarWidth   = (FrustumCorners[4] - FrustumCorners[5]).length(),
			  FarHeigth  = (FrustumCorners[5] - FrustumCorners[6]).length();

		// Now width/heigth setp
		float WidthStep  = (FarWidth-NearWidth)/NumberOfPlanes,
			  HeigthStep = (FarHeigth-NearHeigth)/NumberOfPlanes;
		
		// Add billboards
		Ogre::Billboard *CurrentBB = 0;
		for(int k = 0; k < NumberOfPlanes; k++)
		{
			CurrentBB = Billboard->createBillboard(Ogre::Vector3(0, 0,-LightCamera->getNearClipDistance() -k*DistanceBetweenPlanes), ColourValue::White);
			CurrentBB->setDimensions(NearWidth + k*WidthStep, NearHeigth + k*HeigthStep);
		}

	    return true;
    }

	void createLightCameraRTT( SceneManager * mSceneMgr )
	{

		int TextureSize = 256;
		// Creat a texture for use as rtt
		Ogre::TexturePtr LightCameraRTT = Ogre::TextureManager::getSingleton()
			.createManual("LightDepthMap",
			              "General",
                          Ogre::TEX_TYPE_2D,
						  // 256*256 must be sufficient
                          TextureSize, 
						  TextureSize, 
						  0,
                          Ogre::PF_R8G8B8,
                          Ogre::TU_RENDERTARGET);

        Ogre::RenderTarget* RT_Texture = LightCameraRTT->getBuffer()->getRenderTarget();
            
        Ogre::Viewport *RT_Texture_Viewport = RT_Texture->addViewport(mLightCamera);
        RT_Texture_Viewport->setClearEveryFrame(true);
		RT_Texture_Viewport->setBackgroundColour(Ogre::ColourValue::White);
        RT_Texture_Viewport->setOverlaysEnabled(false);
        RT_Texture_Viewport->setSkiesEnabled(false);
		
		// Add our depth listener
		RT_Texture->addListener(new LightDepthMapRttListener(mSceneMgr));

		// Fill the texture in our material
		static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
		getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName("LightDepthMap");
	}

 

namespace Nx {


//point light attenuation
//http://www.ogre3d.org/tikiwiki/-Point+Light+Attenuation

NxScene3DObjectLight::NxScene3DObjectLight( NxNode * ParentNode, const std::string & LightName, NxLightType Type ) : NxEntity() 
{
	mType = NxTypeLight;

	mLight = ParentNode->GetNxScene().GetNxSceneManager()->createLight( LightName );

	Ogre::Light::LightTypes OgreType;
	switch( Type )
	{
		case PointLight:
			OgreType = Ogre::Light::LT_POINT;
		break;
		case SpotLight:
			OgreType = Ogre::Light::LT_SPOTLIGHT;
		break;
		case DirectionalLight:
			OgreType = Ogre::Light::LT_DIRECTIONAL;
		break;
		default :
			LogMsg("NxLight Type not Implemented !");
		break;
	}

	mLight->setType( OgreType ); 
    mLight->setDiffuseColour(0.7, 0.7, 0.7);
    mLight->setSpecularColour( 1.0, 1.0, 1.0 );
	mLight->setCastShadows( true );
	mLight->setDebugDisplayEnabled(false); 

	/*
	Ogre::Vector3 Direction;
	Direction.x = 0.4;
	Direction.y = -0.6;
	Direction.z = 0.4f;//-1.0f;
	Direction.normalise();	

	//light2->setRenderingDistance(60);

    mLight0->setPosition( 0,20,0 );
	mLight0->setDirection(Direction);
	*/

	mLight->setShadowNearClipDistance(0.1);
	mLight->setShadowFarClipDistance(1024);

	if(mLight->getType() == Light::LT_SPOTLIGHT )
	{
		mLight->setSpotlightFalloff(1.0f);
		mLight->setSpotlightInnerAngle( Ogre::Degree(35.0f)  );
		mLight->setSpotlightOuterAngle( Ogre::Degree(50.0f)  );	
	}
	// where is the node ????




/*
	SceneManager * mSceneMgr = ParentNode->GetNxScene().GetNxSceneManager(); 
	mLight0 = mSceneMgr->createLight( LightName  );
	mLight0->setType( ( Ogre::Light::LightTypes ) Type ); 
    mLight0->setDiffuseColour( 1.0, 1.0, 1.0 );
    mLight0->setSpecularColour( 1.0, 1.0, 1.0 );
	mLight0->setCastShadows(true);
	mLight0->setDebugDisplayEnabled(false); 
	//mLight0->setDirection( 0,-1,0);


	Ogre::Vector3 Direction;
	Direction.x = 0.4;
	Direction.y = -0.6;
	Direction.z = 0.4f;//-1.0f;
	Direction.normalise();	

	//light2->setRenderingDistance(60);

    mLight0->setPosition( 0,0,0 );
	mLight0->setDirection(Direction);

	mLight0->setShadowNearClipDistance(0.1);
	mLight0->setShadowFarClipDistance(1024);




	mNode = mSceneMgr->createSceneNode( "Light" + Ogre::StringConverter::toString( instance_count() ) );
	//SetParent( ParentNode );
	//mNode->attachObject( mLight0  );

	
*/



 

	




	/*

	Nx3D_Scene * Scene = NxScene3DManager::getSingleton().GetNx3DSceneFromOgreSceneManager( mSceneMgr );

	// Set up our light camera
	mLightCamera = mSceneMgr->createCamera("LightCamera" + Ogre::StringConverter::toString( mCounter ) );
	mLightCamera->setProjectionType(Ogre::PT_PERSPECTIVE);
	// Not forget to set near+far distance in materials
	mLightCamera->setNearClipDistance(1);//8
	mLightCamera->setFarClipDistance(10);//40
	mLightCamera->setAspectRatio(1);
	mLightCamera->setFOVy( Ogre::Degree(45.0f) ); // Ogre::Degree(17.5f));
	mLightCamera->setDebugDisplayEnabled(false);
	mLightCamera->setVisible(true);

	mLightCameraSN = mSceneMgr->getRootSceneNode()->createChildSceneNode();
	mLightCameraSN->attachObject(mLightCamera);

	mLightCameraSN->setPosition( LightSceneNode->getPosition() );
	Quaternion quat;
	quat.FromAngleAxis( (Radian)Ogre::Math::DegreesToRadians( -90 ) , Ogre::Vector3( 1,0,0) ); // look down
	mLightCameraSN->setOrientation( quat );// setDirection(0, -1, 0);

	// Create our billboardset for volumetric rendering
	mBillboardSet = mSceneMgr->createBillboardSet("LightBillboardSet"+ Ogre::StringConverter::toString( mCounter ) , 1);
    mBillboardSet->setMaterialName("LightShafts");
	mBillboardSet->setBillboardRotationType(Ogre::BBR_VERTEX);
	mBillboardSet->setCastShadows(false);
	mLightCameraSN->attachObject(mBillboardSet);

	

	// Set up light 0
	mLight0 = mSceneMgr->createLight("LightS"+ Ogre::StringConverter::toString( mCounter ) );
	mLight0->setType( Light::LT_SPOTLIGHT ); 
    mLight0->setDiffuseColour(0.0, 1.0, 0.0);
    mLight0->setSpecularColour( 0.1, 0.1, 0.1 );
	mLight0->setCastShadows(true);
	mLight0->setDebugDisplayEnabled(false); 

	Ogre::Vector3 Direction;
	Direction.x = 0.0;
	Direction.y = 0.0;
	Direction.z = -1.0f;
	Direction.normalise();	

	//light2->setRenderingDistance(60);

    mLight0->setPosition( 0,0,0 );
	mLight0->setDirection(Direction);

	if(mLight0->getType() == Light::LT_SPOTLIGHT )
	{
		mLight0->setSpotlightFalloff(1.0f);
		mLight0->setSpotlightInnerAngle( Ogre::Degree(35.0f)  );
		mLight0->setSpotlightOuterAngle( Ogre::Degree(50.0f)  );	
	}



	mLight0->setShadowNearClipDistance(0.1);
	mLight0->setShadowFarClipDistance(200);

 
	mLightCameraSN->attachObject( mLight0 );
	mLightCameraSN->setPosition( LightSceneNode->getPosition() );

	// Create a rush of billboards according to the frustum of the camera(mLightCamera)
	// After it, we can use the lightcamera/billboards scenenode like a light projector

	int NumPlanes = 50;//100
	createLightShafts(mBillboardSet, mLightCamera, NumPlanes);

	// Creating a RRT for depth/shadow map
	createLightCameraRTT(  mSceneMgr  );

	mCurrentCookie = 0;

	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getTextureUnitState(1)->setTextureName("Cookie" + Ogre::StringConverter::toString(mCurrentCookie) + ".png");

	// Light camera/frustum (near+far) clip distance
	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uLightFarClipDistance", mLightCamera->getNearClipDistance( ) + mLightCamera->getFarClipDistance()   );

	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uAttenuation",  0.02f   );

	mCounter++;

	// Add frame listener
	Ogre::Root::getSingleton().addFrameListener(new LightShaftsListener( mSceneMgr));
	*/
}

NxScene3DObjectLight::~NxScene3DObjectLight()
{

}

void NxScene3DObjectLight::SetLightType( const NxLightType & Type )
{
	mLight->setType( ( Ogre::Light::LightTypes ) Type ); 

	//here

/*
Range Constant Linear Quadratic
3250, 1.0, 0.0014, 0.000007
600, 1.0, 0.007, 0.0002
325, 1.0, 0.014, 0.0007
200, 1.0, 0.022, 0.0019
160, 1.0, 0.027, 0.0028
100, 1.0, 0.045, 0.0075
65, 1.0, 0.07, 0.017
50, 1.0, 0.09, 0.032
32, 1.0, 0.14, 0.07
20, 1.0, 0.22, 0.20
13, 1.0, 0.35, 0.44
7, 1.0, 0.7, 1.8
*/


	//For example, if you wish to have a light that gradually dims until it has no effect at distance 100: 
	float MaxDistance = 7;
	if( Type == PointLight )
	{
		 mLight->setAttenuation(MaxDistance, 1.0, 0.7, 1.8);
	}
}

const NxLightType & NxScene3DObjectLight::GetLightType() const
{
	return ( NxLightType ) mLight->getType(); 
}

void NxScene3DObjectLight::Update( float time )
{
	//NxScene3DObjectEntity::Update(time);
	/*
	mLightCameraSN->setPosition( mSceneNode->getPosition() );
	Quaternion quat;
	quat.FromAngleAxis( (Radian)Ogre::Math::DegreesToRadians( -90 ) , Ogre::Vector3( 1,0,0) ); // look down
	mLightCameraSN->setOrientation( mSceneNode->getOrientation() * quat );
	*/
}

void NxScene3DObjectLight::SetDebugEnabled( bool Debug )
{
	//mLightCamera->setDebugDisplayEnabled( Debug );
	mLight->setDebugDisplayEnabled( Debug ); 
}

void NxScene3DObjectLight::SetPosition( const Nx::Vector3 & Position )
{
	Ogre::Vector3 pos;
	NxVec3ToOgre( pos, Position );
	mLight->setPosition( pos );
}

void NxScene3DObjectLight::SetDirection( const Nx::Vector3 & Direction )
{
	Ogre::Vector3 dir;
	NxVec3ToOgre( dir, Direction );
	mLight->setDirection( dir );
}

void NxScene3DObjectLight::SetTexture( const std::string & TextureName )
{
	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getTextureUnitState(1)->
	setTextureName( Ogre::String( TextureName ) );
}

void NxScene3DObjectLight::SetDiffuseColour( const NxColourValue & Colour )
{
	Ogre::ColourValue col;
	NxColourtoOgre( col, Colour );
	mLight->setDiffuseColour( col );
}
void NxScene3DObjectLight::SetSpecularColour( const NxColourValue & Colour )
{
	Ogre::ColourValue col;
	NxColourtoOgre( col, Colour );
	mLight->setSpecularColour( col ); // Highlights
}

void NxScene3DObjectLight::SetIntensity( float Intensity )
{
	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uAttenuation",  Intensity  );
	mLight->setPowerScale(Intensity);//no effet -> use in shader 
}

void NxScene3DObjectLight::SetConeAngle( float Angle )
{
	mLightCamera->setFOVy( Ogre::Degree( Angle ) );

	if( mLight->getType(  ) == Light::LT_SPOTLIGHT ) {
		mLight->setSpotlightInnerAngle( Ogre::Degree( Angle - 10.0f)  );
		mLight->setSpotlightOuterAngle( Ogre::Degree( Angle + 5.0f)  );
	}
}

void NxScene3DObjectLight::SetFrustum( float NearPlane, float FarPlane )
{
	mLightCamera->setNearClipDistance(NearPlane); 
	mLightCamera->setFarClipDistance(FarPlane); 
	// Light camera/frustum (near+far) clip distance
	static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().getByName("LightShafts"))->
	getTechnique(0)->getPass(0)->getFragmentProgramParameters()->setNamedConstant( "uLightFarClipDistance", NearPlane + FarPlane );
}

 
}