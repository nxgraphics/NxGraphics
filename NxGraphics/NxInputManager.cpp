#include "NxGraphicsPreCompiled.h"
#include "../nxdeps/include/OIS/OIS.h"
 
namespace Nx {

template<> NxInputManager* NxSingleton<NxInputManager>::ms_Singleton = 0;

class NxInput : public OIS::KeyListener, public OIS::MouseListener, public OIS::JoyStickListener, public OIS::MultiTouchListener
{
public :

	NxInput( NxInputManager * Manager, NxRenderWindow * Window ) : mManager( Manager ), mWindow(Window), mMouseButtonsActive( true )
	{

		mKeyboard = 0;
		mMouse = 0;
		mMultitouch = 0;
 
		//android doesnt need this, just create injecttouch and inject key
#ifdef __ANDROID__
		LogMsg("Window->UpdateWindowMetrics()  "  );
		Window->UpdateWindowMetrics();

		LogMsg("GetWindowNativeHandle  "  );
		size_t windowHnd = 0;//= dynamic_cast<size_t>( Window->GetWindowNativeHandle() );
		Window->GetWindow()->getCustomAttribute("WINDOW", &windowHnd);
		
		LogMsg("OIS::InputManager::createInputSystem...   "  );
		mInputManager = OIS::InputManager::createInputSystem(windowHnd);
		LogMsg("OIS::InputManager::createInputSystem.OK");


		// Create Multitouch
		LogMsg("mInputManager->createInputObject( OIS::OISMultiTouch, true));");
		mMultitouch = static_cast<OIS::MultiTouch*>( mInputManager->createInputObject( OIS::OISMultiTouch, true));
		mMultitouch->setEventCallback( this ); 
#else

		OIS::ParamList params;
		size_t windowHnd = 0;
		std::ostringstream windowHndStr;

		LogMsg("Window->GetWindow()->getCustomAttribute");
		Window->GetWindow()->getCustomAttribute("WINDOW", &windowHnd);
		
		windowHndStr << windowHnd;
		//show the mouse
		params.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));


		params.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
		params.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE"))); 

		params.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_BACKGROUND"))); 
		params.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE"))); 
		//if( kbSettings == 0 ) kbSettings = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY;


		LogMsg("OIS::InputManager::createInputSystem");
		mInputManager = OIS::InputManager::createInputSystem( params ); 


		// Create keyboard
		mKeyboard = static_cast<OIS::Keyboard*>( mInputManager->createInputObject( OIS::OISKeyboard, true )); 
		mKeyboard->setEventCallback( this ); 
		// Create Mouse
		mMouse = static_cast<OIS::Mouse*>( mInputManager->createInputObject( OIS::OISMouse, true));
		mMouse->setEventCallback( this ); 
 
