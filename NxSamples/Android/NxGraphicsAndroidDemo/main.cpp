/*===============================================================================================
 NxGraphics Engine Example
 Copyright (c), Perspective[S] Technologies 2014.

 This example shows how to basically instance the engine
===============================================================================================*/

#include <android/log.h>
#include <stdlib.h>
#include <math.h>
#include "main.h"

#include <EGL/egl.h>
#include <android/api-level.h>
#include <android/native_window_jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "../../nxdeps/include/OIS/OIS.h"
 
#define  LOG_TAG    "JNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
 
using namespace Nx;
 
NxGraphics * NxGraph = NULL;
Nx3D_Scene * Scene3DPlayer;
//bool isReady = false;
//bool runEngine = true;
NxEntity * NxBoxLeft = NULL;


NxGuiManager * mGuiManager = NULL;
std::vector<std::vector< Nx::Vector2 > > mPartsCoords;
class SourcePanel;
std::vector<SourcePanel*> mPartsObject;
class DropContainer;
DropContainer * mContainer;
std::string textureName = "NxLogo.jpg";

class AndroidInputInjector;
static AndroidInputInjector* mInputInjector;

//static bool EngineCreated = false;
bool mLostFocus = false;


struct TOUCHSTATE
{
	int down;
	int x;
	int y;
}; 

const int numBoxes = 10;
std::vector<NxEntity * > mBoxes;
std::vector<int> mBoxesRotValues;
std::vector<float> mBoxesDirection;

 

 static JavaVM* gVM = NULL;
android_app * mState = NULL;

/* density equals
.75 on ldpi (120 dpi)
1.0 on mdpi (160 dpi; baseline)
1.5 on hdpi (240 dpi)
2.0 on xhdpi (320 dpi)
3.0 on xxhdpi (480 dpi)
4.0 on xxxhdpi (640 dpi) */
float mDensity = 0.0f;



/*
* Return the current width in pixels of the window surface.  Returns a
* negative value on error.
*/
int mWidthPixels = 0;
int mHeightPixels = 0;

float dpToPx(float dp) {
	return (dp * mDensity);
}

float pxToDp(float px) {
	return (px / mDensity );
}

 float dpToPxInt(  float dp) {
	return (int)(dpToPx( dp) + 0.5f);
}

 float pxToDpInt( float px) {
	return (int)(pxToDp(  px) + 0.5f);
}


 
JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved) 
{
	gVM = vm;
	return JNI_VERSION_1_6; 
}  

void showtext( const std::string & mTxt ) {

	ANativeActivity* activity = mState->activity;
	JavaVM* jvm = mState->activity->vm;
	JNIEnv* env = NULL;
	jvm->GetEnv( (void **)&env, JNI_VERSION_1_6);
	jint res =  activity->vm->AttachCurrentThread( &env, 0);
	if (res == JNI_ERR) return; //  Failed to retrieve JVM environment
	jclass clazz =  env ->GetObjectClass(  activity->clazz);
	jmethodID methodID =  env ->GetMethodID(  clazz, "ShowText", "(Ljava/lang/String;)V");
	env ->CallVoidMethod(  activity->clazz, methodID, env->NewStringUTF( mTxt.c_str() ));
	jvm ->DetachCurrentThread();

}



class DropContainer: public NxPanel, public NxDragAndDropContainer {
public:
	DropContainer( const std::string & HeaderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager) : NxPanel( HeaderText, PosX, PosY, Width, Height, Manager), 
		NxDragAndDropContainer( Manager ) {
	}
};


class SourcePanel: public NxPanel, public NxDragAndDropTarget {
public:

	bool isDragging;
	NxDragAndDropContainer* dragC  ;
	int mIndex;
	int mHostedId;

	bool mEnableColors;


	SourcePanel( int index, const std::string & HeaderText, float PosX, float PosY, float Width, float Height, NxGuiManager * Manager) : NxPanel( HeaderText, PosX, PosY, Width, Height, Manager)  {
		dragC = 0;
		isDragging = false;
		SetHeaderVisible( false );
		mIndex = index;
		mHostedId = index;
		mEnableColors = false;

		//to do enable colors on over + resize drag COMPONENT to fixed size ?? its too big
	}

