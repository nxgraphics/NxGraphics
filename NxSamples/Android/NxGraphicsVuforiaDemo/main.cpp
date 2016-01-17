
/*===============================================================================================
 NxGraphics Engine Example
 Copyright (c), Perspective[S] Technologies 2014.

 This example shows how to basically instance the engine
===============================================================================================*/

 
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

/*
#include <QCAR/QCAR.h>
#include <QCAR/CameraDevice.h>
#include <QCAR/Renderer.h>
#include <QCAR/VideoBackgroundConfig.h>
#include <QCAR/Trackable.h>
#include <QCAR/TrackableResult.h>
#include <QCAR/Tool.h>
#include <QCAR/Tracker.h>
#include <QCAR/TrackerManager.h>
#include <QCAR/ImageTracker.h>
#include <QCAR/CameraCalibration.h>
#include <QCAR/DataSet.h>
#include <QCAR/VideoBackgroundTextureInfo.h> */


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

const int numBoxes = 0;
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


static JavaVM* gVM = NULL;

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
 
JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_CreateEngine(JNIEnv *env, jobject obj, jobject surface, jobject assetManager ) 
{



	//if(gVM->AttachCurrentThread(&env, NULL) < 0) {	 LOGD("----> NDK ENGINE EXITED  !!! "); return; }

	/*
	void jni_playMusic(int idx) {
	JNIEnv *pEnv;
	(*cachedJvm)->AttachCurrentThread(cachedJvm, &pEnv, NULL);
	(*pEnv)->CallVoidMethod(pEnv, cachedMainActObj, playMusicMID,
	idx);
	}

	*/
	
	LOGD("----> NDK CreateEngine CALLED !!! ");

	if( NxGraph != NULL ){  LOGD("----> NDK CreateEngine already done !!! "); return;}


	assetMgr = AAssetManager_fromJava( env, assetManager );
	ANativeWindow* nativeWnd = ANativeWindow_fromSurface(env, surface);


	mOgreScreenWidth = ANativeWindow_getWidth( nativeWnd );
	mOgreScreenHeight = ANativeWindow_getHeight( nativeWnd );

	//Initialize the engine
	NxGraphicsDesc EngineDesc; 
	EngineDesc.Vsync = true ;


	//EGLContext cont = eglGetCurrentContext( ); 	
	//if( cont  == EGL_NO_CONTEXT ) LOGD("----> NO CONTEXT !!! ");
	//if( cont  == 0 ) LOGD("----> NO CONTEXT ZEROOOOO !!! ");
	//EngineDesc.GlContext = &cont;//&cont;
	EngineDesc.Renderer = NxGraphics_GLES2; 
	EngineDesc.mAssetMgr = assetMgr;
	EngineDesc.mNativeWnd = nativeWnd;



	float screenRatio = (float)mOgreScreenWidth/(float)mOgreScreenHeight;


	EngineDesc.mSplashSize[0] = 0.25f;
	EngineDesc.mSplashSize[1] = 0.0f;
	EngineDesc.mSplashSize[2] = 0.5f;
	EngineDesc.mSplashSize[3] = 0.9f;

	EngineDesc.mSplashTexture = "NxLogo.jpg";


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
 
	Desc.mRenderWindow->GetCustomAttribute( "GLCONTEXT", &context  );
	LOGD("----> GLCONTEXT id : %d ", (int)context );
	 

 
	//std::string texname = "NxLogo2.png";
	//NxTextureManager::getSingleton().LoadTexture( texname, "Bootstrap" );
 
	 
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
	MatLeftUnit->SetTextureName("NxLogo.jpg");

	//BackGroundColour->getTechnique(0)->getPass(0)->createTextureUnitState( "NxBorderCenter.png" );
	// MatLeftUnit->SetTextureName(texname); 
	// MatLeftUnit->SetTextureName(texname);  


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
 
	Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,0, 0 ) );

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



JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_OpenTheoraVideo(JNIEnv * env, jobject obj, jstring filepath  ) {

	const char *Stringp = env->GetStringUTFChars(  filepath, NULL );
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



	}else{

		PlaneNode = Scene3DPlayer->CreateNxNode( "PlaneVideo" );
		NxPlaneVideo = PlaneNode->CreateNxPlane( "PlaneVideoGeometry", Nx::Vector2(widthVideo,widthVideo/2), Nx::Vector2(3,3) );

		

		MatVideo =  NxMaterialManager::getSingleton().CreateMaterial("videomaterial");
		NxTechnique * MatVideoTechnique = MatVideo->CreateTechnique("");
		MatVideoPass =  MatVideoTechnique->CreatePass("");
		MatVideoPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA );
		MatVideoPass->SetCullingMode(NXCULL_NONE);
		MatVideoPass->SetDepthCheckEnabled(true);
		MatVideoPass->SetDepthWriteEnabled(true);


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
		MatVideoPass->SetFragmentParameterValue( "tex0", 0 );
		MatVideoPass->SetFragmentParameterValue( "tex1", 1 );
		MatVideoPass->SetFragmentParameterValue( "tex2", 2 );


	}
	








	NxPlaneVideo->SetVisible(true);


	 

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


	mBackGroundUnit->SetTextureName( tex->GetTextureName() );

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
 


JNIEXPORT void JNICALL Java_com_hotstuff_main_OgreActivityJNI_SetModelPose(JNIEnv * env, jobject obj, jfloatArray mat  ) {
 
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
	//LOGD("----> VIEWPORT ACTUAL SIZE width: %d, height: %d ", viewport->GetActualWidth(), viewport->GetActualHeight() );


	//Nx::Vector2 screensize = GetScreenDimensions();

	//LOGD("----> SCREEN ACTUAL SIZE width: %d, height: %d ", mScreenWidth  , mScreenHeight   );

	float ratio = (float)mScreenWidth/(float)mScreenHeight;

	 Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetAspectRatio( ratio );//1920.0f/1440.0f );//aspect ); // <<< --- nexus 1920 x 1080     || 1920 x 1440

	//Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetAspectRatio( ratio );//1920.0f/1080.0f );//aspect ); // <<< --- nexus 1920 x 1080     || 1920 x 1440
	 
	//Nx::Vector3 position = Nx::Vector3(fltmat[12]   , -fltmat[13]  , -fltmat[14] );

	
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
	//Ogre::Quaternion y180 = Ogre::Quaternion(Ogre::Radian(Ogre::Degree(180.0f)), Ogre::Vector3::UNIT_Y);
	//Ogre::Quaternion z180 = Ogre::Quaternion(Ogre::Radian(Ogre::Degree(360.0f)), Ogre::Vector3::NEGATIVE_UNIT_Z);
	
	Nx::Quaternion minusz90 = Nx::Quaternion(Nx::Radian(Nx::Degree(-90.0f)), Nx::Vector3::UNIT_Z);
	Nx::Quaternion x90 = Nx::Quaternion(Nx::Radian(Nx::Degree(90.0f)), Nx::Vector3::UNIT_X);
	Nx::Quaternion y180 = Nx::Quaternion( Nx::Degree(180.0f), Nx::Vector3::UNIT_Y);
 
	if( mBoxes.size() ){

		mBoxes[0]->SetOrientation( Nx::Quaternion::IDENTITY );
		mBoxes[0]->SetPosition( Nx::Vector3(0,0,0));

		mBoxesNodes[0]->SetOrientation( orientation * x90 ); // right orientation
		mBoxesNodes[0]->SetPosition(position);
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
	return;

 
}


 
 
 
 