		// Create Joysticks if any
		LogMsg( "Found Num Joysticks : " + Ogre::StringConverter::toString( mInputManager->getNumberOfDevices(  OIS::OISJoyStick )) );
		if( mInputManager->getNumberOfDevices(OIS::OISJoyStick) > 0) {
			for(int i = 0 ; i < mInputManager->getNumberOfDevices(OIS::OISJoyStick); i++ ) {
				OIS::JoyStick * Joy = static_cast<OIS::JoyStick*>( mInputManager->createInputObject(  OIS::OISJoyStick, true )); 
				Joy->setEventCallback( this ); mJoysticks.push_back( Joy );
			}
		}
#endif
 
 
	}
	
	~NxInput()
	{
	
	}

	void Update( float ms )
	{
		if( !IsActive() ) return;
		if( mMouse ) mMouse->capture();
		if( mKeyboard ) mKeyboard->capture();
		if( mMultitouch ) mMultitouch->capture();
		for(int i = 0 ; i < mJoysticks.size(); i++ ){
			mJoysticks[i]->capture();
		}

	 
	}

	bool IsActive() {
		return mWindow->HasFocus();
	}

	bool IsKeyDown( NxKeyCode Code ) {
		return mKeyboard->isKeyDown( (OIS::KeyCode )Code );
	}

	bool IsMouseButtonDown( NxMouseButtonID ButtonId ) {
		return mMouseButtonsActive ? mMouse->getMouseState().buttonDown( (OIS::MouseButtonID) ButtonId ) : false ;
	}

	void SetMouseButtonsActive( bool Active ) {
		mMouseButtonsActive = Active;
	}

	void UpdateBounds( float Width, float Height ) {
		const OIS::MouseState & ms = mMouse->getMouseState();
		ms.width  = (int) Width ;
		ms.height = (int) Height ;
	}

	const OIS::MouseState& GetMouseState() {
		return mMouse->getMouseState();
	}

	NxRenderWindow * GetRenderWindow() {
		return mWindow;
	}

	void AddListener( NxInputListener * Listen ) {
		mListerners.push_back(Listen);
	}

	//multitouch virtuals
	bool touchMoved( const OIS::MultiTouchEvent &arg ) {  
		LogMsg( "OIS touchMoved..." );
		return true;
	}

	bool touchPressed( const OIS::MultiTouchEvent &arg ) {  
		LogMsg( "OIS touchPressed..." );
		return true;
	}

	bool touchReleased( const OIS::MultiTouchEvent &arg ) {  
		LogMsg( "OIS touchReleased..." );
		return true;
	}

	bool touchCancelled( const OIS::MultiTouchEvent &arg ) {  
		LogMsg( "OIS touchCancelled..." );
		return true;
	}


	//keyboard virtuals
	bool keyPressed( const OIS::KeyEvent &e ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnKeyPressed( (NxKeyCode)e.key);
		}
		return true;
	}

	bool keyReleased( const OIS::KeyEvent &e ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnKeyReleased( (NxKeyCode) e.key);
		}
		return true ;
	}

	//mouse virtuals
	bool mouseMoved( const OIS::MouseEvent &e ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnMouseMoved(e);
		}
		return true ;
	}

	bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			if(mMouseButtonsActive) (*Iter)->OnMouseButtonPressed(e,(NxMouseButtonID)id);
		}
		return true ;
	}

	bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			if(mMouseButtonsActive) (*Iter)->OnMouseButtonReleased(e,(NxMouseButtonID)id);
		}
		return true ;
	}

	//joystick virtuals
	bool buttonPressed( const OIS::JoyStickEvent &e, int button ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnJoystickButtonPressed( e, button );
		}
		return true ;
	}

	bool buttonReleased( const OIS::JoyStickEvent &e, int button ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnJoystickButtonReleased( e, button );
		}
		return true ;
	}

	bool axisMoved( const OIS::JoyStickEvent &e, int axis ) {
		for( std::vector< NxInputListener * >::iterator Iter = mListerners.begin(); Iter != mListerners.end(); Iter++ ) {
			(*Iter)->OnJoystickAxisMoved(e,axis);
		}
		return true;
	}

private :
	NxRenderWindow * mWindow;
	NxInputManager * mManager;
	OIS::InputManager * mInputManager;
	OIS::Keyboard * mKeyboard;
	OIS::Mouse * mMouse;
	OIS::MultiTouch *  mMultitouch;
	std::vector< OIS::JoyStick * > mJoysticks;	
	bool mMouseButtonsActive;
	std::vector<NxInputListener* > mListerners;

};


// Input Manager

NxInputManager::NxInputManager()
{

}

NxInputManager::~NxInputManager()
{

}

void NxInputManager::AddListener( NxRenderWindow * Window, NxInputListener * ListenerClass )
{
	std::vector<NxInput*>::iterator Iter;
	for( Iter = mInputs.begin(); Iter != mInputs.end(); Iter++  ) {
		if( (*Iter)->GetRenderWindow() == Window ){ (*Iter)->AddListener( ListenerClass );  break;}
	}
}

void NxInputManager::RemoveListener( NxInputListener * ListenerClass )
{
	//mListenerList.remove( ListenerClass );
}

bool NxInputManager::IsKeyDown( NxKeyCode Code )
{
	return HasActiveInput() ? GetActiveInput()->IsKeyDown( Code ) : false;
}

bool NxInputManager::IsMouseButtonDown( NxMouseButtonID ButtonId )
{
	return HasActiveInput() ? GetActiveInput()->IsMouseButtonDown( ButtonId ) : false;
}