	int getIndex() {
		return mIndex;
	}
	
	void setIndex( int index ) {
		mIndex = index;
	}
	
	int getHostedIndex() {
		return mHostedId;
	}

	void setHostedIndex( int index ) {
		mHostedId = index;
	}

	void mouseReleased(int, int y, int b) {

		if ( dragC ) {
			dragC->stopDragging();
		}


		//if( isDragging )
		//getParentComponent()->RemoveComponent( this );
		

		

	}

	bool mousePressed( int x, int y, int button ) { 
 
		 if(mBaseRectangle->IsInside(x, y)) {
			 /*
			if(dragC) {
				delete dragC;
				dragC = 0;
			}*/
 
			 
			dragC = NxDragAndDropContainer::findParentDragContainerFor(this);
			if ( dragC ) {
				dragC->startDragging(  NxStringConverter::toString( mIndex ), this );
				isDragging = true;
  
			}


		 } 

		return true;
	}

	bool isInterestedInDragSource( const std::string& sourceDescription, NxWidget* sourceComponent )
	{

		if(  sourceComponent == this ) return false;
		//Log("is interested in : " + sourceDescription );
		return true;
	}

	void itemDragEnter( const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y )
	{
		 
		if( mEnableColors )
		SetColour( NxPanel::panelColourId, NxColourValue( 0.0f, 0.0f, 1.0f, 1.0f ) );


	}

	void itemDragMove( const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y )
	{
	}

	void itemDragExit (const std::string& sourceDescription, NxWidget* sourceComponent)
	{
		 
		if( mEnableColors )
		SetColour( NxPanel::panelColourId, NxColourValue( 0.0f, 1.0f, 0.0f, 1.0f ) );


	}

	void itemDropped (const std::string& sourceDescription, NxWidget* sourceComponent, int x, int y)
	{
		int dstIndex = atoi( sourceDescription.c_str()  );
		//Log( "Drop index:" + sourceDescription + " on index:" + NxVideoUtils::ToString( mIndex) );


		int id1 = mPartsObject[dstIndex]->getHostedIndex();
		mPartsObject[mIndex]->setTextureCoordinates( mPartsCoords[id1][0], mPartsCoords[id1][1], mPartsCoords[id1][2], mPartsCoords[id1][3] );
		

		int id2 = mPartsObject[mIndex]->getHostedIndex();
		mPartsObject[dstIndex]->setTextureCoordinates( mPartsCoords[id2][0], mPartsCoords[id2][1], mPartsCoords[id2][2], mPartsCoords[id2][3] );
		
		
		mPartsObject[mIndex]->setHostedIndex( id1 ); 
		mPartsObject[dstIndex]->setHostedIndex( id2 ); 

		if( mEnableColors )
		SetColour( NxPanel::panelColourId, NxColourValue( 1.0f, 1.0f, 0.0f, 1.0f ) );

		 
	}

};

 

class EngineListener : public NxEngineListener
{
public :
	EngineListener(){}

	~EngineListener(){}

	void OnWindowMoved( unsigned int PosX, unsigned int PosY ){
		LOGD( "OnWindowMoved %d X %d : " , PosX, PosY );
	}

