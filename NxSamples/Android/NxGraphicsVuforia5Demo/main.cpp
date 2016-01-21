
/*===============================================================================================
 NxGraphics Engine Example
 Copyright (c), Perspective[S] Technologies 2014.

 This example shows how to basically instance the engine
===============================================================================================*/

 

#include <android/sensor.h>

#include <android/log.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <EGL/egl.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>

#include <QCAR/QCAR.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/TrackableResult.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/TrackerManager.h>
//#include <QCAR5/ImageTracker.h>
#include <QCAR/CameraCalibration.h>
#include <QCAR/DataSet.h>
#include <QCAR/VideoBackgroundTextureInfo.h>

#include "VuforiaMath.h"
#include "Shaders.h"

#define  LOG_TAG    "JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

using namespace Nx;

NxGraphics * NxGraph = NULL;
Nx3D_Scene * Scene3DPlayer;
bool isReady = false;
NxEntity * NxBoxLeft = NULL;
 
NxNode * ModelNode = NULL;
NxEntity * NxModel = NULL;
 
NxNode * TheaterNode = NULL;
NxEntity * NxTheaterModel = NULL;
 
NxNode * ScreenNode = NULL;
NxEntity * NxScreenModel = NULL;

float mThresholdDistance = 1.0f; // is set later in function


const int numBoxes = 1;
std::vector<NxEntity * > mBoxes;
std::vector<NxNode * > mBoxesNodes;
std::vector<int> mBoxesRotValues;
std::vector<float> mBoxesDirection;

NxEntity * NxPlaneVideo = NULL;
NxNode * PlaneNode = NULL;
NxNode * StaticNode = NULL;
NxNode * DynamicNode = NULL;

NxRectangle2D * mBackgroundRectangle = NULL;

//background material
MaterialNx * mBackGroundMaterial = 0;
NxTechnique * mBackGroundTechnique = 0;
NxPass * mBackGroundPass = 0;
NxTextureUnit * mBackGroundUnit = 0;
AAssetManager* assetMgr = 0;
EGLContext context;

MaterialNx * MatVideo = NULL;
NxTextureUnit * MatVideoUnit0 =  NULL;
NxTextureUnit * MatVideoUnit1 = NULL; 
NxTextureUnit * MatVideoUnit2 = NULL;


NxPass * MatVideoPass = NULL;
int widthVideo = 512;
int widthBox= 20;


int mOgreScreenWidth = 0;
int mOgreScreenHeight = 0;
int mScreenWidth = 0;
int mScreenHeight = 0;


 
bool calibrateTrackingDone = false;
float mOffsetRotationY = 0.0f;


////////////
#define LOOPER_ID 1
#define SAMP_PER_SEC 100

ASensorEventQueue* sensorEventQueue;

int accCounter = 0;
int64_t lastAccTime = 0;

int gyroCounter = 0;
int64_t lastGyroTime = 0;

int magCounter = 0;
int64_t lastMagTime = 0;

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/HelloJni/HelloJni.java
 */

static int get_sensor_events(int fd, int events, void* data);


static void   getQuaternionFromVector( float * Q , float * rv) {
        float w = (float) sqrtf(1 - rv[0]*rv[0] - rv[1]*rv[1] - rv[2]*rv[2]);
        //In this case, the w component of the quaternion is known to be a positive number
        
        Q[0] = w;
        Q[1] = rv[0];
        Q[2] = rv[1];
        Q[3] = rv[2];
    }


/////////////

static JavaVM* gVM = NULL;


#define ASENSOR_TYPE_ROTATION_VECTOR     11



/*
static int64_t time_stamp=-1;
static int64_t gyro_time_stamp=-1;
static float gyr_x=999,gyr_y=999,gyr_z=999; 
static float EPSILON = 0.000000001;
static float N2S = 1.0/1000000000.0;
static Nx::Quaternion deltaGyroQuaternion(  0,0,0,0);


void multiplyQuat(Nx::Quaternion* q1, Nx::Quaternion* q2){
    float nx = (q1->w)*(q2->x) + (q1->x)*(q2->w) + (q1->y)*(q2->z) - (q1->z)*(q2->y);
    float ny = (q1->w*q2->y - q1->x*q2->z + q1->y*q2->w + q1->z*q2->x);
    float nz = (q1->w*q2->z + q1->x*q2->y - q1->y*q2->x + q1->z*q2->w);
    float nw = (q1->w*q2->w - q1->x*q2->x - q1->y*q2->y - q1->z*q2->z);
    q1->x = nx;
    q1->y = ny;
    q1->z = nz;
    q1->w = nw;
}

*/


 Nx::Quaternion  mGyroscopeOrientation;
 float mGyroscopeOrientationYOsset =  -177;
 

 JNIEXPORT jfloat JNICALL Java_com_hotstuff_main_OgreActivityJNI_GetGyroscopeY(JNIEnv * env, jobject obj ) {
	 
	 return (jfloat) mGyroscopeOrientation.getRoll().valueDegrees();
	 
	 
 }
 
 
 JNIEXPORT jfloat JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetGyroscopeOffset(JNIEnv * env, jobject obj, jfloat value ) {
	 
	
	 mGyroscopeOrientationYOsset = value;
	 
	 
	 calibrateTrackingDone = false;
	 
	  LOGD( "SETTING OFFSET TO %f : " , mGyroscopeOrientationYOsset  ); // -83.0
	 
	 
	 
 }
 
 
 
 


// from http://stackoverflow.com/questions/8989686/access-faster-polling-accelerometer-via-nativeactivity-ndk
static int get_sensor_events(int fd, int events, void* data) {
	
	
	
Nx3DSceneDefault  * ptr = (Nx3DSceneDefault*) data;
  ASensorEvent event;
  while (ASensorEventQueue_getEvents(sensorEventQueue, &event, 1) > 0) {
        if(event.type == ASENSOR_TYPE_ACCELEROMETER) {
               
        }
        else if(event.type == ASENSOR_TYPE_GYROSCOPE) {
     
        }
        else if(event.type == ASENSOR_TYPE_MAGNETIC_FIELD) {
 
               
        }
        else if(event.type == ASENSOR_TYPE_ROTATION_VECTOR) {

            float q[4];
            getQuaternionFromVector(q, event.data );
            Nx::Quaternion  ori( q[0], -q[2],  q[1],  q[3]);
			
			mGyroscopeOrientation = ori;
			
            Nx::Quaternion x = Nx::Quaternion( Nx::Degree( -90 ), Nx::Vector3::UNIT_X); //  -90 landscape mode
            
            
            //Nx::Quaternion modelOri = ModelNode->GetOrientation();  
            
          //  Nx::Quaternion d  =  modelOri * ori.Inverse();
            
            
         //  float val = ori.getRoll().valueDegrees() -  modelOri.getYaw().valueDegrees(); // to do calculate auto dif between
            
            
        //      Nx::Quaternion y180 = Nx::Quaternion( Nx::Degree(180.0f), Nx::Vector3::UNIT_Y);// 180.0f
            
            
        
		 
			/*
			LOGD( "rotation YAW %f : " , ori.getYaw().valueDegrees()  ); //  
			LOGD( "rotation PITCH %f : " , ori.getPitch().valueDegrees()  ); */
			// LOGD( "rotation ROLL %f : " , ori.getRoll().valueDegrees()  ); //is Y
		 
  			if( !calibrateTrackingDone ) { 
			
			
			 
			
				mOffsetRotationY = 	(   (-177 - mGyroscopeOrientationYOsset) ) ;//- (  180.0f + ori.getRoll().valueDegrees()); // if Y  -180 to 180
			
				calibrateTrackingDone = true;
				
				LOGD( "OFFSET rotation ROOL %f : " , mOffsetRotationY  ); // -83.0
			}
			
			 
            
            //
            Nx::Quaternion z = Nx::Quaternion( Nx::Degree(  mOffsetRotationY   )  , Nx::Vector3::UNIT_Z);  
			
			Nx::Quaternion mGyroscopeOrientation =  x * z * ori;
			

            
            
            
            ptr->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetOrientation(  mGyroscopeOrientation );	// x * z * ori

        }

		  

  }
  //should return 1 to continue receiving callbacks, or 0 to unregister
  return 1;
}



JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	gVM = vm;
	return JNI_VERSION_1_4;
}

class EngineListener : public NxEngineListener
{
public :
	EngineListener(){}

	~EngineListener(){}

	void OnWindowMoved( unsigned int PosX, unsigned int PosY ){
		LOGD( "OnWindowMoved %d X %d : " , PosX, PosY );
	}

	void OnWindowResized( unsigned int Width , unsigned int Height ) {
		LOGD( "OnWindowResized %d X %d : " , Width, Height );
	}

	bool OnWindowClosing() {
		LOGD( "OnWindowClosing" );
		return true;
	}
	void OnWindowClosed() {
		LOGD( "OnWindowClosed" );
	}

	void OnWindowFocusChange(){
		LOGD( "OnWindowFocusChange" );
	}
};

class NxInputCallBack : public NxInputListener
{
public :
	 NxInputCallBack(){ }

	~NxInputCallBack(){ }

	void OnKeyPressed( const NxKeyCode &e ) {
		LOGD("OnKeyPressed");

		if(  NXKC_ESCAPE == e ){
			 
		}

	}

	void OnKeyReleased( const NxKeyCode &e ){
		LOGD("OnKeyReleased");
	}

	void OnMouseMoved( const OIS::MouseEvent &e ){
		LOGD("OnKeyMouseMoved");
	}

	void OnMouseButtonPressed( const OIS::MouseEvent &e, NxMouseButtonID id ){
		LOGD("OnKeyMousePressed");
	}

	void OnMouseButtonReleased( const OIS::MouseEvent &e, NxMouseButtonID id ){
		LOGD("OnKeyMouseReleased");
	}

	void OnJoystickButtonPressed( const OIS::JoyStickEvent &arg, int button ){
		LOGD("OnJoystickButtonPressed");
	}

	void OnJoystickButtonReleased( const OIS::JoyStickEvent &arg, int button ){
		LOGD("OnJoystickButtonReleased");
	}

	void OnJoystickAxisMoved( const OIS::JoyStickEvent &arg, int axis ){
		LOGD("OnJoystickAxisMoved");
	}
};



JNIEXPORT jlong JNICALL Java_com_hotstuff_main_OgreActivityJNI_GetEngineContext() {
	 
	return (long)&context;
}
 
JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_CreateEngine(JNIEnv *env, jobject obj, jobject surface, jobject assetManager, jstring splashName ) 
{
 
	LOGD("----> NDK CreateEngine CALLED !!! ");

	if( NxGraph != NULL ){  LOGD("----> NDK CreateEngine already done !!! "); return;}


	assetMgr = AAssetManager_fromJava( env, assetManager );
	ANativeWindow* nativeWnd = ANativeWindow_fromSurface(env, surface);


	mOgreScreenWidth = ANativeWindow_getWidth( nativeWnd );
	mOgreScreenHeight = ANativeWindow_getHeight( nativeWnd );

	//Initialize the engine
	NxGraphicsDesc EngineDesc; 
	EngineDesc.Vsync = true ;
	EngineDesc.Renderer = NxGraphics_GLES2; 
	EngineDesc.mAssetMgr = assetMgr;
	EngineDesc.mNativeWnd = nativeWnd;
 
	float screenRatio = (float)mOgreScreenWidth/(float)mOgreScreenHeight;

/*
	EngineDesc.mSplashSize[0] = 0.25f;
	EngineDesc.mSplashSize[1] = 0.0f;
	EngineDesc.mSplashSize[2] = 0.5f;
	EngineDesc.mSplashSize[3] = 0.9f;*/
    
    EngineDesc.mSplashSize[0] = 0.0f;
	EngineDesc.mSplashSize[1] = 0.0f;
	EngineDesc.mSplashSize[2] = 1.0f;
	EngineDesc.mSplashSize[3] = 1.0f;
	
	
		const char *splashNameJni = env->GetStringUTFChars(  splashName, NULL );
		
		EngineDesc.mSplashTexture = splashNameJni;
 
	env->ReleaseStringUTFChars( splashName, splashNameJni );

	//EngineDesc.mSplashTexture =  "/sdcard/intro.jpg";//"NxLogo.jpg";


	//LOGD("----> mNativeWnd : %d ", (int)EngineDesc.mNativeWnd  );
	//LOGD("----> EngineDesc.GlContext : %d ", (int)EngineDesc.GlContext  );
	


	NxGraph = new NxGraphics();
	NxGraph->CreateNxEngine( EngineDesc );

	NxDeviceManager * DeviceMgr = NxDeviceManager::getSingletonPtr(); 
	NxSoundManager  * SoundMgr  = NxSoundManager::getSingletonPtr();
	NxMocapManager  * MocapMgr  = NxMocapManager::getSingletonPtr();
  
	Nx3DSceneDesc Desc;
    Desc.mType = Nx3D_Scene_Default;
	Desc.mRenderWindow = NxEngine::getSingleton().GetNxWindow();
	Scene3DPlayer = NxScene3DManager::getSingleton().CreateNx3DScene( Desc );
    Scene3DPlayer->SetViewportColour( NxColourValue(0,0,1,1) ); 
    //Scene3DPlayer->SetAmbientLight( NxColourValue(1,1,1,1) );	 
	Desc.mRenderWindow->GetCustomAttribute( "GLCONTEXT", &context  );
	LOGD("----> GLCONTEXT id : %d ", (int)context );
	 

 
	//std::string texname = "NxLogo2.png";
	//NxTextureManager::getSingleton().LoadTexture( texname, "Bootstrap" );
	
	
	ModelNode = Scene3DPlayer->CreateNxNode( "TheaterNode" + NxVideoUtils::ToString( 0 ) );
	
	/*
	NxModel = ModelNode->CreateNxEntity( "room.mesh" );
	NxModel->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxModel->SetScale( Nx::Vector3(1,1,1)); */
	
	NxModel = ModelNode->CreateNxEntity( "room1.mesh" );
	NxModel->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxModel->SetScale( Nx::Vector3(1,1,1)); 

 
	NxEntity * NxChairsRoom1BackLeft = ModelNode->CreateNxEntity( "chairs_room1_back_left.mesh" );
	NxChairsRoom1BackLeft->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom1BackLeft->SetScale( Nx::Vector3(1,1,1)); 	
	
	
	NxEntity * NxChairsRoom1BackRight = ModelNode->CreateNxEntity( "chairs_room1_back_right.mesh" );
	NxChairsRoom1BackRight->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom1BackRight->SetScale( Nx::Vector3(1,1,1)); 		
	
	
	NxEntity * NxChairsRoom1FrontLeft = ModelNode->CreateNxEntity( "chairs_room1_front_left.mesh" );
	NxChairsRoom1FrontLeft->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom1FrontLeft->SetScale( Nx::Vector3(1,1,1)); 		

	NxEntity * NxChairsRoom1FrontRight = ModelNode->CreateNxEntity( "chairs_room1_front_right.mesh" );
	NxChairsRoom1FrontRight->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom1FrontRight->SetScale( Nx::Vector3(1,1,1)); 		
	
	
	
	
	NxEntity * NxRoom2 = ModelNode->CreateNxEntity( "room2.mesh" );
	NxRoom2->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxRoom2->SetScale( Nx::Vector3(1,1,1)); 

	 
	NxEntity * NxChairsRoom2BackLeft = ModelNode->CreateNxEntity( "chairs_room2_back_left.mesh" );
	NxChairsRoom2BackLeft->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom2BackLeft->SetScale( Nx::Vector3(1,1,1)); 	
	
	
	NxEntity * NxChairsRoom2BackRight = ModelNode->CreateNxEntity( "chairs_room2_back_right.mesh" );
	NxChairsRoom2BackRight->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom2BackRight->SetScale( Nx::Vector3(1,1,1)); 	


	NxEntity * NxChairsRoom2FrontLeft = ModelNode->CreateNxEntity( "chairs_room2_front_left.mesh" );
	NxChairsRoom2FrontLeft->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom2FrontLeft->SetScale( Nx::Vector3(1,1,1)); 		
	
	
	NxEntity * NxChairsRoom2FrontRight = ModelNode->CreateNxEntity( "chairs_room2_front_right.mesh" );
	NxChairsRoom2FrontRight->SetPosition( Nx::Vector3( 0, 0, 0)); 	
	NxChairsRoom2FrontRight->SetScale( Nx::Vector3(1,1,1)); 		
	 
	
 
	
	
    
    
	ScreenNode = Scene3DPlayer->CreateNxNode( "ScreenNode" + NxVideoUtils::ToString( 0 ) );
	NxScreenModel = ScreenNode->CreateNxEntity( "screen.mesh" );
	NxScreenModel->SetPosition( Nx::Vector3( 0 , 0 , 0)); 	
	NxScreenModel->SetScale( Nx::Vector3( 1,1,1));  
    
    
 
    
    
    LOGD("----> loading test.jpg on sdcard... "  );
    NxTextureImage * tex =  NxTextureManager::getSingleton().CreateTextureImage( "photo" + NxVideoUtils::ToString( 0 ), "/sdcard/test.jpg" );
	MaterialNx * MatLeft1 =  NxMaterialManager::getSingleton().CreateMaterial("photomat" + NxVideoUtils::ToString( 0) );
	NxTechnique * MatLeftTechnique1 = MatLeft1->CreateTechnique("");
	NxPass * MatLeftPass1 =  MatLeftTechnique1->CreatePass("");
	MatLeftPass1->SetLightingEnabled(false);
	LOGD("----> assigning file to texture unit... "  );
	NxTextureUnit * MatLeftUnit1 = MatLeftPass1->CreateTextureUnit("");
	MatLeftUnit1->SetTextureName(  "photo" + NxVideoUtils::ToString( 0 )  );
    MatLeftUnit1->SetTextureScale( 1.0, -1.0f );
     LOGD("----> loading test.jpg on sdcard: DONE "  );
    
   //NxScreenModel->SetMaterialName( MatLeft1->GetName()  );
    
    
    
    
    
    
    
    
 
        
        
        
        
    
    
    /*
      NxNode * triggerNode = Scene3DPlayer->CreateNxNode( "trigger" );
	  triggerNode->SetPosition( Nx::Vector3( -15 , 124 , 3021)); 
	  NxEntity * tester = triggerNode->CreateNxBox( "trigger", Nx::Vector3(10,10,10 ), Nx::Vector3(1,1,1) );*/
    
    //do camera front texture !
    
   /* 
    NxNode * lightNode = Scene3DPlayer->CreateNxNode( "LightNode" + NxVideoUtils::ToString( 0 ) );
    
    NxScene3DObjectLight * light =  lightNode->CreateNxLight( "light",  PointLight  );
    
    light->SetPosition(  Nx::Vector3(  -15,-1864,467  ) ); */
      //light->SetLightType( PointLight );
    
    
    //Scene3DPlayer->createNxLight();
    
 
	
	//mBoxesNodes.push_back( TheaterNode );
	//	mBoxes.push_back( NxTheaterModel );
 
	 
	 
	 /*
	MaterialNx * MatLeft =  NxMaterialManager::getSingleton().CreateMaterial("BoxesMaterial");
	NxTechnique * MatLeftTechnique = MatLeft->CreateTechnique("");
	NxPass * MatLeftPass =  MatLeftTechnique->CreatePass("");
	MatLeftPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA ); //<<---- cest ca la merde opengles ne supporte pas glAlphaFunc et GL_ALPHA_TEST
	MatLeftPass->SetDepthCheckEnabled(true);
	MatLeftPass->SetDepthWriteEnabled(true);
	MatLeftPass->SetLightingEnabled(false);
	MatLeftPass->SetCullingMode(NXCULL_CLOCKWISE);
	NxTextureUnit * MatLeftUnit = MatLeftPass->CreateTextureUnit("");
	MatLeftUnit->SetTextureAddressingMode(TEXTURE_BORDER);
	MatLeftUnit->SetTextureName("NxLogo.jpg"); */

	//BackGroundColour->getTechnique(0)->getPass(0)->createTextureUnitState( "NxBorderCenter.png" );
	// MatLeftUnit->SetTextureName(texname); 
	// MatLeftUnit->SetTextureName(texname);  
	
	
	// load a model if necessary
	/*	ModelNode = Scene3DPlayer->CreateNxNode( "ModelNode" + NxVideoUtils::ToString( 0 ) );
		NxModel = ModelNode->CreateNxEntity( "dedetable.mesh" );
 
		NxModel->SetPosition( Nx::Vector3( 0 , 0 , -1)); 	
		NxModel->SetScale( Nx::Vector3( 500.0, 500.0 ,500.0)); 
	


	 LOGD("----> CREATED BOXES MATERIAL !!!!!  ");

	 // create a node far away that its invisible... for the alpha blending...
	 //NxScene3DObject * VisibleNode = Scene3DPlayer->CreateNxNode( "Visiblenode" );
	 //VisibleNode->SetPosition( Nx::Vector3( 0 , 0 , -20000)); 
	 //NxScene3DObjectEntity * tester = VisibleNode->CreateNxBox( "tester", Nx::Vector3(widthBox,widthBox,widthBox ), Nx::Vector3(1,1,1) );
	 //tester->SetMaterialName( MatLeft->GetName() );

	 StaticNode = Scene3DPlayer->CreateNxNode( "StaticNode" );
	 StaticNode->SetPosition( Nx::Vector3( 0, 0, 0)); 
 
	 DynamicNode = Scene3DPlayer->CreateNxNode( "DynamicNode" );
	 DynamicNode->SetPosition( Nx::Vector3( 0, 0, 0));

	 DynamicNode->SetAutoTracking( true, StaticNode );
	
 
	for( int i = 0 ; i < numBoxes; i++ ) {
		NxNode * FloorNode = Scene3DPlayer->CreateNxNode( "BaseNode" + NxVideoUtils::ToString( i ) );
		NxBoxLeft = FloorNode->CreateNxBox( "PlaneLeft"+ NxVideoUtils::ToString( i ), Nx::Vector3(widthBox,widthBox,widthBox ), Nx::Vector3(1,1,1) );


		//NxBoxLeft = FloorNode->CreateNxEntity( "dedemymesh.mesh" );
		

		NxBoxLeft->SetPosition( Nx::Vector3( 0 , 0 , -1)); 


		// 	NxBoxLeft->SetScale( Nx::Vector3( 500.0, 500.0 ,500.0)); 


		 NxBoxLeft->SetMaterialName( MatLeft->GetName()  );
		mBoxesNodes.push_back( FloorNode );
		mBoxes.push_back( NxBoxLeft );
	}
 */
	 
	 
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,180, -200 ) );

	//double Distance = 500.0;
	//// Calculate FOV H in degrees from Camera Distance.
	//double heightscreen = 810.0f;//800.0f;
	//double FOVRad = 2.0 *  atan(( heightscreen / 2.0 ) / Distance );
	//double FOVDeg = FOVRad * 180.0 / Nx::Math::PI;

	//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,0, Distance ) );
	//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetAspectRatio(1440.0f/heightscreen);
	//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFieldOfView( Nx::Degree( FOVDeg ).valueDegrees()  );
	  
	mBackGroundMaterial = NxMaterialManager::getSingleton().CreateMaterial("backgroundMaterial2");
	mBackGroundTechnique = mBackGroundMaterial->CreateTechnique("");
	mBackGroundPass =  mBackGroundTechnique->CreatePass("");
	//mBackGroundPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA ); 
	mBackGroundPass->SetDepthCheckEnabled(false);
	mBackGroundPass->SetDepthWriteEnabled(false);
	mBackGroundPass->SetLightingEnabled(false);
	mBackGroundPass->SetCullingMode( NXCULL_CLOCKWISE );//NXCULL_NONE);  
	mBackGroundUnit = mBackGroundPass->CreateTextureUnit("");
	mBackGroundUnit->SetTextureAddressingMode(TEXTURE_CLAMP);


	

	//Scene3DPlayer->CreateViewportBackground( mBackGroundMaterial->GetName() );

	NxNode * BackgroundNode = Scene3DPlayer->CreateNxNode( "BackgroundNode" );
	mBackgroundRectangle = BackgroundNode->CreateNxRectangle2D( "rectangle", true );
	mBackgroundRectangle->SetMaterialName( mBackGroundMaterial->GetName() );

  
	//////////////////////////////////////////////////////////////////////////
	
    ASensorEvent event;
    int events, ident;
    ASensorManager* sensorManager;
    const ASensor* accSensor;
    const ASensor* gyroSensor;
    const ASensor* magSensor;
	const ASensor* rotVector;
    void* sensor_data = malloc(1000);

    //LOGI("sensorValue() - ALooper_forThread()");

    ALooper* looper = ALooper_forThread();

    if( looper == NULL ) {
        looper = ALooper_prepare(ALOOPER_PREPARE_ALLOW_NON_CALLBACKS);
    }

    sensorManager = ASensorManager_getInstance();

    accSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
    gyroSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
    magSensor = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);    
    rotVector = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ROTATION_VECTOR);


    sensorEventQueue = ASensorManager_createEventQueue(sensorManager, looper, 4, get_sensor_events, Scene3DPlayer);

    ASensorEventQueue_enableSensor(sensorEventQueue, accSensor);
    ASensorEventQueue_enableSensor(sensorEventQueue, gyroSensor);
    ASensorEventQueue_enableSensor(sensorEventQueue, magSensor);
    ASensorEventQueue_enableSensor(sensorEventQueue, rotVector);
	
	

    //Sampling rate: 100Hz
    int a = ASensor_getMinDelay(accSensor);
    int b = ASensor_getMinDelay(gyroSensor);
    int c = ASensor_getMinDelay(magSensor);
	int d = ASensor_getMinDelay(rotVector);
    //LOGI("min-delay: %d, %d, %d",a,b,c);
    ASensorEventQueue_setEventRate(sensorEventQueue, accSensor, 100000);
    ASensorEventQueue_setEventRate(sensorEventQueue, gyroSensor, 100000);
    ASensorEventQueue_setEventRate(sensorEventQueue, magSensor, 100000);
    ASensorEventQueue_setEventRate(sensorEventQueue, rotVector, 100000);
	

    //LOGI("sensorValue() - START");

	
	
	
	//////////////////



	EngineListener * liste = new EngineListener();
	NxEngine::getSingleton().GetNxWindow()->AddListener(  liste );

	NxInputCallBack  * InputCallback; 
	NxEngine::getSingleton().GetNxWindow()->AddInputListener( InputCallback = new NxInputCallBack() ); 

	isReady = true;
	 
	return;
}


JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_DeleteEngine(JNIEnv *env, jobject obj) 
{
 
	NxGraph->ReleaseEngine();

	delete NxGraph;
	NxGraph = 0;

	isReady = false;


	return;
}



JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetThreshHoldDistance(JNIEnv * env, jobject obj,  jfloat distance  ) {

	mThresholdDistance = distance;
 
}
 

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetViewportSize(JNIEnv * env, jobject obj,  jfloat Left, jfloat Top, jfloat Width, jfloat Height ) {

	NxEngine::getSingleton().GetNxViewport()->SetDimensions(  Left,  Top,  Width, Height );
 
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetScreenSize(JNIEnv * env, jobject obj,  jint Width, jint Height ) {

	mScreenWidth = Width;
	mScreenHeight = Height;
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetTextureTransform( JNIEnv * env, jobject obj, jfloatArray trans, jfloatArray rotate, jfloatArray scale ) {

	jfloat* fltrotate = env->GetFloatArrayElements( rotate,0);
	jfloat* fltscale  = env->GetFloatArrayElements( scale, 0);
	jfloat* flttrans  = env->GetFloatArrayElements( trans, 0);


	Nx::Matrix4 xform = Nx::Matrix4::IDENTITY;
	Nx::Quaternion qx, qy, qz, qfinal;
	qx.FromAngleAxis(Nx::Degree( fltrotate[0] ), Nx::Vector3::UNIT_X);
	qy.FromAngleAxis(Nx::Degree( fltrotate[1] ), Nx::Vector3::UNIT_Y);
	qz.FromAngleAxis(Nx::Degree( fltrotate[2] ), Nx::Vector3::UNIT_Z);
	qfinal = qx * qy * qz;

	Nx::Vector3 translate;
	translate.x = flttrans[0];
	translate.y = flttrans[1];
	translate.z = flttrans[2];

	Nx::Matrix3 rot3x3, scale3x3;
	qfinal.ToRotationMatrix(rot3x3);
	scale3x3 = Nx::Matrix3::ZERO;
	scale3x3[0][0] = fltscale[0];
	scale3x3[1][1] = fltscale[1];
	scale3x3[2][2] = fltscale[2];

	xform = rot3x3 * scale3x3;
	xform.setTrans(translate);

	mBackGroundUnit->SetTextureTransform( xform );

	env->ReleaseFloatArrayElements(trans,  flttrans, 0);
	env->ReleaseFloatArrayElements(rotate, fltrotate, 0);
	env->ReleaseFloatArrayElements(scale,  fltscale, 0);

}

#define PI 3.1415926f
JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetTextureFOV(JNIEnv * env, jobject obj, jdouble fovyRadians, jdouble fovRadians ) {

		//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFieldOfView(  180 * fovyRadians / PI  );
}
 


JNIEXPORT jobjectArray JNICALL  Java_com_hotstuff_main_OgreActivityJNI_CopyFilesToPath( JNIEnv * env, jobject obj, jstring dstfilepath, jobject assetManager ) {

	jobjectArray ret;

	const char *Stringp = env->GetStringUTFChars(  dstfilepath, NULL );
	int StringLen = env-> GetStringUTFLength( dstfilepath );

	AAssetManager* pAssetManager = AAssetManager_fromJava( env, assetManager );

	std::vector<std::string> mFiles;

	std::string FolderPath = "VideoData"; 
	AAssetDir* assetDir = AAssetManager_openDir( pAssetManager , FolderPath.c_str());
	const char* filename = (const char*)NULL;
	while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL) { 
		std::string LocalFilePath =  FolderPath+"/"+std::string( filename );
		AAsset* asset = AAssetManager_open( pAssetManager, LocalFilePath.c_str(), AASSET_MODE_BUFFER);
		if( asset ){
		off_t length = AAsset_getLength(asset);
		char * membuf = new char[length];
		int nb_read = 0;
		std::string filepathStr = std::string( Stringp ) + "/"+std::string( filename );
		FILE* out = fopen( filepathStr.c_str(), "w");
		mFiles.push_back( filepathStr );
		while ((nb_read = AAsset_read(asset, membuf, length)) > 0)
			fwrite(membuf, nb_read, 1, out);

		fclose(out);
		delete membuf;
		AAsset_close(asset);
		}//if asset
	}
	AAssetDir_close(assetDir);
	env->ReleaseStringUTFChars( dstfilepath, Stringp );
 
	ret= (jobjectArray)env->NewObjectArray(mFiles.size(),env->FindClass("java/lang/String"),0);

	for(int i=0;i<mFiles.size();i++) 
		env->SetObjectArrayElement(ret,i,env->NewStringUTF( mFiles[i].c_str() ));

	mFiles.clear();

    return(ret);

}


  int indexer = 0;
JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_CreateTextureFromPath(JNIEnv * env, jobject obj, jstring filepath  ) { 

	const char *Stringp = env->GetStringUTFChars(  filepath, NULL );
	std::string videofilepath =  std::string( Stringp );
	env->ReleaseStringUTFChars( filepath, Stringp );
    
    
    LOGD( "----- CREATING TEXTURE : "   );
    
    
    float Z = NxUtils::GetRandom(3000, 4500) ;
     float X = NxUtils::GetRandom(-1000, 1000) ;
    
    
    if( indexer == 0 ){
   // Scene3DPlayer->AddResourceLocation( "/sdcard/diego_img/", "General" , true );
    }
    
 
    
      
     NxNode * triggerNode = Scene3DPlayer->CreateNxNode( "trigger" + NxVideoUtils::ToString( indexer ) );
	  triggerNode->SetPosition( Nx::Vector3( X , 124 , Z)); 
	  NxEntity * tester = triggerNode->CreateNxBox( "triggerEntoty" + NxVideoUtils::ToString( indexer), Nx::Vector3(90,160,2 ), Nx::Vector3(1,1,1) );
    
     Nx::Quaternion y180 = Nx::Quaternion( Nx::Degree(90.0f), Nx::Vector3::UNIT_Z);// 180.0f
    
    tester->SetOrientation(  y180  );
    
    
   
    NxTextureImage * tex =  NxTextureManager::getSingleton().CreateTextureImage( "texturePhoto" + NxVideoUtils::ToString( indexer), videofilepath );
    
    
 
	MaterialNx * MatLeft =  NxMaterialManager::getSingleton().CreateMaterial("photoMaterial" + NxVideoUtils::ToString( indexer) );
	NxTechnique * MatLeftTechnique = MatLeft->CreateTechnique("");
	NxPass * MatLeftPass =  MatLeftTechnique->CreatePass("");
	//MatLeftPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA ); //<<---- cest ca la merde opengles ne supporte pas glAlphaFunc et GL_ALPHA_TEST
	 //MatLeftPass->SetDepthCheckEnabled(true);
	//MatLeftPass->SetDepthWriteEnabled(true); 
	MatLeftPass->SetLightingEnabled(false);
	//MatLeftPass->SetCullingMode(NXCULL_CLOCKWISE);
	NxTextureUnit * MatLeftUnit = MatLeftPass->CreateTextureUnit("");
	//MatLeftUnit->SetTextureAddressingMode(TEXTURE_BORDER);
	MatLeftUnit->SetTextureName( "texturePhoto" + NxVideoUtils::ToString( indexer) );
    
   // MatLeftUnit->SetTextureFile( videofilepath );
    
    
     tester->SetMaterialName( MatLeft->GetName() ); 
    
    
    indexer++;
    
    /*
    
    	MatVideo =  NxMaterialManager::getSingleton().CreateMaterial("videomaterial");
		NxTechnique * MatVideoTechnique = MatVideo->CreateTechnique("");
		MatVideoPass =  MatVideoTechnique->CreatePass("");
		//MatVideoPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA );
		MatVideoPass->SetCullingMode(NXCULL_NONE);
		//MatVideoPass->SetDepthCheckEnabled(true);
		//MatVideoPass->SetDepthWriteEnabled(true);
        
        
       
        
        // MatVideoUnit0 = MatVideoPass->CreateTextureUnit(""); */
    
    


}



JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_OpenTheoraVideo(JNIEnv * env, jobject obj, jstring filepath  ) {

	const char *Stringp = env->GetStringUTFChars( filepath, NULL );
	std::string videofilepath =  std::string( Stringp );
	env->ReleaseStringUTFChars( filepath, Stringp );


	if( PlaneNode != NULL ) {

		NxTextureManager::getSingleton().Remove("TheoraTexture"); 

		NxVideoPixelFormatInfo info; 
		info.mDstVideoFormat = NxVideoPixelFormaty420; //theora
		bool useopengl = true;
		NxTextureVideo *  mVideo = NxTextureManager::getSingleton().CreateTextureVideo( "TheoraTexture" , videofilepath, info , useopengl );
		mVideo->Play();

		 NxPlaneVideo->SetMaterialName( MatVideo->GetName() );


		MatVideoUnit0->SetTextureName( mVideo->GetTextureName(0) );
		MatVideoUnit1->SetTextureName( mVideo->GetTextureName(1) );
		MatVideoUnit2->SetTextureName( mVideo->GetTextureName(2) );
		 
		MatVideoPass->SetVertexAutoParameterValue( "worldViewProj", ACT_WORLDVIEWPROJ_MATRIX ); 
		MatVideoPass->SetFragmentParameterValue( "tex0", 0 );
		MatVideoPass->SetFragmentParameterValue( "tex1", 1 );
		MatVideoPass->SetFragmentParameterValue( "tex2", 2 ); 
        
         NxScreenModel->SetMaterialName(  MatVideo->GetName() ); // MatVideo
        
        
        



	}else{

		PlaneNode = Scene3DPlayer->CreateNxNode( "PlaneVideo" );
		NxPlaneVideo = PlaneNode->CreateNxPlane( "PlaneVideoGeometry", Nx::Vector2(widthVideo,widthVideo/2), Nx::Vector2(3,3) );

		

		MatVideo =  NxMaterialManager::getSingleton().CreateMaterial("videomaterial");
		NxTechnique * MatVideoTechnique = MatVideo->CreateTechnique("");
		MatVideoPass =  MatVideoTechnique->CreatePass("");
		//MatVideoPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA );
		MatVideoPass->SetCullingMode(NXCULL_NONE);
		//MatVideoPass->SetDepthCheckEnabled(true);
		//MatVideoPass->SetDepthWriteEnabled(true);
        
        
        /*
        
         MatVideoUnit0 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit0->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit0->SetTextureFiltering( TFO_NONE);
        
        
		MatVideoUnit1 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit1->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit1->SetTextureFiltering( TFO_NONE);

		MatVideoUnit2 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit2->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit2->SetTextureFiltering( TFO_NONE);
        
        
        
        NxPlaneVideo->SetMaterialName( MatVideo->GetName() );
        
        
	NxVertexShader * vert = NxMaterialManager::getSingleton().CreateVertexProgram( "yuvtorgbvert", "glsles" );
		NxPixelShader * frag = NxMaterialManager::getSingleton().CreateFragmentProgram( "yuvtorgbfrag", "glsles" ); 

		vert->SetSource( vShaderStr );
		vert->Load();

		frag->SetSource( fShaderStr );
		frag->Load();

		MatVideoPass->SetVertexProgram(vert->GetName());
		MatVideoPass->SetFragmentProgram(frag->GetName() ); 

		NxVideoPixelFormatInfo info; 
		info.mDstVideoFormat = NxVideoPixelFormaty420; //theora
		bool useopengl = true;
		NxTextureVideo *  mVideo = NxTextureManager::getSingleton().CreateTextureVideo( "TheoraTexture" , videofilepath, info , useopengl );
		mVideo->Play();

		MatVideoUnit0->SetTextureName( mVideo->GetTextureName(0) );
		MatVideoUnit1->SetTextureName( mVideo->GetTextureName(1) );
		MatVideoUnit2->SetTextureName( mVideo->GetTextureName(2) );

		MatVideoPass->SetVertexAutoParameterValue( "worldViewProj", ACT_WORLDVIEWPROJ_MATRIX ); 
		MatVideoPass->SetFragmentParameterValue( "tex0", 0 );      */  


        
		 MatVideoUnit0 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit0->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit0->SetTextureFiltering( TFO_NONE);

		MatVideoUnit1 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit1->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit1->SetTextureFiltering( TFO_NONE);

		MatVideoUnit2 = MatVideoPass->CreateTextureUnit("");
		MatVideoUnit2->SetTextureAddressingMode(TEXTURE_BORDER);
		MatVideoUnit2->SetTextureFiltering( TFO_NONE);

		NxPlaneVideo->SetMaterialName( MatVideo->GetName() );

		//create 2 shaders
		NxVertexShader * vert = NxMaterialManager::getSingleton().CreateVertexProgram( "yuvtorgbvert", "glsles" );
		NxPixelShader * frag = NxMaterialManager::getSingleton().CreateFragmentProgram( "yuvtorgbfrag", "glsles" ); 

		vert->SetSource( vShaderStr );
		vert->Load();

		frag->SetSource( fShaderStrSimple );
		frag->Load();

		MatVideoPass->SetVertexProgram(vert->GetName());
		MatVideoPass->SetFragmentProgram(frag->GetName() ); 

		NxVideoPixelFormatInfo info; 
		info.mDstVideoFormat = NxVideoPixelFormaty420; //theora
		bool useopengl = true;
		NxTextureVideo *  mVideo = NxTextureManager::getSingleton().CreateTextureVideo( "TheoraTexture" , videofilepath, info , useopengl );
		mVideo->Play();

		MatVideoUnit0->SetTextureName( mVideo->GetTextureName(0) );
		MatVideoUnit1->SetTextureName( mVideo->GetTextureName(1) );
		MatVideoUnit2->SetTextureName( mVideo->GetTextureName(2) );

		MatVideoPass->SetVertexAutoParameterValue( "worldViewProj", ACT_WORLDVIEWPROJ_MATRIX ); 
		MatVideoPass->SetFragmentParameterValue( "tex0", 0 );
		MatVideoPass->SetFragmentParameterValue( "tex1", 1 );
		MatVideoPass->SetFragmentParameterValue( "tex2", 2 );
        
        
        // NxScreenModel->SetMaterialName(  MatVideo->GetName() ); // MatVideo
        
        
       


	}
	








	NxPlaneVideo->SetVisible(false);


	 

}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_CloseTheoraVideo(JNIEnv * env, jobject obj  ) {

	NxTextureManager::getSingleton().Remove("TheoraTexture"); 

}



 

JNIEXPORT jint JNICALL Java_com_hotstuff_main_OgreActivityJNI_CreateBackGroundTexture( JNIEnv * env, jobject obj ) {


	QCAR::VideoBackgroundTextureInfo videodata = QCAR::Renderer::getInstance().getVideoBackgroundTextureInfo();  
	int TextureWidth  = videodata.mTextureSize.data[0];
	int TextureHeight = videodata.mTextureSize.data[1];
	int VideoWidth  = videodata.mImageSize.data[0];
	int VideoHeight = videodata.mImageSize.data[1];
	QCAR::PIXEL_FORMAT pixelformat =  videodata.mPixelFormat;

	// Setup background UVs to match texture info:
	float scaleFactorX = (float)VideoWidth/(float)TextureWidth;//  640.0f / 1024.0f;
	float scaleFactorY = (float)VideoHeight/(float)TextureHeight;//480.0f / 512.0f;

	float c = 0.0f;
	float r = 0.0f;
	// 4 vertices.
	int numCols = 2;
	int numRows = 2;

	Nx::Vector2 topleft = Nx::Vector2(((float)c) / ((float)(numCols-1)) * scaleFactorX, ((float)r) / ((float)(numRows-1)) * scaleFactorY);
	Nx::Vector2 bottomLeft =   Nx::Vector2(((float)c) / ((float)(numCols - 1)) * scaleFactorX, ((float)(r + 1)) / ((float)(numRows - 1)) * scaleFactorY);
	Nx::Vector2 topRight =  Nx::Vector2(((float)(c + 1)) / ((float)(numCols - 1)) * scaleFactorX, ((float)r) / ((float)(numRows - 1)) * scaleFactorY);
	Nx::Vector2 bottomRight = Nx::Vector2(((float)(c + 1)) / ((float)(numCols - 1)) * scaleFactorX,((float)(r + 1)) / ((float)(numRows - 1)) * scaleFactorY);
	mBackgroundRectangle->SetUVs( topleft, bottomLeft, topRight, bottomRight );

	// Create the background texture
	LOGD("----> Creating material with parameters width: %d, height: %d, pixelformat: %d ", TextureWidth, TextureHeight, pixelformat   );
	NxPixelFormat format = NXPF_R5G6B5; 
	// NxTexture * tex = NxTextureManager::getSingleton().CreateTexture( "backtexture1", TextureWidth, TextureHeight, format );
	NxTexture * tex = NxTextureManager::getSingleton().CreateTexture( "backtexture1", "NxMaterialVideo", TextureWidth, TextureHeight, 0, NXTEX_TYPE_2D, format, NXTU_DYNAMIC_WRITE_ONLY_DISCARDABLE  );

	//const std::string & TexName, const std::string & textureGroupName, int Width = 256, int Height = 256, int numMips = 0, NxTextureType type = NXTEX_TYPE_2D, NxPixelFormat format = NXPF_R8G8B8, NxTextureUsage usage = NXTU_DEFAULT

//ENABLE HERE TO MAKE WORK BACKGROUND
	//mBackGroundUnit->SetTextureName( tex->GetTextureName() );
	
	//mBackGroundUnit->SetTextureName( "blacker" );

	int textureID = tex->GetTextureID(0);
	if(!QCAR::Renderer::getInstance().setVideoBackgroundTextureID( textureID )) {
		LOGD("---> ERROR : Renderer.getInstance().setVideoBackgroundTextureID : %d", textureID );
	}

	return textureID;


}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_ViewportSetCurrent(JNIEnv * env, jobject obj )
{
	NxEngine::getSingleton().GetNxWindow()->UpdateWindowMetrics();
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_ViewportSetClearEveryFrame(JNIEnv * env, jobject obj, jboolean val )
{
	NxEngine::getSingleton().GetNxViewport()->SetClearEveryFrame( val ); 
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_ViewportClear(JNIEnv * env, jobject obj)
{
	NxEngine::getSingleton().GetNxViewport()->Clear( NXFBT_COLOUR | NXFBT_DEPTH, NxColourValue(0,0,0,1) );
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_ViewportSize(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat width, jfloat height )
{
	NxEngine::getSingleton().GetNxViewport()->SetDimensions( x, y , width, height );
}

JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_ViewportUpdate(JNIEnv * env, jobject obj)
{
	NxEngine::getSingleton().GetNxViewport()->Update();
}


JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_renderOneFrame(JNIEnv * env, jobject obj)
{
	if( isReady )//gRenderWnd != NULL && gRenderWnd->isActive())
	{
		//try
		//{
			if(gVM->AttachCurrentThread(&env, NULL) < 0) 					
				return;


			 /*

			for( int i = 0 ; i < numBoxes; i++ ) {

				Nx::Quaternion quat;
				float dir = mBoxesDirection[i];
				quat.FromAngleAxis(Nx::Degree(mBoxesRotValues[i]),Nx::Vector3(dir,dir,dir));
				mBoxes[i]->Rotate( quat , NxWorld );
			}*/
 
				 
			
			NxGraph->Update();

			//gRenderWnd->windowMovedOrResized();
			//gRoot->renderOneFrame();

			//gVM->DetachCurrentThread();				
		//}catch(Ogre::RenderingAPIException ex) {}
	}
}

Nx::Vector2 GetScreenDimensions() {

	Nx::Vector2 size;
	QCAR::VideoBackgroundConfig config = QCAR::Renderer::getInstance().getVideoBackgroundConfig();
	size.x = config.mSize.data[0];
	size.y = config.mSize.data[1];
}
 
 

 


bool misFirstScene = true;
 
JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetModelPose(JNIEnv * env, jobject obj, jfloatArray mat  ) {
	
	
	// check to see if its the first time we see the marker, in order to calculate the offset rotation
	
	 
	
 

	jfloat* fltmat = env->GetFloatArrayElements( mat ,0);
	
	QCAR::Matrix44F modelViewMatrix;
	for( int i = 0 ; i < 16; i++ ) {
		modelViewMatrix.data[i] =  fltmat[i];
	}	
	Nx::Matrix4 modelViewMatrixOgre;
	VuforiaMatrix2Nx( modelViewMatrix, modelViewMatrixOgre );
    
    /* extract camera position
    QCAR::Matrix44F inverseMV = SampleMath::Matrix44FInverse(modelViewMatrix);
    QCAR::Matrix44F invTranspMV = SampleMath::Matrix44FTranspose(inverseMV);
     // Extract the camera position from the last column of the matrix computed before the following occurs:
    float cam_x = invTranspMV.data[12];
    float cam_y = invTranspMV.data[13];
    float cam_z = invTranspMV.data[14];
    */
    
	
	
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition(Nx::Vector3(0, 300, 0));
	//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->LookAt(Nx::Vector3(0,300,-2500.0f));
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetNearPlane(10.0f);
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFarPlane(5000.0f);//500 ok
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFieldOfView( 90.0f  );//GetFovDegrees() );
	
	Nx::Matrix4 transform = Nx::Matrix4::IDENTITY;
	transform.setTrans( Nx::Vector3(0,0,widthBox/2)  ); // << add offset Z here
	transform.setScale( Nx::Vector3(1,1,1) );

	//here continue


	Nx::Matrix4 res = modelViewMatrixOgre * transform;
    
    
   // faire camera texture front

	
// ===>longeur : 6.5 metres
	
	
	//float height = 180.0f;
	//float height = 180.0f;
	Nx::Vector3 position = Nx::Vector3(res[0][3], -res[1][3]  , -res[2][3]     );

	Nx::Vector3 xr = Nx::Vector3((fltmat[0]), -(fltmat[1]), -(fltmat[2]));
	Nx::Vector3 yr = Nx::Vector3(( fltmat[4]), -(fltmat[5]), -(fltmat[6]));
	Nx::Vector3 zr = Nx::Vector3(( fltmat[8]), -(fltmat[9]), -(fltmat[10]));
	Nx::Vector3 x = xr.normalisedCopy();
	Nx::Vector3 y = yr.normalisedCopy();
	Nx::Vector3 z = zr.normalisedCopy();
	Nx::Quaternion orientation = Nx::Quaternion(x, y, z);
	
	
	if( NxModel ) { 
        
        Nx::Quaternion y180 = Nx::Quaternion( Nx::Degree(0.0f), Nx::Vector3::UNIT_Y);// 180.0f
        
        ScreenNode->SetOrientation( Nx::Quaternion::IDENTITY );
        ScreenNode->SetPosition( Nx::Vector3(0,0,0));
		NxScreenModel->SetOrientation(  y180  ); // right orientation  
        
        
		NxModel->SetOrientation( Nx::Quaternion::IDENTITY );
		NxModel->SetPosition( Nx::Vector3(0,0,0));
		ModelNode->SetOrientation(  y180  ); // right orientation
        
        
        float ZReduc = 700.0f;//500.0f;
 
        position.z = position.z + ZReduc ;
 
        if( misFirstScene && position.z < mThresholdDistance ) {
			
			position.y = 180.0f; // height
			position.x = 0.0f;
            
            Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition(  position  );
 
        }else {  // second room
            
            misFirstScene = false;
            
            Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition(  Nx::Vector3(  -15, 300, 3021 )  );
        
        
            
        }
        
        
        
        
        
        
        
        
        
        
        
       
        
        
        /*
        if( position.x > 180.0f )  position.x = 180.0f;
        if( position.x < -180.0f ) position.x = -180.0f;*/
        
       // ScreenNode->SetPosition( position );
    //    ModelNode->SetPosition( position );	
	
        
        

        
        
	}
	

	
	
	env->ReleaseFloatArrayElements(mat, fltmat, 0);
	
	
//ptr->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition(Nx::Vector3(0,180, 0));	
	
	
return;	 
	
 /*
	jfloat* fltmat = env->GetFloatArrayElements( mat ,0);


 
	// Compute the inverse of the model-view matrix, and transpose it:
	QCAR::Matrix44F modelViewMatrix;
	for( int i = 0 ; i < 16; i++ ) {
		modelViewMatrix.data[i] =  fltmat[i];
	}

	// rotatePoseMatrix(180.0f, 1.0f, 0, 0, &modelViewMatrix.data[0]);

	Nx::Matrix4 modelViewMatrixOgre;
	VuforiaMatrix2Nx( modelViewMatrix, modelViewMatrixOgre );
 
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition(Nx::Vector3(0, 0, 0));
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->LookAt(Nx::Vector3(0,0,-2500.0f));
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetNearPlane(10.0f);
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFarPlane(5000.0f);//500 ok
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFieldOfView( GetFovDegrees() );
 

	NxViewport * viewport =  NxEngine::getSingleton().GetNxViewport();
	double aspect = (double)viewport->GetActualWidth() / (double)viewport->GetActualHeight();
	 

	float ratio = (float)mScreenWidth/(float)mScreenHeight;

	 Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetAspectRatio( ratio );//1920.0f/1440.0f );//aspect ); // <<< --- nexus 1920 x 1080     || 1920 x 1440

	 

	
	Nx::Matrix4 transform = Nx::Matrix4::IDENTITY;
	transform.setTrans( Nx::Vector3(0,0,widthBox/2)  ); // << add offset Z here
	transform.setScale( Nx::Vector3(1,1,1) );

	//here continue


	Nx::Matrix4 res = modelViewMatrixOgre * transform;

	Nx::Vector3 position = Nx::Vector3(res[0][3], -res[1][3], -res[2][3] );

	Nx::Vector3 xr = Nx::Vector3((fltmat[0]), -(fltmat[1]), -(fltmat[2]));
	Nx::Vector3 yr = Nx::Vector3(( fltmat[4]), -(fltmat[5]), -(fltmat[6]));
	Nx::Vector3 zr = Nx::Vector3(( fltmat[8]), -(fltmat[9]), -(fltmat[10]));
	Nx::Vector3 x = xr.normalisedCopy();
	Nx::Vector3 y = yr.normalisedCopy();
	Nx::Vector3 z = zr.normalisedCopy();
	Nx::Quaternion orientation = Nx::Quaternion(x, y, z);
	 
	
	Nx::Quaternion minusz90 = Nx::Quaternion(Nx::Radian(Nx::Degree(-90.0f)), Nx::Vector3::UNIT_Z);
	Nx::Quaternion x90 = Nx::Quaternion(Nx::Radian(Nx::Degree(90.0f)), Nx::Vector3::UNIT_X);
	Nx::Quaternion y90 = Nx::Quaternion(Nx::Radian(Nx::Degree(90.0f)), Nx::Vector3::UNIT_Y);
	Nx::Quaternion y180 = Nx::Quaternion( Nx::Degree(180.0f), Nx::Vector3::UNIT_Y);
 
	if( mBoxes.size() ){

		mBoxes[0]->SetOrientation( Nx::Quaternion::IDENTITY );
		mBoxes[0]->SetPosition( Nx::Vector3(0,0,0));

		mBoxesNodes[0]->SetOrientation( orientation * x90 ); // right orientation
		mBoxesNodes[0]->SetPosition(position);
	}
	
	
	if(  NxModel ) { 
		
		NxModel->SetOrientation( Nx::Quaternion::IDENTITY );
		NxModel->SetPosition( Nx::Vector3(0,0,0));

		ModelNode->SetOrientation( orientation    ); // right orientation
		ModelNode->SetPosition(position);		
		
	
	}
 

 
	  ///////////////////

	//vuforia axis system :
	//y up
	//x right
	//z towards me

	NxPlaneVideo->SetVisible(true);

 
	  Nx::Matrix4 transformPlane = Nx::Matrix4::IDENTITY;
	  transformPlane.setTrans( Nx::Vector3((widthVideo/4),0,(widthVideo/2)/2)  ); // << add offset Z here
	  transformPlane.setScale( Nx::Vector3(1,1,1) );
	  Nx::Matrix4 planeMatrix = modelViewMatrixOgre * transformPlane;
	  Nx::Vector3 poser = Nx::Vector3(planeMatrix[0][3], -planeMatrix[1][3], -planeMatrix[2][3] );
	  if( NxPlaneVideo ) NxPlaneVideo->SetPosition( poser ); 

	  
	   
	  //orientation
	  Nx::Quaternion quat2;
	  quat2.FromAngleAxis( Nx::Degree( -90 ), Nx::Vector3(0,1,0)   );

	  Nx::Quaternion quat3;
	  quat3.FromAngleAxis( Nx::Degree( 180.0f ), Nx::Vector3(1,0,0)   );

	  Nx::Quaternion offset = orientation  * quat2 ;//* quat3;

	  if( NxPlaneVideo )
	  NxPlaneVideo->SetOrientation( offset ); 

	env->ReleaseFloatArrayElements(mat, fltmat, 0);
	
	*/
	return;

 
}


 
 
 
 