void NxInputManager::SetMouseButtonsActive( bool Active )
{
	if( HasActiveInput() ) GetActiveInput()->SetMouseButtonsActive( Active ) ;
}

bool NxInputManager::SetKey( NxKeyCode Code, bool KeyUp )
{
#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	#define UP_ARROW 72
	#define LEFT_ARROW 75
	#define DOWN_ARROW 80
	#define RIGHT_ARROW 77

	BYTE bVk;
	BYTE bScan;
	DWORD dwFlags;
	ULONG_PTR dwExtraInformation;

	if( Code == NXKC_UP )
	{
		bVk = VK_UP;
		bScan = UP_ARROW;
		dwFlags = KeyUp ? KEYEVENTF_KEYUP : 0;
		dwExtraInformation = 0;	
	}
	else if( Code == NXKC_DOWN )
	{
		bVk = VK_DOWN;
		bScan = DOWN_ARROW;
		dwFlags = KeyUp ? KEYEVENTF_KEYUP : 0;
		dwExtraInformation = 0;	
	}
	else if( Code == NXKC_LEFT )
	{
		bVk = VK_LEFT;
		bScan = LEFT_ARROW;
		dwFlags = KeyUp ? KEYEVENTF_KEYUP : 0;
		dwExtraInformation = 0;	
	}
	else if( Code == NXKC_RIGHT )
	{
		bVk = VK_RIGHT;
		bScan = RIGHT_ARROW;
		dwFlags = KeyUp ? KEYEVENTF_KEYUP : 0;
		dwExtraInformation = 0;	
	}

	::keybd_event( bVk, bScan, dwFlags, dwExtraInformation );
#endif

	return true;

}

void NxInputManager::ShowMouseCursor( bool Show )
{
#if NXGRAPHICS_PLATFORM == NXGRAPHICS_PLATFORM_WIN32
	ShowCursor( Show );
#endif
}

bool NxInputManager::HasActiveInput()
{
	bool Active = false;
	std::vector<NxInput*>::iterator Iter;
	for( Iter = mInputs.begin(); Iter != mInputs.end(); Iter++  ) {
		if( (*Iter)->IsActive() ){ Active = true; break;}
	}
	return Active;
}

NxInput * NxInputManager::GetActiveInput()
{
	NxInput * In = 0;
	std::vector<NxInput*>::iterator Iter;
	for( Iter = mInputs.begin(); Iter != mInputs.end(); Iter++  ) {
		if( (*Iter)->IsActive() ){ In = (*Iter); break; }
	}
	return In;
}

const OIS::MouseState& NxInputManager::GetMouseState()
{
	return GetActiveInput()->GetMouseState();
}

int NxInputManager::GetMouseRelativeZ()
{
	return GetMouseState().Z.rel;
}

void NxInputManager::UpdateBounds( float Width, float Height )
{
	if( HasActiveInput() ) GetActiveInput()->UpdateBounds( Width, Height ) ;
}

void NxInputManager::Update( float ms )
{
	std::vector<NxInput*>::iterator Iter;
	for( Iter = mInputs.begin(); Iter != mInputs.end(); Iter++  ) {
		(*Iter)->Update( ms );
	}
}

void NxInputManager::AddWindow( NxRenderWindow * Window ) 
{
	NxInput * In = new NxInput( this, Window );
	mInputs.push_back( In );
	In->UpdateBounds( Window->GetWidth(), Window->GetHeight() );
}

NxInputManager& NxInputManager::getSingleton()
{
	assert( ms_Singleton );  return ( *ms_Singleton );
}

NxInputManager * NxInputManager::getSingletonPtr()
{
	return ms_Singleton;
}



bool IsDoubleClick()
{
	static unsigned long mOldTimer;
	static unsigned long mTimer;
	static int mIncrement = 0;
	mIncrement++ ;

	if(mIncrement >= 1) { //if clicks exceed 2
		mTimer = Ogre::Root::getSingleton().getTimer()->getMilliseconds();
		if( mTimer - mOldTimer < 300 ) { //300 ms time between 2 click
			mIncrement = 0; return true ;
		}
		mIncrement = 0 ;
		mOldTimer = mTimer ;
	}
	return false ;
}

}//namespace