	void OnWindowResized( unsigned int Width , unsigned int Height ) {
		LOGD( "OnWindowResized %d X %d : ", Width, Height );
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

	void OnMouseMoved( const OIS::MouseEvent &e ) {
		if( mGuiManager ) mGuiManager->mouseMoved( e.state.X.abs, e.state.Y.abs );
	}

	void OnMouseButtonPressed( const OIS::MouseEvent &e, NxMouseButtonID id ) {
		if( mGuiManager ) mGuiManager->mousePressed( e.state.X.abs, e.state.Y.abs, id ); 
	}

	void OnMouseButtonReleased( const OIS::MouseEvent &e, NxMouseButtonID id ) {
		if( mGuiManager ) mGuiManager->mouseReleased( e.state.X.abs, e.state.Y.abs, id );
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


class NxButtonCallback : public NxButtonListener
{
	void ButtonClicked( NxButton* button )  {
		//showtext( "Button Clicked" );
	}

	void ButtonStateChanged( NxButton * button ) {

		bool isOn = button->getToggleState();
		LOGD( "button state is : " + isOn ? "ON":"OFF" );

	}
};


class NxSliderCallback : public NxSliderListener
{
	void SliderValueChanged ( NxSlider* slider )  {
		//LOGD( "slider value changed: %f", slider->GetValue()  );
	}

};


NxInputCallBack  * mInputCallback = NULL;// declare our mixer callback class




void createPart( int index, const std::string & name ,  int x ,  int y,  int width, int height,
				const Vector2 & topleft,
				const Vector2 & bottomleft, const Vector2 & topright, const Vector2 & bottomright )
{
	SourcePanel * part0 = new SourcePanel( index, name, x, y, width, height, mGuiManager);
	std::vector< Nx::Vector2 > part0Coordinates;
	part0Coordinates.push_back( topleft  );
	part0Coordinates.push_back( bottomleft);
	part0Coordinates.push_back( topright);
	part0Coordinates.push_back( bottomright);
	mPartsCoords.push_back( part0Coordinates );

	part0->SetBackgroundTexture( textureName );
	part0->setTextureCoordinates( mPartsCoords[index][0], mPartsCoords[index][1], mPartsCoords[index][2], mPartsCoords[index][3] );



	mContainer->AddComponent( part0 );
	mPartsObject.push_back( part0 );
}

 
void CreateScene()
{
	NxDeviceManager * DeviceMgr = NxDeviceManager::getSingletonPtr(); 
	NxSoundManager  * SoundMgr  = NxSoundManager::getSingletonPtr();
	NxMocapManager  * MocapMgr  = NxMocapManager::getSingletonPtr();
  
	Nx3DSceneDesc Desc;
	Desc.mType = Nx3D_Scene_Default;
	Desc.mRenderWindow = NxEngine::getSingleton().GetNxWindow();
	Scene3DPlayer = NxScene3DManager::getSingleton().CreateNx3DScene( Desc );


	Scene3DPlayer->SetViewportColour( NxColourValue(0,0,0,1) );

 
	MaterialNx * MatLeft =  NxMaterialManager::getSingleton().CreateMaterial("tester");
	NxTechnique * MatLeftTechnique = MatLeft->CreateTechnique("");
	NxPass * MatLeftPass =  MatLeftTechnique->CreatePass("");
	MatLeftPass->SetSceneBlending(  NXBLEND_TRANSPARENT_ALPHA ); 
	MatLeftPass->SetDepthCheckEnabled(true);
	MatLeftPass->SetDepthWriteEnabled(true);
	MatLeftPass->SetLightingEnabled(false);
	MatLeftPass->SetCullingMode(NXCULL_NONE);
	NxTextureUnit * MatLeftUnit = MatLeftPass->CreateTextureUnit("");
	MatLeftUnit->SetTextureAddressingMode(TEXTURE_BORDER);
	MatLeftUnit->SetTextureName("NxLogo.jpg");
 
	 for( int i = 0 ; i < numBoxes; i++ ) {
		 NxNode * FloorNode = Scene3DPlayer->CreateNxNode( "BaseNode" + NxVideoUtils::ToString( i ) );
		 NxBoxLeft = FloorNode->CreateNxBox( "PlaneLeft"+ NxVideoUtils::ToString( i ), Nx::Vector3(40,40,40), Nx::Vector3(1,1,1) );
		 NxBoxLeft->SetPosition( Nx::Vector3( NxUtils::GetRandom(-150,150)   ,NxUtils::GetRandom(-150,150), NxUtils::GetRandom(-150,-1200)) );
		 NxBoxLeft->SetMaterialName( MatLeft->GetName()  );
		 mBoxes.push_back( NxBoxLeft );

		 int rot = NxUtils::GetRandom(1.5,20);
		 mBoxesRotValues.push_back( rot );

		 float dir = NxUtils::GetRandom(0.1,1.0);
		 mBoxesDirection.push_back( dir );
	 }

	 // create gui
	 
	 mGuiManager = new NxGuiManager( NxEngine::getSingleton().GetContentManager(), NxEngine::getSingleton().GetNxViewport() );


	 mGuiManager->setDefaultFontSize( pxToDp( 18.0f / (float)mGuiManager->GetViewportOriginalHeight()   ) /*pxToDpInt(  )*/  );

	 

	 NxButtonCallback * nxButtonCallback = new NxButtonCallback();
	 NxSliderCallback * nxSliderCallback = new NxSliderCallback();



	 /*
	 // create first panel
	 NxPanel * mPanel1 = mGuiManager->CreatePanel( "[ DELAUNAY TRIANGULATION PARAMETERS ]", 0,0,pxToDpInt(350),pxToDpInt(900)); // pxToDp(350)
	 mPanel1->SetColour( NxPanel::panelColourId, NxColourValue( 1,1,1,0.05 ) );
	 mPanel1->SetColour( NxPanel::panelHeaderColourId, NxColourValue( 1,1,1,0 ) );
	 mPanel1->SetColour( NxPanel::panelHeaderTextColourId, NxColourValue( 1.0,1.0,1.0,1.0 ) );
	 mPanel1->SetHeaderHeight( pxToDpInt(30) );
	 mPanel1->SetHeaderTextHeight( pxToDpInt(19) );
	 mPanel1->SetHeaderVisible( true );
	 mPanel1->SetMovable( false );

	 float OffsetY = mPanel1->GetHeaderHeight();

	 NxButton * mButtonImport = mPanel1->CreateButton( "[ Import Picture ]", 0, OffsetY, pxToDpInt(350), pxToDpInt(30));  
	 mButtonImport->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButtonImport->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0.1 ) );
	 mButtonImport->SetColour( NxButton::buttonClickedColourId, NxColourValue( 1,0,0,0.3 ) );
	 //mButtonImport->SetTextHeight(pxToDpInt( 25 ));
	 mButtonImport->AddButtonListener( nxButtonCallback );

	 OffsetY += mButtonImport->GetHeight();

	 NxLabel * mLabelViewMode = mPanel1->CreateLabel( "View Mode :", 0, OffsetY, pxToDpInt(87), pxToDpInt(25) );
	 mLabelViewMode->SetTextPixelsSpacing( pxToDpInt(3) );
	 mLabelViewMode->SetHorizontalAlignment(NxTextHRight);

	 NxButtonRadio * mButtonViewModeEdit = mPanel1->CreateButtonRadio( "[ Edit ]", 5, pxToDpInt(87), OffsetY, pxToDpInt(131), pxToDpInt(25));  

	 
	 mButtonViewModeEdit->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButtonViewModeEdit->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mButtonViewModeEdit->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 mButtonViewModeEdit->setToggleState(true,false);
	 mButtonViewModeEdit->AddButtonListener( nxButtonCallback );
 


	 NxButtonRadio * mButtonViewModeNavigate = mPanel1->CreateButtonRadio( "[ Navigate ]", 5,  pxToDpInt(218), OffsetY, pxToDpInt(131), pxToDpInt(25));  
	 mButtonViewModeNavigate->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButtonViewModeNavigate->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mButtonViewModeNavigate->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 mButtonViewModeNavigate->setRadioGroupId(5);
	 mButtonViewModeNavigate->AddButtonListener( nxButtonCallback );

	 OffsetY += mButtonViewModeNavigate->GetHeight();

	 NxLabel * mLabelPointsEdit = mPanel1->CreateLabel( "Points :", 0, OffsetY, pxToDpInt(87), pxToDpInt(25) );
	 mLabelPointsEdit->SetTextPixelsSpacing( 3 );
	 mLabelPointsEdit->SetHorizontalAlignment(NxTextHRight);

	

	 NxButtonRadio * mButton1 = mPanel1->CreateButtonRadio( "[ Add ]", 10,  pxToDpInt(87), OffsetY, pxToDpInt(87), pxToDpInt(25));  
	 mButton1->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButton1->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mButton1->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 
	 mButton1->setToggleState(true,false);

	 NxButtonRadio * mButton2 = mPanel1->CreateButtonRadio( "[ Edit ]", 10,  pxToDpInt(87*2), OffsetY, pxToDpInt(87), pxToDpInt(25));  
	 mButton2->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButton2->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mButton2->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 

	 NxButtonRadio * mButton3 = mPanel1->CreateButtonRadio( "[ Delete ]", 10,  pxToDpInt(87*3), OffsetY, pxToDpInt(87), pxToDpInt(25));  
	 mButton3->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mButton3->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mButton3->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 

	 OffsetY += mButton3->GetHeight();

	 NxButtonToggle * mToggleButton1 = mPanel1->CreateButtonToggle( "[ Toggle ]", 0, OffsetY, pxToDpInt(350), pxToDpInt(40));  
	 mToggleButton1->SetColour( NxButton::buttonColourId, NxColourValue( 1,1,1,0 ) );
	 mToggleButton1->SetColour( NxButton::buttonOnColourId, NxColourValue( 1,1,1,0 ) );
	 mToggleButton1->SetColour( NxButton::buttonClickedColourId, NxColourValue( 0,1,0,0.3 ) );
	 mToggleButton1->setToggleState(true,false);

	 OffsetY += mToggleButton1->GetHeight();

	 NxSlider * mSlider = mPanel1->CreateSlider( 0, OffsetY, pxToDpInt(350), pxToDpInt(25) );
	 mSlider->SetColour( NxSlider::sliderBackgroundColourId, NxColourValue::Blue );
	 mSlider->SetColour( NxSlider::sliderColourId, NxColourValue::Red );
	 mSlider->SetColour( NxSlider::sliderTextColourId , NxColourValue::White);
	 mSlider->AddSliderListener( nxSliderCallback );
	 mSlider->SetRange( 0, 100 ,1 );
	 mSlider->SetValue( 50 );
	 */
 

	 /*
	 NxPanel * mPanel2 = mGuiManager->CreatePanel( "[ NEW PANEL ]", pxToDpInt(350),0,pxToDpInt(350),pxToDpInt(900));
	 mPanel2->SetColour( NxPanel::panelColourId, NxColourValue( 0.2f, 0.2f, 0.2f, 0.8f ) );
	 mPanel2->SetColour( NxPanel::panelHeaderColourId, NxColourValue( 0.8f, 0.0f, 0.0f, 0.95f ) );
	 mPanel2->SetColour( NxPanel::panelHeaderTextColourId, NxColourValue( 1.0f, 1.0f, 1.0f, 1.0f ) );
	 mPanel2->SetHeaderHeight( pxToDpInt(30) );
	 mPanel2->SetHeaderTextHeight( pxToDpInt(25) );
	 mPanel2->SetHeaderVisible( true );
	 mPanel2->SetMovable( true );
	 mPanel2->SetZOrder( 150 );
	 mPanel2->SetBackgroundTexture( "NxLogo.jpg" );*/



	// HERE need to make it work (  drag and drop )  when pane is far right... some coordinates probz...


	mContainer = new DropContainer( "[ DRAG AND DROP DEMO ]",  0 , 0, 1280, 960, mGuiManager);
	 mContainer->SetColour( NxPanel::panelColourId, NxColourValue( 0.2f, 0.2f, 0.2f, 1.0f ) );
	 mContainer->SetColour( NxPanel::panelHeaderColourId, NxColourValue( 0.8f, 0.0f, 0.0f, 1.0f ) );
	 mContainer->SetColour( NxPanel::panelHeaderTextColourId, NxColourValue( 1.0f, 1.0f, 1.0f, 1.0f ) );
	 mContainer->SetHeaderHeight( 30 );
	 mContainer->SetHeaderTextHeight( 25 );
	 mContainer->SetHeaderVisible( true );
	 mContainer->SetMovable( false );
	 mContainer->SetZOrder( 5 );
	 //container->SetBackgroundTexture( "NxLogo.jpg" );


	 mGuiManager->AddPanel( mContainer );

	 createPart( 0, "part0",  30 ,  40 ,  1220 , 350,  Vector2( 0, 0),  Vector2( 0, 0.4 ), Vector2( 1, 0   ) , Vector2( 1, 0.4 )  );

	 //
	 createPart( 1, "part1",  30 ,  350+40 ,  1220/2 , 260,  Vector2( 0, 0.4),  Vector2(  0, 0.85 ), Vector2( 0.5, 0.4   ) , Vector2( 0.5, 0.85  )  );
	 createPart( 2, "part2",  30+1220/2 ,  350+40 ,  1220/2 , 260,  Vector2( 0.5, 0.4),  Vector2( 0.5, 0.85 ), Vector2(1.0, 0.4    ) , Vector2( 1.0, 0.85   )  );

	 //
	 createPart( 3, "part3",  30 ,  350+260+40 ,  1220/4 , 180,  Vector2( 0, 0.85),  Vector2( 0, 1.0 ), Vector2(0.25, 0.85 ) , Vector2( 0.25, 1.0 )  );
	 createPart( 4, "part4",  30+ (1220/4)*1 ,  350+260+40 ,  1220/4 , 180,  Vector2( 0.25, 0.85),  Vector2( 0.25, 1.0 ), Vector2(0.50, 0.85 ) , Vector2( 0.50, 1.0 )  );

	 createPart( 5, "part5",  30+ (1220/4)*2 ,  350+260+40 ,  1220/4 , 180,  Vector2( 0.50, 0.85 ),  Vector2( 0.50, 1.0 ), Vector2(0.75, 0.85 ) , Vector2( 0.75, 1.0 )  );
	 createPart( 6, "part6",  30+ (1220/4)*3 ,  350+260+40 ,  1220/4 , 180,  Vector2( 0.75, 0.85 ),  Vector2( 0.75, 1.0 ), Vector2(1.0, 0.85 ) , Vector2( 1.0, 1.0 )  );

	 mGuiManager->Update( 0.0f );


	/* old implementqtion
	 DropContainer * container = new DropContainer( "[ DRAG AND DROP DEMO ]", pxToDpInt(175) , 0, pxToDpInt(350), pxToDpInt(900), mGuiManager);
	 container->SetColour( NxPanel::panelColourId, NxColourValue( 0.2f, 0.2f, 0.2f, 0.8f ) );
	 container->SetColour( NxPanel::panelHeaderColourId, NxColourValue( 0.8f, 0.0f, 0.0f, 0.95f ) );
	 container->SetColour( NxPanel::panelHeaderTextColourId, NxColourValue( 1.0f, 1.0f, 1.0f, 1.0f ) );
	 container->SetHeaderHeight( pxToDpInt(30) );
	 container->SetHeaderTextHeight( pxToDpInt(25) );
	 container->SetHeaderVisible( true );
	 container->SetMovable( false );
	 container->SetZOrder( 150 );
	 //container->SetBackgroundTexture( "NxLogo.jpg" );


	 mGuiManager->AddPanel( container );
 
	 DragAndDropDemoTarget * target = new DragAndDropDemoTarget( "Target", 0, pxToDpInt(25), pxToDpInt(175), 100, mGuiManager);
	 target->SetMovable( false );
	 target->SetColour( NxPanel::panelColourId, NxColourValue( 1.0f, 0.0f, 0.0f, 1.0f ) );
	 container->AddComponent( target );  


	 SourceButton * source = new SourceButton( "Source", pxToDpInt(175), pxToDpInt(25), pxToDpInt(175), 100, mGuiManager);
	 source->SetMovable( false );
	 container->AddComponent( source  );  
 
	 mGuiManager->Update( 0.0f ); */




 
	 
	 //Nx::Vector3 pos = NxBoxLeft->GetPosition();
	 //LOGD("","Position : " + NxVideoUtils::ToString( pos.x ) + " " +   NxVideoUtils::ToString( pos.y ) + " " +   NxVideoUtils::ToString( pos.y ));
	 
	 Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0, 0, 0 ) );
 
	 //double Distance = 500.0;
	 //// Calculate FOV H in degrees from Camera Distance.
	 //double heightscreen = 810.0f;//800.0f;
	 //double FOVRad = 2.0 *  atan(( heightscreen / 2.0 ) / Distance );
	 //double FOVDeg = FOVRad * 180.0 / Nx::Math::PI;

	 //Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetPosition( Nx::Vector3( 0,0, Distance ) );
	 //Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetAspectRatio(1440.0f/heightscreen);
	 //Scene3DPlayer->GetNxNode("CameraEditorNode")->GetNxController("CameraEditor")->SetFieldOfView( Nx::Degree( FOVDeg ).valueDegrees()  );
	  

	// NxGraph->StartRendering();

	 EngineListener * liste = new EngineListener();
	 NxEngine::getSingleton().GetNxWindow()->AddListener(  liste );

	 
	 NxEngine::getSingleton().GetNxWindow()->AddInputListener( mInputCallback = new NxInputCallBack() ); 
 

	// NxInputManager::getSingleton().AddWindow( NxEngine::getSingleton().GetNxWindow() ) ;
  
	 
	return;
}

 

class AndroidInputInjector
{
public:

	AndroidInputInjector()  {
	
	}

	void injectKeyEvent(int action, int32_t keyCode, struct android_app* app)
	{
		if( keyCode == AKEYCODE_BACK && action == 1 ) {		

			LOGD("keyCode == AKEYCODE_BACK  called" );
			ANativeActivity_finish( app->activity );
		}
	}

	void injectTouchEvent( int action, float x, float y, int pointerId /*= 0*/)
	{

		//LOGD("Touched X:%f Y:%f ",x,y);
		//LOGD("Touched action %d", action );

		switch( action ) {
		case 0:
			if( mGuiManager )mGuiManager->touchPressed( x, y, pointerId );
			break;
		case 1:
			if( mGuiManager )mGuiManager->touchReleased( x, y, pointerId );
			break;
		case 2:
			if( mGuiManager )mGuiManager->touchMoved( x, y, pointerId );
			break;
		case 3:;
			if( mGuiManager )mGuiManager->touchCancelled( x, y, pointerId );
			break;
		default:
			LOGD("Touched type not implemented ");
		}

		if( mGuiManager ) mGuiManager->Update( 0.0f );
 
	}
};
 
static int32_t handleInput( struct android_app* app, AInputEvent* event ) {

	if( mInputInjector ) {
		if( AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION ) {

			int nPointerCount = AMotionEvent_getPointerCount( event );
			int nSourceId = AInputEvent_getSource( event );
			struct TOUCHSTATE *touchstate = 0;

			for( int n = 0 ; n < nPointerCount ; ++n )
			{
				int nPointerId = AMotionEvent_getPointerId( event, n );
				int nAction = AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction( event );

	 /*
				if( nAction == AMOTION_EVENT_ACTION_POINTER_DOWN || nAction == AMOTION_EVENT_ACTION_POINTER_UP )
				{
					int nPointerIndex = (AMotionEvent_getAction( event ) & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
					nPointerId = AMotionEvent_getPointerId( event, nPointerIndex );
				}

				if( nAction == AMOTION_EVENT_ACTION_DOWN || nAction == AMOTION_EVENT_ACTION_POINTER_DOWN )
				{
					touchstate[nPointerId].down = 1;
				}
				else if( nAction == AMOTION_EVENT_ACTION_UP || nAction == AMOTION_EVENT_ACTION_POINTER_UP || nAction == AMOTION_EVENT_ACTION_CANCEL )
				{
					touchstate[nPointerId].down = 0;
				} */

				//if(nAction == 0)
				//	mInputInjector->injectTouchEvent(2, AMotionEvent_getRawX(event, nPointerId), AMotionEvent_getRawY(event, nPointerId), nPointerId  );
 
				mInputInjector->injectTouchEvent(nAction, AMotionEvent_getRawX(event, nPointerId), AMotionEvent_getRawY(event, nPointerId) , nPointerId );
			
			}


		}
		else 
		{
			mInputInjector->injectKeyEvent(AKeyEvent_getAction(event), AKeyEvent_getKeyCode(event), app);
		}

		return 1;
	}
	return 0;
}

 


static void handleCmd(struct android_app* app, int32_t cmd)
{
 
	switch (cmd) 
	{
	case APP_CMD_SAVE_STATE:

		LOGD("APP_CMD_SAVE_STATE called" );
 
		break;
	case APP_CMD_INIT_WINDOW:

		LOGD("APP_CMD_INIT_WINDOW called" );
		 
		if( app->window != NULL )
		{
			AConfiguration* config = AConfiguration_new();
			AConfiguration_fromAssetManager(config, app->activity->assetManager);
	 
			if (!NxGraph) 
			{
				LOGD("Creating engine ..." );
				mWidthPixels = ANativeWindow_getWidth( app->window );
				mHeightPixels = ANativeWindow_getHeight( app->window );
				mDensity =  160.0f / AConfiguration_getDensity( config );
				int32_t ori = AConfiguration_getOrientation( config );

 
				NxGraphicsDesc EngineDesc;
				EngineDesc.Renderer = NxGraphics_GLES2; 
				EngineDesc.mNativeWnd = app->window;
				EngineDesc.mConfig = config;
				EngineDesc.mAssetMgr = app->activity->assetManager;
				NxGraph = new NxGraphics();
				NxGraph->CreateNxEngine( EngineDesc );

				mInputInjector = new AndroidInputInjector();

				CreateScene();
 
			} else {

				LOGD("CreateInternalResources  called" );
				NxEngine::getSingleton().GetNxWindow()->CreateInternalResources( app->window, config );
			}

			AConfiguration_delete(config);
		}
		break;
	case APP_CMD_TERM_WINDOW:
		LOGD("APP_CMD_TERM_WINDOW called" );
		NxEngine::getSingleton().GetNxWindow()->DestroyInternalResources(); // leave it there !! else no recover from home button
		break;
	case APP_CMD_GAINED_FOCUS:
		LOGD("APP_CMD_GAINED_FOCUS called" );
		mLostFocus = false;
		break;
	case APP_CMD_LOST_FOCUS:
		LOGD("APP_CMD_LOST_FOCUS called" );
		mLostFocus = true;
		break;
	case APP_CMD_CONFIG_CHANGED:
		LOGD("APP_CMD_CONFIG_CHANGED called could be ORIENTATION !" );
		break;
	case APP_CMD_PAUSE:
		LOGD("APP_CMD_PAUSE called" );
		break;
	case APP_CMD_STOP:
		LOGD("APP_CMD_STOP called" );
		break;
	case APP_CMD_DESTROY:
		LOGD("APP_CMD_DESTROY called" );
		break;
	}
}
 
void android_main(struct android_app* state) { 
 
	app_dummy();
 
	mState = state;
 
	JNIEnv *env;
	gVM->AttachCurrentThread(&env, 0);
  
	state->onAppCmd = &handleCmd;
	state->onInputEvent = &handleInput;
 
	int ident, events;
	struct android_poll_source* source;
	while( 1 ) { 
 
		while ((ident = ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0) // poll input events
		{
			if (source != NULL)
				source->process(state, source);

			if (state->destroyRequested != 0) {

				LOGD(" state->destroyRequested called" );
 
				LOGD(" Release engine..." );
				NxGraph->ReleaseEngine();
 
				LOGD(" Detach thread 1 ..." );
				gVM->DetachCurrentThread();
 
				gVM = 0;

				exit(1) ;
 

				LOGD(" RETURN ..." );

				 return;

			}
	 
		}

		if( NxGraph && !mLostFocus ) {
			for( int i = 0 ; i < numBoxes; i++ ) {
				Nx::Quaternion quat;
				float dir = mBoxesDirection[i];
				quat.FromAngleAxis(Nx::Degree(mBoxesRotValues[i]),Nx::Vector3(dir,dir,dir));
				mBoxes[i]->Rotate( quat , NxWorld );
			}
			NxGraph->Update();
 	
		}

	}
 
} 

 


 
 
 
 
